#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct
{
    char posFixa;
    char inFixa;
    float Valor;
} Expressao;

char *getFormaPosFixa(char *StrInfixa);
char *getFormaInFixa(char *StrPosFixa);
float getValorPosFixa(char *StrPosFixa);
float getValorInFixa(char *StrInfixa);

#endif