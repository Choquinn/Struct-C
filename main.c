#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

typedef enum {
    ANALFABETO,
    FUNDAMENTAL,
    MEDIO,
    SUPERIOR,
    POSGRADUACAO   
} NivelEscolaridade;

struct endereco {
    char rua[60];
    char bairro[60];
    int numero;
    char complemento[40];
};

struct endereco lerEndereco() {
    struct endereco e;

    printf("Endereco:\n");

    printf("Digite a rua: ");
    scanf("%59s", e.rua);

    printf("Digite o bairro: ");
    scanf("%59s", e.bairro);

    printf("Digite o numero: ");
    scanf("%d", &e.numero);

    printf("Digite o complemento: ");
    scanf("%39s", e.complemento);

    return e;
}

struct data {
    int dia;
    int mes;
    int ano;
};

struct data lerData() {
    struct data d;

    printf("Digite o dia (dd): ");
    scanf("%d", &d.dia);

    printf("Digite o mes (mm): ");
    scanf("%d", &d.mes);

    printf("Digite o ano (aaaa): ");
    scanf("%d", &d.ano);

    return d;
}

struct escolaridade {
    NivelEscolaridade nivel;
    int completo;
};

struct escolaridade lerEscolaridade() {
    struct escolaridade esc;
    int opcao;

    do {
        printf("Selecione o nível de Escolaridade: \n");
        printf("0 - Analfabeto\n");
        printf("1 - Fundamental\n");
        printf("2 - Médio\n");
        printf("3 - Superior\n");
        printf("4 - Pós\n");
        scanf("%d", &opcao);

        if (opcao < 0 || opcao > 4) {
            printf("Opção inválida! tente novamente\n\n");
        } 
    } while (opcao < 0 || opcao > 4);

    esc.nivel = (NivelEscolaridade)opcao;

    do {
        printf("\nO nível selecionado está concluído?\n");
        printf("1 - Sim\n");
        printf("0 - Não\n");
        printf("Opção: ");
        scanf("%d", &esc.completo);
        
        if (esc.completo != 0 && esc.completo != 1) {
            printf("Opção inválida! Digite 1 ou 0.\n");
        }
    } while (esc.completo != 0 && esc.completo != 1);

    return esc;
}

struct funcionario {
    char nome[50];
    struct endereco endereco;
    char cpf[12];
    char estadoCivil[30];
    struct data dataNasc;
    struct escolaridade escolaridade;
    char cargo[50];
    float salario;
    struct data dataAdmis;
};

void delay(int segundos) {
    #ifdef _WIN32
        Sleep(segundos * 1000);
    #else
        sleep(segundos);
    #endif
}

void limparConsole() {
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif
}

void menuAdicionar() {
    struct funcionario funcionarioAtual;

    funcionarioAtual.id = proximoId();

    printf("Nome do funcionário: \n");
    scanf("%49s", funcionarioAtual.nome);
    limparConsole();

    printf("Endereço do funcionário: \n");
    funcionarioAtual.endereco = lerEndereco();
    limparConsole();

    printf("CPF do funcionário: \n");
    scanf("%11s", &funcionarioAtual.cpf);
    limparConsole();

    printf("Estado civil do funcionário: \n");
    scanf("%49s", funcionarioAtual.estadoCivil);
    limparConsole();

    printf("Data de nascimento do funcionário: \n");
    funcionarioAtual.dataNasc = lerData();
    limparConsole();

    printf("Escolaridade do funcionário: \n");
    funcionarioAtual.escolaridade = lerEscolaridade();
    limparConsole();

    printf("Cargo do funcionário: \n");
    scanf("%49s", funcionarioAtual.cargo);
    limparConsole();

    printf("Salário do funcionário: \n");
    scanf("%f", &funcionarioAtual.salario);
    limparConsole();

    printf("Data de admissão do funcionário: \n");
    funcionarioAtual.dataAdmis = lerData();
    limparConsole();

    salvarFuncionario(funcionarioAtual);
}

