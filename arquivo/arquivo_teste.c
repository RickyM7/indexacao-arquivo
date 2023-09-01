#include <stdio.h>
#include <stdlib.h>
#include "arquivo_teste.h"
#include <string.h>

int inicializarTabela(tabela *tab){
    inicializar(&tab->indices);

    tab->arquivo_dados = fopen(DATABASE_FILE, "a+");
    tab->indices = carregar_arquivo(INDEXES_FILE, tab->indices);

    return tab->arquivo_dados != NULL ? 1 : 0;
}

dado * ler_dados() {
    dado *novo = (dado *) malloc(sizeof(dado));
    char *buffer = (char *) malloc(sizeof(char ) * 256);

    getchar();
    printf("ID: ");
    scanf("%d", &novo->id);
    getchar();

    printf("Curso: ");
    fgets(buffer, 255,  stdin);
    tirar_enter(buffer);
    novo->curso = strdup(buffer);

    printf("Nome: ");
    fgets(buffer, 255,  stdin);
    tirar_enter(buffer);
    novo->nome = strdup(buffer);

    printf("Ano: ");
    scanf("%d", &novo->ano);
    getchar();

    free(buffer);

    return novo;
}

void adicionarAluno (tabela *tabela, dado *aluno){
    //Significa que o arquivo foi aberto com sucesso
    if(tabela->arquivo_dados != NULL){
        //Cria um novo índice
        tipo_dado * indice = (tipo_dado *) malloc(sizeof (tipo_dado));
        indice->id = aluno->id;
        fseek(tabela->arquivo_dados, 0L, SEEK_END);

        //Procura a posição final do arquivo e adiciona ao índice
        indice->indice = ftell(tabela->arquivo_dados);

        //Estratégia para salvar o aluno
        char * format = "%d|%s|%s|%d\n";
        fprintf(tabela->arquivo_dados, format, aluno->id, aluno->curso, aluno->nome, aluno->ano);

        adicionar(indice, &tabela->indices);
    }
}

void tirar_enter(char *string) {
    string[strlen(string) -1] = '\0';
}

void finalizar (tabela *tab) {
    fclose(tab->arquivo_dados);
    salvar_arquivo(INDEXES_FILE, tab->indices);
}

void salvar_arquivo(char *nome, arvore a) {
    FILE *arq;
    arq = fopen(nome, "wb");
    if(arq != NULL) {
        salvar_auxiliar(a, arq);
        fclose(arq);
    }
}

void salvar_auxiliar(arvore raiz, FILE *arq){
    if(raiz != NULL) {
        if(arq != NULL){
            fwrite(raiz->valor, sizeof(tipo_dado), 1, arq);
            salvar_auxiliar(raiz->esquerda, arq);
            salvar_auxiliar(raiz->direita, arq);
        }
    }
}

dado * buscar_aluno(tabela *table, int id){
    if (table->arquivo_dados != NULL) {
        arvore temp = table->indices;

        while (temp != NULL) {
            if (temp->valor->id == id) {
                dado *aluno = (dado *) malloc(sizeof(dado));
                char *buffer = (char *) malloc(456 * sizeof(char));

                fseek(table->arquivo_dados, temp->valor->indice, SEEK_SET);
                fgets(buffer, 456, table->arquivo_dados);

                char *tempString;

                tempString = strtok(buffer, "|");
                aluno->id = atoi(tempString) ;
                tempString = strtok(NULL, "|");
                aluno->curso = strdup(tempString);
                tempString = strtok(NULL, "|");
                aluno->nome = strdup(tempString);
                tempString = strtok(NULL, "|");
                aluno->ano = atoi(tempString);

                free(buffer);

                return aluno;
            } else {
                if (id > temp->valor->id)
                    temp = temp->direita;
                else
                    temp = temp->esquerda;
            }
        }
    }

    return NULL;
}

arvore carregar_arquivo(char *nome, arvore a) {
    FILE *arq;
    arq = fopen(nome, "rb");
    tipo_dado * temp;
    if(arq != NULL) {
        temp = (tipo_dado *) malloc(sizeof(tipo_dado));

        while(fread(temp, sizeof(tipo_dado), 1, arq)) {
            adicionar(temp, &a);
            temp = (tipo_dado *) malloc(sizeof(tipo_dado));
        }
        fclose(arq);
    }

    return a;
}

void imprimir_elemento(arvore raiz, tabela *tab) {
    dado * temp = (dado *) malloc (sizeof(dado));
	fseek(tab->arquivo_dados, raiz->valor->indice, SEEK_SET);
	fread(temp, sizeof(dado), 1, tab->arquivo_dados);
	printf("[id : %d, curso: %s, nome: %s, ano: %d ]\n", raiz->valor->id, temp->curso, temp->nome, temp->ano);
	free(temp);
}

//Funções padrões da AVL...