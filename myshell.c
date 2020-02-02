/* 
 * ====================
 * Filename: myshell.c
 * Author: Yuanhang Shao
 * Class: CPDP
 * Created: 2019.10.05
 * ====================
 */
#include "myshell.h"
#define TRUE 1
#define FALSE 0
#define MAXPIDTABLE 1024

pid_t Table[MAXPIDTABLE];

void sig_handler(int sig)
{
	#ifdef DEBUG
		printf("enter the sig handler with %d\n",sig);
	#endif
	/*pid_t pid_sig;
	while ((pid_sig = waitpid(-1,NULL,WNOHANG))>0)
	{
		#ifdef DEBUG
			printf("the pid_sig is:%ld\n",(long)pid_sig);
		#endif
		printf("background process %d finished.\n",pid_sig);
		//bg_finished = 1;
		//child = pid_sig;
	}
	//else printf("received sig = %d \n", sig);
	return;*/

	pid_t pid;
    	int i;
    	for(i=0;i<MAXPIDTABLE;i++)
        	if(Table[i] != 0) //only handler the background processes
        	{
            		pid = waitpid(Table[i],NULL,WNOHANG);
            		if(pid > 0)
            		{
                		printf("Background process %d finished.\n",pid);
                		Table[i] = 0; //clear
            		}
            		else if(pid < 0)
            		{
				if(errno != ECHILD)
                    			perror("waitpid error");
            		}
            //else:do nothing.
            //Not background processses has their waitpid() in wshell.
	}
	return;
}

