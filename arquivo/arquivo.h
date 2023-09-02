#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "../arvore_avl/avl.h"
#include "../cJSON/cJSON.h"
#include <stdio.h>

typedef struct materia {
	char nome[50];
    float media;
} materia;

typedef struct aluno {
	int removido;
	char* nome;
    int codigo;
    materia materia;
} dado;

typedef struct tabela {

	FILE *arquivo_dados;
	arvore indice;

} tabela;

int inicializarArquivo (tabela *tab);

dado* ler_dados();
int inserir_aluno(tabela *tab, dado *aluno, int *cresceu);
void salvar_aluno(FILE *arq, dado *aluno);

cJSON *criarMateria(char nome[50], float media);
cJSON *criarAluno(int id, char *nome, int removido, cJSON *materias);
cJSON *criarJSON(dado *aluno);

void criarArquivoJson(dado *aluno, FILE *arquivo);
void carregarArquivo(tabela *tab);

void tirar_enter(char *string);

#endif