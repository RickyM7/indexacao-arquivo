#ifndef ARQUIVO_TESTE_H
#define ARQUIVO_TESTE_H
#include <stdio.h>
#define INDEXES_FILE "db/indexes.dat"
#define DATABASE_FILE "db/db.txt"

typedef struct aluno {
    char *curso;
    char *nome;
    int ano;
    int id;
} dado;

//Índice
typedef struct indice {
    int id;
    long indice;
} tipo_dado;

typedef struct no {
	tipo_dado *valor;
	int fb;
	struct no *esquerda, *direita;
} no;

typedef no* arvore;

//Tabela de índices
typedef struct tabela {
    FILE *arquivo_dados;
    arvore indices;
} tabela;


int inicializarTabela(tabela *tab);
void inicializar(arvore *raiz);
void finalizar(tabela *tab);

void adicionar(tipo_dado *valor, arvore *raiz);
void adicionarAluno(tabela *tab, dado *aluno);

dado * buscar_aluno(tabela *table, int id);

void remover (int valor, arvore *raiz);


//Função auxiliar para pegar os dados do aluno
dado *ler_dados();
void tirar_enter(char *string);

dado *procurarAluno(tabela *tab, int id);
void salvar_auxiliar(arvore raiz, FILE *arq);

//Arquivos
arvore carregar_arquivo(char *nome, arvore a);
void salvar_arquivo(char *nome, arvore a);

#endif