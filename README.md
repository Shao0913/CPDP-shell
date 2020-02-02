Project 1: A Simplified Unix Shell Program

Author: Yuanhang Shao 

Achieved features:
 - This shell support four built-in commands "exit", "cd", and "pwd", "set", and "help"
 1. exit: terminating myshell. 		
	Command example: exit
 2. cd: change the current working directory. (support absulote path and relative path, "~") 		
	Command example: cd /home, cd dir, cd ~
 3. pwd:  print the absolute path of the current working directory		
	Command example: pwd
 4. set: set the value of the environment variables.			
	Command example: set MYPATH=path1:path2
 5. help: print welcome message			
	Command example: help
 - This shell will terminate if CTRL-D is encountered.
 - I/O redirection (it can support ">>",">","<", and the combination of these three) 
	Command example: command < file, command > file, command >> file, command < file1 > file2, command < file1 >> file2
 - Pipeline is working, but it only support 2 pipe lines. It can support 2 or 3 commands.		
	Command example: command1 | command2, command1 | command2 | command3
 - I/O redirection and pipeline is support limited in this shell.  		
	Command example:command < file | command2
 - Background processes is supported. (There will be some print text during running at background)
	Command example: command &
 - Myls external command is include in project.(It can accept empty dir or relative path)
	Command example: myls, myls dir

Bugs and Limitation
 - Pipe line only support 2 command line. It will work for most of command. 
	e.g. cat temp | head -7 | tail -5,ls | more | wc,ls | more,ls | head -2,grep xxx < temp | head -2,
		grep xxx < temp | head -5 | tail -2,
 - This shell can support pipiline and background and redirect in one command line, but this feature is not well tested. It may have some unknow bugs.


Building
 - Simply type "make". It will generate two execuatable file. myshell for shell and myls for external command.
 - Clear the build. Simply type "make clean".

File list
 - myshell.h
	header file
 - myshell.c
	main program
 - type_prompt.c
	type_prompt function, print prompt of shell
 - read_command.c
	read_command function, read command from terminal
 - parsing.c
	parsing function, parsing the command that read from terminal
 - buildin_command.c
	buildin_command function, including all the buildin command
 - myls.c
	myls main program, external command myls


Here is some command that I used to test my shell: (attention: redirection, pipeline, and background need space between command, parameter, and symbol)
ls,ls -l,rm temp,vim temp,cd myshell,cd /home/grads/shao/,cd CPDP/project1,cd ~,
pwd,exit,set MYPATH=/home/grads/shao/CPDP,help,
ls > temp,ls -l >> temp,grep shell < temp,grep shell < temp >> temp2,
ls | more,grep shell < temp | more,bg_test & | ls,(attention:background can only use at first command of pipeline.)
bg_test &,(attention:bg_test is my background test program which will sleep for longtime.)
cat temp | head -7 | tail -5,ls | more | wc,ls | more,ls | head -2,grep xxx < temp | head -2,
grep xxx < temp | head -5 | tail -2,
