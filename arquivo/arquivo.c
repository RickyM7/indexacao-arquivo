#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

int inicializarArquivo(tabela *tab) {
	tab->arquivo_dados = fopen("alunos.json", "r+b");
	if(tab->arquivo_dados == NULL) {
		tab->arquivo_dados = fopen("alunos.json", "a+");
		return inicializarArquivo(tab);
	}
	tab->root_dados = carregarConteudoArquivoJson(tab->arquivo_dados, tab->root_dados);

	tab->indice = carregar_arquivo_index(tab);
	
    if(tab->arquivo_dados != NULL)
		return 1;
	else
		return 0;
}


int inserir_aluno(tabela *tab, dado *aluno, int *cresceu) {
    
    int posicaoNovoRegistro, retorno;
	retorno = tab->arquivo_dados != NULL;

	if(retorno) {
		fseek(tab->arquivo_dados, 0L, SEEK_END);
		
		posicaoNovoRegistro = ftell(tab->arquivo_dados);
		
		tab->indice = inserir_avl(inicializar_indice_avl(posicaoNovoRegistro, aluno->codigo), tab->indice, cresceu);
		
		salvar_aluno(tab->arquivo_dados, tab->root_dados, aluno);
	}

	return retorno;
}

dado* ler_dados() {

	dado* novo = (dado*) malloc(sizeof(dado));
	char * aux = (char *) malloc(256 * sizeof(char));
	
	printf("DADOS DO ALUNO\n");
	printf("Codigo: ");
	while (scanf("%d", &novo->codigo) != 1) {
		printf("Insira apenas numeros: ");
		while ((getchar()) != '\n');
	}
	while (getchar() != '\n');

    printf("Nome: ");
	fgets(aux, 256, stdin);

	char *token = strtok(aux, "\n");

	novo->nome = strdup(aux);

	free(aux);
	novo->removido = 0;

	return novo;
}

// Função para criar e preencher a estrutura de um aluno
cJSON *criarAluno(int id, char *nome, int removido) {
    cJSON *aluno = cJSON_CreateObject();
    cJSON_AddNumberToObject(aluno, "id", id);
    cJSON_AddNumberToObject(aluno, "removido", removido);
    cJSON_AddStringToObject(aluno, "nome", nome);
    return aluno;
}

// Função para criar o JSON completo a partir dos dados
cJSON *criarJSON(dado *aluno, cJSON *root) {

	cJSON *alunoArray = cJSON_GetObjectItemCaseSensitive(root, "aluno");
	if ( alunoArray == NULL)
	{
		//root = cJSON_CreateObject();
    	alunoArray = cJSON_AddArrayToObject(root, "aluno");
	}

    cJSON *a = criarAluno(aluno->codigo, aluno->nome, aluno->removido);
    cJSON_AddItemToArray(alunoArray, a);

    return root;
}



void salvar_aluno(FILE *arquivo, cJSON *root, dado *aluno) {
    criarJSON(aluno, root);

    // Converta o objeto para uma string JSON formatada
    char *json_str = cJSON_Print(root);
    fseek(arquivo, 0, SEEK_SET);
    fwrite(json_str, 1, strlen(json_str), arquivo);

    // Libere a memória
    free(json_str);

}


void remover_aluno(tabela *tab, dado *aluno, int *diminuiu, int chave) {
	if(tab != NULL) {
		dado aluno = buscar_aluno(tab->arquivo_dados, chave);
		if(!aluno.removido){
			tab->indice = remover_avl(tab->indice, aluno.codigo, diminuiu);
			fseek(tab->arquivo_dados, chave, SEEK_SET);
			char str[] = "1";
			fwrite(str, 1, sizeof(char), tab->arquivo_dados);
			fseek(tab->arquivo_dados, 0L, SEEK_END);
		}
		else
			printf("Erro ao remover\n");
	}
}


