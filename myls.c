#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

#include "myshell.h"

int main(int argc, char const *argv[])
{
	#ifdef DEBUG
		printf("the argc:%d, the argv:%s.\n", argc, argv[0]);
	#endif
	
	
 	/*struct dirent {
               ino_t          d_ino;       // Inode number 
               off_t          d_off;       // Not an offset; see below 
               unsigned short d_reclen;    // Length of this record 
               unsigned char  d_type;      // Type of file; not supported by all filesystem types
               char           d_name[256]; // Null-terminated filename
           };*/

	DIR *dirp;
	struct dirent *d;
	char path[MAX_LEN];
	
	if(argc == 1)
	{
		sprintf(path, "./%c", '\0');
		#ifdef	DEBUG
			printf("the path is :%s\n",path);
		#endif
		if((dirp = opendir(path)) == NULL)
			printf("myls: not able to get access %s: No such directory.\n",path);
		else
		{
			printf("%s:\n",path);
			while((d = readdir(dirp)) != NULL)
			{
				#ifdef DEBUG
					printf("the path: %s,the dname:%s.\n",path,d->d_name);
				#endif
				print_list(path, d->d_name);
			}
			closedir(dirp);
			printf("\n");
		}
	}

	int i = 1;
	struct stat buf2;
	int flag;
	while(i < argc)
	{
		sprintf(path, "%s%c", argv[i], '\0');
		#ifdef DEBUG
			printf("the path is: %s\n",path);
		#endif
		if(stat(path,&buf2))
			flag = 0;
		else
			flag = S_ISDIR(buf2.st_mode);
		#ifdef DEBUG
			printf("the flag is :%d\n",flag);
		#endif
		if(flag)
		{
			#ifdef DEBUG
				printf("the dir is: %s\n", path);
			#endif
			if((dirp = opendir(path)) == NULL)
			{
				printf("there is no such directory\n");
				++i;
				continue;
			}
			printf("under dir %s:\n", path);
			while((d = readdir(dirp)) != NULL)
			{
				print_list(path, d->d_name);
			}
			closedir(dirp);
			printf("\n");
			++i;
		}
		else
		{
			printf("it's not a dir.\n");
			print_list("\0",argv[i]);
			printf("\n");
			++i;
		}
	}
	return 0;
}	


int print_list(const char *p, const char *filename)
{
	struct stat buf;
	char path[MAX_LEN];

	if(strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0)
		return 0;

	if(p[0] == '\0')
		sprintf(path, "%s", filename);
	else
		sprintf(path, "%s/%s", p, filename);

	if(stat(path,&buf))
	{
		printf("No such file or directory.\n", path);
		return 0;
	}

	if (S_ISREG(buf.st_mode)) printf("-");
  	else if (S_ISDIR(buf.st_mode)) printf("d");
  	else if (S_ISCHR(buf.st_mode)) printf("c");
  	else if (S_ISBLK(buf.st_mode)) printf("b");
  	else if (S_ISFIFO(buf.st_mode)) printf("f");
  	else if (S_ISLNK(buf.st_mode)) printf("l");
  	else if (S_ISSOCK(buf.st_mode)) printf("s");
  	else printf("u");

	if(S_IRUSR & buf.st_mode) printf("r");
	else printf("-");
	if(S_IWUSR & buf.st_mode) printf("w");
	else printf("-");
	if(S_IXUSR & buf.st_mode) printf("x");
	else printf("-");
												
	if(S_IRGRP & buf.st_mode) printf("r");
	else printf("-");
	if(S_IWGRP & buf.st_mode) printf("w");
	else printf("-");
	if(S_IXGRP & buf.st_mode) printf("x");
	else printf("-");
												         
	if(S_IROTH & buf.st_mode) printf("r");
	else printf("-");
	if(S_IWOTH & buf.st_mode) printf("w");
	else printf("-");
	if(S_IXOTH & buf.st_mode) printf("x");
	else printf("-");

	printf("  ");
	printf("%2d", (int)buf.st_nlink);
	printf(" ");
	printf("%6d", buf.st_uid);
	printf(" ");
	printf("%6d", buf.st_gid);
	printf("%13d ", (int)buf.st_size);
	
	char print_buf[MAX_LEN];
	int i = 0;
	memcpy(print_buf, ctime(&buf.st_mtime)+4, strlen(ctime(&buf.st_mtime)+4));
	for(i = 0; i < strlen(print_buf); ++i)
	{
		if(print_buf[i] == '\n')
		{
			print_buf[i] = '\0';
			break;
		}
	}
	
	printf("%s", print_buf);
  	printf(" %s\n", filename);
	return 0;
	
}
