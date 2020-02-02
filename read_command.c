/*
 * ===============
 * Filename: read_command.c
 * Author: Yuanhang Shao
 * ==============
 */

#include "myshell.h"
#ifdef READLINE_ON
#include <readline/readline.h>
#include <readline/history.h>
#endif

//return value: number of parameters
//0 represents only command without any parameters
//-1 represents wrong input

int read_command(char **command,char **parameters,char *prompt)
{
	#ifdef READLINE_ON
    		free(buffer);
    		buffer = readline(prompt);
    		if(feof(stdin)) 
		{
        		printf("\n");
        		exit(0);
    		}
	#else
    		printf("%s",prompt);
    		char* line = fgets(buffer,MAX_LINE,stdin);
    		if(line == NULL)
    		{
        		printf("\n");
        		exit(0);
    		}		
	#endif	

	if(buffer[0] == '\0')
        	return -1;

	char *start,*end;
	int count = 0;
	int isFinished = 0;
	start = buffer;
	end = buffer;
	
	while(isFinished == 0)
	{
		
		
		//start with letters ignore space or tab
		while((*end == ' ' && *start == ' ') || (*end == '\t' && *start == '\t'))
		{
			start++;
			end++;
		}
		//if no input
		if(*end == '\0' || *end == '\n')
		{
			if(count == 0)
				return -1;
			break;
		}


		//end with space tab or enter add end pointer
		while(*end != ' ' && *end != '\0' && *end != '\n')
			end++;
		//first command read
		if(count == 0)
        	{
            		char *p = end;
            		*command = start;
            		while((p!=start) && (*p !='/'))
                		p--;
            		if(*p == '/')
                		p++;
                        parameters[0] = p;
                        count += 2;

    			#ifdef DEBUG
                        	printf("\ncommand:%s\n",*command);
                        #endif
		}
		else if(count <= MAX_ARG)
        	{
            		parameters[count-1] = start;
           	 	count++;
        	}
        	else
        	{
            		break;
        	}

        	if(*end == '\0' || *end == '\n')
        	{
            		*end = '\0';
            		isFinished = 1;
        	}
        	else
        	{
            		*end = '\0';
            		end++;
			start = end;
     		}
	}
	parameters[count-1] = NULL;

	#ifdef DEBUG
		/*input analysis*/
	    	printf("input analysis:\n");
    		printf("pathname:[%s]\ncommand:[%s]\ncount:%d\nparameters:\n",*command,parameters[0],count);
    		int i;
    		for(i=0;i<count-1;i++)
        		printf("[%s]\n",parameters[i]);
	#endif

    	return count;
}
	

