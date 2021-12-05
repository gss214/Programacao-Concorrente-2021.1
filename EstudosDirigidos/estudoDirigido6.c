#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define PR 1 //número de produtores
#define CN 1 // número de consumidores
#define N 5  //tamanho do buffer

void * produtor(void * meuid);
void * consumidor (void * meuid);

pthread_mutex_t lock_contador = PTHREAD_MUTEX_INITIALIZER; //lock para o buffer
sem_t vazio; 
sem_t ocupado;

int buffer = 0;


void main(argc, argv)
int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;
  
  //inicializa semaforos
  sem_init(&vazio,0,N); 
  sem_init(&ocupado,0,0);

  pthread_t tid[PR];
   
  for (i = 0; i < PR; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, produtor, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  pthread_t tCid[CN];

  for (i = 0; i < CN; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tCid[i], NULL, consumidor, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
 
  pthread_join(tid[0],NULL);

}

void * produtor (void* pi)
{

    while(1)
    {
        sem_wait(&vazio); //remove 1 do semaforo vazio para indicar que vai incluir um item no buffer
        pthread_mutex_lock(&lock_contador); //lock para controlar o contador
            buffer++;
            printf("Produtor está produzindo  buffer = %d\n", buffer);
        pthread_mutex_unlock(&lock_contador);
        sem_post(&ocupado); //adiciona 1 no semaro ocupado para indicar que tem uma posicao ocupada
        sleep(rand() % 2);
    }
    pthread_exit(0);
  
}

void * consumidor (void* pi)
{
 while(1)
  {
      
        sem_wait(&ocupado); //remove 1 do semaro ocupado para indicar que vai remover um item no buffer
        pthread_mutex_lock(&lock_contador); //lock para controlar o contador
            buffer--;
            printf("Consumidor está consumindo  buffer = %d\n", buffer);
        pthread_mutex_unlock(&lock_contador);
        sem_post(&vazio); //adiciona 1 no semaro vazio para indicar que tem uma posicao vazia
        sleep(rand() % 2);
  }
  pthread_exit(0);
  
}