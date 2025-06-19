#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // Para isnan()
#include "expressao.h"

// Função auxiliar para imprimir os resultados de forma organizada
void executarTeste(int id, const char *infixa, const char *posfixa_esperada)
{
    printf("--- Teste %d ---\n", id);
    printf("Infixa Original: %s\n", infixa);

    // Teste 1: Converter Infixa para Posfixa
    char *posfixa_calculada = getFormaPosFixa((char *)infixa);
    if (posfixa_calculada)
    {
        printf("Posfixa Esperada:  %s\n", posfixa_esperada);
        printf("Posfixa Calculada: %s\n", posfixa_calculada);
        if (strcmp(posfixa_calculada, posfixa_esperada) == 0)
        {
            printf("  -> Conversao Infixa->Posfixa: SUCESSO\n");
        }
        else
        {
            printf("  -> Conversao Infixa->Posfixa: FALHA\n");
        }
    }
    else
    {
        printf("Posfixa Calculada: ERRO (NULL)\n");
        printf("  -> Conversao Infixa->Posfixa: FALHA\n");
    }

    // Teste 2: Avaliar valor a partir da Infixa
    float valor_infixa = getValorInFixa((char *)infixa);
    if (!isnan(valor_infixa))
    {
        printf("Valor (via Infixa):  %.4f\n", valor_infixa);
    }
    else
    {
        printf("Valor (via Infixa):  ERRO (NAN)\n");
    }

    // Teste 3: Avaliar valor a partir da Posfixa (calculada)
    if (posfixa_calculada)
    {
        float valor_posfixa = getValorPosFixa(posfixa_calculada);
        if (!isnan(valor_posfixa))
        {
            printf("Valor (via Posfixa): %.4f\n", valor_posfixa);
            if (!isnan(valor_infixa) && fabs(valor_infixa - valor_posfixa) < 1e-4)
            {
                printf("  -> Avaliacao: SUCESSO\n");
            }
            else
            {
                printf("  -> Avaliacao: FALHA\n");
            }
        }
        else
        {
            printf("Valor (via Posfixa): ERRO (NAN)\n");
            printf("  -> Avaliacao: FALHA\n");
        }
    }

    // Teste 4: Converter Posfixa (calculada) de volta para Infixa
    if (posfixa_calculada)
    {
        char *infixa_revertida = getFormaInFixa(posfixa_calculada);
        if (infixa_revertida)
        {
            printf("Infixa Revertida:   %s\n", infixa_revertida);
            free(infixa_revertida); // Libera memória
        }
        else
        {
            printf("Infixa Revertida:   ERRO (NULL)\n");
        }
    }

    // Libera a memória da string posfixa calculada
    if (posfixa_calculada)
    {
        free(posfixa_calculada);
    }
    printf("\n");
}

int main()
{
    printf("=============================================\n");
    printf("  Avaliador de Expressões Numéricas\n");
    printf("=============================================\n\n");

    // Testes do documento
    executarTeste(1, "(3+4)*5", "3 4 + 5 *");
    executarTeste(2, "7*2+4", "7 2 * 4 +");
    executarTeste(3, "8+(5*(2+4))", "8 5 2 4 + * +");
    executarTeste(4, "(6/2+3)*4", "6 2 / 3 + 4 *");
    executarTeste(5, "9+(5*(2+8*4))", "9 5 2 8 4 * + * +");
    executarTeste(6, "log(2+3)/5", "2 3 + log 5 /");
    executarTeste(7, "(log(10))^3+2", "10 log 3 ^ 2 +");
    executarTeste(8, "(45+60)*cos(30)", "45 60 + 30 cos *");
    executarTeste(9, "sen(45)^2+0.5", "45 sen 2 ^ 0.5 +");

    // Testes adicionais
    printf("=============================================\n");
    printf("  Testes Adicionais\n");
    printf("=============================================\n\n");
    executarTeste(10, "10 * -5", "10 ~5 *");                   // Menos unário
    executarTeste(11, "log(sen(90) * 10)", "90 sen 10 * log"); // Funções aninhadas
    executarTeste(12, "100 / (10 - 10)", "100 10 10 - /");     // Divisão por zero
    executarTeste(13, "(5 + 2 * 3", "5 2 3 * +");              // Erro de sintaxe (parêntese)
    executarTeste(14, "2 ^ 3 ^ 2", "2 3 2 ^ ^");               // Associatividade à direita
    executarTeste(15, "raiz(9) * (10 + log(100)) - 5", "9 raiz 10 100 log + * 5 -");

    return 0;
}