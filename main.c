#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

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

    printf("Endereço:\n\n");

    printf("Digite a rua: ");
    scanf(" %59[^\n]", e.rua);

    printf("Digite o bairro: ");
    scanf(" %59[^\n]", e.bairro);

    printf("Digite o número: ");
    scanf("%d", &e.numero);

    printf("Digite o complemento: ");
    scanf(" %39[^\n]", e.complemento);

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

    printf("Digite o mês (mm): ");
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
        printf("Selecione o nível de escolaridade:\n\n");
        printf("0 - Analfabeto\n");
        printf("1 - Fundamental\n");
        printf("2 - Médio\n");
        printf("3 - Superior\n");
        printf("4 - Pós-graduação\n\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        if (opcao < 0 || opcao > 4) {
            printf("\nOpção inválida! Tente novamente.\n\n");
        }
    } while (opcao < 0 || opcao > 4);

    esc.nivel = (NivelEscolaridade)opcao;

    do {
        printf("\nO nível selecionado está concluído?\n\n");
        printf("1 - Sim\n");
        printf("0 - Não\n\n");
        printf("Opção: ");
        scanf("%d", &esc.completo);

        if (esc.completo != 0 && esc.completo != 1) {
            printf("\nOpção inválida! Digite 1 ou 0.\n");
        }
    } while (esc.completo != 0 && esc.completo != 1);

    return esc;
}

struct funcionario {
    int id;
    char nome[50];
    struct endereco endereco;
    char cpf[12];
    char estadoCivil[49];
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

void configurarConsole() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        setlocale(LC_ALL, ".UTF8");
    #else
        setlocale(LC_ALL, "");
    #endif
}

void pausar() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }

    printf("\nPressione Enter para continuar...");
    getchar();
}

