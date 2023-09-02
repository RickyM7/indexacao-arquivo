#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

void inicializar(arvore *raiz) {
	*raiz = NULL;
}

indice_avl* inicializar_indice_avl(int indice, int codigo) {
	indice_avl *novo = (indice_avl*) malloc(sizeof(indice_avl));
	novo->indice = indice;
	novo->codigo = codigo;
	return novo;
}

int buscar_indice_avl (arvore raiz, int codigo) {
	if(raiz != NULL) {
		if(raiz->index->codigo == codigo) {
			return raiz->index->indice;
		}
		else {
			if(raiz->index->codigo > codigo)
				return buscar_indice_avl(raiz->esquerda, codigo);
			else 
				return buscar_indice_avl(raiz->direita, codigo);
		}
	} else {
		return -1;
	}
}

void imprimir_elemento(arvore raiz) {
	printf("%d [%d]", raiz->valor, raiz->fb);
}

arvore inserir_avl (indice_avl *v, arvore raiz, int *cresceu){
	if (raiz == NULL){
		arvore nova = (arvore) malloc(sizeof(struct no));
		
		nova->esquerda = NULL;
		nova->direita = NULL;
		nova->index = v;
		nova->fb = 0;

		*cresceu = 1;
		
		return nova;
	} else {
		if (v->codigo > raiz->index->codigo){ 
			raiz->direita = inserir_avl(v, raiz->direita, cresceu);

			if (*cresceu){
				switch(raiz->fb){
					case 0:
						raiz->fb = 1;
						*cresceu = 1;
						break;
					case -1:
						raiz->fb = 0;
						*cresceu = 0;
						break;
					case 1:
						*cresceu = 0;
						return rotacionar(raiz);
				}
			}

		} else {
			raiz->esquerda = inserir_avl(v, raiz->esquerda, cresceu);

			if (*cresceu){
				switch(raiz->fb){
					case 0:
						raiz->fb = -1;
						*cresceu = 1;
						break;
					case 1:
						raiz->fb = 0;
						*cresceu = 0;
						break;
					case -1:
						*cresceu = 0;
						return rotacionar(raiz);
				}
			}
		}
		return raiz;
	}
}

arvore rotacionar(arvore raiz) {
	if (raiz->fb > 0){
		switch(raiz->direita->fb) {
			case 0:
				return rotacao_simples_esquerda(raiz);
			case 1:
				return rotacao_simples_esquerda(raiz);
			case -1:
				return rotacao_dupla_esquerda(raiz);
		}
	} else {
		switch(raiz->esquerda->fb) {
			case 0:
				return rotacao_simples_direita(raiz);
			case 1:
				return rotacao_dupla_direita(raiz);
			case -1:
				return rotacao_simples_direita(raiz);
		}
	}
}

arvore rotacao_simples_direita(arvore raiz) {
	arvore pivo, u, t1, t2, t3;

	pivo = raiz;
	u = raiz->esquerda;
	t3 = raiz->direita;
	t2 = u->direita;
	t1 = u->esquerda;

	pivo->esquerda = t2;
	u->direita = pivo;

	if(u->fb == 1) {
		pivo->fb = 0;
		u->fb = 0;
	} else {
		pivo->fb = -1;
		u->fb = 1;
	}

	return u;
}

arvore rotacao_simples_esquerda(arvore raiz) {
	// u é a arvore resultante
	arvore pivo, u, t1, t2, t3;

	pivo = raiz;
	u = raiz->direita;
	t1 = raiz->esquerda;
	t2 = u->esquerda;
	t3 = u->direita;

	pivo->direita = t2;
	u->esquerda = pivo;

	if(u->fb == 1) {
		pivo->fb = 0;
		u->fb = 0;
	} else {
		pivo->fb = 1;
		u->fb = -1;
	}	

	return u;
}

arvore rotacao_dupla_direita(arvore raiz){
	arvore pivo, u, v, t1, t2, t3, t4;

	pivo = raiz;
	u = pivo->esquerda;
	v = u->direita;
	t1 = pivo->direita;
	t2 = v->esquerda;
	t3 = v->direita;
	t4 = u->esquerda;

	v->direita = pivo;
	pivo->esquerda = t3;
	v->esquerda = u;
	u->direita = t2;

	switch(v->fb) {
        case -1:
			pivo->fb = 1;
            u->fb = 0;
			break;
        case 0:
			pivo->fb = 0;
			u->fb = 0;
            break;
        case 1:
			u->fb = 1;
            pivo->fb = 0;
			break;
    }

    return v;
}

