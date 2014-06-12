/*#####################################
#EJERCICIO1
#Trabajo Práctico Nº3
#Ejercicio Número 1
#Arias, Rodrigo DNI: 34.712.865
#Culen, Fernando DNI: 35.229.859
#García Alves, Pablo DNI: 34.394.775
#Juffar, Sebastian DNI: 34.497.148
#Nogueiras, Jorge DNI: 34.670.613
#PRIMERA ENTREGA
#####################################*/
#include"mesas.h"

int sem;int Id_Memoria;
	int *Memoria = NULL;
//Manejador  para borrar los semaforos cuando se pasa CTRL-C
void handler(int sig)
{	borrar_semaforo(sem);
        shmdt ((char *)Memoria);
	shmctl (Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);
	exit(0);
}

int main(int argc ,char * argv[])
{	

validacion(argc,argv);
	key_t Clave;	
	int *cant=NULL;
int llave,status1,status2,status3,status4,status5;
	pid_t hijo1,hijo2,hijo3,hijo4,hijo5;
	int a=0;
	int b=0;
	int c=0;
	int d=0;
	int e=0;

	Clave = ftok ("/bin/ls", 33);
	if (Clave == -1)
	{
		printf("No consigo clave para memoria compartida");
		exit(0);
	}
	Id_Memoria = shmget (Clave, sizeof(int)*5, 0777 | IPC_CREAT);
	if (Id_Memoria == -1)
	{
		printf("No consigo Id para memoria compartida");
		exit (0);
	}
	Memoria = (int *)shmat (Id_Memoria, (char *)0, 0);
	if (Memoria == NULL)
	{
		printf("No consigo memoria compartida");
		exit (0);
	}
      	

	
	signal(2,handler);
 
	
	// Se crea la llave del semaforo
	llave = ftok("/bin/ls",33);
	if(llave<0)
	{	perror("No se pudo crear la llave\n");
		exit(1);
	}
	
	// Se crean los semaforos
	if((sem = crear_semaforo(llave,3))<0)
	{	perror("No se pudo crear los semaforos\n");
		exit(1);
	}
	
	// Se inicializa los tres semaforos
	inicializar_semaforo(sem,0,1);//Semaforo 1
	inicializar_semaforo(sem,1,0);//Semaforo 2
	inicializar_semaforo(sem,2,0);//Semaforo 3
	printf("Presione CTRL-C para terminar\n");
       if(hijo1=fork()==0)
	{	while(1)
		{	P(sem,0);
			if(Memoria[0] < 2)
			{       if(atoi(argv[1])==Memoria[4]) 
                               {
                               exit(0); 
                               }
                                a++;
			        Memoria[0]++;
				printf("Fabricación del marco: %d \n",a);
                                printf("Total de marcos en stock:%d\n",Memoria[0]);
				usleep(500000);
                             
                               
			}
			else
				{  if(atoi(argv[1])==Memoria[4]) 
                                  {
                                  exit(0); 
                                   }
					
				}
		       V(sem,1);
		}
	}
	if(hijo2=fork()==0)
	{	while(1)
		{	P(sem,1);
		        if(Memoria[1] < 6)
			{  if(atoi(argv[1])==Memoria[4]) 
                               {
                               exit(0); 
                               }
                                b++;
                                Memoria[1]++;
				printf("Fabricación de la pata: %d \n",b);
		                printf("Total de patas en stock:%d\n",Memoria[1]); 
				usleep(200000);
                              
			}
			else
				{
                               if(atoi(argv[1])==Memoria[4]) 
                               {
                                exit(0); 
                               }
				
				}			
			V(sem,1);V(sem,0);
		}	
	}		
	if(hijo3=fork()==0)
	{ 	while(1)
		      { 
                        if(Memoria[1] >3 && Memoria[0]>0 && Memoria[2]<2)
			{    if(atoi(argv[1])==Memoria[4]) 
                               {exit(0); 
                               } 
                             //vidrios
                              c++;
                                Memoria[2]++;
                                Memoria[0]--;//marcos
                                Memoria[1]=Memoria[1]-4;//patas
		                printf("Terminación de ensamblaje de vidrio y patas de la mesa número:%d\n",c);        
                                printf("Total de marcos en stock:%d\n",Memoria[0]);
                                printf("Total de patas en stock:%d\n",Memoria[1]);                          
				sleep(2);
                            
                            
			}
                         else 
                        {  if(atoi(argv[1])==Memoria[4]) 
                               {
                                exit(0); 
                               }
                       
                        }
		      
                      }	
	}		

	if(hijo4=fork()==0)
	{ 	while(1)
		      { 
                        if(Memoria[2] >0 && Memoria[3]<1)
			{      if(atoi(argv[1])==Memoria[4]) 
                               {
                                printf("Ensamblados %d",Memoria[3]);exit(0); 
                               }
                                d++;   //cajones
                                Memoria[3]++;  //sumo emsamblaje
                                Memoria[2]--; //resto ensamblado de marco y vidrio
		                printf("Terminación de ensamblaje de los cajones de la mesa:%d\n",d);     
				sleep(2);  
                           
			}
                         else 
                        {
                         if(atoi(argv[1])==Memoria[4]) 
                               {
                                exit(0); 
                               }
                        }
		      
                      }	
	}		
if(hijo5=fork()==0)
	{ 	while(1)
		      { 
                        if(Memoria[3] >0)
			{     
                                Memoria[4]++; //sumo mesa
                                Memoria[3]--; //Resto stock de mesa con cajas ensambladas
		                printf("Terminación final de la mesa nro.:%d\n",Memoria[4]);        
				if(atoi(argv[1])==Memoria[4]) 
                               {
                                printf("Mesas terminadas %d\n",Memoria[4]);exit(0); 
                               }else{
                             sleep(9);
                                }
			}
                         else 
                        {
                        }
		      
                      }	
	}	


	wait(NULL);wait(NULL);wait(NULL);wait(NULL);wait(NULL);
	
        //Terminada de usar la memoria compartida, la liberamos.
        borrar_semaforo(sem);	
	shmdt ((char *)Memoria);
	shmctl (Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);	
	return 0;
		


}

