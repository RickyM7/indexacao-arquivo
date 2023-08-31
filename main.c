#include "arvore_avl/avl.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char*argv[]) {

	//declaração da árvore
	arvore a;
	int opcao, valor, cresceu = 0;

	a = NULL;

	while(1){
		scanf("%d", &opcao);
		switch(opcao){
			case 1:
				scanf("%d", &valor);
				a = inserir_avl(valor, a, &cresceu);
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
			case 99:
				exit(0);
		}
	}
}