# 🏦 Sistema de Manutenção de Contas Bancárias

Atividade de **Programação Imperativa Funcional** em linguagem C.  
Implementa um sistema de gerenciamento de contas usando arquivo binário com registros de tamanho fixo e acesso direto via `fseek()`.

---

## 📋 Enunciado

Criar um sistema de manutenção de contas com menu contendo:

1. Cadastrar um novo cliente em uma posição específica  
2. Consultar um cliente pelo número da conta  
3. Atualizar o saldo de um cliente  
4. Encerrar conta (remover cliente)  
5. Listar todos os clientes  
6. Restaurar a leitura do arquivo do início com `rewind()`  
7. Encerrar  

---

## 🗂 Estrutura do Projeto

```
contas/
├── contas.c      # código-fonte principal
├── Makefile      # script de compilação
├── README.md     # este arquivo
└── contas.dat    # arquivo binário gerado em tempo de execução
```

---

## 🧱 Estrutura do Registro

```c
typedef struct {
    int    numero;          // número da conta (chave primária)
    char   nome[50];        // nome do titular
    char   cpf[15];         // CPF do titular
    double saldo;           // saldo atual
    int    ativo;           // 1 = ativa | 0 = encerrada
} Conta;
```

Cada registro ocupa exatamente `sizeof(Conta)` bytes no arquivo, garantindo acesso direto por posição.

---

## ⚙️ Como Compilar e Executar

### Pré-requisitos

- GCC instalado (`gcc --version`)
- Sistema Linux/macOS ou Windows com MinGW/WSL

### Com Makefile

```bash
make        # compila
make run    # compila e executa
make clean  # remove binário e arquivo de dados
```

### Manualmente

```bash
gcc -Wall -Wextra -std=c11 -pedantic -o contas contas.c
./contas
```

---

## 📖 Explicação das Funções de E/S

### `fseek(fp, posicao * sizeof(Conta), SEEK_SET)`

Posiciona o ponteiro de leitura/escrita diretamente no byte inicial do registro desejado. Como todos os registros têm tamanho fixo (`sizeof(Conta)`), a posição em bytes de qualquer registro `i` é simplesmente `i * sizeof(Conta)`.

```
Arquivo binário:
┌──────────┬──────────┬──────────┬──────────┐
│ Conta[0] │ Conta[1] │ Conta[2] │ Conta[3] │
│ (pos. 0) │ (pos. 1) │ (pos. 2) │ (pos. 3) │
└──────────┴──────────┴──────────┴──────────┘
     ↑
fseek(fp, 0 * sizeof(Conta), SEEK_SET)
```

### `fread(&c, sizeof(Conta), 1, fp)`

Lê **1 registro** de `sizeof(Conta)` bytes a partir da posição atual do ponteiro e armazena na variável `c`.

### `fwrite(&c, sizeof(Conta), 1, fp)`

Escreve **1 registro** de `sizeof(Conta)` bytes na posição atual do ponteiro — sobrescrevendo dados existentes ou adicionando novo registro ao final.

### `rewind(fp)`

Equivalente a `fseek(fp, 0, SEEK_SET)`. Reposiciona o ponteiro no **byte 0** do arquivo, permitindo releitura completa do início sem fechar e reabrir o arquivo.

---

## 🔄 Lógica de Exclusão (Soft Delete)

A conta **não é apagada fisicamente**. O campo `ativo` é alterado de `1` para `0`, marcando o registro como inativo. Isso:

- Preserva a integridade dos índices de posição (`fseek` continua funcionando)
- Permite reutilizar a posição em um cadastro futuro
- Evita fragmentação desnecessária do arquivo

---

## 🧩 Modos de Abertura do Arquivo

| Operação         | Modo   | Motivo                                          |
|-----------------|--------|-------------------------------------------------|
| Apenas leitura  | `"rb"` | Lê registros sem risco de modificação acidental |
| Leitura + escrita | `"r+b"` | Atualiza registros existentes (saldo, encerramento) |
| Criação inicial | `"wb"` | Cria o arquivo se ainda não existir             |

---

## 📺 Exemplo de Execução

```
╔══════════════════════════════════════╗
║   SISTEMA DE CONTAS BANCÁRIAS        ║
╚══════════════════════════════════════╝

┌─────────────────────────────────────┐
│              MENU PRINCIPAL         │
├─────────────────────────────────────┤
│ 1. Cadastrar novo cliente            │
│ 2. Consultar cliente (nº da conta)   │
│ 3. Atualizar saldo                   │
│ 4. Encerrar conta                    │
│ 5. Listar todos os clientes          │
│ 6. Repetir listagem (rewind)         │
│ 7. Sair                              │
└─────────────────────────────────────┘
Opção: 1

--- CADASTRAR CLIENTE ---
Total de posições no arquivo: 0
Informe a posição (0 para nova posição): 0
Número da conta: 1001
Nome do titular: Ana Souza
CPF: 111.222.333-44
Saldo inicial: R$ 1500.00

Cliente cadastrado na posição 0 com sucesso!
```

---

## 👨‍💻 Autor

Desenvolvido como atividade acadêmica de Programação Imperativa Funcional.