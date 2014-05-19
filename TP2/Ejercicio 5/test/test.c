#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void main(){
	char * test[] = {"/bin/rm","./hello/carola","./hello/hasta_luego",NULL};
	execve("/bin/rm", test, NULL);

	printf("Terminé!");
}