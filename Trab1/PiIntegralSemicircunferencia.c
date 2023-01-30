/* Aluno: Lucas de Queiroz Silva e Silva
DRE: 115197960                             */

// O programa a seguir aproxima o valor de pi realizando integração numérica sobre uma semicircunferência de raio 1

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


//variável compartilhada que acumula as somas dos retânguloa da função
double soma = 0.0;

//variáveis que armazenam quantidade de retângulos calculados e número de threads
int nthreads, retangulos; 

//variável de exclusão mútua para quando o programa escrever na variável compartilhada
pthread_mutex_t mutex;

//Função que calcula a área abaixo da curva
void *Area(void *tid){
    int id = *((int *)tid);
    double n = 2.0 / retangulos;
    double somaParcial = 0.0;
    double x;

//cada thread fica responsável por uma operação, pulando de acordo com seu id até calcular a área de todos os retângulos
    for (int i = id; i < retangulos; i += nthreads)
    {
        x = -1 + (i + 0.5f) * n;
        somaParcial += sqrt(1.0 - x*x) * n;
    }


//acessa a seção crítica do código
    pthread_mutex_lock(&mutex);
    printf("Sou a thread %d assumindo o controle da SC \n", id);

//soma as variáveis que acumularam as somas de cada thread na variável compartilhada
    soma += somaParcial;
    pthread_mutex_unlock(&mutex);
//sai da seção crítica do código

    return 0;
}
 

int main(int argc, char const *argv[])
{
    //inicializacao das variaveis
    double inicio, fim, delta;
    pthread_t *tid_sistema;
    int *tid;
    pthread_mutex_init(&mutex, NULL);
    
    GET_TIME(inicio);

    if (argc<3)
    {
        printf("Digite: %s <nthreads> <numero de retangulos> \n", argv[0]);
        return 1;
    }

    nthreads = atoi(argv[1]);
    retangulos = atoi(argv[2]);

//cria, executa e aguarda as threads terminarem sua execucao
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema==NULL)
    {
        printf("Erro malloc tid_sistema\n");
    }

    for (int i = 0; i < nthreads; i++)
    {
        tid = malloc(sizeof(int));
        if(tid==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
        *tid = i;
        if (pthread_create(&tid_sistema[i], NULL, Area, (void*) tid)) 
        {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }
    for (int i = 0; i < nthreads; i++)
    {
        if (pthread_join(tid_sistema[i], NULL)) 
        {
            printf("--ERRO: pthread_join()\n"); exit(-1);
        }    
    }
    soma *= 2.0;
    printf("Valor de Pi = %12.9f\n", soma );
    GET_TIME(fim);

    delta = fim - inicio;
    printf("Tempo de execução: %lf\n", delta);


    pthread_mutex_destroy(&mutex);
    return 0;
}