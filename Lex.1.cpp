//Use el estado como el estado actual y luego escriba el programa de acuerdo con el diagrama de transición de estado anterior para ayudar a algunas funciones
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>

using namespace std;


const int KEY_NUM = 18; // Definir la cantidad de palabras clave ademas de los 3 tipos que vamos a utilizar 
const string KEY_SET[]={ // Colección de palabras clave
        "for",
        "while",
        "do",
        "switch",
        "case",
        "break",
        "continue",
        "if",
        "else",
        "char",//tipo de dato
        "int",//tipo de dato
        "float",//tipo de dato 
        "return",
        "main",
        "const",
        "extern",
        "printf",
        "vector",
};
map<string,int> word_num; //Número de palabras
const char* FILE_NAME="./infile.txt";
char strBuffer[1024]; // Búfer de cadenas
string token;
const string operadores_relacionales[] = {">", "<", ">=", "<=", "=", "<>"};
const string operadores_aritmeticos[] = {"*", "/", "%", "^"};
int line_num=0; // El número de líneas del programa a contar, que representa el número de línea actual, que se utiliza para localizar la ubicación del error.
int char_num=0; // Cuente el número real de caracteres que no son espacios ni comentarios
int state=0; // El estado actual del programa

char getChar(char* &str) // Obtener el carácter en la posición actual del puntero de la cadena
{
    return *str++;
}
//verificamos si la entrada es digito
bool esunDigito(char c)
{
    return (c>='0' && c<='9');
}
//es una letra
bool esunaLetra(char c)
{
    return (c>='a'&&c<='z') || (c>='A'&& c<='Z');
}

bool operadorAritmetico(char c) // Determinar si es un signo de puntuación
{
    return c=='*' || c=='/' || c=='%' || c=='^';
}
//es una palabra clave, funcin que nos sirve para poder verificar si es palabra clave 
bool palabraReservada(string str)
{
    for(int i=0;i<KEY_NUM;i++)
        if( str==KEY_SET[i] )
            return true;
    return false;
}

//funcion de error  
void error()
{
    printf("en la linea %d hay un error \n",line_num);
}

