#include "funcoes.c"

int main(int argc, char** argv){

	int qtdPalavra;
	FILE *e;
	if((e = fopen("indices.bin","w+b")) == NULL){
		printf("Erro ao carregar o arquivo binario!\nArquivos sobrescritos\n");
		e = fopen("indices.bin","w+b");
		scanf("%*c");                                   
	}

	Lista *stopwords = NULL;
	stopwords = carregarStopWords(argv[2]);
   
	inicializa(e,argv[1],stopwords);

	int resp=1;
	while(1){
        system("cls");
        printf("\n *--------------------------------------------*\n");
        printf(" | |01| - Imprimir Lista StopWords            |\n");
        printf(" | |02| - Imprimir Indice                     |\n");
        printf(" | |03| - Consultar                           |\n");
        printf(" *--------------------------------------------*\n");
        printf(" | |00| - Sair                                |\n");
        printf(" *--------------------------------------------*\n\n");
        printf(" ESCOLHA: ");
        fflush(stdin);
        scanf("%d", &resp);
        system("cls");
        
        switch(resp){
			case 1:
			   
                imprimelista(stopwords);
                scanf("%*c");
                scanf("%*c");
				
				break;
			case 2:
				
				imprimirIndices(e);
				scanf("%*c");
                scanf("%*c");	
				
				break;
			case 3:

				buscaPalavra(e);
				scanf("%*c");
                scanf("%*c");
                
				break;
			case 0:
			   exit(1);
		 default:
			printf("Invalido\n");
		}
	}
	return 0;
}


