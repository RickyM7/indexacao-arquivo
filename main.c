#include "arvore_avl/avl.h"
#include "arquivo/arquivo.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

	// declaração da árvore
	tabela tab;
	int opcao, valor, cresceu = 0;
	int indice, codigo;
	dado aluno;

	if (inicializarArquivo(&tab))
	{
		while (1)
		{
			printf("Digite uma opcao (1 para inserir e 2 para buscar): \n");
			scanf("%d", &opcao);
			switch (opcao)
			{
			case 1:
				if (inserir_aluno(&tab, ler_dados(), &cresceu))
					printf("Aluno Cadastrado com sucesso\n");
				else
					printf("Aluno nao inserido\n");
				break;
			case 2:
				//tab.indice = carregar_arquivo_index(&tab);
				printf("Buscar Aluno por codigo\n\n");
				printf("Codigo: ");
				scanf("%d", &opcao);

				if (tab.indice != NULL) {
					indice = buscar_indice_avl(tab.indice, opcao);
					printf("Indice: %d\n", indice);
					if(indice >= 0) {
						aluno = buscar_aluno(tab.arquivo_dados, indice);
						if(!aluno.removido)
							imprimir_elementos(aluno);
						else
							printf("Erro ao buscar\n");
					} else
						printf("Nao encontrado\n");
						break;
				} else {
					printf("Indice da tabela nao inicializado\n");
				}
				break;
			case 99:
				finalizar_arquivo(&tab);
				exit(0);
			}
		}
	}
}