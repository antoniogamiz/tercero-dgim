    /*------Sección de Declaraciones------*/
%{
#include <stdio.h>

int nc, np, nl;

void escribir(int dato1, int dato2, int dato3);
%}

letter [a-zA-Z]
number [0-9]
protocol ("http"|"https")"://"
address "www"\..+\.{letter}+
path \/({letter}|{number})+
%%

({letter}|{number}|[\._-])+@{letter}+\.{letter}+ {  printf("Correo electrónico encontrado: %s\n", yytext); }
{protocol}{address}{path}*  {  printf("Página Web encontrada: %s\n", yytext); }
.   {}                                        
\n  {}      

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

    return 0;

}

void escribir_datos(int dato1, int dato2, int dato3) {
    printf("Num_caracteres=%d \t Num_palabras=%d \t Num_lineas=%d \n", dato1, dato2, dato3);
}