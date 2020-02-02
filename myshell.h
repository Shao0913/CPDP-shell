/*
 * ==============
 * Filename: myshell.h
 * Author: Yuanhang Shao
 * ==============
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_PROMPT 1024
#define MAX_LINE 4096
#define MAX_ARG 20
#define MAX_LEN 256
//#define DEBUG

struct parse_info;
struct passwd *pwd;
//struct sigaction sigact;

char *buffer;
//pid_t child, child2;
//int bg_finished;


//void type_promp(void);
void type_prompt(char *);
//void ctrlc(int);
void proc(void);
int read_command(char **,char **, char *);
int buildin_command(char *, char **);
int parsing(char **, int, struct parse_info *);
void sig_handler(int sig);

#ifndef STRUCT_PARSE_INFO
#define STRUCT_PARSE_INFO
#define BACKGROUND 			1
#define IN_REDIRECT 			2
#define OUT_REDIRECT 			4
#define OUT_REDIRECT_APPEND		8
#define IS_PIPED 			16
struct parse_info 
{
    	int flag;
	int pipe2nd;
    	char* in_file;
    	char* out_file;
	char* out_append;
	char* command;
	char** parameters;
	char* command3;
	char** parameters3;
};
#endif

