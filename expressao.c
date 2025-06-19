#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expressao.h"

#define MAX_PILHA 512

typedef struct
{
    float items[MAX_PILHA];
    int topo;
} PilhaFloat;

void initPilhaFloat(PilhaFloat *p) { p->topo = -1; }
int isFullPilhaFloat(PilhaFloat *p) { return p->topo == MAX_PILHA - 1; }
int isEmptyPilhaFloat(PilhaFloat *p) { return p->topo == -1; }
void pushPilhaFloat(PilhaFloat *p, float value)
{
    if (!isFullPilhaFloat(p))
    {
        p->items[++p->topo] = value;
    }
}
float popPilhaFloat(PilhaFloat *p)
{
    if (!isEmptyPilhaFloat(p))
    {
        return p->items[p->topo--];
    }
    return NAN;
}

typedef struct
{
    char *items[MAX_PILHA];
    int topo;
} PilhaString;

void initPilhaString(PilhaString *p) { p->topo = -1; }
int isFullPilhaString(PilhaString *p) { return p->topo == MAX_PILHA - 1; }
int isEmptyPilhaString(PilhaString *p) { return p->topo == -1; }
void pushPilhaString(PilhaString *p, char *value)
{
    if (!isFullPilhaString(p))
    {
        p->items[++p->topo] = value;
    }
}
char *popPilhaString(PilhaString *p)
{
    if (!isEmptyPilhaString(p))
    {
        return p->items[p->topo--];
    }
    return NULL;
}
char *peekPilhaString(PilhaString *p)
{
    if (!isEmptyPilhaString(p))
    {
        return p->items[p->topo];
    }
    return NULL;
}

int isOperador(const char *token)
{
    return strcmp(token, "+") == 0 ||
           strcmp(token, "-") == 0 ||
           strcmp(token, "*") == 0 ||
           strcmp(token, "/") == 0 ||
           strcmp(token, "%") == 0 ||
           strcmp(token, "^") == 0 ||
           strcmp(token, "~") == 0;
}

int isFuncao(const char *token)
{
    return strcmp(token, "raiz") == 0 ||
           strcmp(token, "sen") == 0 ||
           strcmp(token, "cos") == 0 ||
           strcmp(token, "tg") == 0 ||
           strcmp(token, "log") == 0;
}

int getPrecedencia(const char *op)
{
    if (strcmp(op, "^") == 0)
        return 4;
    if (strcmp(op, "~") == 0)
        return 3;
    if (strcmp(op, "*") == 0 ||
        strcmp(op, "/") == 0 ||
        strcmp(op, "%") == 0)
        return 2;
    if (strcmp(op, "+") == 0 ||
        strcmp(op, "-") == 0)
        return 1;
    return 0;
}

int isAssociativoEsquerda(const char *op)
{
    if (strcmp(op, "^") == 0 ||
        strcmp(op, "~") == 0)
        return 0;
    return 1;
}

int isNumero(const char *token)
{
    if (!token || *token == '\0')
        return 0;

    int i = 0;
    if (token[0] == '-')
        i++;

    int ponto = 0;
    int digitos = 0;

    for (; token[i] != '\0'; i++)
    {
        if (isdigit((unsigned char)token[i]))
        {
            digitos = 1;
        }
        else if (token[i] == '.')
        {
            if (ponto)
                return 0;
            ponto = 1;
        }
        else
        {
            return 0;
        }
    }
    return digitos;
}

char **tokenizar(const char *expressao, int *num_tokens)
{
    char **tokens = malloc(sizeof(char *) * MAX_PILHA);
    int count = 0;
    const char *p = expressao;

    while (*p)
    {
        if (isspace((unsigned char)*p))
        {
            p++;
            continue;
        }

        char buffer[64] = {0};
        int i = 0;
        char *prev_token = (count > 0) ? tokens[count - 1] : NULL;

        if (*p == '-' && (prev_token == NULL ||
                          strcmp(prev_token, "(") == 0 ||
                          isOperador(prev_token) ||
                          isFuncao(prev_token)))
        {
            buffer[i++] = '~';
            p++;
        }
        else if (isdigit((unsigned char)*p) ||
                 (*p == '.' && isdigit((unsigned char)p[1])))
        {
            while (isdigit((unsigned char)*p) || *p == '.')
            {
                buffer[i++] = *p++;
            }
        }
        else if (isalpha((unsigned char)*p))
        {
            while (isalpha((unsigned char)*p))
            {
                buffer[i++] = *p++;
            }
        }
        else
        {
            buffer[i++] = *p++;
        }

        buffer[i] = '\0';
        tokens[count] = strdup(buffer);
        count++;
    }

    *num_tokens = count;
    return tokens;
}