dado buscar_aluno(FILE *arquivo, int indice) {
    dado temp;
    if (indice >= 0 && arquivo != NULL) {

		fseek(arquivo, 0, SEEK_END);
		long tamanho_arquivo = ftell(arquivo);
		rewind(arquivo);

		if (tamanho_arquivo <= 0) {
		    printf("O arquivo JSON está vazio\n");
		    fclose(arquivo);
		    return temp;
		}

		char *conteudo = (char *)malloc(tamanho_arquivo + 1);

		if (conteudo == NULL) {
		    printf("Erro ao alocar memória\n");
		    fclose(arquivo);
		    return temp;
		}

		size_t bytes_lidos = fread(conteudo, 1, tamanho_arquivo, arquivo);
		conteudo[bytes_lidos] = '\0';

		if (bytes_lidos != tamanho_arquivo) {
		    printf("Erro ao ler o conteúdo do arquivo\n");
		    free(conteudo);
		    fclose(arquivo);
		    return temp;
		}

		cJSON *root = cJSON_Parse(conteudo);

		if (root == NULL) {
		    printf("Erro ao analisar o conteúdo JSON\n");
		    free(conteudo);
		    fclose(arquivo);
		    return temp;
		}

        cJSON *alunoArray = cJSON_GetObjectItemCaseSensitive(root, "aluno");
        if (alunoArray != NULL && cJSON_IsArray(alunoArray)) {
            cJSON *aluno = cJSON_GetArrayItem(alunoArray, indice);
            if (aluno != NULL) {
                temp.removido = cJSON_GetObjectItemCaseSensitive(aluno, "removido")->valueint;
                temp.codigo = cJSON_GetObjectItemCaseSensitive(aluno, "id")->valueint;
                strcpy(temp.nome, cJSON_GetObjectItemCaseSensitive(aluno, "nome")->valuestring);
            } else {
                printf("Aluno não encontrado\n");
            }
        } else {
            printf("Erro ao buscar aluno\n");
        }

        cJSON_Delete(root);
        return temp;
    } else {
        printf("Arquivo ou índice inválido\n");
        temp.removido = 1;
        return temp;
    }
}

void imprimir_elementos(dado aluno){
	printf("ALUNO\n");
	printf("Codigo: %d\n", aluno.codigo);
	printf("Nome: %s\n", aluno.nome);
	printf("\n");
}

void listar_por_codigo(FILE *arquivo, arvore raiz) {
	if(raiz != NULL) {
		listar_por_codigo(arquivo, raiz->esquerda);
		printf("\n_______________________________\n");
		imprimir_elementos(buscar_aluno(arquivo, raiz->index->indice));
		listar_por_codigo(arquivo, raiz->direita);
	}
}


arvore carregar_arquivo_index(tabela *tab) {
    FILE *arquivo;
	size_t len;
	char nome[12], *linha = (char*) malloc(len), delim[] = "|";
	strcpy(nome, "indices.txt");

	arquivo = fopen(nome, "r+");
	if(arquivo == NULL) {
		arquivo = fopen(nome, "w");
	}

	// fseek(arquivo, 0, SEEK_END);

    if(arquivo != NULL){
		if(ftell(arquivo) == 0){
			fclose(arquivo);
			return NULL;
		}
		while(getdelim(&linha, &len, ',', arquivo) > 0){
  			char *ptr;
			ptr = strtok(linha, delim);
			int indice = atoi(ptr);
			ptr = strtok(NULL, delim);
            tab->indice = inserir_avl(inicializar_indice_avl(indice, atoi(ptr)),tab->indice, 0);
        }
		fclose(arquivo);
 	}
 	free(linha);
	return tab->indice;
}

cJSON *carregarConteudoArquivoJson(FILE *arquivo, cJSON *root) {
    root = cJSON_CreateObject();

    fseek(arquivo, 0, SEEK_END);
    long tamanho_arquivo = ftell(arquivo);

    if (tamanho_arquivo > 0) {
        rewind(arquivo);

        char *conteudo = (char *)malloc(tamanho_arquivo + 1);
        fread(conteudo, 1, tamanho_arquivo, arquivo);
        conteudo[tamanho_arquivo] = '\0';

        root = cJSON_Parse(conteudo);

        free(conteudo);
    }

    return root;
}

void salvar_arquivo(char *nome, arvore raiz) {
	FILE *arquivo;
	arquivo = fopen(nome, "a+");
	if(arquivo != NULL) {
		salvar_auxiliar(raiz, arquivo);
		fclose(arquivo);
	}
}

void salvar_auxiliar(arvore raiz, FILE *arquivo){
	if(raiz != NULL) {
		
		salvar_auxiliar(raiz->esquerda, arquivo);
		fprintf(arquivo, "%d|%d,", raiz->index->indice, raiz->index->codigo);
		salvar_auxiliar(raiz->direita, arquivo);
	}

}

void finalizar_arquivo(tabela *tab) {
    // Salve o JSON no arquivo
    char *json_str = cJSON_Print(tab->root_dados);
    fseek(tab->arquivo_dados, 0, SEEK_SET);
    fwrite(json_str, 1, strlen(json_str), tab->arquivo_dados);

    // Libere a memória
    free(json_str);
    cJSON_Delete(tab->root_dados);

    // Feche o arquivo e salve os indices
    fclose(tab->arquivo_dados);
    salvar_arquivo("indices.txt", tab->indice);
}