void proc(void)
{
	int read,status;
	char *command = NULL;
	char **parameters;
	char prompt[MAX_PROMPT];
	parameters = malloc(sizeof(char *) * (MAX_ARG+2));
	buffer = malloc(sizeof(char) * MAX_LINE);
	pid_t child,child2,child3;
	struct parse_info info;
	if (parameters == NULL || buffer == NULL)
	{
		printf("myshell error: malloc failed. \n");
		return;
	}
	if(signal(SIGCHLD, sig_handler) == SIG_ERR)
		perror("signal() error");
	//child = -255;
	//bg_finished = 0;
	//sigact.sa_handler = sig_handler;
	//sigact.sa_flags = 0;
	//sigaction(SIGCHLD,&sigact,0);
	while(TRUE)
	{
		int pipe_fd[2], in_fd, out_fd;
		//printf("before type_prompt the prompt: %s \n", prompt);	
		type_prompt(prompt);
		//signal(SIGUSR1, ctrlc);
		read = read_command(&command, parameters, prompt);
		if(-1 == read)
			continue;
		read--;
		/*int j=0;
		for(j=0;j<sizeof(*parameters);j++)
			printf("befor parsing the parameters, read, info:%s, %d /n",*parameters[j],read);*/
		parsing(parameters,read,&info);
		/*if(fork() != 0)	//fork off child process
 		{
			//parent code
			waitpid(-1, &status, 0);	//wait for child to exit
		}
		else 
		{
			//child code
			execve(command, parameters, 0);	//execute command
		}*/
		int flag_build = 2;
		flag_build = buildin_command(command, parameters);
		#ifdef DEBUG
			printf("buildin_command return: %d\n",flag_build);
		#endif
		if(flag_build)
			continue;
		if(info.flag & IS_PIPED)
		{
			if(pipe(pipe_fd) < 0)
			{
				printf("myshell error: pipe failed. \n");
				exit(0);
			}
		}
		if((child = fork())!=0)	//parent
		{
			#ifdef DEBUG
				printf("this is child fork, parent, and wait child.\n");
			#endif
			if(info.flag & IS_PIPED)
			{
				if((child2=fork())!=0)	//parent2
				{
					#ifdef DEBUG
                                		printf("this is child2 fork, parent2.\n");
                        		#endif
					if(info.pipe2nd == 1)
					{
						if((child3=fork())!=0)  //parent3
						{
							#ifdef DEBUG
								printf("this is child3 fork,parent3.\n");
							#endif
							close(pipe_fd[1]);
							close(pipe_fd[0]);
							waitpid(child3,&status,0);
						}
						else   //child3
						{
							#ifdef DEBUG
								printf("this is child3 fork,child3,execute command3.\n");
							#endif
							close(pipe_fd[1]);
							close(fileno(stdin));
							dup2(pipe_fd[0],fileno(stdin));
							close(pipe_fd[0]);
							execvp(info.command3,info.parameters3);
							//waitpid(child3,&status,0);
						}
						/*close(pipe_fd[1]);
                    				close(pipe_fd[0]);*/
						//waitpid(child2,&status,0);
					}
					else
					{
						#ifdef DEBUG
							printf("there is no 3rd command\n");
						#endif
						close(pipe_fd[1]);
						close(pipe_fd[0]);
						waitpid(child2,&status,0);
					}
				}
				else		//child2
				{
					#ifdef DEBUG
                                		printf("this is child2 fork, child2,do command2.\n");
                        		#endif
                    			close(pipe_fd[1]);
					close(fileno(stdin));
					dup2(pipe_fd[0],fileno(stdin));
					close(pipe_fd[0]);
					execvp(info.command,info.parameters);
					//waitpid(child2,&status,0);
				}
			}

			if(info.flag & BACKGROUND)
			{
				printf("Child pid:%u\n",child);
                		int i;
                		for(i=0;i<MAXPIDTABLE;i++)
                    			if(Table[i]==0)
					{
						#ifdef DEBUG2
							printf("the table index contain child is : %d\n",i);
						#endif
                        			Table[i] = child;
					}
                		/*if(i==MAXPIDTABLE)
                    			perror("Too much background processes\nThere will be zombine process");*/
				
			}
			else
			{
				#ifdef DEBUG
                                	printf("this is child fork, parent waiting.\n");
                        	#endif
				waitpid(child, &status, 0);	
			}
		}
		else		//child
		{
			#ifdef DEBUG
                                printf("this is child fork, child.\n");
                        #endif
			if(info.flag & IS_PIPED) //command2 is not null
            		{                
                		if(!(info.flag & OUT_REDIRECT) && !(info.flag & OUT_REDIRECT_APPEND)) // ONLY PIPED
	           		{
					#ifdef DEBUG
                                		printf("this is child fork, child, only piped.\n");
                        		#endif
                    			close(pipe_fd[0]);
                    			close(fileno(stdout)); 
                    			dup2(pipe_fd[1], fileno(stdout));
                    			close(pipe_fd[1]);
                		}
                		else //OUT_REDIRECT and PIPED
	           		{
					#ifdef DEBUG
                                		printf("this is child fork, child, out_redirect and piped.\n");
                        		#endif
                   	 		close(pipe_fd[0]);
                    			close(pipe_fd[1]);//send a EOF to command2
                    			if(info.flag & OUT_REDIRECT)
    	               				out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    			else
    	               				out_fd = open(info.out_file, O_WRONLY|O_APPEND|O_TRUNC, 0666);
                    			close(fileno(stdout)); 
                    			dup2(out_fd, fileno(stdout));
                    			close(out_fd);	        
                		}
            		}
            		else
            		{
				#ifdef DEBUG
                                	printf("this is child fork, child, without piped.\n");
                        	#endif
                		if(info.flag & OUT_REDIRECT) // OUT_REDIRECT WITHOUT PIPE
	           		{
                    			out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    			close(fileno(stdout)); 
                    			dup2(out_fd, fileno(stdout));
                    			close(out_fd);
                		}
                		if(info.flag & OUT_REDIRECT_APPEND) // OUT_REDIRECT_APPEND WITHOUT PIPE
	           		{
					
                    			#ifdef DEBUG
						int flag = O_WRONLY|O_TRUNC|O_CREAT|O_APPEND;
						printf("the info:%s, the flag: %d.\n",info.out_append, flag);
					#endif
					out_fd = open(info.out_append, O_WRONLY|O_CREAT|O_APPEND, 0666);
                    			close(fileno(stdout)); 
                    			dup2(out_fd, fileno(stdout));
                    			close(out_fd);
                		}
            		}
            
            		if(info.flag & IN_REDIRECT)
            		{
                		in_fd = open(info.in_file, O_CREAT |O_RDONLY, 0666);
                		close(fileno(stdin)); 
                		dup2(in_fd, fileno(stdin));
                		close(in_fd); 
            		}
			#ifdef DEBUG
				printf("this is execvp\n");
				printf("command is:%s, parameters: %s\n",command,parameters);
			#endif	
			execvp(command, parameters);
			/*if(info.flag & IS_PIPED)
			{
				close(fileno(stdout));
			}*/
		}
	}
	free(parameters);
	free(buffer);
}


int main(){
	int i;
	for(i=0; i<MAXPIDTABLE; i++)
		Table[i] = 0;
	proc();
	return 0;
}