char *getFormaPosFixa(char *StrInfixa)
{
    int num_tokens;
    char **tokens = tokenizar(StrInfixa, &num_tokens);
    if (!tokens)
        return NULL;

    PilhaString op_pilha;
    initPilhaString(&op_pilha);
    char **saida = malloc(sizeof(char *) * (num_tokens * 2));
    int saida_count = 0;

    for (int i = 0; i < num_tokens; i++)
    {
        char *token = tokens[i];

        if (isNumero(token))
        {
            saida[saida_count++] = strdup(token);
        }
        else if (isFuncao(token))
        {
            pushPilhaString(&op_pilha, strdup(token));
        }
        else if (isOperador(token))
        {
            while (!isEmptyPilhaString(&op_pilha) &&
                   strcmp(peekPilhaString(&op_pilha), "(") != 0 &&
                   (getPrecedencia(peekPilhaString(&op_pilha)) > getPrecedencia(token) ||
                    (getPrecedencia(peekPilhaString(&op_pilha)) == getPrecedencia(token) &&
                     isAssociativoEsquerda(token))))
            {
                saida[saida_count++] = popPilhaString(&op_pilha);
            }
            pushPilhaString(&op_pilha, strdup(token));
        }
        else if (strcmp(token, "(") == 0)
        {
            pushPilhaString(&op_pilha, strdup(token));
        }
        else if (strcmp(token, ")") == 0)
        {
            while (!isEmptyPilhaString(&op_pilha) &&
                   strcmp(peekPilhaString(&op_pilha), "(") != 0)
            {
                saida[saida_count++] = popPilhaString(&op_pilha);
            }
            if (isEmptyPilhaString(&op_pilha) ||
                strcmp(peekPilhaString(&op_pilha), "(") != 0)
            {
                for (int j = 0; j < num_tokens; j++)
                    free(tokens[j]);
                free(tokens);
                for (int j = 0; j < saida_count; j++)
                    free(saida[j]);
                free(saida);
                while (!isEmptyPilhaString(&op_pilha))
                    free(popPilhaString(&op_pilha));
                return NULL;
            }
            free(popPilhaString(&op_pilha));

            if (!isEmptyPilhaString(&op_pilha) && isFuncao(peekPilhaString(&op_pilha)))
            {
                saida[saida_count++] = popPilhaString(&op_pilha);
            }
        }
    }

    while (!isEmptyPilhaString(&op_pilha))
    {
        char *op = popPilhaString(&op_pilha);
        if (strcmp(op, "(") == 0)
        {
            free(op);
            for (int j = 0; j < num_tokens; j++)
                free(tokens[j]);
            free(tokens);
            for (int j = 0; j < saida_count; j++)
                free(saida[j]);
            free(saida);
            while (!isEmptyPilhaString(&op_pilha))
                free(popPilhaString(&op_pilha));
            return NULL;
        }
        saida[saida_count++] = op;
    }

    int total_len = 1;
    for (int i = 0; i < saida_count; i++)
    {
        total_len += strlen(saida[i]) + 1;
    }

    char *resultado = malloc(total_len);
    resultado[0] = '\0';

    for (int i = 0; i < saida_count; i++)
    {
        strcat(resultado, saida[i]);
        if (i < saida_count - 1)
            strcat(resultado, " ");
        free(saida[i]);
    }

    for (int i = 0; i < num_tokens; i++)
        free(tokens[i]);
    free(tokens);
    free(saida);

    return resultado;
}

