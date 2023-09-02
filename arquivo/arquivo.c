#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

int inicializarArquivo(tabela *tab) {
	tab->arquivo_dados = fopen("teste.json", "r+b");
	tab->indice = NULL;

    carregarArquivo(tab);
	
    if(tab->arquivo_dados != NULL)
		return 1;
	else
		return 0;
}

int inserir_aluno(tabela *tab, dado *aluno, int *cresceu) {
    
    int posicaoNovoRegistro, retorno;
	retorno = tab->arquivo_dados != NULL;

    printf("%d", retorno);

	if(retorno) {
        printf("Inserir aluno entrou: ");
		fseek(tab->arquivo_dados, 0L, SEEK_END);
		
		posicaoNovoRegistro = ftell(tab->arquivo_dados);
		
		tab->indice = inserir_avl(inicializar_indice_avl(posicaoNovoRegistro, aluno->codigo), tab->indice, cresceu);
		
		salvar_aluno(tab->arquivo_dados, aluno);
	}

	return retorno;
}

dado* ler_dados() {

	dado* novo = (dado*) malloc(sizeof(dado));
	char * aux = (char *) malloc(256 * sizeof(char));

	printf("Codigo: ");
	while (scanf("%d", &novo->codigo) != 1) {
		printf("Insira apenas numeros: ");
		while ((getchar()) != '\n');
	}
	while (getchar() != '\n');

    printf("Nome: ");
	fgets(aux, 256, stdin);
	novo->nome = strdup(aux);

	printf("Materia: \n");
    printf("Nome: ");
	scanf(" %[^\n]%*c", novo->materia.nome);

	printf("Media: ");
	while (scanf("%f", &novo->materia.media) != 1) {
		printf("Insira apenas numeros: ");
		while ((getchar()) != '\n');
	}
	while (getchar() != '\n');

	free(aux);
	novo->removido = 0;

	return novo;
}

void salvar_aluno(FILE *arq, dado *aluno) {
    criarArquivoJson(aluno, arq);
}

// Função para criar e preencher a estrutura de uma matéria
cJSON *criarMateria(char nome[50], float media) {
    cJSON *materia = cJSON_CreateObject();
    cJSON_AddStringToObject(materia, "nome-materia", nome);
    cJSON_AddNumberToObject(materia, "media", media);
    return materia;
}

// Função para criar e preencher a estrutura de um aluno
cJSON *criarAluno(int id, char *nome, int removido, cJSON *materias) {
    cJSON *aluno = cJSON_CreateObject();
    cJSON_AddNumberToObject(aluno, "id", id);
    cJSON_AddNumberToObject(aluno, "removido", removido);
    cJSON_AddStringToObject(aluno, "nome", nome);
    cJSON_AddItemToObject(aluno, "matéria", materias);
    return aluno;
}

// Função para criar o JSON completo a partir dos dados
cJSON *criarJSON(dado *aluno) {
    cJSON *root = cJSON_CreateObject();
    cJSON *alunoArray = cJSON_AddArrayToObject(root, "aluno");

    // Crie e adicione alunos com matérias
    cJSON *materia = cJSON_CreateArray();
    cJSON_AddItemToArray(materia, criarMateria(aluno->materia.nome, aluno->materia.media));

    cJSON *a = criarAluno(aluno->codigo, aluno->nome, aluno->removido, materia);
    cJSON_AddItemToArray(alunoArray, a);

    return root;
}

void criarArquivoJson(dado *aluno, FILE *arquivo) {
    // Criar o JSON completo a partir dos dados
    cJSON *root = criarJSON(aluno);

    // Converta o objeto para uma string JSON formatada
    char *json_str = cJSON_Print(root);

    // Escrever a string JSON no arquivo
    fprintf(arquivo, "%s", json_str);

    // Fechar o arquivo e liberar a memória do objeto JSON
    fclose(arquivo);
    cJSON_Delete(root);
    free(json_str);

}

void carregarArquivo(tabela *tab) {
    FILE *arq;
	size_t len = 256;
	char nome[16], *linha = (char*) malloc(len), delim[] = ";";
	strcpy(nome, "indices_avl.dat");

    printf("index\n\n");
	arq = fopen(nome, "r+");
    
    if(arq != NULL){
		while(getdelim(&linha, &len, '\n', arq) > 0){
  			char *ptr;
			ptr = strtok(linha, delim);
			int indice = atoi(ptr);
			ptr = strtok(NULL, delim);
			//tirar_enter(ptr);
            tab->indice = inserir_avl(inicializar_indice_avl(indice, atoi(ptr)),tab->indice, 0);
            printf("aloooooooo\n");
        }
		fclose(arq);
 	}
 	free(linha);
}
