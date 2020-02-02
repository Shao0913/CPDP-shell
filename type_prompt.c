/*
 * ===============
 * Filename: type_prompt.c
 * Author: Yuanhang Shao
 * ===============
 */

#include "myshell.h"
const int max_name_len = 256;
const int max_path_len = 1024;

void type_prompt(char *prompt)
{
	//printf("this is type_prompt \n");
	extern struct passwd *pwd;
	char pathname[max_path_len];
	char hostname[max_name_len];
	int length;
	pwd = getpwuid(getuid());
	getcwd(pathname, max_path_len);
	if(gethostname(hostname, max_name_len) == 0)
		sprintf(prompt, "[myshell]%s@%s:", pwd->pw_name, hostname);
	else
		sprintf(prompt, "[myshell]%s@unknow:", pwd->pw_name);
	
	length = strlen(prompt);
    	if(strlen(pathname) < strlen(pwd->pw_dir) || strncmp(pathname,pwd->pw_dir,strlen(pwd->pw_dir))!=0)
        	sprintf(prompt+length,"%s",pathname);
    	else
        	sprintf(prompt+length,"~%s",pathname+strlen(pwd->pw_dir));

	//printf("$");
    	length = strlen(prompt);
    	if(geteuid()==0)
        	sprintf(prompt+length,"#");
    	else
        	sprintf(prompt+length,"$");
    	return;

}


/*void type_prompt(void)
{
        write(STDOUT_FILENO, "$ ",2);
}*/

void ctrlc(int signum)
{
        (void)signum;

        write(STDOUT_FILENO, "Ctrl+c can't end me, try exit!\n$ ", 3);
}

