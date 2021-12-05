#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MA 10 //macacos que andam de A para B
#define MB 10 //macacos que andam de B para A

pthread_mutex_t lock_corda = PTHREAD_MUTEX_INITIALIZER; //lock para controlar a corda
pthread_mutex_t lock_ab = PTHREAD_MUTEX_INITIALIZER; //lock do contador de macacos ab
pthread_mutex_t lock_ba = PTHREAD_MUTEX_INITIALIZER; // lock do contador de macacos ba
pthread_mutex_t lock_fluxo = PTHREAD_MUTEX_INITIALIZER; //lock pra controlar o fluxo
int cont_ab = 0;
int cont_ba = 0;

void * macacoAB(void * a) {
    int i = *((int *) a);    
    while(1){
        //Procedimentos para acessar a corda
        /*
            para acessar a corda o macacoAB pega o lock do fluxo, parando o
            fluxo de macacos passando de BA, incrementa 1 no contador ab, e 
            verifica se ele é o primeiro, se for ele pega o ock da corda e 
            libera o fluxo. 
        */
        pthread_mutex_lock(&lock_fluxo);
            pthread_mutex_lock(&lock_ab);
                cont_ab++;
                if (cont_ab == 1)
                    pthread_mutex_lock(&lock_corda);
            pthread_mutex_unlock(&lock_ab);
        pthread_mutex_unlock(&lock_fluxo);
            
        printf("Macaco %d passado de A para B \n",i);
        sleep(1);
        
        //Procedimentos para quando sair da corda
        /*
            para sair da corda o macacoAB, decrementa o contador ab, se ele for o
            ultimo macaco, ele libera o lock da corda 
        */
        pthread_mutex_lock(&lock_ab);
            cont_ab--;
            if (cont_ab == 0)
                pthread_mutex_unlock(&lock_corda);
        pthread_mutex_unlock(&lock_ab);
    }
    pthread_exit(0);
}

void * macacoBA(void * a) {
    int i = *((int *) a);    
    while(1){
        //Procedimentos para acessar a corda
        /*
            para acessar a corda o macacoBA pega o lock do fluxo, incrementa 1 
            no contador ba, e verifica se ele é o primeiro, se for ele pega o
            lock da corda e libera o fluxo. 
        */
        pthread_mutex_lock(&lock_fluxo);
            pthread_mutex_lock(&lock_ba);
                cont_ba++;
                if (cont_ba == 1)
                    pthread_mutex_lock(&lock_corda);
            pthread_mutex_unlock(&lock_ba);
        pthread_mutex_unlock(&lock_fluxo);
        
        printf("Macaco %d passado de B para A \n",i);
        sleep(1);
        
	    //Procedimentos para quando sair da corda
        /*
            para sair da corda o macacoBA, decrementa o contador ba, se ele for o
            ultimo macaco, ele libera o lock da corda 
        */
        pthread_mutex_lock(&lock_ba);
            cont_ba--;
            if (cont_ba == 0)
                pthread_mutex_unlock(&lock_corda);
        pthread_mutex_unlock(&lock_ba);
    }
    pthread_exit(0);
}

void * gorila(void * a){
    while(1){
	//Procedimentos para acessar a corda    
    /*
        para o golira atravessar sem macacos na corda, ele pega o lock do fluxo, parando o fluxo de macacos e pega o lock da corda
    */
    pthread_mutex_lock(&lock_fluxo);
        pthread_mutex_lock(&lock_corda);
    pthread_mutex_unlock(&lock_fluxo);

	printf("Gorila passado de A para B \n");
	sleep(5);

    //Procedimentos para quando sair da corda
    pthread_mutex_unlock(&lock_corda);
    }
    pthread_exit(0);
}

int main(int argc, char * argv[])
{
    pthread_t macacos[MA+MB];
    int *id;
    int i = 0;

    for(i = 0; i < MA+MB; i++){
        id = (int *) malloc(sizeof(int));
        *id = i;
        if(i%2 == 0){
          if(pthread_create(&macacos[i], NULL, &macacoAB, (void*)id)){
            printf("Não pode criar a thread %d\n", i);
            return -1;
          }
        }else{
          if(pthread_create(&macacos[i], NULL, &macacoBA, (void*)id)){
            printf("Não pode criar a thread %d\n", i);
            return -1;
          }
        }
    }
    pthread_t g;
    pthread_create(&g, NULL, &gorila, NULL);

  
    pthread_join(macacos[0], NULL);
    return 0;
}