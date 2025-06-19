# 🧮 Calculadora de Expressões Pós-fixadas

## 📝 Descrição

Este projeto é uma implementação em **C** de uma calculadora capaz de avaliar expressões matemáticas escritas na notação pós-fixada (ou polonesa reversa). O programa utiliza a estrutura de dados de **pilha (stack)**, definida em `expressao.h`, para realizar os cálculos de forma eficiente.

---

## ✨ Funcionalidades

* **Avaliação de Expressões Pós-fixadas:** Calcula o resultado de expressões como `5 3 + 8 *`.
* **Suporte aos Operadores Básicos:** Adição (`+`), Subtração (`-`), Multiplicação (`*`) e Divisão (`/`).
* **Interface de Linha de Comando:** O usuário insere a expressão diretamente no terminal.
* **Validação de Expressão (Opcional):** Verifica se a expressão fornecida é válida antes de calcular.

---

## 🛠️ Tecnologias Utilizadas

O projeto foi desenvolvido utilizando as seguintes tecnologias:

* **Linguagem:** C
* **Compilador:** gcc
* **Estrutura de Dados:** Pilha (Stack) implementada em `expressao.h`
* **Bibliotecas Padrão:** `<stdio.h>`, `<stdlib.h>`, `<string.h>`

---

## 🚀 Como Compilar e Executar

Para compilar e executar o projeto em seu ambiente local, siga os passos abaixo. É necessário ter um compilador C (como o gcc) instalado.

```bash
# 1. Clone o repositório (ou baixe o ZIP)
git clone [https://github.com/SEU-USUARIO/SEU-REPOSITORIO.git](https://github.com/SEU-USUARIO/SEU-REPOSITORIO.git)

# 2. Navegue até o diretório do projeto
cd SEU-REPOSITORIO

# 3. Compile o código-fonte
# Este comando irá compilar 'expressao.c' e gerar um executável chamado 'expressao'
gcc -o expressao expressao.c

# 4. Execute o programa
./expressao
```

Após a execução, o programa solicitará que você insira a expressão pós-fixada.

**Exemplo de Entrada:**
```
5 2 + 8 *
```

**Saída Esperada:**
```
Resultado: 56
```

<!-- Adapte os comandos e nomes de arquivos de acordo com o seu projeto. -->

---

## ✒️ Autor

Desenvolvido por **[Matheus Ferreira de Souza]**.

* **GitHub:** [matfsouza](https://github.com/matfsouza)
* **LinkedIn:** [Matheus Ferreira de Souza ](https://www.linkedin.com/in/matheus-f-souza)
