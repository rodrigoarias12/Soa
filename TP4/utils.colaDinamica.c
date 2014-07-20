#include <stdio.h>
#include <stdlib.h>
 
#define COLA_LLENA 0;
#define COLA_VACIA 0;
#define TODO_OK 0;

/*Estructura de datos*/
typedef struct s_nodo {
	void* info;
    struct s_nodo * psig;
} t_nodo;
 
typedef struct{
	t_nodo * pfrente, * pcola;
} t_cola;
 
/*Definicion de las funciones basicas*/
void crear_cola(t_cola *);
int vaciar_cola(t_cola *);
int poner_en_cola(t_cola *, void *);
int sacar_de_cola(t_cola *, void* *);
int cola_llena(const t_cola *);
int cola_vacia(const t_cola *);


/*Implementacion de las funciones*/
void crear_cola(t_cola * pc)
{
    pc->pfrente = pc->pcola = NULL;
}
int cola_vacia(const t_cola *pc)
{
    if(pc->pcola == NULL || pc->pfrente == NULL )
        return 1;
    return 0;
}
int cola_llena(const t_cola *pc)
{
    t_nodo * pn = (t_nodo *) malloc (sizeof(t_nodo));
    if(!pn)
        return 1;
    return 0;
}
int sacar_de_cola(t_cola * pc, void* *pi)
{
    t_nodo * pne;
    if(pc->pcola == NULL || pc->pfrente == NULL)
        return COLA_VACIA;
 
    pne = pc->pfrente;
    *pi = pne->info;
    pc->pfrente = pne->psig;
    free(pne);
    if(!pc->pfrente)
        pc->pcola = NULL;
    return TODO_OK;
}
int vaciar_cola(t_cola * pc)
{
    t_nodo *pne;
    if(pc->pcola == NULL || pc->pfrente == NULL)
        return COLA_VACIA;
    while(!(pc->pcola == NULL || pc->pfrente == NULL))
    {
        pne = pc->pfrente;
        pc->pfrente = pne->psig;
        free(pne);
        if(!pc->pfrente)
            pc->pcola = NULL;
    }
}
int poner_en_cola(t_cola * pc, void * pi)
{
    t_nodo *pn = (t_nodo *) malloc (sizeof(t_nodo));;
    if(!pn)
        return COLA_LLENA;
 
    pn->info = pi;
    pn->psig = NULL;
 
    if(pc->pfrente == NULL)
        pc->pfrente = pn;
    else
        pc->pcola->psig = pn;
    pc->pcola = pn;
    return TODO_OK;
 
}