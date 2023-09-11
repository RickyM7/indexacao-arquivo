#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arquivo.h"

int inicializarArquivo(tabela *tab, int *cresceu) {
	tab->arquivo_dados = fopen("alunos.json", "r+b");
	if(tab->arquivo_dados == NULL) {
		tab->arquivo_dados = fopen("alunos.json", "a+");
		return inicializarArquivo(tab, cresceu);
	}
	tab->root_dados = carregarConteudoArquivoJson(tab->arquivo_dados, tab->root_dados);

	tab->indice = carregar_arquivo_index(tab, cresceu);
	
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

	printf("Idade: ");
	while (scanf("%d", &novo->idade) != 1) {
        printf("Insira apenas numeros: ");
        while ((getchar()) != '\n');
    }
    while (getchar() != '\n');

	printf("Data de Nascimento (Formato dd/mm/yyyy): ");
    fgets(novo->data_nascimento, sizeof(novo->data_nascimento), stdin);
    strtok(novo->data_nascimento, "\n");

	printf("CEP: ");
	while (scanf("%d", &novo->CEP) != 1) {
        printf("Insira apenas numeros: ");
        while ((getchar()) != '\n');
    }
    while (getchar() != '\n');

	printf("Telefone: ");
    scanf("%lld", &novo->telefone);
    while (getchar() != '\n');

	printf("Email: ");
    fgets(aux, 256, stdin);
    token = strtok(aux, "\n");
    novo->email = strdup(aux);

	printf("Nome dos Pais: ");
    fgets(aux, 256, stdin);
    token = strtok(aux, "\n");
    novo->nome_pais = strdup(aux);

	novo->removido = 0;
	free(aux);

	return novo;
}

// Função para criar e preencher a estrutura de um aluno
cJSON *criarAluno(int id, char *nome, int removido, int idade, char *data_nascimento, int CEP,long long int telefone, char *email, char *nome_pais) {
    cJSON *aluno = cJSON_CreateObject();
    cJSON_AddNumberToObject(aluno, "id", id);
    cJSON_AddNumberToObject(aluno, "removido", removido);
    cJSON_AddStringToObject(aluno, "nome", nome);
	cJSON_AddNumberToObject(aluno, "idade", idade);
    cJSON_AddStringToObject(aluno, "data_nascimento", data_nascimento);
    cJSON_AddNumberToObject(aluno, "CEP", CEP);
    cJSON_AddNumberToObject(aluno, "telefone", telefone);
    cJSON_AddStringToObject(aluno, "email", email);
    cJSON_AddStringToObject(aluno, "nome_pais", nome_pais);
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

    cJSON *a = criarAluno(aluno->codigo, aluno->nome, aluno->removido, aluno->idade, aluno->data_nascimento, aluno->CEP, aluno->telefone, aluno->email, aluno->nome_pais);
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

char *limpar_linha(const char *linha_original) {
    char *linha = strdup(linha_original);  // Crie uma cópia da linha original
    if (linha == NULL) {
        printf("Erro ao alocar memória\n");
        return NULL;
    }

    while (isspace((unsigned char)(*linha))) {
        linha++;
    }

    char *fim = linha + strlen(linha) - 1;
    while (fim >= linha && (isspace((unsigned char)(*fim)) || iscntrl((unsigned char)(*fim)))) {
        *fim = '\0';
        fim--;
    }

    return linha;
}

dado buscar_aluno(FILE *arquivo, int indice) {
    dado temp;
    if (indice >= 0 && arquivo != NULL) {
        size_t tamanho_buffer = 256;
        char *linha = (char *)malloc(tamanho_buffer);
        if (linha == NULL) {
            printf("Erro ao alocar memória\n");
            return temp;
        }

        fseek(arquivo, indice, SEEK_SET);

        int estado = 0;  // 0 = procurando por "aluno", 1 = procurando por elementos dentro do array "aluno"
		while (fgets(linha, tamanho_buffer, arquivo) != NULL) {
		    char *cleaned_line = limpar_linha(linha);
		
		    if (estado == 0) {
		        if (strstr(cleaned_line, "\"aluno\":") != NULL) {
		            estado = 1;  // Encontrou "aluno", mude para o estado de procurar elementos dentro de "aluno"
		        }
		    } else if (estado == 1) {
		        if (strstr(cleaned_line, "\"id\":") != NULL) {
		            temp.codigo = atoi(strstr(cleaned_line, ":") + 1);
		        }
		
		        if (strstr(cleaned_line, "\"removido\":") != NULL) {
		            temp.removido = atoi(strstr(cleaned_line, ":") + 1);
		        }
		
		        if (strstr(cleaned_line, "\"nome\":") != NULL) {
		            char *valor_nome = strstr(cleaned_line, ":") + 3;
    				size_t tamanho_nome = strlen(valor_nome) + 1; // +1 para o caractere nulo
    				temp.nome = (char *)malloc(tamanho_nome);

    				if (temp.nome != NULL) {
    				    strcpy(temp.nome, valor_nome);
    				    char *aspas_fim = strchr(temp.nome, '"');
    				    if (aspas_fim != NULL) {
    				        *aspas_fim = '\0';
    				    }
    				} else {
    				    printf("Erro ao alocar memória para temp.nome\n");
    				}
		
		            // Retorne ao estado de procurar "aluno" após encontrar um elemento completo
		            estado = 0;
		        }
		    }
		}

        free(linha);
        return temp;
    } else {
        printf("Posição de arquivo ou arquivo inválido\n");
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


arvore carregar_arquivo_index(tabela *tab, int *cresceu) {
    FILE *arquivo;
	size_t len;
	char nome[12], *linha = (char*) malloc(len), delim[] = "|";
	strcpy(nome, "indices.txt");

	arquivo = fopen(nome, "a+");

	fseek(arquivo, 0, SEEK_SET);

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
            tab->indice = inserir_avl(inicializar_indice_avl(indice, atoi(ptr)),tab->indice, cresceu);
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