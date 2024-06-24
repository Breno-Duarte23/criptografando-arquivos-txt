#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void criptografar_txt(const char *filename, char key) {
    FILE *file = fopen(filename, "r+b");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    printf("Criptografando arquivo...\n");

    int c;
    while ((c = fgetc(file)) != EOF) {
        fseek(file, -1, SEEK_CUR);
        fputc(c ^ key, file);
        fflush(file);
    }

    fclose(file);
    printf("Texto criptografado!!\n");
}

char* gerar_nome(int min_length, int max_length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int charset_size = sizeof(charset) - 1;

    srand(time(NULL));

    int length = min_length + rand() % (max_length - min_length + 1);

    char *filename = (char *)malloc((length + 1) * sizeof(char));
    if (!filename) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < length; i++) {
        int key = rand() % charset_size;
        filename[i] = charset[key];
    }

    filename[length] = '\0';

    return filename;
}

void gravar_texto(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    char input[1024];
    printf("Digite o texto a ser salvo no arquivo (pressione Enter para terminar):\n");

    if (fgets(input, sizeof(input), stdin) != NULL) {
        fprintf(file, "%s", input);
        printf("Texto salvo no arquivo: %s\n", filename);
    } else {
        fprintf(stderr, "Erro ao ler a entrada do usuário.\n");
    }

    fclose(file);
}

void descriptografar_txt(const char *filename, char key) {
    criptografar_txt(filename, key);
    printf("Texto descriptografado!!\n");
}

void ler_e_descriptografar_arquivo(const char *filename, char key) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    printf("Descriptografando e lendo o arquivo...\n");

    int c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c ^ key);
    }

    fclose(file);
    printf("\nLeitura completa do arquivo %s.\n", filename);
}

void editar_texto(const char *filename, char key) {
    descriptografar_txt(filename, key);

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    printf("Conteúdo atual do arquivo:\n");
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    fclose(file);

    printf("\nDigite o novo texto para o arquivo (pressione Enter para terminar):\n");
    char novo_texto[1024];
    if (fgets(novo_texto, sizeof(novo_texto), stdin) != NULL) {
        file = fopen(filename, "w");
        if (!file) {
            fprintf(stderr, "Erro ao abrir o arquivo.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%s", novo_texto);
        fclose(file);
    } else {
        fprintf(stderr, "Erro ao ler a entrada do usuário.\n");
    }

    criptografar_txt(filename, key);
}

void adicionar_texto(const char *filename, char key) {
    descriptografar_txt(filename, key);

    FILE *file = fopen(filename, "a");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    printf("Digite o texto a ser adicionado ao arquivo (pressione Enter para terminar):\n");
    char novo_texto[1024];
    if (fgets(novo_texto, sizeof(novo_texto), stdin) != NULL) {
        fprintf(file, "%s", novo_texto);
        printf("Texto adicionado ao arquivo: %s\n", filename);
    } else {
        fprintf(stderr, "Erro ao ler a entrada do usuário.\n");
    }

    fclose(file);

    criptografar_txt(filename, key);
}

int main() {
    int opcao, min_length = 4, max_length = 10;
    char key = 'ç';

    do {
        printf("\n-------------------\n");
        printf("Digite o número da ação que deseja realizar\n");
        printf("0- Sair\n");
        printf("1- Criar arquivo\n");
        printf("2- Ler arquivo\n");
        printf("3- Salvar arquivo\n");
        printf("4- Reescrever arquivo\n");
        printf("5- Adicionar texto ao arquivo\n");
        printf("\n-------------------\n");
        scanf("%d", &opcao);
        getchar();

        switch(opcao){
            case 0:
                opcao = 0;
                break;
            case 1: {
                char *nome_arquivo = gerar_nome(min_length, max_length);
                printf("O nome do seu arquivo é: %s\n", nome_arquivo);
                printf("Guarde o nome do seu arquivo, pois será necessário para abri-lo futuramente!!!\n");
                gravar_texto(nome_arquivo);
                criptografar_txt(nome_arquivo, key);
                free(nome_arquivo);
                break;
            }
            case 2: {
                printf("Digite o nome do arquivo que deseja abrir: ");
                char nome_arquivo_para_abrir[1024];
                scanf("%1023s", nome_arquivo_para_abrir);
                getchar();
                ler_e_descriptografar_arquivo(nome_arquivo_para_abrir, key);
                break;
            }
            case 3: {
                printf("Arquivo salvo\n"); // Todos os nossos métodos salvam o arquivo ao finalizar sua execução
                break;
            }
            case 4: {
                printf("Digite o nome do arquivo que deseja editar: ");
                char nome_arquivo_para_editar[1024];
                scanf("%1023s", nome_arquivo_para_editar);
                getchar();
                editar_texto(nome_arquivo_para_editar, key);
                break;
            }
            case 5: {
                printf("Digite o nome do arquivo ao qual deseja adicionar texto: ");
                char nome_arquivo_para_adicionar[1024];
                scanf("%1023s", nome_arquivo_para_adicionar);
                getchar();
                adicionar_texto(nome_arquivo_para_adicionar, key);
                break;
            }
            default:
                printf("Digite uma opção válida.\n");
        }

    } while(opcao != 0);

    return 0;
}
