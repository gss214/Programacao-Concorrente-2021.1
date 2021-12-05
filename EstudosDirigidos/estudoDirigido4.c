#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PR 1 //número de produtores
#define CN 1 // número de consumidores
#define N 5  //tamanho do buffer

pthread_cond_t consumidor_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t produtor_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock_buffer = PTHREAD_MUTEX_INITIALIZER;

int buffer = 0;

void * produtor(void * meuid);
void * consumidor (void * meuid);


void main(argc, argv)
int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;

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
	int i = *((int *)pi);

    while(1)
    {
      /*
        eh usado um lock para quando o produtor quer produzir e
        se o buffer for igual a N eh usado a variavel de condição
        para adormecer o produtor
      */
        sleep(rand() % 2);
        pthread_mutex_lock(&lock_buffer);
            while (buffer == N){
                pthread_cond_wait(&produtor_cond, &lock_buffer);
                printf("Dorme produtor! Tamanho do buffer = %d\n", buffer);
            }
            buffer++;
            printf("O produtor %d produziu um item   Tamanho do buffer = %d\n", i, buffer);
          
            /*
              verifica se o buffer não esta vazio, entao acorda o produtor
            */

            if (buffer == 1) {
                pthread_cond_signal(&consumidor_cond);
                printf("Consumidores acordados! Tamanho do buffer = %d\n", buffer);
            }
        pthread_mutex_unlock(&lock_buffer);
        sleep(rand() % 2);
    } 
    pthread_exit(0);

}

void * consumidor (void* pi)
{
	int i = *((int *)pi);

    while(1)
    {
        sleep(rand() % 2);

        /*
          eh usado um lock quando o consumidor quer consumir, eh verificado
          se o buffer esta vazio, se estiver o consumidor adomerce 
        */

        pthread_mutex_lock(&lock_buffer);
            while (buffer == 0) {
                printf("Buffer vazio\n");
                pthread_cond_wait(&consumidor_cond, &lock_buffer);
            }
            buffer--;
            printf("O consumidor %d consumiu um item   Tamanho do buffer = %d\n", i, buffer);

          
            //eh verificado se o buffer não encheu, se sim acorda o produtor
            if (buffer == N-1){
                printf("Acorda produtor! Tamanho do buffer = %d\n", buffer);
                pthread_cond_signal(&produtor_cond);
            }
            sleep(2);
        pthread_mutex_unlock(&lock_buffer);
        sleep(rand() % 2);
    }
    pthread_exit(0);
  
}