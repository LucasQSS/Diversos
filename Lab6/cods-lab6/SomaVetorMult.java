/* Autor: Lucas de Queiroz */
/* Codigo: Somar 2 vetores concorrentemente */
/* -------------------------------------------------------------------*/

import java.io.*; 
import java.util.*; 


//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   //identificador da thread
  private int id;
  private Vector<Integer> vet;
  private Vector<Integer> res;

   //construtor
   public T(int tid, Vector<Integer> vet, Vector<Integer> res) { 
      this.id = tid;
      this.vet = vet;
      this.res = res;
   }

   //metodo main da thread
   public synchronized void run() {
      System.out.println("Thread " + this.id + " iniciou!");
      for (int i = 0; i < vet.size(); i++) {
    	  int aux = res.get(i);
    	  res.set(i, aux + vet.get(i));
      }
      System.out.println("Thread " + this.id + " terminou!"); 
   }
}

//classe da aplicacao
class SomaVetorMult {
	
   //variaveis de numero de threads e tamanho do vetor, respectivamente
   static final int nthreads = 2;
   static final int tamanho = 100;


   public static void main (String[] args) {
      //reserva espaco para um vetor de threads
      Thread[] threads = new Thread[nthreads];

      //cria o vetor de resultados
      Vector<Integer> res = new Vector<Integer>();
      
      //cria os vetores a e b preenchendo seus valores com vetor[i] = vetor[i-1] + 1
      Vector<Integer> a = new Vector<Integer>();
      Vector<Integer> b = new Vector<Integer>();
      for (int i=0; i<tamanho; i++) {
          a.add(i);
          b.add(i);
          res.add(0);
       }
      System.out.println("Valor inicial de A " + a); 
      System.out.println("Valor inicial de B = " + b); 
      System.out.println("Valor inicial do vetor soma = " + res); 

      //cria as threads da aplicacao
      for (int i=0; i<threads.length; i++) {
        if (i==0){
          threads[i] = new T(i, a, res);
        }
        else{
          threads[i] = new T(i, b, res);
        }
      }

      //inicia as threads
      for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }

      //espera pelo termino de todas as threads
      for (int i=0; i<threads.length; i++) {
         try { threads[i].join(); } catch (InterruptedException e) { return; }
      }

      System.out.println("Valor final do vetor soma = " + res); 
   }
}
