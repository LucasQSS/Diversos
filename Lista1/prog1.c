#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

//variaveis globais/compartilhadas
//numero de threads e tamanho do vetor
int nthreads, tam;
//vetor de elementos
int *vet;
//vetor de resultados
int *resultados;
//funcao executada pelas threads
void *negativos (void *tid) {
	int id = * (int *) tid;
//intervalo de elementos processados por cada thread
	int inicio, fim, j;
	int tam_bloco = tam/nthreads;
//variavel local para a qtde de negativos encontrados pela thread
	int qtde=0;
//calcula o intervalo de elementos da thread
	inicio = id * tam_bloco;
//o ultimo fluxo trata os elementos restantes
	if (id<nthreads-1) fim = inicio + tam_bloco;
	else fim = tam;
	for(j=inicio; j<fim; j++) {
		printf("sou a thread %d executando\n", id);
		if(vet[j] < 0) qtde++;
	}
//carrega o resultado parcial para o vetor de resultados
	resultados[id] = qtde;
	free(tid);
	pthread_exit(NULL);
}

void main(int argc, char const *argv[]) {

	pthread_t *tid_sistema; //vetor identificadores das threads no sistema
	int *tid; //identificadores das threads no programa
	int t; //variavel contadora
	int qtde_negativos=0; //contabiliza a qtde total de numeros negativos encontrados
	tam = atoi(argv[1]); //tamanho do vetor
	nthreads = atoi(argv[2]); //numero de threads
	//limita o numero de threads ao tamanho do vetor
	if(nthreads>tam) nthreads = tam;
	//aloca espaco para o vetor de identificadores das threads no sistema
	tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid_sistema==NULL) {
	printf("--ERRO: malloc()\n"); exit(-1);
	}
	//aloca espaco para o vetor de resultados
	resultados = (int *) malloc(sizeof(int) * nthreads);
	if(resultados==NULL) {
	printf("--ERRO: malloc()\n"); exit(-1);
	}

	//aloca espaço para o vetor de entrada e o preenche randomicamente

	vet = (int *) malloc(sizeof(int) * tam);
	srand(time(NULL));
	for (int i = 0; i < tam; i++)
	{
		vet[i] = ( rand() % 20 ) -10;
		printf("%d\n", vet[i]);
	}


	for(t=0; t<nthreads; t++) {
		tid = malloc(sizeof(int));
		if(tid==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
			*tid = t;
		if (pthread_create(&tid_sistema[t], NULL, negativos, (void*) tid)) {
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}
	//espera todas as threads terminarem e calcula o valor de saida
	for(t=0; t<nthreads; t++) {
		if (pthread_join(tid_sistema[t], NULL)) {
			printf("--ERRO: pthread_join()\n"); exit(-1);
		}
	//atualiza o valor de saida
	qtde_negativos += resultados[t];
	}

	printf("%d\n", qtde_negativos);

}