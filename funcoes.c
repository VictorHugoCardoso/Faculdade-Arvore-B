#include "header.h"

// ------------------------- FUNÇÕES ARQUIVO BINARIO -------------------------------

//Função responsável pela leitura de um cabeçalho
//Entrada: Ponteiro para o arquivo
//Retorno: O cabeçalho lido
cabecalho* le_cabecalho(FILE* arq){
	cabecalho * cab = (cabecalho*) malloc(sizeof(cabecalho));
	fseek(arq,0,SEEK_SET);
	fread(cab,sizeof(cabecalho),1,arq);
	return cab;
}

//Função responsável por escrever um novo cabeçalho no arquivo
//Entrada: Ponteiro para o arquivo, ponteiro para o cabeçalho
//Retorno: void
void escreveCabecalho(FILE* arq, cabecalho* cab){
    fseek(arq,0,SEEK_SET); //posiciona no início do arquivo
    fwrite(cab,sizeof(cabecalho),1,arq);
}

//Função responsável por criar um cabeçalho vazio
//Entrada: Ponteiro para o arquivo
//Retorno: void
void cria_lista_vazia(FILE* arq){
    cabecalho * cab = (cabecalho*) malloc(sizeof(cabecalho));
    cab->pos_cabeca = -1;
    cab->pos_topo = 0;
    cab->pos_livre = -1;
    escreveCabecalho(arq,cab);
    free(cab);
 }

//Função responsável por ler um nó
//Entrada: Ponteiro para o arquivo, posição do nó no arquivo
//Retorno: O nó lido
nodePalavra* le_nodePalavra(FILE* arq, int pos) {
	nodePalavra *x = (nodePalavra*)malloc(sizeof(nodePalavra));
	fseek(arq,sizeof(cabecalho)+ pos*sizeof(nodePalavra),SEEK_SET);
	fread(x,sizeof(nodePalavra),1,arq);
	return x;
}

//Função responsável por escrever um nó
//Entrada: Ponteiro para o arquivo, ponteiro para o nó, e a posição onde escrever o nó
//Retorno: void
void escreveNodePalavra(FILE* arq, nodePalavra* x, int pos){
    fseek(arq,sizeof(cabecalho) + pos*sizeof(nodePalavra),SEEK_SET);
    fwrite(x,sizeof(nodePalavra),1,arq);
}

//Função responsável por inserir o nó no arquivo binário
//Entrada: Ponteiro para o arquivo, ponteiro para o nó
//Retorno: void
void insere_nodePalavra_binario(FILE* arq, nodePalavra* p){
    cabecalho *cab = le_cabecalho(arq); //Cabeçalho do arquivo
    int posInserida;    //Variavel responsável por guardar onde o novo nó foi inserido para 
                        //atualizar para onde o pai do novo nó deve apontar

    if(cab->pos_cabeca == -1){ //Verifica se é a primeira informação a ser adicionado ao arquivo binário
        escreveNodePalavra(arq, p, ++cab->pos_cabeca);
        posInserida = cab->pos_cabeca;
        cab->pos_topo++;
    }
    else if(cab->pos_livre == -1) { //Caso não existe posição livre, insere no final do arquivo
        escreveNodePalavra(arq, p, cab->pos_topo);
        posInserida = cab->pos_topo;
        cab->pos_topo++;
    }
    else { //Caso exista posição livre, insere o no na primeira posição livre 
        nodePalavra* aux = le_nodePalavra(arq, cab->pos_livre);
        escreveNodePalavra(arq, p, cab->pos_livre);
        posInserida = cab->pos_livre;
        cab->pos_livre = aux->r; //A lista de posições livres ficam salvas nos elementos r do nó
        free(aux);
    }
 
    //Atualiza as informações do cabeçalho
    escreveCabecalho(arq, cab); 
    
    nodePalavra* pai;
    int posPai = cab->pos_cabeca;
    int posPaiNext = cab->pos_cabeca;
    pai = le_nodePalavra(arq,posPaiNext);

    
    do{ //Loop responsável por percorrer a arvore e encontrar a posição do nó pai do nó inserido recentemente
        pai = le_nodePalavra(arq,posPaiNext);
        posPai = posPaiNext;
        if(strcmp(p->palavra,pai->palavra) >= 0){
            posPaiNext = pai->r;
        }else{
            posPaiNext = pai->l;
        }
    } while(posPaiNext != -1);
    
 
    if(posInserida!= cab->pos_cabeca){ //Verifica se não é o primeiro elemento inserido
        if(strcmp(p->palavra,pai->palavra) >= 0){
            pai->r = posInserida;
        }else{
            pai->l = posInserida;
        }
        escreveNodePalavra(arq,pai,posPai); //Atualiza o valor de l ou r do nó pai
    }
	

    free(pai);
    free(cab);
}

