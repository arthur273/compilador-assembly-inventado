# Trabalho 1 de Software Básico 

Trabalho 1 da disciplina de Software Básico, que consiste na implementação em C++ de um montador e ligador para o assembly inventado apresentado em sala de aula.

## Alunos

- José Barbosa Araújo (Matrícula: 202014305)
- Arthur Rattes Lacava (Matrícula: 200053523)

## Compilador e SO utilizados

O sistema operacional utilizado para desenvolver o projeto foi o Windows, utilizando como compilador o GNU GCC e Code::Blocks.

## Como compilar os arquivos

Para compilar os arquivos, é necessário utilizar os seguintes comandos:

```
g++ -o montador main.cpp
g++ -o ligador ligador.cpp
```
O primeiro compila o arquivo do montador e o segundo o arquivo do ligador. 

## Execução dos programas

O programa do montador tem dois tipos de execução, indicados por flags. 
### Pré-processamento
O primeiro tipo de execução é o pré-processamento, que pode receber qualquer arquivo escrito no assembly inventado. É indicado pela flag -p como indicado abaixo:

```
./montador -p teste.asm
```
Essa execução resulta em um arquivo com pré-processamento de comandos IF e EQU e com extensão .PRE.

### Geração do código objeto
O segundo tipo de execução é a geração do código objeto, indicado pela flag -o. Aceita como entrada arquivos com a extensão .PRE que já tenham passados pelo pré-processamento realizado na execução anterior. 

```
./montador -o teste.pre
```
Resulta em um arquivo objeto composto de uma única linha de código, com extensão .obj - "teste.obj". Pode ser executado no simulador.

### Ligação
Essa etapa é realizada pelo programa do ligador, que tem apenas um tipo de execução, e funciona com no máximo dois arquivos unicamente. As entradas do ligador devem ser os arquivos obj gerados pelo montador em caso de ter achado diretivas BEGIN e END. Deve ser usado da seguinte forma:
```
./ligador teste.obj teste2.obj
```
Gera como saída um arquivo “.e”, contendo uma única linha de código objeto igual ao OBJ que nao precisa ser ligado. Esse arquivo de saída pode ser executado no simulador.

### Observação 
Notamos que o executavel do simulador é fechado automaticamente ao ler uma operação "STOP" no código objeto, portanto para seu devido funcionamento as operações "STOP" devem ser removidas do codigo .asm inicial e então feito todo o processo de montagem e ligação caso haja begin e end.
