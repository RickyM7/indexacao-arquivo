#include "arquivo.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int inicializarArquivo(tabela *tab, int *cresceu) {
    tab->arquivo_dados = fopen("alunos.txt", "r+");
    if (tab->arquivo_dados == NULL) {
        tab->arquivo_dados = fopen("alunos.txt", "w+");
        if (tab->arquivo_dados == NULL) {
            return 0;  // Erro na abertura do arquivo
        }
    }

    tab->indice = carregar_arquivo_index(tab, cresceu);  // carrega o arquivo de indices

    return 1;
}

int inserir_aluno(tabela *tab, dado *aluno, int *cresceu) {
    if (aluno == NULL) {
        return 0;
    }

    int posicaoNovoRegistro, retorno;
    retorno = tab->arquivo_dados != NULL;

    if (retorno) {
        fseek(tab->arquivo_dados, 0L, SEEK_END);

        posicaoNovoRegistro = ftell(tab->arquivo_dados);

        tab->indice = inserir_avl(inicializar_indice_avl(posicaoNovoRegistro, aluno->codigo), tab->indice, cresceu);

        salvar_aluno(tab->arquivo_dados, aluno);
    }

    return retorno;
}

int aluno_ja_existe(tabela *tab, int codigo) {
    // Verifica se o aluno já existe na árvore
    if (buscar_indice_avl(tab->indice, codigo) >= 0) {
        printf("Um aluno com o codigo %d ja existe! Escolha outro codigo.\n", codigo);
        return 1;
    }
    return 0;
}

dado *ler_dados(tabela *tab) {
    dado *novo = (dado *)malloc(sizeof(dado));

    printf("DADOS DO ALUNO\n");
    printf("Codigo: ");
    scanf("%d", &novo->codigo);
    if (aluno_ja_existe(tab, novo->codigo)) {
        return NULL;
    }

    printf("Nome: ");
    scanf("%s", novo->nome);

    printf("Idade: ");
    scanf("%d", &novo->idade);

    printf("Data de Nascimento (Formato dd/mm/yyyy): ");
    scanf("%s", novo->data_nascimento);

    printf("CEP: ");
    scanf("%d", &novo->CEP);

    printf("Nacionalidade: ");
    scanf("%s", novo->nacionalidade);

    printf("Telefone: ");
    scanf("%lld", &novo->telefone);

    printf("Email: ");
    scanf("%s", novo->email);

    novo->removido = 0;

    return novo;
}

void salvar_aluno(FILE *arquivo, dado *aluno) {
    fprintf(arquivo, "%d|%s|%d|%s|%d|%s|%lld|%s\n", aluno->codigo, aluno->nome, aluno->idade, aluno->data_nascimento, aluno->CEP, aluno->nacionalidade, aluno->telefone, aluno->email);
}

dado buscar_aluno(tabela tab, int codigo) {
    // Verifica se o arquivo de índices está vazio
    FILE *indiceArquivo = fopen("indices.txt", "r");
    if (indiceArquivo != NULL) {
        fseek(indiceArquivo, 0, SEEK_END);
        long tamanho = ftell(indiceArquivo);
        fclose(indiceArquivo);

        if (tamanho == 0) {
            dado aluno_vazio;
            aluno_vazio.removido = 1;  // Marca como aluno não encontrado
            return aluno_vazio;
        }
    }

    int indice = buscar_indice_avl(tab.indice, codigo);

    if (indice >= 0) {
        // Encontrou o índice na árvore AVL, agora vamos buscar no arquivo de dados
        FILE *dadosArquivo = tab.arquivo_dados;
        rewind(dadosArquivo);

        dado temp;
        while (fscanf(dadosArquivo, "%d|%[^|]|%d|%[^|]|%d|%[^|]|%lld|%s\n", &temp.codigo, temp.nome, &temp.idade, temp.data_nascimento, &temp.CEP, temp.nacionalidade, &temp.telefone, temp.email) != EOF) {
            if (temp.codigo == codigo && !temp.removido) {
                return temp;
            }
        }
    }

    dado aluno_nao_encontrado;
    aluno_nao_encontrado.removido = 1;  // Marca como aluno não encontrado
    return aluno_nao_encontrado;
}

arvore carregar_arquivo_index(tabela *tab, int *cresceu) {
    FILE *arquivo;
    arquivo = fopen("indices.txt", "a+");
    if (arquivo != NULL) {
        rewind(arquivo);

        arvore raiz = NULL;
        int indice, codigo;
        while (fscanf(arquivo, "%d|%d,", &indice, &codigo) != EOF) {
            // Inserir o índice na árvore AVL
            raiz = inserir_avl(inicializar_indice_avl(indice, codigo), raiz, cresceu);
        }

        fclose(arquivo);
        return raiz;
    }

    return NULL;  // Erro ao abrir o arquivo de índices
}

void salvar_arquivo(char *nome, arvore raiz) {
    FILE *arquivo = fopen(nome, "w");
    if (arquivo != NULL) {
        salvar_auxiliar(raiz, arquivo);
        fclose(arquivo);
    }
}

void salvar_auxiliar(arvore raiz, FILE *arquivo) {
    if (raiz != NULL) {
        salvar_auxiliar(raiz->esquerda, arquivo);
        fprintf(arquivo, "%d|%d,", raiz->index->indice, raiz->index->codigo);
        salvar_auxiliar(raiz->direita, arquivo);
    }
}

void finalizar_arquivo(tabela *tab) {
    fclose(tab->arquivo_dados);
    salvar_arquivo("indices.txt", tab->indice);
}

void imprimir_elementos(dado aluno) {
    printf("INFOS DO ALUNO\n");
    printf("Codigo: %d\n", aluno.codigo);
    printf("Nome: %s\n", aluno.nome);
    printf("Idade: %d (%s)\n", aluno.idade, aluno.data_nascimento);
    printf("CEP: %d\n", aluno.CEP);
    printf("Nacionalidade: %s\n", aluno.nacionalidade);
    printf("Telefone: %lld\n", aluno.telefone);
    printf("Email: %s\n", aluno.email);
    printf("\n");
}