void process_string(char* buf) // El contenido de una línea de cadena
{
    if(state != 4) // el estado 4 es de comentario , lo que significa que es un bloque de comentarios
      state = 0;
    char C=' ';//inicianizando en c 
    //printf("%d\n",line_num );
    int pos=0;
    C = getChar(buf); // Buffer de datos para poder analizar 
    while( C!='\n' && C!='\0' ) // 
    {
        char_num++; // Cuenta el número de caracteres 
        switch(state){ // Analizar el estado actual
            case 0: // En el estado de lectura de cadena en nombre de, no ha ingresado ningún autómata
                if( esunaLetra(C) || C=='_' )
                {//caso id 
					state = 1; // Ingrese el estado 1
                    token = C;
                    C = getChar(buf);
                }
                else if( esunDigito(C) )
                { 
                    state = 2; //Se va a el estado 2
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='/' ) //comentarios 
                {
                    state =3;
                    C = getChar(buf);
                }
                else if( C==' ' ) //Espacio
                {
                    state = 0;
                    C = getChar(buf);
                }
                 else if (C=='+'){
                	state =9;
                	token = C;
                    C = getChar(buf);
				}
				else if (C=='-'){
                	state =10;
                	token = C;
                    C = getChar(buf);
				}
                else if( C=='<' )
                {
                    state = 11;
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='>' )
                {
                    state = 12;
                    token = C;
                    C = getChar(buf);
                }
                else if( C=='=' )
                {
                    state = 13;
                    token = C;
                    C = getChar(buf);
                }
    			 else if( operadorAritmetico(C) )
                {
                    state = 14;
                    printf("< %c, - >",C);
                    printf("%d\n",line_num );

                    C = getChar(buf);
                }
                else{
                    error();
                    // Omitir estecaracter 
                    C = getChar(buf);
                }
                break;
            case 1:
                if( esunaLetra(C) || esunDigito(C) || C == '_' )
                {
                    state=1;//estado 1 del id 
                    token += C;
                    C = getChar(buf);
                }
                else{ // Al separador
                    word_num[token]++; // Cuenta el número total de cada caract
                    if( palabraReservada(token) )
                    {
                        cout<<"<Pr, "<<token<<" >";
                        printf("%d\n",line_num );

                    }
                    else
                    {
                        cout<<"<id, "<<token<<" >";
                        printf("%d\n",line_num );

                    }

                    state = 0;
                }
                break;
            case 2: // nummero 
                if( esunDigito(C) )
                {
                    state = 2;
                    token += C;
                    C = getChar(buf);
                }else if(C=='.'){//si viene un punto 3.
                	state=7;
                	token+=C;
                	C=getChar(buf);
				}else if(C=='e'){//si viene un e 3e
					state=8;
                	token+=C;
                	C=getChar(buf);
				}
                else
                {
                    //int num=stoi(token); // La cadena se convierte a int para poder imprimirle 
                    //cout<<"< token, "<<num<<" >";
                    printf("%d\n",line_num );

                    state = 0;
                }
                break;
            case 3: //comentarios 
                if(C=='*'){
                    state = 4; // *
         	        token += C;
                    C = getChar(buf);
                }
                else if(C=='/')
                {
                    state = 5;
                    token += C;
                    //deteccion de los comentarios
                    cout<<"< //, - >";
                    printf("%d\n",line_num );
                }
                break;
            case 4:
                if(C=='*') {
                    state = 6;
                    C = getChar(buf);
                }
                else
                {//cualquier otra cosa 
                    state = 4;
                    C = getChar(buf);
                }
                break;
            case 5://cualquier otra cosa en el comentario de // 
                    state = 5;
                    C = getChar(buf);
                break;
            case 6:
                if(C=='/')
                {
                    state = 0;
                    C = getChar(buf);
                
                }
                break;
            case 7:
                 if(esunDigito(C)){
                 	 //int num=stoi(token); // La cadena se convierte a int
                    //cout<<"< num, "<<num<<" >";
                    printf("%d\n",line_num );
                 	state=10;
                 	token+= C;
					C = getChar(buf); 
				 }
                break;
            case 8:
                 if(esunDigito(C)){
                 	state=11;
                 	token+= C;
					C = getChar(buf); 
				 } else if(C == '-'){
				 	state=9;
                 	token+= C;
					C = getChar(buf); 
				 }else if(C == '+'){
				 	state=9;
                 	token+= C;
					C = getChar(buf); 
				 }
                break;
            case 9:
                   if(esunDigito(C)){
                   	state=11;
                 	token+= C;
					C = getChar(buf); 
				   }
                break;
            case 10:
                if(esunDigito(C))
                {
                 
            	    state = 10;
            	    token+=C;
                    C=getChar(buf);
                }
                else if(C=='e')
                {
               	    state = 8;
               	    token+=C;
               	    C=getChar(buf);
                }
                break;
            case 11:
                if(esunDigito(C))
                {
                	state =11;
               	    token+=C;
               	    C=getChar(buf);
                    
                }else
                {
                    //int num=stoi(token); // La cadena se convierte a int
                    //cout<<"< num, "<<num<<" >";
                    printf("%d\n",line_num );
                    state = 0;
                }
                break;
            case 12:
            	if(esunDigito(C)){
            			//numero con signo +
            	state=2;
            	token += C;
                C = getChar(buf);
				}else{
					//solo e simbolo +
                    state = 0;
                    C=getChar(buf);
				}
                //si no viene un digito entonces es solo +
            	state=0;
            break;	
            case 13://numero con signo -
 				if(esunDigito(C)){//numero con signo +
            	state=2;
            	token += C;
                C = getChar(buf);
				} else{
						state=0;
					C=getChar(buf);
				}
				      //si no viene un digito entonces es solo -	state=0;  
            break;
            case 14:
                if(operadorAritmetico(C))
                {
                    cout<<"<OPERADOR:  - >";
                    printf("%d\n",line_num );

                    state=0;
                    C = getChar(buf);
                }
                break;
            case 15:
            	if(C=='='){
            		  cout<<"< =, - >";
                    printf("%d\n",line_num );
            		state = 0;
                    C=getChar(buf);
				}
            break;
            case 16:
            	if(C=='='){
            		  cout<<"< =, - >";
                    printf("%d\n",line_num );
            			state = 0;
                    C=getChar(buf);
				}
            break;	
            case 17:
            	if(C=='='){
            	cout<<"< =, - >";
                    printf("%d\n",line_num );
            	state = 0;
                    C=getChar(buf);
				}
           break;
           default: error();

                    state=0;
                    C = getChar(buf);
                break;
        }
    }
}

int main()
{

    FILE* file = fopen(FILE_NAME,"r"); // Abra el archivo a escanear en modo de solo lectura

    freopen("./outfile.txt","w",stdout); // Redirección de salida, los datos de salida se guardarán en el archivo out.txt en el directorio raíz del Disco D

    if( file==NULL )
    {
        printf("Error! opening xfile");
        exit(1);
    }
    // Realice la lectura línea por línea del archivo
    while( !feof(file) )
    {
        line_num++;
        fgets(strBuffer,1024,file);
        process_string(strBuffer); // Cadena de proceso
    }
    fclose(file);

    cout<<"numero de lineas es : "<<line_num<<endl; // Imprime el número de líneas del contenido del archivo
    cout<<"Numero de caracteres: "<<char_num<<endl;
    map<string,int>::iterator iter;
    iter = word_num.begin();
    while(iter!=word_num.end())
    {
        cout<<iter->first<<' '<<iter->second<<endl;
        iter++;
    }
    return 0;
}