// Função responsável por imprimir um nó
//Entrada: Ponteiro para o nó
//Retorno: void
void imprimeNode(nodePalavra* p){
    printf(" | %-13s             | ",p->palavra, p->par[0].arq, p->par[0].n);
    int i;
    for(i=0;i<MAX_ARQUIVOS;i++){
    	if((p->par[i].arq != 0)&&(p->par[i].n != 0)){
    		printf("(%d,%d) ", p->par[i].arq,p->par[i].n);
    	}
    }
    printf("\n");
}

// Função auxiliar para imprimir os indices do arquivo binário
//Entrada: Ponteiro para o arquivo, posição do nó no arquivo
//Retorno: void
void imprimirIndicesAux(FILE* f, int pos){

	if(pos != -1){
        nodePalavra* p = le_nodePalavra(f,pos);
        imprimirIndicesAux(f, p->l);
        imprimeNode(p);
        imprimirIndicesAux(f, p->r);
        free(p);
    }
}

// Função que imprime todos os indices do arquivo
//Entrada: Ponteiro para o arquivo
//Retorno: void
void imprimirIndices(FILE* f){
	system("cls");

	cabecalho* cab = le_cabecalho(f);

	printf("\n -----------------------------------------------------\n");
	printf(" | Palavra                   | Arquivo/Qtd \n");
	printf(" -----------------------------------------------------\n\n");

	imprimirIndicesAux(f, cab->pos_cabeca);
}

//Função responsável por 
//Entrada: Ponteiro para o arquivo, palavra que se deseja buscar a posição
//Retorno: Posição da palavra
int posPalavra(FILE* f,char *s){

    cabecalho *cab = le_cabecalho(f); //Cabeçalho do arquivo
 
    nodePalavra* pai;
     
    int posPai = cab->pos_cabeca;
    int posPaiNext = cab->pos_cabeca;
    pai = le_nodePalavra(f,posPaiNext);
    
    do{ //Loop responsável por percorrer a arvore e encontrar a posição do nó pai do nó inserido recentemente
        
        pai = le_nodePalavra(f,posPaiNext);
        posPai = posPaiNext;
        if(strcmp(s,pai->palavra) >= 0){
            posPaiNext = pai->r;
        }else{
            posPaiNext = pai->l;
        }
    } while(posPaiNext != -1);
 
 
    free(pai);
 
    return posPai;
}

