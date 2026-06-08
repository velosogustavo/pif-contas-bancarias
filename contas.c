#include <stdio.h>
#include <string.h>

#define ARQUIVO "contas.dat"

typedef struct {
    int    numero;
    char   nome[50];
    char   cpf[15];
    double saldo;
    int    ativo;
} Conta;

/* Retorna quantos registros existem no arquivo */
long total() {
    FILE *fp = fopen(ARQUIVO, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long n = ftell(fp) / sizeof(Conta);
    fclose(fp);
    return n;
}

/* Busca conta pelo número; retorna posição ou -1 se não achar */
long buscar(int numero) {
    Conta c;
    FILE *fp = fopen(ARQUIVO, "rb");
    if (!fp) return -1;
    long n = total(), pos = -1;
    for (long i = 0; i < n; i++) {
        fseek(fp, i * sizeof(Conta), SEEK_SET);
        fread(&c, sizeof(Conta), 1, fp);
        if (c.ativo && c.numero == numero) { pos = i; break; }
    }
    fclose(fp);
    return pos;
}

void listar(FILE *fp) {
    Conta c;
    long n = total();
    int achou = 0;
    printf("\n%-5s %-8s %-30s %-15s %10s\n", "Pos","Conta","Nome","CPF","Saldo");
    printf("--------------------------------------------------------------------\n");
    for (long i = 0; i < n; i++) {
        fseek(fp, i * sizeof(Conta), SEEK_SET);
        fread(&c, sizeof(Conta), 1, fp);
        if (c.ativo) {
            printf("%-5ld %-8d %-30s %-15s %10.2f\n", i, c.numero, c.nome, c.cpf, c.saldo);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum cliente ativo.\n");
}

void cadastrar() {
    Conta c;
    long pos, n = total();
    if (n == 0)
        printf("Arquivo vazio. Digite 0 para cadastrar: ");
    else
        printf("Posicao (0 a %ld para existente, ou %ld para nova): ", n-1, n);
    scanf("%ld", &pos); getchar();
    if (pos < 0 || pos > n) { printf("Posicao invalida.\n"); return; }

    /* Verifica se posição já está ocupada */
    if (pos < n) {
        FILE *fp = fopen(ARQUIVO, "rb");
        fseek(fp, pos * sizeof(Conta), SEEK_SET);
        fread(&c, sizeof(Conta), 1, fp);
        fclose(fp);
        if (c.ativo) { printf("Posicao ocupada pela conta %d.\n", c.numero); return; }
    }

    printf("Numero da conta: "); scanf("%d", &c.numero); getchar();
    printf("Nome: "); fgets(c.nome, 50, stdin); c.nome[strcspn(c.nome,"\n")] = 0;
    printf("CPF: ");  fgets(c.cpf,  15, stdin); c.cpf[strcspn(c.cpf, "\n")] = 0;
    printf("Saldo: "); scanf("%lf", &c.saldo); getchar();
    c.ativo = 1;

    FILE *fp = fopen(ARQUIVO, "r+b");
    if (!fp) fp = fopen(ARQUIVO, "wb");
    fseek(fp, pos * sizeof(Conta), SEEK_SET);
    fwrite(&c, sizeof(Conta), 1, fp);
    fclose(fp);
    printf("Conta cadastrada na posicao %ld.\n", pos);
}

void consultar() {
    int num; printf("Numero da conta:"); scanf("%d", &num); getchar();
    long pos = buscar(num);
    if (pos < 0) { printf("Conta nao encontrada.\n"); return; }
    Conta c;
    FILE *fp = fopen(ARQUIVO, "rb");
    fseek(fp, pos * sizeof(Conta), SEEK_SET);
    fread(&c, sizeof(Conta), 1, fp);
    fclose(fp);
    printf("\nConta: %d | Nome: %s | CPF: %s | Saldo: R$ %.2f\n",
           c.numero, c.nome, c.cpf, c.saldo);
}

void atualizarSaldo() {
    int num; printf("Numero da conta:"); scanf("%d", &num); getchar();
    long pos = buscar(num);
    if (pos < 0) { printf("Conta nao encontrada.\n"); return; }
    Conta c;
    FILE *fp = fopen(ARQUIVO, "r+b");
    fseek(fp, pos * sizeof(Conta), SEEK_SET);
    fread(&c, sizeof(Conta), 1, fp);
    printf("Saldo atual: R$ %.2f\nNovo saldo: R$ ", c.saldo);
    scanf("%lf", &c.saldo); getchar();
    fseek(fp, pos * sizeof(Conta), SEEK_SET);
    fwrite(&c, sizeof(Conta), 1, fp);
    fclose(fp);
    printf("Saldo atualizado.\n");
}

void encerrar() {
    int num; printf("Numero da conta:"); scanf("%d", &num); getchar();
    long pos = buscar(num);
    if (pos < 0) { printf("Conta nao encontrada.\n"); return; }
    Conta c;
    FILE *fp = fopen(ARQUIVO, "r+b");
    fseek(fp, pos * sizeof(Conta), SEEK_SET);
    fread(&c, sizeof(Conta), 1, fp);
    printf("Encerrar conta de %s? (s/n): ", c.nome);
    char conf; scanf("%c", &conf); getchar();
    if (conf == 's' || conf == 'S') {
        c.ativo = 0;
        fseek(fp, pos * sizeof(Conta), SEEK_SET);
        fwrite(&c, sizeof(Conta), 1, fp);
        printf("Conta encerrada.\n");
    }
    fclose(fp);
}

int main() {
    int op;
    do {
        printf("\n1-Cadastrar  2-Consultar  3-Atualizar saldo\n");
        printf("4-Encerrar   5-Listar     6-Repetir listagem  7-Sair\n");
        printf("Opcao: "); scanf("%d", &op); getchar();
        switch (op) {
            case 1: cadastrar();       break;
            case 2: consultar();       break;
            case 3: atualizarSaldo();  break;
            case 4: encerrar();        break;
            case 5: {
                FILE *fp = fopen(ARQUIVO, "rb");
                if (fp) { listar(fp); fclose(fp); }
                else printf("Sem dados ainda.\n");
                break;
            }
            case 6: {
                FILE *fp = fopen(ARQUIVO, "rb");
                if (fp) { rewind(fp); printf("(rewind aplicado)\n"); listar(fp); fclose(fp); }
                else printf("Sem dados ainda.\n");
                break;
            }
            case 7: printf("Encerrando.\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 7);
    return 0;
}