// Aluno: Lucas de Queiroz Silva e Silva

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

//variáveis globais
int x=0;
pthread_mutex_t mutex;
pthread_cond_t cond;

//Função que imprime "tudo bem?" e "bom dia!"
void *func1(void *tid){
	int id = *((int *)tid);
	if (id==1)
	{
		printf("tudo bem?\n");
		pthread_mutex_lock(&mutex);
		x++;
		pthread_mutex_unlock(&mutex);
	}

	else
	{
		printf("bom dia!\n");
		pthread_mutex_lock(&mutex);
		x++;
		pthread_mutex_unlock(&mutex);
	}

//A thread de func1 que terminar primeiro aguarda a segunda para que ambas já tenham executado o print para assim fazer a liberação do bloqueio de func2
	while (x!=2){}
	pthread_cond_broadcast(&cond);
	pthread_exit(NULL);
}

//Função que imprime "até mais!" e "boa tarde!"
void *func2(void *tid){
	int id = *((int *)tid);
	pthread_mutex_lock(&mutex);

//func2 se bloqueia até que func1 tenha terminado a execução de ambos os prints
	pthread_cond_wait(&cond, &mutex);
	if (id==3)
	{
		printf("até mais!\n");
	}
	else
	{
		printf("boa tarde!\n");
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
//Inicializa as variáveis
	int *tid;
	pthread_t *tid_sist;

	tid_sist = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
	if (tid_sist==NULL) { printf("Erro malloc tid_sistema\n"); }

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);
	

//Cria as threads chamando func1 quando o id da thread for menor que 1 e func2 quando maior	
	for (int i = 0; i < NTHREADS; i++)
    {
        tid = malloc(sizeof(int));
        if(tid==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
        *tid = i;
        if (i<2)
        {
        	if (pthread_create(&tid_sist[i], NULL, func1, (void*) tid)) 
        	{
            	printf("--ERRO: pthread_create()\n"); exit(-1);
        	}
        }
        else
        {
        	if (pthread_create(&tid_sist[i], NULL, func2, (void*) tid)) 
        	{
            	printf("--ERRO: pthread_create()\n"); exit(-1);
        	}
        }
        
    }

//Espera as threads terminarem
	for (int i = 0; i < NTHREADS; i++)
    {
        if (pthread_join(tid_sist[i], NULL)) 
        {
            printf("--ERRO: pthread_join()\n"); exit(-1);
        }    
    }

//Desaloca as variáveis
  	pthread_mutex_destroy(&mutex);
  	pthread_cond_destroy(&cond);
	return 0;
}