//Função responsável por buscar uma palavra no arquivo
//Entrada: Ponteiro para o arquivo
//Retorno: void
void buscaPalavra(FILE *f){

	char palavras[50];
	char matrizCaminhos[20][100];
	int pos,qtd, aux[20],i,cont=0;

	for(i=0;i<20;i++){
		aux[i]=-1;
	}

    fflush(stdin);
	scanf("%[^\n]", palavras);

    const char s[2] = " ";
    char *token;

    token = strtok(palavras, s);
    
    while( token != NULL ) {

    	pos = posPalavra(f,token);

		if (pos == -1){
			printf("Palavra nao existe\n");
		}else{
			
			nodePalavra* p = le_nodePalavra(f,pos);

			for(i=0;i<MAX_ARQUIVOS;i++){
		    	if((p->par[i].arq != 0)&&(p->par[i].n != 0)){
		    		aux[cont] = p->par[i].arq;
		    		strcpy(matrizCaminhos[cont] ,p->par[i].caminho);
		    		cont++;
		    	}
		    }

			free(p);
		}

    	token = strtok(NULL, s);
    }

    for(i=0;i<20;i++){
    	if(aux[i] != -1){
    		printf("%d - %s\n", aux[i], matrizCaminhos[i]);


    	}
    }
    /*
	printf("qtd: %d\n", qtd);
	printf("documentos: \n");
	*/
}



// -------------------------- FUNÇÕES LISTA ENCADEADA -----------------------------

//Função responsável por verificar se uma lista está vazia
//Entrada: Ponteiro para lista
//Retorno: 1 ou 0, dependendo se a lista está vazia ou não
int vazia(Lista *l){
    if (l == NULL){
        return 1;
    } else { 
        return 0;
    }
}

//Função responsável por inserir os stopwords na lista, mas já ordenados
//Entrada: Ponteiro para lista, e a palavra
//Retorno: Retorna o elemento inserido ordenado
Lista* insereOrdenado(Lista ** p, char* valor) {
    
    Lista *atual, *aux, *anterior;

    aux = (Lista *) malloc(sizeof(Lista));

    atual = *p;
    anterior = NULL;

    strcpy(aux->info,valor);

    if(atual == NULL){
        aux->prox = NULL;
        *p = aux;
    }else{
        while(atual != NULL && strcmp(atual->info,valor) < 0){
            anterior = atual;
            atual = atual->prox;
        }

        aux->prox = atual;

        if(anterior == NULL){
            *p = aux;
        } else{
            anterior->prox = aux;
        }
    }
}

//Função responsável por buscar uma palavra na lista
//Entrada: Ponteiro para lista, palavra buscada
//Retorno: Se o elemento está na lista ou não
int busca(Lista* l, char* x){
	
	Lista *p = l;
	int flag = 0;
	
	while(p!=NULL){
		if(strcmp(p->info,x) == 0){
			flag = 1;
		}
		p = p->prox;
	}
	return flag;
}

//Função responsável por inserir as palavras na lista, mas sem repeti-las
//Entrada: Ponteiro para lista, palavra
//Retorno: Lista sem os elementos repetidos
Lista* inserirListaSemRepeticao(Lista* l, char* palavra){
	if(!busca(l,palavra)){
		Lista* no = (Lista*) malloc(sizeof(Lista));
		strcpy(no->info,palavra);
		no->prox = l;
		return no;
	}else{
		return l;
	}	
}

//Função responsável por imprimir a lista
//Entrada: Ponteiro para lista
//Retorno: void
void imprimelista(Lista *l){
    if(!vazia(l)){
        printf("[%s]\n",l->info);
        imprimelista(l->prox);
    }
}

//Função responsável por carregar todas as stopwords para lista
//Entrada: Nome do arquivo de dados
//Retorno: Lista de stopwords carregadas
Lista * carregarStopWords(char* txt){
    Lista *stopwords = NULL;
    FILE *arq;
    char s[30];

    if((arq = fopen(txt,"r")) == NULL){
        printf("Erro ao abrir o arquivo stopWords\n"); //erro na abertura do arquivo
        scanf("%*c");
        exit(1);
    }else{
        fseek(arq,0,SEEK_SET);                      //posiciona o cursor no comeco do arquivo texto
        int k,i;
        char aux[30];
        while(fgets(s,30,arq) != 0){
            strip(s);
            insereOrdenado(&stopwords,s);
        }

    }

    return stopwords;
}

// ---------------------------------- FUNÇÕES STRING ----------------------------------

void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

