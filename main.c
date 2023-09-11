#include <stdio.h>
#include <stdlib.h>

#include "arquivo/arquivo.h"
#include "arvore_avl/avl.h"

int main(int argc, char *argv[]) {
    // declaração da árvore
    tabela tab;
    int opcao, cresceu;
    dado aluno;

    if (inicializarArquivo(&tab, &cresceu)) {
        while (1) {
            printf("Digite uma opcao (1 para inserir e 2 para buscar e 99 para sair): \n");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1:
                    printf("INSERIR ALUNO\n");
                    if (inserir_aluno(&tab, ler_dados(&tab), &cresceu))
                        printf("Aluno Cadastrado com sucesso\n");
                    else
                        printf("Aluno nao inserido\n");
                    break;
                case 2:
                    printf("BUSCAR ALUNO POR CODIGO\n");
                    printf("CODIGO: ");
                    scanf("%d", &opcao);
                    while (getchar() != '\n');
                    system("clear");
                    dado aluno = buscar_aluno(tab, opcao);
                    if (!aluno.removido)
                        imprimir_elementos(aluno);
                    else
                        printf("Aluno nao encontrado\n");
                    carregar_arquivo_index(&tab, &cresceu);
                    break;
                case 99:
                    finalizar_arquivo(&tab);
                    exit(0);
            }
        }
    }
}