float getValorPosFixa(char *StrPosFixa)
{
    PilhaFloat operandos;
    initPilhaFloat(&operandos);
    char *str_copy = strdup(StrPosFixa);
    if (!str_copy)
        return NAN;

    char *token = strtok(str_copy, " ");
    while (token != NULL)
    {
        if (isNumero(token))
        {
            pushPilhaFloat(&operandos, atof(token));
        }
        else
        {
            float op1, op2, resultado = NAN;

            if (isFuncao(token) || strcmp(token, "~") == 0)
            {
                if (isEmptyPilhaFloat(&operandos))
                {
                    free(str_copy);
                    return NAN;
                }
                op1 = popPilhaFloat(&operandos);

                if (strcmp(token, "raiz") == 0)
                    resultado = sqrt(op1);
                else if (strcmp(token, "sen") == 0)
                    resultado = sin(op1 * M_PI / 180.0);
                else if (strcmp(token, "cos") == 0)
                    resultado = cos(op1 * M_PI / 180.0);
                else if (strcmp(token, "tg") == 0)
                    resultado = tan(op1 * M_PI / 180.0);
                else if (strcmp(token, "log") == 0)
                    resultado = log10(op1);
                else if (strcmp(token, "~") == 0)
                    resultado = -op1;
            }
            else
            {
                if (isEmptyPilhaFloat(&operandos))
                {
                    free(str_copy);
                    return NAN;
                }
                op2 = popPilhaFloat(&operandos);

                if (isEmptyPilhaFloat(&operandos))
                {
                    free(str_copy);
                    return NAN;
                }
                op1 = popPilhaFloat(&operandos);

                if (strcmp(token, "+") == 0)
                    resultado = op1 + op2;
                else if (strcmp(token, "-") == 0)
                    resultado = op1 - op2;
                else if (strcmp(token, "*") == 0)
                    resultado = op1 * op2;
                else if (strcmp(token, "/") == 0)
                {
                    if (op2 == 0)
                    {
                        free(str_copy);
                        return NAN;
                    }
                    resultado = op1 / op2;
                }
                else if (strcmp(token, "%") == 0)
                {
                    if ((int)op2 == 0)
                    {
                        free(str_copy);
                        return NAN;
                    }
                    resultado = fmod(op1, op2);
                }
                else if (strcmp(token, "^") == 0)
                    resultado = pow(op1, op2);
            }

            if (isnan(resultado))
            {
                free(str_copy);
                return NAN;
            }
            pushPilhaFloat(&operandos, resultado);
        }
        token = strtok(NULL, " ");
    }

    free(str_copy);
    if (operandos.topo == 0)
    {
        return popPilhaFloat(&operandos);
    }
    return NAN;
}

char *getFormaInFixa(char *StrPosFixa)
{
    PilhaString pilha;
    initPilhaString(&pilha);
    char *str_copy = strdup(StrPosFixa);
    if (!str_copy)
        return NULL;

    char *token = strtok(str_copy, " ");
    while (token != NULL)
    {
        if (isNumero(token))
        {
            pushPilhaString(&pilha, strdup(token));
        }
        else
        {
            char *expr = malloc(MAX_PILHA * 3);
            expr[0] = '\0';

            if (isFuncao(token) || strcmp(token, "~") == 0)
            {
                if (isEmptyPilhaString(&pilha))
                {
                    free(str_copy);
                    free(expr);
                    return NULL;
                }
                char *arg = popPilhaString(&pilha);
                snprintf(expr, MAX_PILHA * 3, "%s(%s)", token, arg);
                free(arg);
            }
            else
            {
                if (isEmptyPilhaString(&pilha))
                {
                    free(str_copy);
                    free(expr);
                    return NULL;
                }
                char *dir = popPilhaString(&pilha);

                if (isEmptyPilhaString(&pilha))
                {
                    free(str_copy);
                    free(expr);
                    free(dir);
                    return NULL;
                }
                char *esq = popPilhaString(&pilha);

                snprintf(expr, MAX_PILHA * 3, "(%s %s %s)", esq, token, dir);
                free(esq);
                free(dir);
            }
            pushPilhaString(&pilha, expr);
        }
        token = strtok(NULL, " ");
    }

    free(str_copy);
    if (pilha.topo == 0)
    {
        return popPilhaString(&pilha);
    }

    while (!isEmptyPilhaString(&pilha))
    {
        free(popPilhaString(&pilha));
    }
    return NULL;
}

float getValorInFixa(char *StrInfixa)
{
    char *posfixa = getFormaPosFixa(StrInfixa);
    if (!posfixa)
        return NAN;

    float valor = getValorPosFixa(posfixa);
    free(posfixa);
    return valor;
}