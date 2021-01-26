#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "threads/synch.h"
#include "hash.h"

#define CHILD_LOADED 			1
#define CHILD_NOT_LOADED 	0
#define CHILD_LOAD_FAILED -1
#define WAITING 					1
#define NOT_WAITING 			0
#define EXITING 					0
#define NOT_EXITING 			1
#define ALIVE 						1
#define DEAD 							0
#define ERROR 						-1
#define INITIAL_FD 				3

/* List of open files in a process */
struct proc_fd_list{
	int fd;
	struct file* file;
	struct list_elem elem;
};

/* Used for pushing arguments to the stack */
struct argument
{
	char* token;
	struct list_elem token_list_elem;
};

/* Where we store the arguments after parsing */
struct stack_data
{
	struct list argv;
	int argc;
};

struct lock filesys_lock;

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

#endif /* userprog/process.h */
