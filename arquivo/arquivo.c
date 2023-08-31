#include <stdio.h>
#include <stdlib.h>
#include "../cJSON/cJSON.h"

// Função para criar e preencher a estrutura de uma matéria
cJSON *criarMateria(int id, const char *nome, const char *periodo, const char *professor, double media) {
    cJSON *materia = cJSON_CreateObject();
    cJSON_AddNumberToObject(materia, "id", id);
    cJSON_AddStringToObject(materia, "nome-materia", nome);
    cJSON_AddStringToObject(materia, "periodo", periodo);
    cJSON_AddStringToObject(materia, "professor", professor);
    cJSON_AddNumberToObject(materia, "media", media);
    return materia;
}

// Função para criar e preencher a estrutura de um aluno
cJSON *criarAluno(int id, const char *nome, const char *dataNascimento, const char *sexo, const char *curso, cJSON *materias) {
    cJSON *aluno = cJSON_CreateObject();
    cJSON_AddNumberToObject(aluno, "id", id);
    cJSON_AddStringToObject(aluno, "nome", nome);
    cJSON_AddStringToObject(aluno, "data-de-nascimento", dataNascimento);
    cJSON_AddStringToObject(aluno, "sexo", sexo);
    cJSON_AddStringToObject(aluno, "curso", curso);
    cJSON_AddItemToObject(aluno, "matéria", materias);
    return aluno;
}

// Função para criar o JSON completo a partir dos dados
cJSON *criarJSON() {
    cJSON *root = cJSON_CreateObject();
    cJSON *alunoArray = cJSON_AddArrayToObject(root, "aluno");

    // Crie e adicione alunos com matérias
    cJSON *materias1 = cJSON_CreateArray();
    cJSON_AddItemToArray(materias1, criarMateria(0, "Matéria 1", "1º período", "Professor 1", 8));

    cJSON *aluno1 = criarAluno(0, "Nome Aluno 1", "01/01/2000", "Masculino", "Ciência da Computação", materias1);
    cJSON_AddItemToArray(alunoArray, aluno1);

    cJSON *materias2 = cJSON_CreateArray();
    cJSON_AddItemToArray(materias2, criarMateria(0, "Matéria 2", "2º período", "Professor 2", 7));
    cJSON_AddItemToArray(materias2, criarMateria(1, "Matéria 3", "2º período", "Professor 3", 9));

    cJSON *aluno2 = criarAluno(1, "Nome Aluno 2", "02/02/2001", "Feminino", "Engenharia Elétrica", materias2);
    cJSON_AddItemToArray(alunoArray, aluno2);

    return root;
}

void criarArquivoJson() {
    // Criar o JSON completo a partir dos dados
    cJSON *root = criarJSON();

    // Converta o objeto para uma string JSON formatada
    char *json_str = cJSON_Print(root);

    // Criar e abrir um arquivo para escrita
    FILE *arquivo = fopen("dados.json", "w");
    if (arquivo == NULL) {
        perror("Erro ao criar o arquivo");
        return 1;
    }

    // Escrever a string JSON no arquivo
    fprintf(arquivo, "%s", json_str);

    // Fechar o arquivo e liberar a memória do objeto JSON
    fclose(arquivo);
    cJSON_Delete(root);
    free(json_str);

}
