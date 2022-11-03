#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
using namespace std;
const int KEY_NUM = 18; // Definir la cantidad de palabras clave
const string KEY_SET[]={ // Colecci�n de palabras clave
        "for",
        "while",
        "do",
        "switch",
        "case",
        "break",
        "continue",
        "if",
        "else",
        "char",
        "int",
        "float",
        "return",
        "main",
        "const",
        "string",
        "map",
        "vector",
};
map<string,int> word_num; //N�mero de palabras
const char* FILE_NAME="./nuevo.txt";
char strBuffer[1026]; // B�fer de cadenas
string token;
int line_num=0; // El n�mero de l�neas del programa a contar, que representa el n�mero de l�nea actual, que se utiliza para localizar la ubicaci�n del error.
int char_num=0; // Cuente el n�mero real de caracteres que no son espacios ni comentarios
int state=0; // El estado actual del programa
char getChar(char* &str) // Obtener el car�cter en la posici�n actual del puntero de la cadena
{
    return *str++;
}
bool isDigit(char c)
{
    return (c>='0' && c<='9');
}
bool isLetter(char c)
{
    return (c>='a'&&c<='z') || (c>='A'&& c<='Z');
}
bool isPunctuation(char c) // Determinar si es un signo de puntuaci�n
{
    return c=='+' || c=='-' || c=='*' || c=='(' || c==')' || c=='{'
    || c=='}' || c=='[' || c==']'|| c==';' || c==':' || c=='#' || c=='"' || c==',';
}
bool isKey(string str)
{
    for(int i=0;i<KEY_NUM;i++)
        if( str==KEY_SET[i] )
            return true;
    return false;
}
void error()
{
    printf("the %dth line error\n",line_num);
}

void process_string(char* buf) // El contenido de una l�nea de cadena
{
    if(state != 7) // el estado es 8, lo que significa que es un bloque de comentarios
        state = 0;
    char C=' ';
    int pos=0;
    C = getChar(buf); // Leer un personaje de pero
    while( C!='\n' && C!='\0' ) // No es un car�cter de nueva l�nea, no es un terminador
    {
        char_num++; // Cuenta el n�mero de caracteres
        switch(state){ // Analizar el estado actual
            case 0: // En el estado de lectura de cadena en nombre de, no ha ingresado ning�n aut�mata
                if( isLetter(C) || C=='_' )
                {
                    state = 1; // Ingrese el estado 1
                    token = C;
                    C = getChar(buf);
                }
                else if( isDigit(C) )
                {
                    state = 2;
                    token = C;
                    C = getChar(buf);
                }
                else if( isPunctuation(C) )
                {
                    state = 0;
                    printf("< %c, - >\n",C);
                    C = getChar(buf);
                }
                else if( C==' ' ) //Espacio
                {
                    state = 0;
                    C = getChar(buf);
                }
                else if( C=='<' )
                {
                    state = 3;
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='>' )
                {
                    state = 4;
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='=' )
                {
                    state = 5;
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='/' )
                {
                    state = 6;
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='!' )
                {
                    state = 10;
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='&' )
                {
                    state = 11;
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='|' )
                {
                    state = 12;
                    token = C;
                    C = getChar(buf);
                }
                else{
                    error();
                    // Omitir este personaje
                    C = getChar(buf);
                }
                break;
            case 1:
                if( isLetter(C) || C=='_' || isdigit(C) )
                {
                    state=1;
                    token += C;
                    C = getChar(buf);
                }
                else{ // Al separador
                    word_num[token]++; // Cuenta el n�mero total de cada personaje
                    if( isKey(token) )
                    {
                        cout<<"< key, "<<token<<" >"<<endl;
                    }
                    else
                    {
                        cout<<"< id, "<<token<<" >"<<endl;
                    }

                    state = 0;
                }
                break;
            case 2: // Entero sin signo
                if( isDigit(C) )
                {
                    state = 2;
                    token += C;
                    C = getChar(buf);
                }
                else
                {
                    //int num=stoi(token); // La cadena se convierte a int
                    cout<<"< num, "<<token<<" >"<<endl;
                    state = 0;
                }
                break;
            case 3:
                if(C=='=')
                {
                    state = 0;
                    cout<<"< relop, <= >"<<endl;
                    C = getChar(buf);
                }
                else
                {
                    state = 0;
                    cout<<"< relop, < > "<<endl;
                }
                break;
            case 4:
                if(C=='=')
                {
                    state = 0;
                    cout<<"< relop, >= >"<<endl;
                    C = getChar(buf);
                }
                else
                {
                    state = 0;
                    cout<<"< relop, > >"<<endl;
                }
                break;
            case 5:
                if(C=='=')
                {
                    state = 0;
                    cout<<"< relop, == >"<<endl;
                    C = getChar(buf);
                }
                else
                {
                    state = 0;
                    cout<<"< assign_op, = >"<<endl;
                }
                break;
            case 6:
                if(C=='*')
                {
                    state = 7;
                    token += C;
                    C = getChar(buf);
                }
                else if(C=='/')
                {
                    state = 9;
                    token += C;
                    // Comenta esta l�nea de c�digo, ignora el resto
                    cout<<"< //, - >"<<endl;
                    return;
                }
                else{ // Se puede juzgar como el signo de divisi�n
                    state = 0;
                    cout<<"< /, - >"<<endl;
                }
                break;
            case 7:
                if(C=='*')
                {
                    state = 8;
                    C = getChar(buf);
                }
                else
                {
                    state = 7;
                    C = getChar(buf);
                }
                break;
            case 8:
                if(C=='*')
                {
                    state = 8;
                    C = getChar(buf);
                }
                else if(C=='/')
                {
                    state = 0;
                    C = getChar(buf);
                }
                else{
                    state = 7;
                    C = getChar(buf);
                }
                break;
            case 9:
                    state = 9;
                    C = getChar(buf);
                break;
            case 10:
                if(C=='=')
                {
                    cout<<"< relop, != >"<<endl;
                    state = 0;
                    C=getChar(buf);
                }
                else
                {
                    cout<<"< !, - > "<<endl;
                    state = 0;
                }
                break;
            case 11:
                if(C=='&')
                {
                    cout<<"< &&, - >"<<endl;
                    state = 0;
                    C=getChar(buf);
                }
                else{
                    cout<<"< &, - >"<<endl;
                    state=0;
                }
                break;
            case 12:
                if(C=='|')
                {
                    cout<<"< ||, - >"<<endl;
                    state=0;
                    C = getChar(buf);
                }
                else{
                    error();
                    state=0;
                    C = getChar(buf);
                }
        }
    }
}
int main() {
    FILE* file = fopen(FILE_NAME,"r"); // Abra el archivo a escanear en modo de solo lectura
    freopen("./outfile.txt","w",stdout); // Redirecci�n de salida, los datos de salida se guardar�n en el archivo out.txt en el directorio ra�z del Disco D
    if( file==NULL )
    {
        printf("Error! opening file");
        exit(1);
    }
    // Realice la lectura l�nea por l�nea del archivo
    while( !feof(file) )
    {
        line_num++;
        fgets(strBuffer,1024,file);
        process_string(strBuffer); // Cadena de proceso
    }
    fclose(file);

    cout<<"the number of lines in this programme is: "<<line_num<<endl; // Imprime el n�mero de l�neas del contenido del archivo
    cout<<"the number of characters is: "<<char_num<<endl;
    map<string,int>::iterator iter;
    iter = word_num.begin();
    while(iter!=word_num.end())
    {
        cout<<iter->first<<' '<<iter->second<<endl;
        iter++;
    }
    return 0;
    
}

