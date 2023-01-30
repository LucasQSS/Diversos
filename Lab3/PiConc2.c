//Programa concorrente que calcula o valor de pi utilizando multiplas threads
//Aluno: Lucas de Queiroz

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

double pi();



//inicializacao de variaveis globais
double *vetor; //vetor de entrada
int nthreads; //num de threads
long long int dim;


/*cria uma subrotina de execucao concorrente de multiplicacao de matrizes com a estrategia de paralelizacao por linha(cada linha da matriz de saida e processada por uma thread)*/
void *calculaPi(void *arg){
	long long int id = (long long int) arg;
	double *somaLocal;
	long long int tamBloco = dim/nthreads;
	long long int ini = id*tamBloco;
	long long int fim;

	somaLocal = (double *) malloc(sizeof(double));
	
	if(somaLocal==NULL){ fprintf(stderr, "Erro malloc soma\n\n", ); }

	*somaLocal=0;
	if (id==nthreads-1){ fim = dim; }
	else { fim = ini + tamBloco; }
	for (long long int i = 0; i < fim; i++){ *somaLocal+=vetor[i]; }
	pthread_exit((void *) somaLocal);

}

int main(int argc, char const *argv[])
{
	//inicializacao das variaveis
	long long int dim;
	pthread_t *tid;
	tArgs *args;
	double inicio, fim, delta, *retorno;
	double tempoSeq1, tempoSeq2, tempoConc;
	double somaConc;

	GET_TIME(inicio);

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

	vetor = (double*) malloc(sizeof(double)*dim);
	if (vetor==NULL)
	{
		printf("Erro malloc vetor\n");
		return 2;
	}



	for (int i = 0; i < dim; ++i)
	{
		vetor[i] = 0;
	}

	GET_TIME(fim);
	delta = fim - inicio;
	tempoSeq1 = delta;

	printf("Tempo do primeiro bloco sequencial: %lf\n", delta);




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

	for (int i = 1; i < nthreads; ++i)
	{
		vetor[0] += vetor[i];
	}

//imprime o tempo que levou para executar o bloco concorrente
	GET_TIME(fim);
	delta = fim - inicio;
	tempoConc = delta;

	printf("Tempo do bloco concorrente: %lf\n", delta);




	GET_TIME(inicio);


	puts("Resultado:");
	printf("%lf\n\n", vetor[0]);;

	GET_TIME(fim);
	delta = fim - inicio;
	tempoSeq2 = delta;

	printf("Tempo do bloco sequencial de finalizacao: %lf\n", delta);
	
	printf("Tempo sequencal total: %lf\n", (tempoSeq1 + tempoSeq2));
	printf("Tempo concorrente total: %lf\n\n\n", tempoConc);
	printf("Tempo de execucao: %lf\n", (tempoSeq1+tempoSeq2+tempoConc));
	
	return 0;
}

double pi(long long int id){
	double ret;
	if ((id%2)==0){
		ret = 4*(1/(1+2*id));
	}
	else{
		ret = -4*(1/(1+2*id));
	}
	return ret;
}