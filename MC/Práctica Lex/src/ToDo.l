    /*------Sección de Declaraciones------*/
%{
#include <stdio.h>

int n_linea = 1;

char aux[1000];

char vacio[100000];
char tipo0[100000];
char tipo1[100000];
char tipo2[100000];
char referencia[100000];

%}

letter [a-zA-Z]
number [0-9]

protocol ("http"|"https")"://"
address "www"\..+\.{letter}{1,3}
path \/({letter}|{number})?

url {protocol}{address}{path}?

date {number}{1,2}\/{number}{1,2}\/{number}{4}
any (.+|.?)
restoLinea {any}\n

/* Separdor para indicar comentario (los TODOs se escriben en comentarios del  lengauje*/
separator (\/\/|#)
todoHeader {separator}[ ]?"TODO"[ ]?:
todoVacio ([ ]?|[ ]+)\n
todoTipo0  {restoLinea}
todoTipo1  "Deadline":[ ]?{date}{restoLinea}
todoTipo2  "Deadline":[ ]?{date}{any}"URGENTE"{restoLinea}
referencia  {separator}?[ ]?"See also:"[ ]?{url}

%%

{referencia} { sprintf(aux, "Linea %d: %s", n_linea, yytext); strcat(aux, "\n"); strcat(referencia, aux); }
{todoHeader}[ ]?{todoVacio} { sprintf(aux, "Linea %d: %s", n_linea, yytext); strcat(vacio, aux); n_linea++; }
{todoHeader}[ ]?{todoTipo2} { sprintf(aux, "Linea %d: %s", n_linea, yytext); strcat(tipo2, aux); n_linea++; }
{todoHeader}[ ]?{todoTipo1} { sprintf(aux, "Linea %d: %s", n_linea, yytext); strcat(tipo1, aux); n_linea++; }
{todoHeader}[ ]?{todoTipo0} { sprintf(aux, "Linea %d: %s", n_linea, yytext); strcat(tipo0, aux); n_linea++; }

.   {}                                        
\n  {n_linea++;}      

%%
    /*------Sección de Procedimientos------*/

int main( int argc, char *argv[] ){
    if( argc == 2 ) {
        yyin = fopen ( argv[1], "rt" );
        if( yyin == NULL ) {
            printf("El fichero %s no se puede abrir\n", argv[1]);
            exit(-1);
        }
    }
    else 
        yyin = stdin;

    yylex();

    printf("Numero de lineas del fichero: %d", n_linea);

    //========Escribimos la lista de todos========

    FILE *f = fopen("todos.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "TODOS del archivo: %s", argv[1]);
    fprintf(f, "\n\n===============TODOs/Checks incorrectos (vacios)===============\n");
    fprintf(f, vacio);
    fprintf(f, "\n\n===============TODOs/Checks tipo 0===============\n");
    fprintf(f, tipo0);
    fprintf(f, "\n\n===============TODOs/Checks tipo 1===============\n");
    fprintf(f, tipo1);
    fprintf(f, "\n\n===============TODOs/Checks tipo 2===============\n");
    fprintf(f, tipo2);
    fprintf(f, "\n\n===============Referencias===============\n");
    fprintf(f, referencia);

    fclose(f);

    //=============================================

    return 0;

}