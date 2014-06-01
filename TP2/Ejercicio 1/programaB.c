#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int errorMessage(const char *msg);

int main(int argc, char *argv[]) {
	pid_t pid;
	if(argc<2)
		return errorMessage("Debe ingresar parametros");
	
	if(atoi(argv[1])<0)
		return errorMessage("Error en la cantidad de procesos a crear");

	for(int x=0;x<atoi(argv[1]);x++) {
		pid = fork();
		if (pid==-1)
			return errorMessage("Error en la creaciÃ3n del proceso");
		else
			if (pid==0) {
				printf("soy el proceso hijo con pid %d\n", getpid());
				sleep(1);
			}
		else {
			printf("Soy el proceso padre %d con padre %d \n",getpid(), getppid());
			break;
		}
	}

	for(int x=0;x<atoi(argv[1]);x++)
	wait(NULL);
}

int errorMessage(const char *msg) {
	printf("%s\n",msg);
	return 1;
}
