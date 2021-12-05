#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N_ITENS 10 // numero de itens
#define MAX_ITENS_COMPRA 5 // numero maximo de itens que o cliente pode comprar
#define N_FUNC 5 // numero de funcionarios
#define N_CLI 100 // numero de clientes
#define MAX_CLI 5 // numero maximo de clientes no supermercado
#define MAX_OPERACOES 10 //numero maximo de operacoe

//Declaracao de funcoes
void *entrada_supermercado (void * arg);
void *funcionario(void *arg);
void comprar(int id);
void supermercado(int id);
void fechamento();

//Declarando as threads
pthread_t funcionario_estoque[N_FUNC];                    
pthread_t clientes[N_CLI];         

//Declarando as variaveis de condicao
pthread_cond_t funcionario_estoque_cond = PTHREAD_COND_INITIALIZER;

//Declarando os locks
pthread_mutex_t operacoes_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t arroz_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t feijao_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t macarrao_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ovos_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ta_faltando_lock = PTHREAD_MUTEX_INITIALIZER;

//Declarando os semaforos
sem_t fluxo_sem;

//Declarando os contadores e flag
int produtos[4]; //cada posicao eh um produto, [0] = arroz, [1] = feijao, [2] = macarrao e [3] = ovos
int vendidos[4]; //guarda os produtos vendidos
int fluxo = 0; //contador indicando quantos clientes estao no supermercado
int operacoes = 0; //contador indicando o numero de operacoes feitas no supermercado
int ta_faltando_produto = -1; //flag para sinalizar a falta de um produto

int main(int argc, char**argv) {
    srand(time(NULL));

    int *id;
    int i;

    //inicializando o semafaro para controle de entrada no supermecado
    sem_init(&fluxo_sem, 0, MAX_CLI);

    //inicializando o estoque
    for (int j = 0; j < 4; j++){
        produtos[j] = N_ITENS;
        vendidos[j] = 0;
    }

    //criando as threads de funcionarios
    for (int i = 0; i < N_FUNC; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&funcionario_estoque[i], NULL, funcionario, (void *)(id));
    }
    
    //criando as threads de clientes
    for (i = 0; i < N_CLI; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&clientes[i], NULL, entrada_supermercado, (void *)(id));
    }

    pthread_join(clientes[0], NULL);
    fechamento();
    return 0;
}

void *entrada_supermercado(void *arg){
    int id = *((int *) arg);

    //a entrada do supermercado eh controlada pelo semaforo fluxo_sem
    //o while eh parada quando o supermercado chega a um numero max de operacoes
    while(1){
        if (operacoes >= MAX_OPERACOES){
            break;
        }
        
        sem_wait(&fluxo_sem);
            pthread_mutex_lock(&operacoes_lock);
                fluxo++;
            pthread_mutex_unlock(&operacoes_lock);
            supermercado(id);
        sem_post(&fluxo_sem);
    }
}