int lerFuncionarioLinha(char linha[], struct funcionario *f) {
    return sscanf(
        linha,
        "%d,%49[^,],%59[^,],%59[^,],%d,%39[^,],%11[^,],%48[^,],%d,%d,%d,%d,%d,%49[^,],%f,%d,%d,%d",
        &f->id,
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

int proximoId() {
    FILE *arquivo = fopen("funcionarios.csv", "r");

    if (arquivo == NULL) {
        return 1;
    }

    char linha[500];
    struct funcionario f;
    int maiorId = 0;

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (lerFuncionarioLinha(linha, &f) == 18) {
            if (f.id > maiorId) {
                maiorId = f.id;
            }
        }
    }

    fclose(arquivo);

    return maiorId + 1;
}

void salvarFuncionario(struct funcionario *f) {
    FILE *arquivo = fopen("funcionarios.csv", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    fprintf(
        arquivo,
        "%d,%s,%s,%s,%d,%s,%s,%s,%d,%d,%d,%d,%d,%s,%.2f,%d,%d,%d\n",
        f->id,
        f->nome,
        f->endereco.rua,
        f->endereco.bairro,
        f->endereco.numero,
        f->endereco.complemento,
        f->cpf,
        f->estadoCivil,
        f->dataNasc.dia,
        f->dataNasc.mes,
        f->dataNasc.ano,
        f->escolaridade.nivel,
        f->escolaridade.completo,
        f->cargo,
        f->salario,
        f->dataAdmis.dia,
        f->dataAdmis.mes,
        f->dataAdmis.ano
    );

    fclose(arquivo);
}

void atualizarFuncionario(struct funcionario *atualizado) {
    FILE *arquivo = fopen("funcionarios.csv", "r");
    FILE *temporario = fopen("temporario.csv", "w");

    if (arquivo == NULL || temporario == NULL) {
        printf("Erro ao abrir arquivo.\n");

        if (arquivo != NULL) {
            fclose(arquivo);
        }

        if (temporario != NULL) {
            fclose(temporario);
        }

        return;
    }

    char linha[500];
    struct funcionario f;

    if (fgets(linha, sizeof(linha), arquivo) != NULL) {
        fputs(linha, temporario);
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (lerFuncionarioLinha(linha, &f) == 18) {
            if (f.id == atualizado->id) {
                fprintf(
                    temporario,
                    "%d,%s,%s,%s,%d,%s,%s,%s,%d,%d,%d,%d,%d,%s,%.2f,%d,%d,%d\n",
                    atualizado->id,
                    atualizado->nome,
                    atualizado->endereco.rua,
                    atualizado->endereco.bairro,
                    atualizado->endereco.numero,
                    atualizado->endereco.complemento,
                    atualizado->cpf,
                    atualizado->estadoCivil,
                    atualizado->dataNasc.dia,
                    atualizado->dataNasc.mes,
                    atualizado->dataNasc.ano,
                    atualizado->escolaridade.nivel,
                    atualizado->escolaridade.completo,
                    atualizado->cargo,
                    atualizado->salario,
                    atualizado->dataAdmis.dia,
                    atualizado->dataAdmis.mes,
                    atualizado->dataAdmis.ano
                );

                continue;
            }
        }

        fputs(linha, temporario);
    }

    fclose(arquivo);
    fclose(temporario);

    remove("funcionarios.csv");
    rename("temporario.csv", "funcionarios.csv");

    printf("\nFuncionário atualizado com sucesso!\n");
}

void lerNome(struct funcionario *f) {
    printf("Nome do funcionário: ");
    scanf(" %49[^\n]", f->nome);
}

void lerCpf(struct funcionario *f) {
    printf("CPF do funcionário: ");
    scanf("%11s", f->cpf);
}

void lerEstadoCivil(struct funcionario *f) {
    printf("Estado civil do funcionário: ");
    scanf(" %48[^\n]", f->estadoCivil);
}

void lerCargo(struct funcionario *f) {
    printf("Cargo do funcionário: ");
    scanf(" %49[^\n]", f->cargo);
}

void lerSalario(struct funcionario *f) {
    printf("Salário do funcionário: ");
    scanf("%f", &f->salario);
}

void menuAdicionar() {
    struct funcionario funcionarioAtual;

    limparConsole();

    funcionarioAtual.id = proximoId();

    printf("Cadastro de funcionário\n\n");

    lerNome(&funcionarioAtual);
    limparConsole();

    funcionarioAtual.endereco = lerEndereco();
    limparConsole();

    lerCpf(&funcionarioAtual);
    limparConsole();

    lerEstadoCivil(&funcionarioAtual);
    limparConsole();

    printf("Data de nascimento do funcionário:\n\n");
    funcionarioAtual.dataNasc = lerData();
    limparConsole();

    funcionarioAtual.escolaridade = lerEscolaridade();
    limparConsole();

    lerCargo(&funcionarioAtual);
    limparConsole();

    lerSalario(&funcionarioAtual);
    limparConsole();

    printf("Data de admissão do funcionário:\n\n");
    funcionarioAtual.dataAdmis = lerData();

    salvarFuncionario(&funcionarioAtual);

    limparConsole();

    printf("Funcionário cadastrado com sucesso!\n");
    printf("ID: %d\n", funcionarioAtual.id);

    pausar();
}

void deletarFunc() {
    FILE *arquivo = fopen("funcionarios.csv", "r");
    FILE *temporario = fopen("temporario.csv", "w");

    if (arquivo == NULL || temporario == NULL) {
        printf("Erro ao abrir o arquivo.\n");

        if (arquivo != NULL) {
            fclose(arquivo);
        }

        if (temporario != NULL) {
            fclose(temporario);
        }

        pausar();

        return;
    }

    int idBuscado;
    int encontrado = 0;

    char linha[500];
    struct funcionario f;

    limparConsole();

    printf("Excluir funcionário\n\n");
    printf("Digite o ID do funcionário que deseja deletar: ");
    scanf("%d", &idBuscado);

    if (fgets(linha, sizeof(linha), arquivo) != NULL) {
        fputs(linha, temporario);
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (lerFuncionarioLinha(linha, &f) == 18) {
            if (f.id == idBuscado) {
                encontrado = 1;
                continue;
            }
        }

        fputs(linha, temporario);
    }

    fclose(arquivo);
    fclose(temporario);

    limparConsole();

    if (encontrado) {
        remove("funcionarios.csv");
        rename("temporario.csv", "funcionarios.csv");

        printf("Funcionário deletado com sucesso!\n");
    } else {
        remove("temporario.csv");

        printf("Funcionário não encontrado.\n");
    }

    pausar();
}

void imprimirFuncionario(struct funcionario *f) {
    printf("ID: %d\n", f->id);
    printf("Nome: %s\n", f->nome);
    printf("Endereço: %s, %d - %s\n", f->endereco.rua, f->endereco.numero, f->endereco.bairro);
    printf("Complemento: %s\n", f->endereco.complemento);
    printf("CPF: %s\n", f->cpf);
    printf("Estado civil: %s\n", f->estadoCivil);
    printf("Data de nascimento: %02d/%02d/%04d\n", f->dataNasc.dia, f->dataNasc.mes, f->dataNasc.ano);
    printf("Escolaridade: %d %s\n", f->escolaridade.nivel, f->escolaridade.completo ? "Sim" : "Não");
    printf("Cargo: %s\n", f->cargo);
    printf("Salário: %.2f\n", f->salario);
    printf("Data de admissão: %02d/%02d/%04d\n", f->dataAdmis.dia, f->dataAdmis.mes, f->dataAdmis.ano);
}

void listarFunc() {
    FILE *arquivo = fopen("funcionarios.csv", "r");

    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        pausar();
        return;
    }

    char linha[500];
    struct funcionario f;
    int encontrado = 0;

    limparConsole();

    printf("Funcionários cadastrados\n\n");

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (lerFuncionarioLinha(linha, &f) == 18) {
            imprimirFuncionario(&f);
            printf("\n");
            encontrado = 1;
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("Nenhum funcionário cadastrado.\n");
    }

    pausar();
}

int buscarFuncionarioPorId(int idBuscado, struct funcionario *f) {
    FILE *arquivo = fopen("funcionarios.csv", "r");

    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return 0;
    }

    char linha[500];

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (lerFuncionarioLinha(linha, f) == 18) {
            if (f->id == idBuscado) {
                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);

    return 0;
}

void buscarFunc() {
    int idBuscado;
    struct funcionario f;

    limparConsole();

    printf("Buscar funcionário\n\n");
    printf("Digite o ID: ");
    scanf("%d", &idBuscado);

    limparConsole();

    if (buscarFuncionarioPorId(idBuscado, &f)) {
        printf("Funcionário encontrado\n\n");
        imprimirFuncionario(&f);
    } else {
        printf("Funcionário não encontrado.\n");
    }

    pausar();
}

void editarInformacao(struct funcionario *f, int sel) {
    switch (sel) {
        case 1:
            lerNome(f);
        break;

        case 2:
            f->endereco = lerEndereco();
        break;

        case 3:
            lerCpf(f);
        break;

        case 4:
            lerEstadoCivil(f);
        break;

        case 5:
            printf("Data de nascimento do funcionário:\n\n");
            f->dataNasc = lerData();
        break;

        case 6:
            f->escolaridade = lerEscolaridade();
        break;

        case 7:
            lerCargo(f);
        break;

        case 8:
            lerSalario(f);
        break;

        case 9:
            printf("Data de admissão do funcionário:\n\n");
            f->dataAdmis = lerData();
        break;

        default:
            printf("Opção inválida!\n");
    }
}

void menuEditar() {
    int sel;
    int idBuscado;
    struct funcionario f;

    limparConsole();

    printf("Editar funcionário\n\n");
    printf("Digite o ID do funcionário: ");
    scanf("%d", &idBuscado);

    if (!buscarFuncionarioPorId(idBuscado, &f)) {
        limparConsole();

        printf("Funcionário não encontrado.\n");

        pausar();

        return;
    }

    limparConsole();

    printf("Funcionário: %s\n\n", f.nome);
    printf("Selecione uma opção:\n\n");
    printf("1 - Editar nome\n");
    printf("2 - Editar endereço\n");
    printf("3 - Editar CPF\n");
    printf("4 - Editar estado civil\n");
    printf("5 - Editar data de nascimento\n");
    printf("6 - Editar escolaridade\n");
    printf("7 - Editar cargo\n");
    printf("8 - Editar salário\n");
    printf("9 - Editar data de admissão\n");
    printf("0 - Voltar\n\n");
    printf("Opção: ");
    scanf("%d", &sel);

    if (sel == 0) {
        return;
    }

    if (sel < 1 || sel > 9) {
        limparConsole();

        printf("Opção inválida!\n");

        pausar();

        return;
    }

    limparConsole();

    editarInformacao(&f, sel);
    atualizarFuncionario(&f);

    pausar();
}

int menuPrincipal() {
    int sel;

    limparConsole();

    printf("Sistema de funcionários\n\n");
    printf("Selecione uma opção:\n\n");
    printf("1 - Adicionar um funcionário\n");
    printf("2 - Listar funcionários\n");
    printf("3 - Buscar funcionário\n");
    printf("4 - Editar funcionário\n");
    printf("5 - Deletar funcionário\n");
    printf("0 - Sair\n\n");
    printf("Opção: ");
    scanf("%d", &sel);

    switch (sel) {
        case 1:
            menuAdicionar();
        break;

        case 2:
            listarFunc();
        break;

        case 3:
            buscarFunc();
        break;

        case 4:
            menuEditar();
        break;

        case 5:
            deletarFunc();
        break;

        case 0:
            limparConsole();

            printf("Programa encerrado.\n");

            return 0;

        default:
            limparConsole();

            printf("Opção inválida.\n");

            delay(2);
    }

    return 1;
}

int main() {
    configurarConsole();

    FILE *arquivo = fopen("funcionarios.csv", "r");

    if (arquivo == NULL) {
        arquivo = fopen("funcionarios.csv", "w");

        if (arquivo == NULL) {
            printf("Não foi possível criar o arquivo.\n");
            return 1;
        }

        fprintf(
            arquivo,
            "id,nome,rua,bairro,numero,complemento,cpf,estadoCivil,"
            "diaNasc,mesNasc,anoNasc,escolaridade,completo,cargo,"
            "salario,diaAdmis,mesAdmis,anoAdmis\n"
        );
    }

    fclose(arquivo);

    while (menuPrincipal()) {
    }

    return 0;
}