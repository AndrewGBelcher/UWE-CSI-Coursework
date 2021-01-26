#include "userprog/syscall.h"
#include <stdio.h>
#include <user/syscall.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/vaddr.h"
#include "threads/malloc.h"
#include "lib/user/syscall.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "userprog/process.h"
#include "filesys/file.h"
#include "filesys/filesys.h"


void syscall_init (void) 
{
 	lock_init(&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


static void syscall_handler (struct intr_frame *frame)
{
	
	// Grab stack pointer
	uint32_t *esp = (uint32_t*)frame->esp;

	// Is the Stack Pointer Valid?
	check_userland_addr(esp);
	
	// Grab syscall number
	int syscall_num = *esp;

	DEBUG_PRINT("\nCalling syscall:%d\n\n",syscall_num);

	// Check if its a valid syscall
	ASSERT(syscall_num < SYS_NUM_SYSCALLS);
	
	// Now call our syscall
	int arg[MAX_ARGS];
	
	// Now Branch
	switch(syscall_num)
	{
	
	case SYS_HALT: // 0
		{
		
			frame->eax = sys_halt();
			break;
		}
	case SYS_EXIT: // 1
		{
			int status = (int)get_stack_argument(frame,0);
			
			sys_exit(status);
			break;
		}
	case SYS_EXEC: // 2
		{
			const void* cmd = get_stack_argument(frame,0);
			
			check_userland_addr((void*)cmd);
			
			frame->eax = sys_exec(cmd);
			break;
		}
	case SYS_WAIT: // 3
		{
			pid_t pid = (pid_t)get_stack_argument(frame,0);
		
			frame->eax = sys_wait(pid);
			break;
		}
	case SYS_CREATE: // 4
		{
			const char* filename = (const char*)get_stack_argument(frame,0);
			unsigned size = (unsigned)get_stack_argument(frame,1);
			
			check_userland_addr((void*)filename);
			
			frame->eax = sys_create(filename, size);
			break;
		}
	case SYS_REMOVE: // 5
		{
			const char* filename = (const char*)get_stack_argument(frame,0);
			check_userland_addr((void*)filename);
			
			frame->eax = sys_remove(filename);
			break;
		}
	case SYS_OPEN: // 6
		{
		
			const char* filename = (const char*)get_stack_argument(frame,0);
			check_userland_addr((void*)filename);

			frame->eax =  sys_open(filename);
			break;
		}
	case SYS_FILESIZE: // 7
		{
		
			int fd = (int)get_stack_argument(frame,0);
			
			frame->eax = sys_filesize(fd);
			break;
		}
	case SYS_READ: // 8
		{
			int fd = (int)get_stack_argument(frame,0);
			char *buffer = (const char*)get_stack_argument(frame,1);
			unsigned size = (unsigned)get_stack_argument(frame,2);
			
			check_userland_buffer(buffer,size);
			
			frame->eax = sys_read(fd, buffer, size);
			break;
		}
	case SYS_WRITE: // 9
		{
		
			int fd = (int)get_stack_argument(frame,0);
			const void *buffer = (const void*)get_stack_argument(frame,1);
			unsigned size = (unsigned)get_stack_argument(frame,2);

			check_userland_buffer(buffer,size);
			
			frame->eax = sys_write(fd, buffer, size);
			break;
		}
	case SYS_SEEK: // 10
		{
			int fd = (int)get_stack_argument(frame,0);
			unsigned pos = (unsigned)get_stack_argument(frame,1);
		
			sys_seek(fd, pos);
			break;
		}
	case SYS_TELL: // 11
		{
			int fd = (int)get_stack_argument(frame,0);
		
			frame->eax = sys_tell(fd);
			break;
		}
	case SYS_CLOSE: // 12
		{
			int fd = (int)get_stack_argument(frame,0);
					
			frame->eax = sys_close(fd);
			break;
		}
	case SYS_MMAP: // 13
		{
		
			//mmap();
			break;
		}
	case SYS_MUNMAP: // 14
		{
		
			//munmap();
			break;
		}

	// Oops!
	default:
		DEBUG_PRINT("Syscall Not Implemented!\n\tsyscall_num:%d\n\n",syscall_num);
	
	}
	
}


void sys_halt(void)
{
	shutdown_power_off();
}


void sys_exit(int status)
{
	struct thread* cur = thread_current();
	struct list_elem* e;	
	char* fname, saveptr;

	// print name and status before exit
	printf("%s: exit(%d)\n", cur->name,status);
	
	// assign the thread the status
	cur->exit_status = status;
	
	thread_exit();
	
}


pid_t sys_exec(const char* cmd)
{
	DEBUG_PRINT("EXEC:%s\n",cmd);
	
	tid_t tid = process_execute(cmd);
	
	return tid;
}


int sys_wait(pid_t pid)
{
	return process_wait(pid);
}


int sys_create(const char* filename, unsigned size)
{

	int ret = -1;
	
	lock_acquire(&filesys_lock);
	ret = filesys_create(filename, size);
	lock_release(&filesys_lock);
	
	return ret;
}


int sys_remove(const char* filename)
{
	int ret = -1;
	
	lock_acquire(&filesys_lock);
	ret = filesys_remove(filename);
	lock_release(&filesys_lock);
	
	return ret;
}


int sys_open(const char* filename)
{	
	struct file *f;
	int status = -1;
	int fd;
	struct thread* cur_td = thread_current();
	
	// synch filesystem usage
	lock_acquire(&filesys_lock);
	
	// open file and get the struct file pointer of the file
	f = filesys_open(filename);
	
	// handle filesys open error
	if(f == NULL)
	{
		lock_release(&filesys_lock);
		return -1;
	}

	// time to add this file to our threads open files
	struct proc_fd_list* fd_list = palloc_get_page(0);
	
	// incase we cant allocate for our list
	if(!fd_list)
	{
		lock_release(&filesys_lock);
		return -1;	
	} 

	// grab the current threads file list
	struct list* td_file_list = &cur_td->thread_file_list;

	// add our new fd and file * to our processes list
	fd_list->fd = new_fd();	
	fd_list->file = f;
	file_deny_write(f);
	
	// now add the elem from our list to the threads list
	list_push_back(td_file_list, &(fd_list->elem));

	fd = fd_list->fd;

	lock_release (&filesys_lock);
	
	return fd;		
}


int sys_filesize(int fd)
{

	int ret = -1;
	
	lock_acquire(&filesys_lock);
	
	struct proc_fd_list* proc_file = get_process_file(fd);
	  
	if(proc_file != NULL)
		ret = file_length(proc_file->file);
	
	lock_release(&filesys_lock);
	return ret;
}



int sys_read(int fd, char* buffer, unsigned size)
{
		int i;
		int ret = -1;
		
		// why do anything		
		if(size < 1)return 0;

		// stdio handling
		if(fd == STDOUT_FILENO || fd == STDERR_FILENO)return ret;
		
		if(fd == STDIN_FILENO)
		{
			*(buffer++) = input_getc();
			return 1;
		}

		// normal files		
		struct file *f;

		lock_acquire(&filesys_lock);
	
	  struct proc_fd_list* proc_file = get_process_file(fd);

	  if(proc_file != NULL)
	  	ret = file_read(proc_file->file, buffer, size);
	
		lock_release (&filesys_lock);
		
		return ret;
}

int sys_write(int fd, const void* buffer, unsigned size)
{
		int ret = -1;
		
		// why do anything
		if(size < 1)return 0;

		// stdio handling
		if(fd == STDIN_FILENO || fd == STDERR_FILENO)return ret;
		
		if(fd == STDOUT_FILENO)
		{
			putbuf(buffer,size);
			return size;
		}

		// normal files		
		struct file *f;

		lock_acquire(&filesys_lock);
	
	  struct proc_fd_list* proc_file = get_process_file(fd);
	  
	  if(proc_file != NULL)
	  {
	  	file_allow_write(proc_file->file);
	  	ret = file_write(proc_file->file, buffer, size);
	  	file_deny_write(proc_file->file);
		}
		
		lock_release (&filesys_lock);

		return ret;
}



void sys_seek(int fd, unsigned pos)
{
		struct proc_fd_list* proc_file = get_process_file(fd);
		
	  if(proc_file == NULL || proc_file->file == NULL)
	  	sys_exit(-1);
	  
		lock_acquire(&filesys_lock);
		file_seek(proc_file->file,pos);
		lock_release(&filesys_lock);
}


int32_t sys_tell(int fd)
{
		int32_t cur_offset = -1;
		struct proc_fd_list* proc_file = get_process_file(fd);
		
	  if(proc_file == NULL || proc_file->file == NULL)
	  	sys_exit(-1);

		lock_acquire(&filesys_lock);
		cur_offset = file_tell(proc_file->file);
		lock_release(&filesys_lock);
	
		return cur_offset;
}

int sys_close(int fd)
{
		struct thread* cur = thread_current();

		if(fd < 3 || cur->fd_next >= fd)return -1;

		struct proc_fd_list* proc_file = get_process_file(fd);
		
	  if(proc_file == NULL || proc_file->file == NULL)
	  	sys_exit(-1);
	
		lock_acquire(&filesys_lock);
		
		file_allow_write(proc_file->file);
		file_close(proc_file->file);

		list_remove(&(proc_file->elem));
		palloc_free_page(proc_file);

		
		lock_release(&filesys_lock);
	
		return 0;
}
/*
void sys_mmap(struct init_frame *frame UNUSED)
{

}

void sys_munmap(struct init_frame *frame UNUSED)
{

}*/

//	==============	HELPER FUNCTIONS ===============

// A nice simple hexdump function
int hexDump(const void *data, int size)
{
	unsigned char *d = (unsigned char *)data;
	int consoleSize = 16;
	char b[consoleSize + 3];
	int i;

	if(data == 0){
		return -1;
		}
	b[0] = '|';
	b[consoleSize + 1] = '|';
	b[consoleSize + 2] = '\0';
	
	DEBUG_PRINT("\n-------HEX DUMP------\n");
	for (i = 0; i < size; i++)
	{
		if ((i % consoleSize) == 0)
		{
			if (i != 0){
				DEBUG_PRINT("  %s\n", b);
				}
			DEBUG_PRINT("%08lx ", (unsigned char *)data + i);
		}

		if(i % consoleSize == 8)
			DEBUG_PRINT(" ");
		DEBUG_PRINT(" %02x", d[i]);

		if (d[i] >= ' ' && d[i] <= '~')
			b[i % consoleSize + 1] = d[i];
		else
			b[i % consoleSize + 1] = '.';
	}
	while((i % consoleSize) != 0)
	{
		if(i % consoleSize == 8)
			DEBUG_PRINT("    ");
		else
			DEBUG_PRINT("   ");
		b[i % consoleSize + 1] = '.';
		i++;
	}
	DEBUG_PRINT("  %s\n\n", b);

	return 0;
}

// generates a new file descriptor reserving 0,1,2 for stdio
static int new_fd()
{
	struct thread* cur = thread_current();
	int fd_cur = cur->fd_next;
	
	// update thread +1 after return
	return cur->fd_next++;
}


// evaluate pointer into userland, if invalid exit the process
static void check_userland_addr (const void *ptr)
{

  // check if pointer is to somewhere and is in the userland address space
	if (ptr == NULL || 
			!is_user_vaddr(ptr) ||
			ptr >= PHYS_BASE ||
			pagedir_get_page(thread_current()->pagedir, ptr) == NULL
			)
  {
    // exit if address is not valid
    sys_exit(-1);
    NOT_REACHED();
  }

}


// evaluate useland buffer pointer by pointer
static void check_userland_buffer (void *buffer, size_t size)
{

	char* tmp_buf = (char *)buffer;
	
	for(size_t i = 0; i < size; i++)
	{
		check_userland_addr((void*)tmp_buf);
		tmp_buf++;
	}
}


// gets stack argument with indexex arg to point into stack frame
uint32_t get_stack_argument(struct intr_frame *frame, unsigned int index)
{
	
  uint32_t *p = (uint32_t*)frame->esp + index + 1;

  // evaluate stack pointer before using it
  check_userland_addr ((void *)p);

  return *p;
}


void close_all_files(void)
{

	struct thread* cur_td = thread_current();
	struct list_elem* e;

	if(list_empty(&cur_td->thread_file_list))
	{
		return;
	}
	else
	{
		for(e = list_begin(&cur_td->thread_file_list); 
				e != list_end(&cur_td->thread_file_list);
				e = list_next(e))
		{
		

			struct proc_fd_list* proc_file = list_entry(e, struct proc_fd_list, elem);
			//e = list_pop_front(proc_file);
			
			if(proc_file)
			{
				if(proc_file->file)
				{
					file_close(proc_file->file);
				}
				list_remove(proc_file);
			}
			//palloc_free_page(proc_file);
		}
	}
}


struct proc_fd_list* get_process_file(int fd)
{

	struct thread* cur_td = thread_current();
	struct list_elem* e;

	if(list_empty(&cur_td->thread_file_list))
	{
		return NULL;
	}
	else
	{
		for(e = list_begin(&cur_td->thread_file_list); 
				e != list_end(&cur_td->thread_file_list);
				e = list_next(e))
		{
		
			struct proc_fd_list* proc_file = list_entry(e, struct proc_fd_list, elem);

			if(proc_file->fd == fd)
			{
				return proc_file;
			}
		}
		
		return NULL;
	}

}

