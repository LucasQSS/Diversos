#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 
#define NUM 49

//inicializa o vetor global de 100 posições atribuindo 0 a todos os elementos
int vetor[NUM] = {0};

//Função onde uma thread soma os elementos de índice par e a outra os de índice ímpar
void *Soma_Multithread (void *arg) {
	int idThread = * (int *) arg;
	for (int i = 0; i < NUM ; i+=2)
	{
		if (i + idThread < NUM)
		{
			vetor[i + idThread] += 1;
			printf("Thread %d somando 1\n", idThread);
		}
	}
  	pthread_exit(NULL);
}

int main() {
  	pthread_t tid[NTHREADS]; //identificadores das threads no sistema
  	int idThread[NTHREADS]; //variavel auxiliar


//cria threads
  	for (int i = 0; i < NTHREADS; i++)
  	{
  		idThread[i] =  i;
  		if (pthread_create(&tid[i], NULL, Soma_Multithread, (void *)&idThread[i]))
  		{
  			printf("Erro create thread\n");
  		}
  	}
  	for (int i = 0; i < NTHREADS; i++)
  	{
  		if (pthread_join(tid[i], NULL))
  		{
  			printf("erro create thread\n");
  		}
  	}
  	printf("Vetor Final:\n");
	for (int i = 0; i < NUM; i++)
	{
		printf("Conteúdo: %d ; Índice: %d \n", vetor[i], i);
	}  	
	return 0;
}