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
	cJSON *root_dados;
	arvore indice;

} tabela;

int inicializarArquivo (tabela *tab);

dado* ler_dados();
int inserir_aluno(tabela *tab, dado *aluno, int *cresceu);
void salvar_aluno(FILE *arquivo, cJSON *root, dado *aluno);
void remover_aluno(tabela *tab, dado *aluno, int *diminuiu, int chave);
dado buscar_aluno(FILE *arquivo, int chave);

cJSON *criarMateria(char nome[50], float media);
cJSON *criarAluno(int id, char *nome, int removido, cJSON *materias);
cJSON *criarJSON(dado *aluno, cJSON *root);

void criarArquivoJson(dado *aluno, FILE *arquivo);
void carregarArquivo(tabela *tab);

cJSON *carregarConteudoArquivoJson(FILE *arquivo, cJSON *root);
void imprimir_elementos(dado aluno);
void listar_por_codigo(FILE *arquivo, arvore raiz);

void salvar_arquivo(char *nome, arvore raiz);
void salvar_auxiliar(arvore raiz, FILE *arquivo);
void finalizar_arquivo(tabela *tab);

#endif