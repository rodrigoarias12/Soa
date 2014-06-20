#include "SDL/SDL.h"

typedef struct{
	char ip[30];
	int port;
	int k_up;
	int k_down;
	int k_left;
	int k_right;
}t_config_cli;

typedef struct{
  int posx;
  int posy;
}jug1, jug2;

/*Definición*/

int cargarConfigCliente(t_config_cli *);
int extraerNumero(char *);
int validarNumero(char *);
int validarNumeroIP(char *);
char * extraerNumeroIP(char *);
int extraerTecla(char *);


/*Implementación*/
int cargarConfigCliente(t_config_cli *conf)
{
	char aux[30];	
	int intAux;
	char auxChar[30];
	FILE *ARTXT;
	ARTXT = fopen("config_cliente","r+");
	if(!ARTXT)
		return 0;
	/*Leo la Ip del Servidor*/
		fscanf(ARTXT,"%s",aux);
		strcpy(auxChar, extraerNumeroIP(aux));
		if(auxChar != NULL)
		{		
			strcpy(conf->ip, auxChar); 
			printf("La IP es: %s\n",conf->ip);
		}
		else
			return 0;
	/*Leo el Puerto*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerNumero(aux);
		if(intAux)
		{				
			conf->port = intAux; 
			printf("El Puerto es: %d\n",conf->port);
		}
		else
			return 0;

	/*Leo las teclas*/
	/*Leo KEYUP*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerTecla(aux);		
		if(intAux != -1)
		{						
			conf->k_up = intAux; 
			printf("Tecla UP: %d\n",conf->k_up);
		}
		else
			return 0;
	/*Leo KEYDOWN*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerTecla(aux);
		if(intAux != -1)
		{						
			conf->k_down = intAux; 
			printf("Tecla DOWN: %d\n",conf->k_down);
		}
		else
			return 0;
	/*Leo KEYLEFT*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerTecla(aux);
		if(intAux != -1)
		{						
			conf->k_left = intAux; 
			printf("TECLA Left: %d\n",conf->k_left);
		}
		else
			return 0;
	/*Leo KEYRIGHT*/		
		fscanf(ARTXT,"%s",aux);
		intAux = extraerTecla(aux);
		if(intAux != -1)
		{						
			conf->k_right =intAux; 
			printf("TECLA Right: %d\n",conf->k_right);
		}
		else
			return 0;

	fclose(ARTXT);
	return 1;
}

int extraerNumero(char *cad)
{
	char *aux;	
	aux = strchr(cad,':');	
	if(*aux)
	{
		aux++;		
		if(validarNumero(aux))
		{			
			return atoi(aux); //retorno el valor encontrado//	
		}
	}
return -1;
}

int validarNumero(char *num)
{
	
	while(*num)
	{
		if(*num < '0' || *num > '9')	
			return 0;		
		
		num++;
	}		
	return 1;
}

int validarNumeroIP(char *num)
{
	while(*num)
	{	
		if((*num < '0' || *num > '9') && *num != '.')	
			return 0;
		
		num++;
	}		
	return 1;
}

char * extraerNumeroIP(char *cad)
{
	char *aux;	
	aux = strchr(cad,':');	
	if(*aux)
	{
		aux++;		
		if(validarNumeroIP(aux))
		{			
			return aux; //retorno el valor encontrado//	
		}
	}
return NULL;
}

int extraerTecla(char *cad)
{
	char *aux;	
	aux = strchr(cad,':');	
	if(*aux){
		aux++;
		if(strlen(aux) > 1)
			{
				if(!strcmp(aux,"ARRIBA"))
					return SDLK_UP;
				if(!strcmp(aux,"ABAJO"))
					return SDLK_DOWN;
				if(!strcmp(aux,"IZQUIERDA"))
					return SDLK_LEFT;
				if(!strcmp(aux,"DERECHA"))
					return SDLK_RIGHT;
			}
		else{
			if(*aux >= 'A' && *aux <= 'Z')
				return *aux+32;
			
			return *aux;
		}
	}
return -1;
}