#include <stdio.h>
#include <string.h>

#define MAX_USUARIOS 100
#define MAX_NOME 100

typedef struct {
  int id;
  char nome[MAX_NOME];
  int idade;
  double saldo;
} Usuario;

Usuario usuarios[MAX_USUARIOS];
int proximoId = 1;
int totalUsuarios = 0;

void menu() {
  printf("\n---UAIBANK---\n");
  printf("\nMENU:\n");
  printf("1. Inserir um novo usuário\n");
  printf("2. Inserir vários usuários\n");
  printf("3. Buscar um usuário por ID\n");
  printf("4. Transferir saldo entre usuários\n");
  printf("5. Remover um usuário por ID\n");
  printf("6. Salvar dados e sair\n\n");
  printf("Escolha uma opção: ");
}

void inserirUsuario(char *nome, int idade, double saldo) {
  if (totalUsuarios < MAX_USUARIOS) {
    Usuario novoUsuario;
    novoUsuario.id = proximoId++;
    strncpy(novoUsuario.nome, nome, MAX_NOME - 1);
    novoUsuario.nome[MAX_NOME - 1] = '\0';
    novoUsuario.idade = idade;
    novoUsuario.saldo = saldo;

    usuarios[totalUsuarios++] = novoUsuario;
    printf("Usuário inserido com id %d.\n", novoUsuario.id);
  } else {
    printf("Erro: Capacidade máxima de usuários atingida.\n");
  }
}

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

Usuario *buscarUsuarioPorId(int id) {
  for (int i = 0; i < totalUsuarios; i++) {
    if (usuarios[i].id == id) {
      return &usuarios[i];
    }
  }
  return NULL;
}

void transferirSaldo(int idOrigem, int idDestino, double quantia) {
  if (idOrigem == idDestino) {
    printf("Erro: IDs de origem e destino não podem ser iguais.\n");
    return;
  }

  if (quantia <= 0) {
    printf("Erro: A quantia a ser transferida deve ser positiva.\n");
    return;
  }

  Usuario *usuarioOrigem = buscarUsuarioPorId(idOrigem);
  Usuario *usuarioDestino = buscarUsuarioPorId(idDestino);

  if (usuarioOrigem && usuarioDestino && usuarioOrigem->saldo >= quantia) {
    usuarioOrigem->saldo -= quantia;
    usuarioDestino->saldo += quantia;
    printf("Transferência de R$%.2f do usuário %d para o usuário %d realizada com sucesso.\n",
           quantia, idOrigem, idDestino);
  } else {
    if (!usuarioOrigem) {
      printf("Erro: Usuário de origem (ID %d) não encontrado.\n", idOrigem);
    }
    if (!usuarioDestino) {
      printf("Erro: Usuário de destino (ID %d) não encontrado.\n", idDestino);
    }
    if (usuarioOrigem && usuarioOrigem->saldo < quantia) {
      printf("Erro: Saldo insuficiente para a transferência.\n");
    }
  }
}

void removerUsuario(int id) {
  int encontrado = 0;
  for (int i = 0; i < totalUsuarios; i++) {
    if (usuarios[i].id == id) {
      encontrado = 1;
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

void salvarDados(const char *nomeArquivo) {
  FILE *arquivo = fopen("dados.txt", "w");
  if (!arquivo) {
    printf("Erro ao abrir o arquivo para escrita.\n");
    return;
  }

  for (int i = 0; i < totalUsuarios; i++) {
    fprintf(arquivo, "%d, %s, %d, %.2f\n", usuarios[i].id, usuarios[i].nome,
            usuarios[i].idade, usuarios[i].saldo);
  }

  fclose(arquivo);
}

void carregarDados(const char *nomeArquivo) {
  FILE *arquivo = fopen("dados.txt", "r");
  if (!arquivo) {
    printf("Arquivo de dados não encontrado ou erro ao abrir.\n");
    return;
  }

  int maxId = 0;
  while (!feof(arquivo) && totalUsuarios < MAX_USUARIOS) {
    Usuario novoUsuario;
    fscanf(arquivo, "%d, %99[^,], %d, %lf\n", &novoUsuario.id, novoUsuario.nome,
           &novoUsuario.idade, &novoUsuario.saldo);
    usuarios[totalUsuarios++] = novoUsuario;
    if (novoUsuario.id > maxId) {
      maxId = novoUsuario.id;
    }
  }
  proximoId = maxId + 1;

  fclose(arquivo);
}

int main() {
  carregarDados("dados.txt");

  int opcao;
  do {
    menu();
    scanf("%d", &opcao);

    switch (opcao) {
    case 1: {
      char nome[MAX_NOME];
      int idade;
      double saldo;
      printf("Insira os dados do usuário (nome, idade, saldo): ");
      scanf(" %99[^,], %d, %lf", nome, &idade, &saldo);
      inserirUsuario(nome, idade, saldo);
      break;
    }
    case 2: {
      int qtde;
      printf("Insira a quantidade de usuários: ");
      scanf("%d", &qtde);
      inserirVariosUsuarios(qtde);
      break;
    }
    case 3: {
      int id;
      printf("Insira o ID do usuário a ser buscado: ");
      scanf("%d", &id);
      Usuario *usuario = buscarUsuarioPorId(id);
      if (usuario) {
        printf(
            "Usuário encontrado: ID %d, Nome: %s, Idade: %d, Saldo: R$%.2f\n",
            usuario->id, usuario->nome, usuario->idade, usuario->saldo);
      } else {
        printf("Erro: Usuário %d não encontrado.\n", id);
      }
      break;
    }
    case 4: {
      int idOrigem, idDestino;
      double quantia;
      printf("Insira o ID do usuário de origem, o ID de destino e a quantia a ser transferida (separados por vírgula): ");
      if (scanf("%d,%d,%lf", &idOrigem, &idDestino, &quantia) == 3) {
        transferirSaldo(idOrigem, idDestino, quantia);
      } else {
        printf("Erro: Entrada inválida. Certifique-se de inserir dois IDs inteiros e uma quantia separados por vírgula.\n");
        while (getchar() != '\n'); // Limpa o buffer
      }
      break;
    }
    case 5: {
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
  } while (opcao != 6);

  salvarDados("dados.txt");

  return 0;
}
