#ifndef AVL_H
#define AVL_H
#include <stdlib.h>

typedef struct indice_avl {
	int indice; 
	int codigo;
} indice_avl;

typedef struct no {
	indice_avl *index;
	int valor;
	int fb;
	struct no *esquerda, *direita;
} no;

typedef no* arvore;

void inicializar(arvore *raiz);
indice_avl* inicializar_indice_avl(int indice, int codigo);
int buscar_indice_avl(arvore raiz, int codigo);

arvore inserir_avl (indice_avl *v, arvore raiz, int *cresceu);
void pre_order_avl(arvore raiz);
void in_order_avl(arvore raiz);
void pos_order_avl(arvore raiz);
arvore busca_avl(int valor, arvore raiz);
arvore maior_avl(arvore raiz);
arvore menor_avl(arvore raiz);
arvore remover_avl(arvore raiz, int valor, int *diminuiu);
int altura_avl(arvore raiz);
int maximo(int valor1, int valor2);
//imprimir elemento
arvore rotacionar(arvore raiz);
arvore rotacao_simples_direita(arvore raiz);
arvore rotacao_simples_esquerda(arvore raiz);
arvore rotacao_dupla_direita(arvore raiz);
arvore rotacao_dupla_esquerda(arvore raiz);

#endif
