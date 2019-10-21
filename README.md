# gameOfLife_OpenMP
Parallel Implementation of Game of Live with OpenMP - Programação Paralela PUCRS 2019-02

Para modificar o tamanho da matriz, o número de gerações ou o número de threads, altere uma das diretivas abaixo no programa **game_of_life.c**

#define TAMANHO 3000

#define GERACOES 500

#define THREADS 4

Compilar usando a flag *-fopenmp*, conforme exemplo abaixo:

gcc -o game_of_life game_of_life.c -fopenmp
