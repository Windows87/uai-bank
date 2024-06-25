#include <stdio.h>
#include <string.h>

#define MAX_USUARIOS 100
#define MAX_NOME 100

/*
   Definição da estrutura Usuario:
   - id: Identificador do usuário.
   - nome: Nome do usuário, com tamanho máximo de MAX_NOME.
   - idade: Idade do usuário
   - saldo: Saldo atual
*/

typedef struct {
    int id;
    char nome[MAX_NOME];
    int idade;
    double saldo;
} Usuario;

Usuario usuarios[MAX_USUARIOS];  // Array que armazena os usuários
int proximoId = 1;               // Controla o próximo ID a ser atribuído
int totalUsuarios = 0;   

// Função para exibir o menu de opções
void menu() {
    printf("\n---UAIBANK---\n");
    printf("\nMENU:\n");
    printf("1. Inserir um novo usuário\n");
    printf("2. Inserir vários usuários\n");
    printf("3. Buscar um usuário por ID\n");
    printf("4. Transferir saldo entre usuários\n");
    printf("5. Remover um usuário por ID\n");
    printf("6. Sair\n\n");
    printf("Escolha uma opção: ");
}

// Função para inserir um único usuário no sistema
void inserirUsuario(char *nome, int idade, double saldo) {
    if (totalUsuarios < MAX_USUARIOS) {
        Usuario novoUsuario;
        novoUsuario.id = proximoId++;
        strncpy(novoUsuario.nome, nome, MAX_NOME - 1);
        novoUsuario.nome[MAX_NOME - 1] = '\0'; // Garantir terminação nula
        novoUsuario.idade = idade;
        novoUsuario.saldo = saldo;

        usuarios[totalUsuarios++] = novoUsuario;
        printf("Usuário inserido com id %d.\n", novoUsuario.id);
    } else {
        printf("Erro: Capacidade máxima de usuários atingida.\n");
    }
}

// Função para inserir vários usuários de uma vez no sistema
void inserirVariosUsuarios(int qtde) {
    for (int i = 0; i < qtde; i++) {
        char nome[MAX_NOME];
        int idade;
        double saldo;
        printf("Insira os dados do %dº usuário (nome, idade, saldo): ", i + 1);
        scanf(" %99[^,], %d, %lf", nome, &idade, &saldo);
        inserirUsuario(nome, idade, saldo);
    }
}

// Função para buscar um usuário pelo seu ID
Usuario* buscarUsuarioPorId(int id) {
    for (int i = 0; i < totalUsuarios; i++) {
        if (usuarios[i].id == id) {
            return &usuarios[i];
        }
    }
    return NULL; // Retorna NULL se o usuário não for encontrado
}

// Função para transferir saldo de um usuário para outro
void transferirSaldo(int idOrigem, int idDestino, double quantia) {
    Usuario *usuarioOrigem = buscarUsuarioPorId(idOrigem);
    Usuario *usuarioDestino = buscarUsuarioPorId(idDestino);

    if (usuarioOrigem && usuarioDestino && usuarioOrigem->saldo >= quantia) {
        usuarioOrigem->saldo -= quantia;
        usuarioDestino->saldo += quantia;
        printf("Transferência de R$%.2f do usuário %d para o usuário %d realizada com sucesso.\n", quantia, idOrigem, idDestino);
    } else {
        printf("Erro: Transferência não pôde ser realizada.\n");
    }
}

// Função para remover um usuário pelo seu ID
void removerUsuario(int id) {
    int encontrado = 0;
    for (int i = 0; i < totalUsuarios; i++) {
        if (usuarios[i].id == id) {
            encontrado = 1;
            // Movendo os usuários uma posição para trás para remover o usuário
            for (int j = i; j < totalUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];
            }
            totalUsuarios--;
            printf("Usuário %d removido com sucesso.\n", id);
            break;  
        }
    }
    if (!encontrado) {
        printf("Erro: Usuário %d não encontrado.\n", id);
    }
}

// Função para salvar os dados dos usuários em um arquivo
void salvarDados(const char *nomeArquivo) {
    FILE *arquivo = fopen("dados.txt", "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Escreve cada usuário no arquivo no formato "id, nome, idade, saldo"
    for (int i = 0; i < totalUsuarios; i++) {
        fprintf(arquivo, "%d, %s, %d, %.2f\n", usuarios[i].id, usuarios[i].nome, usuarios[i].idade, usuarios[i].saldo);
    }

    fclose(arquivo);
}

// Função para carregar os dados dos usuários de um arquivo
void carregarDados(const char *nomeArquivo) {
    FILE *arquivo = fopen("dados.txt", "r");
    if (!arquivo) {
        printf("Arquivo de dados não encontrado ou erro ao abrir.\n");
        return;
    }

    // Lê cada linha do arquivo no formato "id, nome, idade, saldo" e adiciona ao array de usuários
    while (!feof(arquivo) && totalUsuarios < MAX_USUARIOS) {
        Usuario novoUsuario;
        fscanf(arquivo, "%d, %99[^,], %d, %lf\n", &novoUsuario.id, novoUsuario.nome, &novoUsuario.idade, &novoUsuario.saldo);
        usuarios[totalUsuarios++] = novoUsuario;
    }

    fclose(arquivo);
}


int main() {
    carregarDados("dados.txt"); // Carrega os dados dos usuários do arquivo ao iniciar o programa

    int opcao;
    do {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Inserir um novo usuário
                char nome[MAX_NOME];
                int idade;
                double saldo;
                printf("Insira os dados do usuário (nome, idade, saldo): ");
                scanf(" %99[^,], %d, %lf", nome, &idade, &saldo);
                inserirUsuario(nome, idade, saldo);
                break;
            }
            case 2: {
                // Inserir vários usuários
                int qtde;
                printf("Insira a quantidade de usuários: ");
                scanf("%d", &qtde);
                inserirVariosUsuarios(qtde);
                break;
            }
            case 3: {
                // Buscar usuário por ID
                int id;
                printf("Insira o ID do usuário a ser buscado: ");
                scanf("%d", &id);
                Usuario *usuario = buscarUsuarioPorId(id);
                if (usuario) {
                    printf("Usuário encontrado: ID %d, Nome: %s, Idade: %d, Saldo: R$%.2f\n", usuario->id, usuario->nome, usuario->idade, usuario->saldo);
                } else {
                    printf("Erro: Usuário %d não encontrado.\n", id);
                }
                break;
            }
            case 4: {
                // Transferir saldo entre usuários
                int idOrigem, idDestino;
                double quantia;
                printf("Insira o ID do usuário de origem, o ID de destino e a quantia a ser transferida: ");
                scanf("%d %d %lf", &idOrigem, &idDestino, &quantia);
                transferirSaldo(idOrigem, idDestino, quantia);
                break;
            }
            case 5: {
                // Remover um usuário
                int id;
                printf("Insira o ID do usuário a ser removido: ");
                scanf("%d", &id);
                removerUsuario(id);
                break;
            }
            case 6:
                printf("Dados salvos. Programa encerrado.\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 6); // Continua exibindo o menu até que o usuário escolha a opção de sair

    salvarDados("dados.txt"); // Ao encerrar o programa, salva os dados dos usuários no arquivo

    return 0;
}