void salvarFuncionario(struct funcionario f) {
    FILE *arquivo = fopen("funcionarios.csv", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    fprintf(arquivo,
        "%s,%s,%s,%d,%s,%lld,%s,%d,%d,%d,%d,%d,%s,%.2f,%d,%d,%d\n",
        f.nome,
        f.endereco.rua,
        f.endereco.bairro,
        f.endereco.numero,
        f.endereco.complemento,
        f.cpf,
        f.estadoCivil,
        f.dataNasc.dia,
        f.dataNasc.mes,
        f.dataNasc.ano,
        f.escolaridade.nivel,
        f.escolaridade.completo,
        f.cargo,
        f.salario,
        f.dataAdmis.dia,
        f.dataAdmis.mes,
        f.dataAdmis.ano
    );

    fclose(arquivo);
}

int lerFuncionarioLinha(char linha[], struct funcionario *f) {
    return sscanf(
        linha,
        "%49[^,],%59[^,],%59[^,],%d,%39[^,],%11[^,],%29[^,],%d,%d,%d,%d,%d,%49[^,],%f,%d,%d,%d",
        f->nome,
        f->endereco.rua,
        f->endereco.bairro,
        &f->endereco.numero,
        f->endereco.complemento,
        f->cpf,
        f->estadoCivil,
        &f->dataNasc.dia,
        &f->dataNasc.mes,
        &f->dataNasc.ano,
        (int *)&f->escolaridade.nivel,
        &f->escolaridade.completo,
        f->cargo,
        &f->salario,
        &f->dataAdmis.dia,
        &f->dataAdmis.mes,
        &f->dataAdmis.ano
    );
}

void imprimirFunc(struct funcionario f) {
    printf("Nome: %s\n", f.nome);
    printf("Endereço: %s, %d - %s\n", f.endereco.rua, f.endereco.numero, f.endereco.bairro);
    printf("Complemento: %s\n", f.endereco.complemento);
    printf("CPF: %s\n", f.cpf);
    printf("Estado civil: %s\n", f.estadoCivil);
    printf("Data de Nascimento: %02d/%02d/%04d\n", f.dataNasc.dia, f.dataNasc.mes. f.dataNasc.ano);
    printf("Escolaridade: %d %s\n", f.escolaridade.nivel, f.escolaridade.completo ? "Sim" : "Não");
    printf("Cargo: %s\n", f.cargo);
    printf("Salario: %.2f\n", f.salario);
    printf("Data admissão: %02d/%02d/%04d\n", f.dataAdmis.dia, f.dataAdmis.mes, f.dataAdmis.ano);
}

void listarFunc() {
    FILE *arquivo = fopen("funcionarios.csv", "r");

    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo\n");
        return;
    }

    char linha[500];
    struct funcionario f;

    fgets(linha, sizeof(linha), arquivo);

    while(fgets(linha, sizeof(linha), arquivo)) {
        if (lerFuncionarioLinha(linha, &f) == 18) {
            imprimirFuncionario(f);
        }
    }

    fclose(arquivo);
}

void buscarFunc() {
    FILE *arquivo = fopen("funcionarios.csv", "r");

    if (arquivo == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return;
    }

    int idBuscado;
    char linha[500];
    struct funcionario f;
    int encontrado = 0;

    printf("Digite o ID: ");
    scanf("%d", &idBuscado);

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (lerFuncionarioLinha(linha, &f) == 18) {
            if (f.id == idBuscado) {
                imprimirFuncionario(f);
                encontrado = 1;
                break;
            }
        }
    }

    if (!encontrado) {
        printf("Funcionario nao encontrado.\n");
    }

    fclose(arquivo);
}

void editarInformacao(struct funcionario f, int sel) {
    switch (sel){
        case 1:
            printf("Nome do funcionário: ");
            scanf("%49s", f.nome);
        break;
        case 2:

        break;
        case 3:

        break;
        case 4:

        break;
        case 5:

        break;
        case 6:

        break;
        case 7:

        break;
        case 8:

        break;
        case 9:

        break;
        default:
            printf("Opção inválida!\n");
    }
}

void menuEditar() {
    int sel;

    printf("Selecione uma opção: \n");
    printf("1 - Editar Nome");
    printf("2 - Editar Endereço");
    printf("3 - Editar CPF");
    printf("4 - Editar Estado Civil");
    printf("5 - Editar Data de Nascimento");
    printf("6 - Editar Escolaridade");
    printf("7 - Editar Cargo");
    printf("8 - Editar Salário");
    printf("9 - Editar Data de Admissão");
    scanf("%d", &sel);
}

void menuPrincipal() {
    int sel;

    printf("Selecione uma opção: \n");
    printf("1 - Adicionar um funcionário\n");
    printf("2 - Listar funcionários\n");
    printf("3 - Buscar funcionário\n");
    printf("4 - Editar funcionário\n");
    printf("5 - Deletar funcionário\n");
    scanf("%d", &sel);

    switch (sel) {
        case 1: 
            limparConsole();
            menuAdicionar();
        break;
        case 2:
            limparConsole();
            listarFunc();
        break;
        case 3:
            limparConsole();
            buscarFunc();
        break;
        case 4:
            limparConsole();
            menuEditar();
        break;
        case 5:
            limparConsole();
            deletarFunc();
        break;
        default:
            limparConsole();
            printf("valor inválido");
            delay(3);
    } 
}

int main() {
    FILE *arquivo = fopen("funcionarios.csv", "r");

    if (arquivo == NULL) {
        arquivo = fopen("funcionarios.csv", "w");

        if (arquivo == NULL) {
            printf("Nao foi possivel criar o arquivo.\n");
            return 1;
        }

        fprintf(arquivo,
            "nome,rua,bairro,numero,complemento,cpf,estadoCivil,"
            "diaNasc,mesNasc,anoNasc,escolaridade,completo,cargo,"
            "salario,diaAdmis,mesAdmis,anoAdmis\n"
        );
    }

    fclose(arquivo);

    menuPrincipal();

    return 0;
}