void supermercado(int id){

    id++;
    printf("o cliente %d acessou o supermercado, fluxo = %d\n", id, fluxo);
    
    int quantidade;
    // o cliente escolhe um produto aleatorio
    int item = (rand() % 4);
    
    sleep(rand() % 10);
   
    // se o numero de itens do produto for menor ou igual a 0
    // o funcionario eh acordado para repor o produto no estoque
    if (produtos[item] <= 0){

        pthread_mutex_lock(&ta_faltando_lock);
            ta_faltando_produto = item;    
            pthread_cond_signal(&funcionario_estoque_cond);
        pthread_mutex_unlock(&ta_faltando_lock);

        if (item == 0){
                printf("Arroz em falta\n");
        } else if (item == 1){
                printf("Feijão em falta\n");
        } else if (item == 2){
                printf("Macarrão em falta\n");
        } else if (item == 3){
                printf("Ovos em falta\n");
        }
        
    // se nao, o cliente escolhe uma quantidade de itens daquele produto e compra
    // eh usado um lock para cada produto, ja que tem varios clientes acessando o supermercado
    } else {
        if (item == 0){
            pthread_mutex_lock(&arroz_lock);
                quantidade = ((rand() % produtos[item]) % MAX_ITENS_COMPRA) + 1;
                produtos[item] -= quantidade;
                printf("\t\tO cliente %d comprou %d quantidades de arroz\n", id, quantidade);
                printf("\t\tquantidade de arroz restante no estoque = %d\n", produtos[item]);
                vendidos[item] += quantidade;
            pthread_mutex_unlock(&arroz_lock);
        } else if (item == 1){
            pthread_mutex_lock(&feijao_lock);
                quantidade = ((rand() % produtos[item]) % MAX_ITENS_COMPRA) + 1;
                produtos[item] -= quantidade;
                printf("\t\tO cliente %d comprou %d quantidades de feijao\n", id, quantidade);
                printf("\t\tquantidade de feijao restante no estoque = %d\n", produtos[item]);
                vendidos[item] += quantidade;
            pthread_mutex_unlock(&feijao_lock);
        } else if (item == 2){
            pthread_mutex_lock(&macarrao_lock);
                quantidade = ((rand() % produtos[item]) % MAX_ITENS_COMPRA) + 1;
                produtos[item] -= quantidade;
                printf("\t\tO cliente %d comprou %d quantidades de macarrao\n", id, quantidade);
                printf("\t\tquantidade de macarrao restante no estoque = %d\n", produtos[item]);
                vendidos[item] += quantidade;
            pthread_mutex_unlock(&macarrao_lock);
        } else if (item == 3) {
            pthread_mutex_lock(&ovos_lock);
                quantidade = ((rand() % produtos[item]) % MAX_ITENS_COMPRA) + 1;
                produtos[item] -= quantidade;
                printf("\t\tO cliente %d comprou %d quantidades de ovos\n", id, quantidade);
                printf("\t\tquantidade de ovos restante no estoque = %d\n", produtos[item]);
                vendidos[item] += quantidade;
            pthread_mutex_unlock(&ovos_lock);
        }
    }

    //no fim o cliente vai finalizar a compra do produto
    comprar(id);
}

void comprar(int id){
    pthread_mutex_lock(&operacoes_lock);
        operacoes++;
        fluxo--;
    pthread_mutex_unlock(&operacoes_lock);
    printf("\no cliente %d saiu do supermercado, operações = %d, fluxo = %d\n\n", id, operacoes, fluxo);
    sleep(rand() % 10);
}


void *funcionario(void *arg){
    int id = *((int *) arg);
    // quando o funcionario acorda ele verifica qual produto esta em falta
    // repoe o produto e volta a dormir
    while(1){
        if (ta_faltando_produto != -1){
            if (ta_faltando_produto == 0){
                pthread_mutex_lock(&arroz_lock);
                    produtos[ta_faltando_produto] = N_ITENS;
                    printf("Arroz reposto\n");
                pthread_mutex_unlock(&arroz_lock);
            } else if (ta_faltando_produto == 1){
                pthread_mutex_lock(&feijao_lock);
                    produtos[ta_faltando_produto] = N_ITENS;
                    printf("Feijão reposto\n");
                pthread_mutex_unlock(&feijao_lock);
            } else if (ta_faltando_produto == 3){
                pthread_mutex_lock(&macarrao_lock);
                    produtos[ta_faltando_produto] = N_ITENS;
                    printf("Macarrão reposto\n");
                pthread_mutex_unlock(&macarrao_lock);
            } else {
                pthread_mutex_lock(&ovos_lock);
                    produtos[ta_faltando_produto] = N_ITENS;
                    printf("Ovos reposto\n");
                pthread_mutex_unlock(&ovos_lock);
            }
            
            pthread_mutex_lock(&ta_faltando_lock);
                ta_faltando_produto = -1;
                pthread_cond_wait(&funcionario_estoque_cond, &ta_faltando_lock);
            pthread_mutex_unlock(&ta_faltando_lock);
        }
    }
}

void fechamento(){
    printf("-------------\nSupermecardo fechou, estatisticas do dia:\n\n");
    printf("Quantidade de arroz vendidos: %d\n", vendidos[0]);
    printf("Quantidade de feijão vendidos: %d\n", vendidos[1]);
    printf("Quantidade de macarrão vendidos: %d\n", vendidos[2]);
    printf("Quantidade de ovos vendidos: %d\n\n", vendidos[3]);
    printf("Operações feitas = %d\n-------------\n", operacoes);
}
