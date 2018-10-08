#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_PALAVRA 30
#define MAX_ARQUIVOS 10

//ESTRUTURAS
typedef struct cabecalhos{
	int pos_cabeca;					//posicao do inicio da lista
	int pos_topo;					//primeira posicao nao usada no fim do arquivo
	int pos_livre;					//posicao do inicio da lista de nos livres
}cabecalho;	

typedef struct parOrdenado{

	int arq;
	int n;
	char caminho[100]; 

}parOrdenado;

typedef struct nodePalavra{
	char palavra[MAX_PALAVRA];
	parOrdenado par[MAX_ARQUIVOS];
	int l;
	int r;
}nodePalavra;

typedef struct no{ //estrutura para lista encadeada
	char info[20];
	struct no * prox; // ponteiro para proximo elemento
}Lista;

void strip(char *s);