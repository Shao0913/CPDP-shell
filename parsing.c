/*
 * =========
 * Filename: parsing.c
 * Author: Yuanhang Shao
 * =========
 */
#include "myshell.h"

int parse_info_init(struct parse_info *info)
{
	info->flag = 0;
	info->pipe2nd = 0;
    	info->in_file = NULL;
    	info->out_file = NULL;
	info->out_append = NULL;
    	info->command = NULL;
    	info->parameters = NULL;
    	return 0;
}

int parsing(char **parameters, int read, struct parse_info * info)
{
	int i,pipenum = 1;
	parse_info_init(info);
	if(strcmp(parameters[read-1], "&") ==0)
	{
		info->flag = info->flag | BACKGROUND;	//make flag to 1
		parameters[read-1] = NULL;
		read--;
	}
	for(i=0;i<read;)
    	{
        	if(strcmp(parameters[i],"<<")==0 || strcmp(parameters[i],"<")==0)
        	{
            		info->flag = info->flag | IN_REDIRECT;	//mark as 1*
            		info->in_file = parameters[i+1];
            		parameters[i] = NULL;
            		i+=2;
        	}
        	else if(strcmp(parameters[i],">")==0)
        	{
            		info->flag = info->flag | OUT_REDIRECT;	//mark as 1**
            		info->out_file = parameters[i+1];
            		parameters[i] = NULL;
            		i+=2;
        	}
        	else if(strcmp(parameters[i],">>")==0)
        	{
			info->out_file = parameters[i+1];
            		info->flag = info->flag | OUT_REDIRECT_APPEND;	//mark as 1***
            		info->out_append = parameters[i+1];
            		parameters[i] = NULL;
            		i+=2;
        	}
        	else if(strcmp(parameters[i],"|")==0)
        	{
            		//char* pipechar;
            		info->flag = info->flag | IS_PIPED;	//mark as 1*****
            		parameters[i] = NULL;
			if(pipenum==1)
			{
				char* pipechar;
				info->command = parameters[i+1];
				info->parameters = &parameters[i+1];
				for(pipechar = info->parameters[0]+strlen(info->parameters[0]);pipechar!=&(info->parameters[0][0]) && *pipechar!='/';pipechar--);
            			if(*pipechar == '/')
                			pipechar++;
            			info->parameters[0] = pipechar;
				pipenum = pipenum+1;
			}
			else
			{
				info->pipe2nd=1;
				char* pipechar3;
            			info->command3 = parameters[i+1];
            			info->parameters3 = &parameters[i+1];
				for(pipechar3 = info->parameters3[0]+strlen(info->parameters3[0]);pipechar3!=&(info->parameters3[0][0]) && *pipechar3!='/';pipechar3--);
            			if(*pipechar3 == '/')
                			pipechar3++;
            			info->parameters3[0] = pipechar3;
			}
            		i+=2;
        	}
        	else
            		i++;
    	}
	#ifdef DEBUG
		printf("\nbackground:%s\n",info->flag&BACKGROUND?"yes":"no");
		printf("in redirect:");
		info->flag&IN_REDIRECT?printf("yes,file:%s\n",info->in_file):printf("no\n");
		printf("out redirect:");
		info->flag&OUT_REDIRECT?printf("yes,file:%s\n",info->out_file):printf("no\n");
		printf("out append redirect:");
		info->flag&OUT_REDIRECT_APPEND?printf("yes, file:%s\n",info->out_append):printf("no\n");
		printf("pipe:");
		info->flag&IS_PIPED?printf("yes,command:%s %s %s\n",info->command,info->parameters[0],info->parameters[1]):printf("no\n");
		printf("2n pipe:");
		info->pipe2nd?printf("yes for second pipe,command3:%s %s %s\n",info->command3,info->parameters3[0],info->parameters3[1]):printf("no\n");
	#endif
    	return 1;
}

