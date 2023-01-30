//Programa concorrente que calcula o valor de pi utilizando multiplas threads
//Aluno: Lucas de Queiroz

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


//inicializacao de variaveis globais
double *vetor; //vetor de somas
int nthreads; //num de threads
typedef struct
{
	int id;  //id do elemento
	long long int dim; //dimensao da entrada
} tArgs;


void *calculaPi(void *arg){
	tArgs *args = (tArgs*) arg;
	int id = args->id;
	printf("Thread %d\n", args->id);
	long long int dim = args->dim;
	vetor[id]=0;
	double denominador, numerador;
	for (long long int i = id; i < dim; i+=nthreads)
	{
		denominador = 1 + 2*i;
		numerador = 4;
		if ((i%2)==0)
		{
			vetor[id] += numerador/denominador;
			//printf("na funcao conc %lf\n", vetor[id] );
			//printf("%lld\n", i);
		}
		else
		{
			vetor[id] += -numerador/denominador;
			//printf("no else da func conc %lf\n", vetor[id] );
			//printf("i=%lld , thread = %d \n", i, id);

		}
	}
	pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
	//inicializacao das variaveis
	long long int dim;
	pthread_t *tid;
	tArgs *args;
	double inicio, fim, delta;
	double tempoConc;
	double resultado;
	double aceleracao, conc, seq;

	if (argc<3)
	{
		printf("Digite: %s <dimensao das matrizes> <numero de threads> \n", argv[0]);
		return 1;
	}

	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	if (nthreads>dim)
	{
		nthreads = dim;
	}

//alocacao de memoria para as somas
	vetor = (double*) malloc(sizeof(double)*dim);
	if (vetor==NULL)
	{
		printf("Erro malloc vetor\n");
		return 2;
	}






	GET_TIME(inicio);


//cria, executa e aguarda as threads terminarem sua execucao
	tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	if (tid==NULL)
	{
		printf("Erro malloc tid\n");
	}

	args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
	if (args==NULL)
	{
		printf("Erro malloc args\n");
	}

	for (int i = 0; i < nthreads; i++)
	{
		(args+i)->id = i;
		(args+i)->dim = dim;
		if (pthread_create((tid+i), NULL, calculaPi, (void*)(args+i)))
		{
			puts("Erro criar threads");
			return 3;
		}
	}
	for (int i = 0; i < nthreads; i++)
	{
		pthread_join(*(tid+i), NULL);
	}

	for (int i = 0; i < nthreads; i++)
	{
		resultado += vetor[i];
	}

//imprime o tempo que levou para executar o bloco concorrente
	GET_TIME(fim);
	delta = fim - inicio;
	tempoConc = delta;

	//printf("Tempo do bloco concorrente: %lf\n", delta);

	puts("Resultado concorrente:");
	printf("%.15f\n\n", resultado);

	conc = tempoConc;

	printf("Tempo de execucao concorrente: %lf\n", conc);



// naive pi sequencial
	GET_TIME(inicio);

	double denominador, numerador;
	double resultado2 = 0;
	for (long long int i = 0; i < dim; i++)
	{
		denominador = 1 + 2*i;
		numerador = 4;
		if ((i%2)==0)
		{
			resultado2 += numerador/denominador;

		}
		else
		{
			resultado2 += -numerador/denominador;
		}
	}

	puts("Resultado sequencial:");
	printf("%.15f\n\n", resultado2);

	GET_TIME(fim);
	delta = fim - inicio;
	seq = delta;
	printf("Tempo de execucao sequencial: %lf\n", seq);
	aceleracao = seq/conc;
	printf("Aceleracao com %d threads = %lf\n",nthreads, aceleracao);
	return 0;
}