//Função responsável por remover simbolos do texto
//Entrada: Simbolo que se deseja remover
//Retorno: Texto sem os simbolos
char* removeSimbolo(char* s){
	int i=0;
	for(i=0;i<strlen(s);i++){
		switch(s[i]){
			case ',':
				removeChar(s, ',');
				break;

			case '.':
				removeChar(s, '.');
				break;
			
			case '!':
				removeChar(s, '!');
				break;
			
			case '?':
				removeChar(s, '?');
				break;

			case ';':
				removeChar(s, ';');
				break;

			case ':':
				removeChar(s, ':');
				break;
		}
	}

	return s;
}

//Função responsável por contar quantas vezes uma string ocorre
//Entrada: Palavra que se deseja contar, ponteiro para o arquivo
//Retorno: Quantas vezes a palavra ocorreu
int getOcorrenciasString(char* str,FILE *arq){

	char s[100];
	fseek(arq,0,SEEK_SET);

	int cont=0;
	while(fgets(s,100,arq) != 0){
		if(strstr(s,str) != NULL){
			cont++;
		}
	}

	return cont;
}

// ---------------------------------- INICIALIZAR ARQUIVO -------------------------------

//Função responsável por inicializar o arquivo
//Entrada: Ponteiro para o arquivo, nome da base de dados, lista de stopwords 
//Retorno: void
void inicializa(FILE *e, char* base, Lista* stopWords){

	cria_lista_vazia(e);	//como o arquivo é sobrescrito, uma nova lista precisa ser criada
	char s[200];
	char u[200];

	Lista* listaValidas = NULL;

	FILE *arq;
	if((arq = fopen(base,"r")) == NULL){
		printf("Erro ao abrir o arquivo texto\n"); //erro na abertura do arquivo
		scanf("%*c");
        exit(1);
	}

	fseek(arq,0,SEEK_SET);

	while(fgets(s,200,arq) != 0){
		strip(s);

		FILE *txt;
		if((txt = fopen(s,"r")) == NULL){
			printf("Erro ao abrir o arquivo texto\n"); //erro na abertura do arquivo
			scanf("%*c");
	        exit(1);
		}

		int i;
		int flag;
		const char branco[2] = " ";
		char *token;
		while(fgets(u,200,txt) != 0){
			strcpy(u,removeSimbolo(u));
			strip(u);
			token = strtok(u, branco);
			while(token != NULL) {
				if(strlen(token) > 1){			
					Lista * aux = stopWords;	
					flag = 0;
					while(stopWords != NULL){
						if(strcmp(token, stopWords->info) == 0){
							flag = 1;
							break;
						}
						stopWords = stopWords->prox;
					}

					if(flag == 0){
						listaValidas = inserirListaSemRepeticao(listaValidas,token);
					}
					
					stopWords = aux;
				}

				token = strtok(NULL, branco);
		    }
		}

        fclose(txt);
	}

	int contArq,contArray,occur;

	while(listaValidas != NULL){

		fseek(arq,0,SEEK_SET);

		contArq = 0;
		contArray = 0;

		nodePalavra* p = (nodePalavra*) malloc(sizeof(nodePalavra));
			
		while(fgets(s,200,arq) != 0){
			contArq++;
			strip(s);
			FILE *arquivoTexto;
			if((arquivoTexto = fopen(s,"r")) == NULL){
				printf("Erro ao abrir o arquivo texto\n"); //erro na abertura do arquivo
				scanf("%*c");
		        exit(1);
			}


			occur = getOcorrenciasString(listaValidas->info,arquivoTexto);

			p->par[contArray].arq = contArq;
			p->par[contArray].n = occur;
			strcpy(p->par[contArray].caminho,s);

			contArray++;	
		}

		strcpy(p->palavra,listaValidas->info);

		p->l = -1;
		p->r = -1;
		
		insere_nodePalavra_binario(e,p);
		free(p);

		listaValidas = listaValidas->prox;
	}
	
	fclose(arq);			
	free(e);
}