arvore rotacao_dupla_esquerda(arvore raiz){
	// v é a arvore resultante

	arvore pivo, u, v, t1, t2, t3, t4;

	pivo = raiz;
	u = pivo->direita;
	v = u->esquerda;
	t1 = pivo->esquerda;
	t2 = v->esquerda;
	t3 = v->direita;
	t4 = u->direita;

	v->esquerda = pivo;
	pivo->direita = t2;
	v->direita = u;
	u->esquerda = t3;

	switch(v->fb) {
        case -1:
            u->fb = 1;
			pivo->fb = 0;
			break;
        case 0:
			pivo->fb = 0;
			u->fb = 0;
            break;
        case 1:
            pivo->fb = -1;
			u->fb = 0;
			break;
    }

    return v;
}

void pre_order_avl(arvore raiz){
	if (raiz != NULL){ 
		imprimir_elemento(raiz);
		pre_order_avl(raiz->esquerda);
		pre_order_avl(raiz->direita);
	}
}

void in_order_avl(arvore raiz){
	if (raiz != NULL){
		in_order_avl(raiz->esquerda);
		imprimir_elemento(raiz);
		in_order_avl(raiz->direita);
	}
}

void pos_order_avl(arvore raiz){
	if (raiz != NULL){
		pos_order_avl(raiz->esquerda);
		pos_order_avl(raiz->direita);
		imprimir_elemento(raiz);
	}
}

arvore busca_avl(int valor, arvore raiz) {
	if (raiz != NULL){
		if (valor == raiz->valor) {
			return raiz;
		}

		if (raiz->direita == NULL && raiz->esquerda == NULL) {
			return NULL;
		}

		if (valor > raiz->valor) {
			if (raiz->direita != NULL){
				return busca_avl(valor, raiz->direita);
			}
		} else {
			if (raiz->esquerda != NULL) {
				return busca_avl(valor, raiz->esquerda);
			}
		}
	}
}

arvore maior_avl(arvore raiz) {
	if (raiz != NULL){
		if (raiz->direita != NULL){
			return maior_avl(raiz->direita);
		}
		return raiz;
	}
}

/* arvore menor_avl (arvore raiz)*/

arvore remover_avl(arvore raiz, int valor, int *diminuiu) {

    if(raiz == NULL) {
        return raiz;
    }
    
    if(raiz->valor == valor) {
        //Verificação de 0 filhos, 1 filho, 2 filhos
        //caso 1 - zero filhos
        if(raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }
        //caso 2 (a): exatamente um filho esquerdo
        if(raiz->esquerda != NULL && raiz->direita == NULL) {
            arvore aux = raiz->esquerda;
            free(raiz);
			*diminuiu = 1;
            return aux;
        }

        //caso 2 (b): exatamente um filho direito
        if(raiz->esquerda == NULL && raiz->direita != NULL) {
            arvore aux = raiz->direita;
            free(raiz);
			*diminuiu = 1;
            return aux;
        }

        //caso 3: dois filhos
        //não precisa de if

        int maiorValorSubarvoreEsquerda = maior_avl(raiz->esquerda)->valor;
        raiz->valor = maiorValorSubarvoreEsquerda;
        raiz->esquerda = remover_avl(raiz->esquerda, maiorValorSubarvoreEsquerda, diminuiu);

        return raiz;
    }

    if(valor > raiz->valor) {
       	raiz->direita = remover_avl(raiz->direita, valor, diminuiu);

		if (*diminuiu){
				switch(raiz->fb){
					case 0:
						raiz->fb = -1;
						*diminuiu = 0;
						break;
					case 1:
						raiz->fb = 0;
						*diminuiu = 1;
						break;
					case -1:
						return rotacionar(raiz);
				}
			}
    } else {
       	raiz->esquerda = remover_avl(raiz->esquerda, valor, diminuiu);

		if (*diminuiu){
				switch(raiz->fb){
					case 0:
						raiz->fb = 1;
						*diminuiu = 0;
						break;
					case -1:
						raiz->fb = 0;
						*diminuiu = 1;
						break;
					case 1:
						return rotacionar(raiz);
				}
			}
    }

    return raiz;
}

int maximo(int valor1, int valor2) {
	return valor1 > valor2 ? valor1 : valor2;
}

int altura_avl(arvore raiz){
	if (raiz != NULL){
		return 1 + maximo(altura_avl(raiz->direita), altura_avl(raiz->esquerda));
	}
}

