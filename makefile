all: myshell myls

myshell: myshell.h myshell.c type_prompt.c read_command.c parsing.c buildin_command.c
	gcc myshell.c type_prompt.c read_command.c parsing.c buildin_command.c -o myshell

myls: myls.c myshell.h
	gcc myls.c -o myls

clean:
	rm -f myshell myls
