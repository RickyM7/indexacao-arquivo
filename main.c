#include "arvore_avl/avl.h"
#include "arquivo/arquivo.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char*argv[]) {

	//declaração da árvore
	arvore a;
	tabela tab;
	int opcao, valor, cresceu = 0;

	a = NULL;

	if (inicializarArquivo(&tab)) {
		while(1){
		scanf("%d", &opcao);
		switch(opcao){
			case 1:
				scanf("%d", &valor);
				//a = inserir_avl(valor, a, &cresceu);
				break;
			case 2:
				pre_order_avl(a);
				printf("\n");
				break;
			case 3:
				scanf("%d", &valor);
				remover_avl(a, valor, &cresceu);
				break;
				break;
			case 4:
				pos_order_avl(a);
				printf("\n");
				break;
			case 5:
				printf("Insira os dados do Aluno\n\n");
				if(inserir_aluno(&tab, ler_dados(), &cresceu))
					printf("Aluno Cadastrado com sucesso\n"); 
				else 
					printf("Aluno nao inserido\n");
				break;
			case 99:
				exit(0);
		}
	}
	}
	

	while(1){
		scanf("%d", &opcao);
		switch(opcao){
			case 1:
				scanf("%d", &valor);
				//a = inserir_avl(valor, a, &cresceu);
				break;
			case 2:
				pre_order_avl(a);
				printf("\n");
				break;
			case 3:
				scanf("%d", &valor);
				remover_avl(a, valor, &cresceu);
				break;
				break;
			case 4:
				pos_order_avl(a);
				printf("\n");
				break;
			case 5:
				printf("Insira os dados do Aluno\n\n");
				if(inserir_aluno(&tab, ler_dados(), &cresceu))
					printf("Aluno Cadastrado com sucesso\n"); 
				else 
					printf("Aluno nao inserido\n");
				break;
			case 99:
				exit(0);
		}
	}
}