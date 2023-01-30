/* Aluno: Lucas de Queiroz Silva e Silva */


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "timer.h"

int nthreads;

/* int testeThreads = 0; */

//Variáveis para medir o tempo gasto na execução */
double inicio, fim;

// Estrutura de dados para facilitar a inicialização da thread criada
typedef struct qsort_starter
{
  int *arr;
  int low;
  int high;
} quickSort_parameters;

void swap(int* a, int* b)
{
  int t = *a;
  *a = *b;
  *b = t;
}


int partition (int *arr, int low, int high, int pivot)
{
  int pivotValue = arr[pivot];
  swap(&arr[pivot],&arr[high]);
  int s = low;

  for (int i = low; i <high; i++)
  {
    if (arr[i] <= pivotValue)
    {
      swap(&arr[i], &arr[s]);
      s++;
    }
  }
  swap(&arr[s], &arr[high]);
  return s;
}


//Implementação sequencial de quicksort
void quickSort(int *arr, int low, int high)
{
  if (low < high)
  {
    int pivotPosition = low+ (high-low)/2;
    pivotPosition= partition(arr, low, high, pivotPosition);
    quickSort(arr, low, pivotPosition - 1);
    quickSort(arr, pivotPosition + 1, high);
  }
}

void concurrent_quickSort(int *arr, int low, int high);




void* worker_quickSort(void * initialValues){
  quickSort_parameters* parameters = initialValues;
  concurrent_quickSort(parameters->arr, parameters->low, parameters->high);
  return NULL;
}


//Função que decide a quantidade de threads a serem executadas paralelamente em função do valor de nthreads passado
void concurrent_quickSort(int *arr, int low, int high){
  if (low < high){

    int pivotPos = low + (high - low)/2; // define o pivot como sendo o ponto médio entre os índices do primeiro e ultimo elementos do sorting
    pivotPos = partition(arr, low, high, pivotPos);
    if (nthreads > 0){
      nthreads--;
      pthread_t thread;
      quickSort_parameters thread_param = {arr, low, pivotPos-1};
      //Cria a thread para o bloco esquerdo
      pthread_create(&thread, NULL, worker_quickSort, &thread_param);
      /* testeThreads++; */
      /* printf("Criei %d threads até agora\n", testeThreads); */

      //Chama a função original novamente para a parte direita (e avalia se será criada uma nova thread)
      concurrent_quickSort(arr, pivotPos+1, high);


      pthread_join(thread, NULL);
    } 
    //Quando chegamos ao máximo de threads definidas, executamos o quicksort concorrentemente(para execução inteiramente sequencial, passar como 0 o parâmetro das threads)
    else{
            quickSort(arr, low, pivotPos-1);
            quickSort(arr, pivotPos+1, high);
    }
  }
}


int main(int argc, char **argv){

  int maiorElemento = 100;
  int tamanhoVetor;


  if (argc<3)
    {
        printf("Digite: %s <nthreads> <tamanho do vetor> \n", argv[0]);
        return 1;
    }
  
  nthreads = atoi(argv[1]);
  tamanhoVetor = atoi(argv[2]);

  int *vetor = malloc(tamanhoVetor * sizeof(int));
/* Cria um vetor de elementos randômicos num range de 0 até o valor de "maiorElemento" */
  for (int i=0 ; i<tamanhoVetor ; i++){
    vetor[i] = rand()%maiorElemento;
  }

//  printf("Vetor inicial: \n");
//  for(int i=0; i<tamanhoVetor; i++){
//    printf(" %d ", vetor[i]);
//  }

  GET_TIME(inicio);
  concurrent_quickSort(vetor, 0, tamanhoVetor-1);
  GET_TIME(fim);


// printf("\n");
//  printf("Vetor final: \n");

//  for(int i=0; i<tamanhoVetor; i++){
//    printf(" %d ", vetor[i]);
//  }
//  printf("\n");

  double delta = fim - inicio;
  printf("Tempo de execução: %lf\n", delta);

  free(vetor);
  return 0;
}