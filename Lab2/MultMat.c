//Programa concorrente que multiplica 2 matrizes baseado no programa que multiplica matriz e vetor visto nos videos
//Aluno: Lucas de Queiroz

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


//header das subrotinas declaradas apos main
void imprimeMatriz();
void preencheMatriz();


//inicializacao de variaveis globais
float *mat1; //matriz 1
float *mat2; //matriz 2
float *matSaida; //matriz de saida
int nthreads; //num de threads
typedef struct
{
	int id;  //id do elemento
	int dim; //dimensao da entrada
} tArgs;



/*cria uma subrotina de execucao concorrente de multiplicacao de matrizes com a estrategia de paralelizacao por linha(cada linha da matriz de saida e processada por uma thread)*/
void *multMatriz(void *arg){
	tArgs *args = (tArgs*) arg;
	printf("Thread %d\n", args->id);
	int dim = args->dim;
	for (int i = args->id; i < dim; i+=nthreads)
	{
		for (int j = 0; j < dim; j++)
		{
			for (int k = 0; k < dim; k++)
			{
				matSaida[i*dim+j] += mat1[i*dim+k]*mat2[k*dim+j];
			}
		}
	}
	pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
	//inicializacao das variaveis
	int dim;
	pthread_t *tid;
	tArgs *args;
	double inicio, fim, delta;
	double tempoSeq1, tempoSeq2, tempoConc;

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

//alocacao de memoria para as matrizes
	mat1 = (float*) malloc(sizeof(float)*dim*dim);
	mat2 = (float*) malloc(sizeof(float)*dim*dim);
	matSaida = (float*) malloc(sizeof(float)*dim*dim);
	if ((mat1==NULL) || (mat2==NULL) || (matSaida==NULL))
	{
		printf("Erro malloc matrizes\n");
		return 2;
	}

//preenche as matrizes com valores inteiros randomicos entre 0 e 29 (afim de facilitar o teste da corretude)
	preencheMatriz(mat1, dim);
	preencheMatriz(mat2, dim);
	//imprimeMatriz(mat1, dim);
	//imprimeMatriz(mat2, dim);

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
		if (pthread_create((tid+i), NULL, multMatriz, (void*)(args+i)))
		{
			puts("Erro criar threads");
			return 3;
		}
	}
	for (int i = 0; i < nthreads; i++)
	{
		pthread_join(*(tid+i), NULL);
	}


//imprime o tempo que levou para executar o bloco concorrente
	GET_TIME(fim);
	delta = fim - inicio;
	tempoConc = delta;

	printf("Tempo do bloco concorrente: %lf\n", delta);




	GET_TIME(inicio);


	//puts("Matriz Resultado:");
	//imprimeMatriz(matSaida, dim);

	GET_TIME(fim);
	delta = fim - inicio;
	tempoSeq2 = delta;

	printf("Tempo do bloco sequencial de finalizacao: %lf\n", delta);
	
	printf("Tempo sequencal total: %lf\n", (tempoSeq1 + tempoSeq2));
	printf("Tempo concorrente total: %lf\n\n\n", tempoConc);
	printf("Tempo de execucao: %lf\n", (tempoSeq1+tempoSeq2+tempoConc));
	
	return 0;
}


//funcao que da fill nas posicoes de memoria alocadas para uma determinada matriz
void preencheMatriz(float *mat, int dim){
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			mat[i*dim+j] = rand()%30; //limitarei os elementos como numeros randomicos entre 0 e 29
		}
	}
}


//funcao que imprime os valores da matriz tentando imprimir num formato amigavel ao usuario (e falhando)
void imprimeMatriz(float *mat, int dim){
	for (int i = 0; i < dim; i++)
	{
		printf("\n");
		for (int j = 0; j < dim; j++)
		{
			printf("%.lf  |", mat[i*dim+j]);
		}
	}
	printf("\n\n\n");
}