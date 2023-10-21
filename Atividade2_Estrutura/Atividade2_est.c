#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Musica {
    char artista[100];
    char titulo[100];
    struct Musica* anterior;
    struct Musica* proxima;
};

struct Musica* criarMusica(char artista[], char titulo[]) {
    struct Musica* novaMusica = (struct Musica*)malloc(sizeof(struct Musica));
    strcpy(novaMusica->artista, artista);
    strcpy(novaMusica->titulo, titulo);
    novaMusica->anterior = NULL;
    novaMusica->proxima = NULL;
    return novaMusica;
}

void inserirNoInicio(struct Musica** cabeca, char artista[], char titulo[]) {
    struct Musica* novaMusica = criarMusica(artista, titulo);
    if (*cabeca == NULL) {
        *cabeca = novaMusica;
        (*cabeca)->proxima = novaMusica;
        (*cabeca)->anterior = novaMusica;
    } else {
        novaMusica->proxima = *cabeca;
        novaMusica->anterior = (*cabeca)->anterior;
        (*cabeca)->anterior->proxima = novaMusica;
        (*cabeca)->anterior = novaMusica;
        *cabeca = novaMusica;
    }
}

void exibirPlaylistPorOrdem(struct Musica* cabeca, struct Musica* atual) {
    if (cabeca == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    struct Musica* iterador = cabeca;
    printf("Playlist pela ordem de cadastro:\n");
    do {
        printf("%s - %s", iterador->artista, iterador->titulo);
        if (iterador == atual) {
            printf(" (Tocando Agora)");
        }
        printf("\n");
        iterador = iterador->proxima;
    } while (iterador != cabeca);
}

void exibirPlaylistPorNome(struct Musica* cabeca, struct Musica* atual) {
    int i, j;
    if (cabeca == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }

    struct Musica* iterador = cabeca;
    char titulos[100][100];
    int contador = 0;

    do {
        strcpy(titulos[contador], iterador->titulo);
        iterador = iterador->proxima;
        contador++;
    } while (iterador != cabeca);

    printf("Playlist ordenada pelo nome das musicas:\n");

    for (i = 0; i < contador; i++) {
        for (j = i + 1; j < contador; j++) {
            if (strcmp(titulos[i], titulos[j]) > 0) {
                char temp[100];
                strcpy(temp, titulos[i]);
                strcpy(titulos[i], titulos[j]);
                strcpy(titulos[j], temp);
            }
        }
    }

    for (i = 0; i < contador; i++) {
        iterador = cabeca;
        do {
            if (strcmp(iterador->titulo, titulos[i]) == 0) {
                printf("%s - %s", iterador->artista, iterador->titulo);
                if (iterador == atual) {
                    printf(" (Tocando Agora)");
                }
                printf("\n");
                break;
            }
            iterador = iterador->proxima;
        } while (iterador != cabeca);
    }
}

struct Musica* proximaMusica(struct Musica* atual) {
    return atual->proxima;
}

struct Musica* musicaAnterior(struct Musica* atual) {
    return atual->anterior;
}

struct Musica* buscarMusica(struct Musica* cabeca, char titulo[]) {
    if (cabeca == NULL) {
        return NULL;
    }

    struct Musica* iterador = cabeca;
    do {
        if (strcmp(iterador->titulo, titulo) == 0) {
            return iterador;
        }
        iterador = iterador->proxima;
    } while (iterador != cabeca);

    return NULL;
}

void removerMusica(struct Musica** cabeca, char titulo[]) {
    struct Musica* musicaARemover = buscarMusica(*cabeca, titulo);
    if (musicaARemover == NULL) {
        printf("A musica nao foi encontrada na playlist.\n");
        return;
    }

    if (*cabeca == musicaARemover) {
        if (musicaARemover->proxima == musicaARemover) {
            free(musicaARemover);
            *cabeca = NULL;
        } else {
            (*cabeca)->proxima = musicaARemover->proxima;
            (*cabeca)->anterior = musicaARemover->anterior;
            musicaARemover->proxima->anterior = *cabeca;
            musicaARemover->anterior->proxima = *cabeca;
            *cabeca = (*cabeca)->proxima;
            free(musicaARemover);
        }
    } else {
        musicaARemover->anterior->proxima = musicaARemover->proxima;
        musicaARemover->proxima->anterior = musicaARemover->anterior;
        free(musicaARemover);
    }
    printf("A musica escolhida foi removida!\n");
}

void salvarPlaylistEmArquivo(struct Musica* cabeca) {
    if (cabeca == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }

    FILE* arquivo = fopen("C:\\Users\\Yanns\\OneDrive\\Área de Trabalho\\Atividade2_Estrutura\\musicas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    struct Musica* iterador = cabeca;
    do {
        fprintf(arquivo, "%s;%s\n", iterador->artista, iterador->titulo);
        iterador = iterador->proxima;
    } while (iterador != cabeca);

    fclose(arquivo);
}

int main() {
    char tituloParaBuscar[100];
    char novoArtista[100];
    char novaMusica[100];
    char musicaParaRemover[100];
    struct Musica* cabeca = NULL;
    FILE* arquivo = fopen("C:\\Users\\Yanns\\OneDrive\\Área de Trabalho\\Atividade2_Estrutura\\musicas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de musicas.\n");
        return 1;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        char artista[100];
        char titulo[100];
        sscanf(linha, "%99[^;];%99[^\n]", artista, titulo);
        inserirNoInicio(&cabeca, artista, titulo);
    }

    fclose(arquivo);

    int escolha;
    struct Musica* atual = cabeca;

    do {
        printf("\n=========================INICIO=========================\n\n");
        printf("1. Exibir playlist pela ordem de cadastro\n");
        printf("2. Exibir playlist ordenada pelo nome das musicas\n");
        printf("3. Inserir nova musica\n");
        printf("4. Remover musica\n");
        printf("5. Buscar musica\n");
        printf("6. Avancar para a proxima musica\n");
        printf("7. Voltar a musica anterior\n");
        printf("8. Qual musica esta tocando\n");
        printf("9. Sair\n");
        printf("Escolha uma opcao: ");
         char input[10];
        fgets(input, sizeof(input), stdin);
        if (isdigit(input[0])) {
            escolha = atoi(input);
        } else {
            printf("Opcao invalida. Digite um valor valido!\n");
            continue; 
        }

        printf("\n=======================RESULTADO=======================\n\n");

        switch (escolha) {
            case 1:
                exibirPlaylistPorOrdem(cabeca, atual);
                break;
            case 2:
                exibirPlaylistPorNome(cabeca, atual);
                break;
            case 3:
                printf("Informe o nome do artista: ");
                fgets(novoArtista, sizeof(novoArtista), stdin);
                novoArtista[strcspn(novoArtista, "\n")] = 0; 
                printf("Informe o nome da musica: ");
                fgets(novaMusica, sizeof(novaMusica), stdin);
                novaMusica[strcspn(novaMusica, "\n")] = 0; 
                inserirNoInicio(&cabeca, novoArtista, novaMusica);
                salvarPlaylistEmArquivo(cabeca);
                break;
            case 4:
                printf("Informe o nome da musica a ser removida: ");
                fgets(musicaParaRemover, sizeof(musicaParaRemover), stdin);
                musicaParaRemover[strcspn(musicaParaRemover, "\n")] = 0; 
                removerMusica(&cabeca, musicaParaRemover);
                salvarPlaylistEmArquivo(cabeca);
                break;
            case 5:
                printf("Informe o nome da musica a ser buscada: ");
                fgets(tituloParaBuscar, sizeof(tituloParaBuscar), stdin);
                tituloParaBuscar[strcspn(tituloParaBuscar, "\n")] = 0; 
                struct Musica* musicaEncontrada = buscarMusica(cabeca, tituloParaBuscar);
                if (musicaEncontrada) {
                    printf("Musica encontrada: %s - %s\n", musicaEncontrada->artista, musicaEncontrada->titulo);
                } else {
                    printf("Musica nao encontrada!.\n");
                }
                break;
            case 6:
                atual = proximaMusica(atual);
                printf("Musica atual: %s - %s\n", atual->artista, atual->titulo);
                break;
            case 7:
                atual = musicaAnterior(atual);
                printf("Musica atual: %s - %s\n", atual->artista, atual->titulo);
                break;
            case 8:
                printf("Musica atual: %s - %s\n", atual->artista, atual->titulo);
                break;
            case 9:
                printf("Muito obrigado por ouvir o... fechando playlist.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (escolha != 9);

    return 0;
}

