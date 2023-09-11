#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "../arvore_avl/avl.h"
#include "../cJSON/cJSON.h"
#include <stdio.h>

typedef struct aluno {
	int removido;
	char* nome;
    int codigo;
	int idade;
	char data_nascimento[11];
	int CEP;
	long long int telefone;
	char* email;
	char* nome_pais;
} dado;

typedef struct tabela {
	FILE *arquivo_dados;
	cJSON *root_dados;
	arvore indice;
} tabela;

int inicializarArquivo (tabela *tab, int *cresceu);

dado* ler_dados();
int inserir_aluno(tabela *tab, dado *aluno, int *cresceu);
void salvar_aluno(FILE *arquivo, cJSON *root, dado *aluno);
dado buscar_aluno(FILE *arquivo, int chave);

cJSON *criarAluno(int id, char *nome, int removido, int idade, char *data_nascimento, int CEP,long long int telefone, char *email, char *nome_pais);
cJSON *criarJSON(dado *aluno, cJSON *root);

void criarArquivoJson(dado *aluno, FILE *arquivo);
arvore carregar_arquivo_index(tabela *tab, int *cresceu);

cJSON *carregarConteudoArquivoJson(FILE *arquivo, cJSON *root);
void imprimir_elementos(dado aluno);

void salvar_arquivo(char *nome, arvore raiz);
void salvar_auxiliar(arvore raiz, FILE *arquivo);
void finalizar_arquivo(tabela *tab);

#endif