#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>

#include "../arvore_avl/avl.h"

typedef struct aluno {
    int removido;
    int codigo;
    char nome[256];
    int idade;
    char data_nascimento[11];
    int CEP;
    char nacionalidade[256];
    long long int telefone;
    char email[256];
} dado;

typedef struct tabela {
    FILE *arquivo_dados;
    arvore indice;
} tabela;

int inicializarArquivo(tabela *tab, int *cresceu);
dado *ler_dados(tabela *tab);
int inserir_aluno(tabela *tab, dado *aluno, int *cresceu);
int aluno_ja_existe(tabela *tab, int codigo);
void salvar_aluno(FILE *arquivo, dado *aluno);
dado buscar_aluno(tabela tab, int codigo);
arvore carregar_arquivo_index(tabela *tab, int *cresceu);

void salvar_arquivo(char *nome, arvore raiz);
void salvar_auxiliar(arvore raiz, FILE *arquivo);
void finalizar_arquivo(tabela *tab);
void imprimir_elementos(dado aluno);

#endif
