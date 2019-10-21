#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define TAMANHO 3000
#define GERACOES 500
#define THREADS 4

int** matriz = NULL;
int** matrizResultado = NULL;

int tamanhoTotal = TAMANHO + 2;
int tamanhoParcial = TAMANHO + 1;

void alocaMatrizes(){
    int linha = 0;

        matriz = (int **)malloc(sizeof(int*)*(tamanhoTotal));
        matrizResultado = (int **)malloc(sizeof(int*)*(tamanhoTotal));

    for(linha = 0; linha < tamanhoTotal ; linha++)
    {
        matriz[linha] = (int *)malloc(sizeof(int)*(tamanhoTotal));
        matrizResultado[linha] = (int *)malloc(sizeof(int)*(tamanhoTotal));
    }

    if(matriz == NULL || matrizResultado == NULL){
        printf("Falha ao alocar matrizes. Encerrando o programa!\n");
        exit(1);
    }
}


void inicializarMatriz(){
    int linha = 0;
    int coluna = 0;
        srand(time(NULL));

    for(linha = 0; linha < tamanhoTotal ; linha++)
    {
        for(coluna = 0; coluna < tamanhoTotal ; coluna++)
        {
            // Inicializa as bordas da matriz com 0
            if(linha == 0 || linha == tamanhoParcial || coluna == 0 || coluna == tamanhoParcial)
            {
                matriz[linha][coluna] = 0;
                matrizResultado[linha][coluna] = 0;
            }
            else
            {
                if(rand() % 100 > 50){
                    matriz[linha][coluna] = 1;

                } else {
                    matriz[linha][coluna] = 0;
                }
                matrizResultado[linha][coluna] = 0;
            }
        }
    }
}

void imprimirMatriz(char tipo, int numGeracao)
{
    FILE * arquivo;
    arquivo = fopen("saida.txt", "a+");
    int linha = 1;
    int coluna = 1;

    if(tipo == 'm'){
        fprintf(arquivo,"Matriz original\n");
        for(linha = 1; linha < tamanhoParcial ; linha++){
            for(coluna = 1; coluna < tamanhoParcial ; coluna++){
                fprintf(arquivo,"%d ",matriz[linha][coluna]);
            }
            fprintf(arquivo,"\n");

        }
    } else {
        fprintf(arquivo,"Geração %d\n", numGeracao);
        for(linha = 1; linha < tamanhoParcial ; linha++){
            for(coluna = 1; coluna < tamanhoParcial ; coluna++){
                fprintf(arquivo,"%d ",matrizResultado[linha][coluna]);
            }
            fprintf(arquivo,"\n");

        }
    }
    fprintf(arquivo,"\n\n");

    fclose(arquivo);
}

int processaCelula(int linha, int coluna){
    int vizinhos = 0;
    vizinhos = (matriz[linha-1][coluna-1] +
                matriz[linha-1][coluna] +
                matriz[linha-1][coluna+1] +
                matriz[linha][coluna-1] +
                matriz[linha][coluna+1] +
                matriz[linha+1][coluna-1] +
                matriz[linha+1][coluna] +
                matriz[linha+1][coluna+1]);
				
    // se célula está morta
	if(matriz[linha][coluna] == 0)
	{
		if(vizinhos == 3){
			matrizResultado[linha][coluna] = 1;
		} else {
			matrizResultado[linha][coluna] = 0;
		}
	}
	// se célula está viva
	else {
		if(vizinhos == 2 || vizinhos == 3){
			matrizResultado[linha][coluna] = matriz[linha][coluna];
		} else {
			matrizResultado[linha][coluna] = 0;
		}
	}
}

void evoluir(int geracao){
    int linha = 1;
    int coluna = 1;
    int vizinhos = -1;

    #pragma omp parallel for private(linha,coluna) shared(matriz) schedule(dynamic)
    for(linha = 1; linha < tamanhoParcial ; linha++)
    {
        for(coluna = 1 ; coluna < tamanhoParcial ; coluna++){
            processaCelula(linha, coluna);
//            printf("Geracao %d, usando thread %d na matriz[%d][%d]\n",geracao, omp_get_thread_num(), linha, coluna);
        }
    }
}

void copiarMatriz(){
        int linha = 1;
        int coluna = 1;
    for(linha = 1; linha < tamanhoParcial; linha++)
    {
        for(coluna = 1 ; coluna < tamanhoParcial ; coluna++)
        {
            matriz[linha][coluna] = matrizResultado[linha][coluna];
        }
    }
}

int main()
{
    omp_set_dynamic(0);
    omp_set_nested(1);
    omp_set_num_threads(THREADS);

    double starttime, stoptime;
    starttime = omp_get_wtime();

    alocaMatrizes();
    inicializarMatriz('m');
//    imprimirMatriz('m', 0);

    int contagem = 1;

    while(contagem <= GERACOES){
        evoluir(contagem);
//        imprimirMatriz('r', contagem);
        copiarMatriz();
        contagem++;
    }
    stoptime = omp_get_wtime();
    printf("Tempo de execução: %3.2f segundos\n", stoptime-starttime);
    return 0;
}