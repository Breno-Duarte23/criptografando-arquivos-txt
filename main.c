#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void criptografar_NOME(const char *chave, const char *nome_arquivo){
    int tam_chave = strlen(chave);
    int tam_nome_arquivo = strlen(nome_arquivo);
    char nome_arquivo_cript[tam_nome_arquivo + 1];

    // Encontrar a posição do ponto (.) na extensão do arquivo
    const char *ponto = strrchr(nome_arquivo, '.');
    int posicao_ponto = ponto ? ponto - nome_arquivo : tam_nome_arquivo;

    for (int i = 0; i < posicao_ponto; i++) {
        int indice_chave = i % tam_chave;
        nome_arquivo_cript[i] = nome_arquivo[i] + chave[indice_chave];
    }

    // Copiar a extensão do arquivo sem criptografar
    strcpy(nome_arquivo_cript + posicao_ponto, nome_arquivo + posicao_ponto);

    FILE *arquivo = fopen(nome_arquivo_cript, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fclose(arquivo);
}

void criptografar(const char *chave, const char *texto) {
    int tam_chave = strlen(chave);
    int tam_texto = strlen(texto);
    char texto_cript[tam_texto + 1];

    char nomeArquivo[100];

    for (int i = 0; i < tam_texto; i++) {
        int indice_chave = i % tam_chave;
        texto_cript[i] = texto[i] + chave[indice_chave];
    }
    texto_cript[tam_texto] = '\0';
    FILE *arquivo = fopen("criptografado.txt", "w");
    if (arquivo == NULL) {
        printf("\nERRO AO CRIAR ARQUIVO!!!!\n\n");
        return;
    }
    fprintf(arquivo, "%s", texto_cript);
    fclose(arquivo);
}

void descriptografar(const char *chave) {
    char texto_decript[1000];
    int pos_texto = 0;

    FILE *arquivo = fopen("criptografado.txt", "r");
    if (arquivo == NULL) {
        printf("\nERRO AO ABRIR ARQUIVO!!!!\n\n");
        return;
    }

    char caractere;
    int indice_chave = 0;
    while ((caractere = fgetc(arquivo)) != EOF) {
        texto_decript[pos_texto] = caractere - chave[indice_chave];
        indice_chave = (indice_chave + 1) % strlen(chave);
        pos_texto++;
    }
    texto_decript[pos_texto] = '\0'; // Adiciona o terminador de string

    fclose(arquivo);

    printf("\nTexto descriptografado:\n");
    printf("%s\n\n", texto_decript);
}

int main() {
    int opcao;
    char texto[1000];
    char chave[100];

    printf("Criptografando em C:\n");
    printf("\nMenu\n");
    printf("1 - Criptografar\n2 - Descriptografar \n3 - Fechar\n");
    printf("\nDigite a opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Digite o texto a ser criptografado:\n");
            getchar(); // Limpa o buffer de entrada
            fgets(texto, sizeof(texto), stdin); // Lê uma linha de texto

            printf("Digite a palavra secreta da criptografia:\n");
            scanf("%s", chave);

            criptografar(chave, texto);
            break;
        case 2:
            printf("Digite a palavra secreta da criptografia:\n");
            scanf("%s", chave);

            descriptografar(chave);
            break;
        case 3:
            break;
        default:
            printf("Opcao invalida!!! Tente novamente!!!\n\n");
    }

    return 0;
}
