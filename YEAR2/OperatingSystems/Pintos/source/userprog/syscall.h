#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "hash.h"
#include "lib/user/syscall.h"
#include "threads/interrupt.h"

#define NOT_LOADED 0
#define SUCCESSFUL_LOAD 1
#define FAILED_LOAD 2


/* Debug Logging */
//#define DEBUG true

#ifdef DEBUG
#define DEBUG_PRINT(...)                       \
    do {                                    \
             printf(__VA_ARGS__);           \
    } while(0)
#else
#define	DEBUG_PRINT(...) \
		while(0)
#endif


/* Stdio defines */
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// For Bounds Checking
#define USER_VIRTUAL_ADDR_BOTTOM 0x8048000

#define MAX_ARGS 5



// Stack Pointer
uint32_t *esp;

/* Syscall Handler */
void syscall_init (void);
static void syscall_handler (struct intr_frame *frame);

/* Helper functions */
static int new_fd(void);
static void check_userland_addr (const void *ptr);
static void check_userland_buffer (void *buffer, size_t size);
uint32_t get_stack_argument(struct intr_frame *frame, unsigned int index);
void close_all_files(void);
struct proc_fd_list* get_process_file(int fd);
int hexDump(const void *data, int size);


/* System calls */
void sys_exit(int status);
pid_t sys_exec(const char* cmd);
int sys_wait(pid_t pid);
int sys_create(const char* filename, unsigned size); 
int sys_remove(const char* filename);
int sys_open(const char* filename);
int sys_filesize(int fd);
int sys_read(int fd, char* buffer, unsigned size);
int sys_write(int fd, const void* buffer, unsigned size);
void sys_seek(int fd, unsigned pos);
int32_t sys_tell(int fd);
int sys_close(int fd);
void sys_munmap(struct init_frame *frame UNUSED);
void sys_mmap(struct init_frame *frame UNUSED);

#endif /* userprog/syscall.h */
