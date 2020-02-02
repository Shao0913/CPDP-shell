/*
 * ===========
 * Filename: buildin_command.c
 * Author: Yuanhang Shao
 * ===========
 */

#include "myshell.h"

int buildin_command(char *command, char **parameters)
{
	extern struct passwd *pwd;
	if(strcmp(command, "exit") == 0)
	{
		printf("\n");
		exit(0);
	}
	else if(strcmp(command, "help") == 0)
	{
		printf("This is myshell for CPDP created by Yuanhang Shao. \n");
		return 1;
	}
	else if(strcmp(command, "cd") == 0)
	{
		char *path = NULL;
		if(parameters[1] == NULL)
		{
			parameters[1] = malloc(3*sizeof(char));
			parameters[1][0] = '.';
			parameters[1][1] = '.';
			parameters[1][2] = '\0';
		}
		if(parameters[1][0] == '~')
		{
			path = malloc(strlen(pwd->pw_dir)+strlen(parameters[1]));
            		//'~' makes length 1 more,but instead of '\0'
            	 	if(path == NULL)
                        {
                        	printf("cd:malloc failed.\n");
			}
                        strcpy(path,pwd->pw_dir);
            		strncpy(path+strlen(pwd->pw_dir),parameters[1]+1,strlen(parameters[1]));
			//printf("path with ~:\n%s\n",path);
		}
		else
		{
			path = malloc(strlen(parameters[1]+1));
			if(path == NULL)
			{
				printf("cd failed to malloc. \n");
			}
			strcpy(path, parameters[1]);
		}
		if(chdir(path) != 0)
			printf("there is no such dir: %s:%s \n", path, strerror(errno));
		free(path);
		return 1;
	}
	else if(strcmp(command, "pwd") == 0)
	{
		char *path;
		char pwd[MAX_LEN];
		path = getcwd(pwd, MAX_LEN);
		if(path == NULL)
			printf("error! Couldn't getcwd! \n");
		else
		{
			printf("pwd is: %s \n", path);
			return 1;
		}
	}
	else if(strcmp(command, "set") == 0)
	{
		if(parameters[1] == NULL)
			printf("Syntax: set MYPATH=env1:env2.\n");
		
		char *path=parameters[1];
		int flag=0;
		while(*path != '\0')
		{
			if(*path == '=')
			{
				flag=1;
				break;
			}
			++path;
		}
		if(!flag)
		{
			printf("Syntax:set MYPATH=env1:env2.\n");
			return 0;
		}
		char *env_mark;
		char *env_path;
		env_mark = strtok(parameters[1], "=");
		env_path = strtok(NULL, "=");

		if(strcmp(env_mark, "MYPATH"))
		{
			printf("Syntax: set MYPATH=env1:env2. \n");
			return 0;
		}
		
		if(!strncmp(env_path, "$MYPATH", strlen("$MYPATH")))
		{
			env_path = env_path + strlen("$MYPATH");
			if(*env_path == ':')
				++env_path;
		}
		char buf[MAX_PROMPT];	//1024
		char *old_env = getenv("MYPATH");
		if(old_env == NULL)
			sprintf(buf, "%s\0", env_path);
		else	
			sprintf(buf, "%s:%s\0", old_env, env_path);
		
		if(setenv("MYPATH", buf, 1))
			printf("unknow error: set env is failed. \n");
		return 1;
	}
	return 0;
}
