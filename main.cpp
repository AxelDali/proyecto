//
//  main.cpp
//  Proyecto Final: Guia telefonica
//
//  Basado en codigos anteriores
//  Created by:
//  Axel Dali Gomez Morales 329881
//  Daniel Alberto Cota Ochoa 329701

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream> /// Para leer archivo de texto
#define SIZE 150

using namespace std;

typedef struct tnode{ /// Nodo Trie
    ///char letter;
    struct tnode *children[28];
    bool endWord;
} tnode;
tnode *root= NULL;

struct node{ /// Nodo Tabla Hash
    char name[25];
    char phoneNumber[11];
    node *next;
};

node *arr[SIZE]; /// Tabla Hash

tnode *newNode(char letter); /// Crear nuevo nodo del trie
int index(char letter); /// Indice de un char dentro del trie
tnode *searchTrie(char *key, int *pos); /// Buscar una cadena en el trie
bool searchWord(char *word);
void insertWord(char *word); /// Insertar palabra al trie
tnode *deleteWord(tnode *aux, char *word, int depth=0); /// Borrar palabra del trie
char *append (char *slice, char part); /// Agregar caracter al final de una cadena
void print(tnode *aux, char *slice); /// Imprimir el autocompletado
void autocomplete(tnode *aux, char *prefix);
bool isLastNode(tnode* root); /// Revisar que un nodo sea el ultimo dentro del trie


void menu();
void initialize(); /// Inicializar tabla hash en null
int hashf(char *key); /// Funcion hash
void insertNode(node newNode); /// Insertar nodo a tabla hash
node* searchNode(char *key); /// Buscar nodo en tabla hash
void deleteNode(char *key); /// Borrar nodo en tabla hash

void initializePhoneBook(); /// Inicializar la tabla hash con el archivo de texto

int main()
{
    initialize();
    string aux;
    node auxNode;
    root= newNode(0);
    int len;
    int opt;
    do{
        node *aux_pointer = NULL;
        char key[25];
        system("cls");
        menu();
        getline(cin, aux); /// Lee y verifica la opcion del usuario
        try{
            opt= stoi(aux);
        }
        catch(...){
            opt= 9;
        }
        switch(opt){
        case 1:
            cout<< "Contacto nuevo\n";
            cout<< "Ingrese el nombre: ";
            cin.getline(auxNode.name, 25);
            len= strlen(auxNode.name);
            for(int i=0 ; i<len ; i++){
                auxNode.name[i]= toupper(auxNode.name[i]); /// Convierte la entrada a mayusculas
            }
            cout<< "Ingrese el numero de telefono: ";
            cin.getline(auxNode.phoneNumber, 11);
            insertNode(auxNode);
            cout<< auxNode.name;
            insertWord(auxNode.name);
            break;
        case 2:
            cout<< "Buscar contacto\n";
            cout<< "Ingrese el nombre del contacto a buscar: ";
            cin.getline(key, 25);
            len= strlen(key);
            for(int i=0 ; i<len ; i++){
                key[i]= toupper(key[i]);
            }
            aux_pointer = searchNode(key);
            if(aux_pointer != NULL){
                cout<< "\nNombre: " << aux_pointer->name;
                cout<< "\nTelefono: " << aux_pointer->phoneNumber;
            }
            else{
                cout<< "\nNo existe ese contacto" << key;
            }
            break;
        case 3:
            cout<< "Autocompletar\n";
            cout<< "Ingrese la busqueda: ";
            cin.getline(key, 25);
            autocomplete(root, key);
            break;
        case 4:
            cout<< "Eliminar contacto\n";
            cout<< "Ingrese el nombre del contacto a borrar: ";
            cin.getline(key, 25);
            len= strlen(key);
            for(int i=0 ; i<len ; i++){
                key[i]= toupper(key[i]);
            }
            deleteNode(key);
            deleteWord(root, key);
            break;
        case 5:
            initializePhoneBook();
            cout<< "Se han leido los contactos del archivo con exito :D";
            break;
        case 0:
            break;
        default:
            cout<< "Error: Ingrese una opcion valida\n";
            break;
        }
        if(opt!= 0){
            cout<< "\n\n";
            system("pause");
        }
    }while(opt!= 0);
    cout<< "\n\nArious amigou"; /// Snoop se despide del amigou en cuestion
    return 0;
}

//https://soundcloud.com/user-16256337/policia-espacial

tnode *newNode(char letter){ /// Recibe un char y crea el nodo correspondiente en el trie
    tnode *newN;
    int i;
    newN = (tnode *) malloc(sizeof(tnode));
    if(newN != NULL){
        newN->  endWord = false;
        for(i=0 ; i<28; i++){
            newN->children[i]=NULL;
        }
    }
    return newN;
}

int index(char letter){ ///Regresa el indice de un caracter dentro del trie
    int x = 26;
    if(letter >= 'a' && letter <= 'z'){
        x = letter -  'a' ;
    }
    if(letter >= 'A' && letter <='Z'){
        x = letter -  'A' ;
    }
    return x;
}

tnode *searchTrie(char *key, int *pos){ ///
    int keySize, i, letter;
    tnode *aux;
    aux = root;
    keySize = strlen(key);
    for(i=0 ; i<= keySize ; i++){
        letter = index(key[i]);
        if(aux->children[letter] != NULL){
            aux = aux->children[letter];
        }
        else{
            break;
        }
    }
    *pos = i;
    return aux;
}

bool searchWord(char *word){ /// Revisa si la subcadena es una palabra
    int x;
    tnode *where = searchTrie(word, &x);
    if(where->endWord){
        return true;
    }
    else{
        return false;
    }
}

void insertWord(char *word){ /// Inserta una palabra dentro del trie
    int pos;
    tnode *aux, *newN;
    aux = searchTrie(word, &pos);
    for( ; pos < strlen(word) ; pos++){
        newN = newNode(word[pos]);
        aux->children[index(word[pos])] = newN;
        aux = newN;
    }
    aux->endWord= true;
}

bool isEmpty(tnode* aux){ /// Revisa si un nodo del trie es una hoja
    for (int i = 0; i < 28; i++)
        if(root->children[i])
            return false;
    return true;
}

tnode *deleteWord(tnode *aux, char* word, int depth){ /// Elimina una palabra del trie
    if(depth == strlen(word)){
        if(aux->endWord){
            aux->endWord = false;
        }
        if(isEmpty(aux)){
            delete(aux);
            aux= NULL;
        }
        return aux;
    }
    int pos = index(word[depth]);
    aux->children[pos] = deleteWord(aux->children[pos], word, depth + 1);

    if(isEmpty(aux) && aux->endWord == false){
        delete(aux);
        aux = NULL;
    }
    return aux;
}

char *append (char *slice, char part) { /// Recibe una cadena y un char y agrega el char al final de la cadena
	char *aux = (char*)malloc(sizeof(char) * (strlen(slice) + 2));
	int i = 0;
	while(slice[i] != '\0'){
        aux[i] = slice[i++];
	}
	if(part<26)
        part+=65;
    else{
        part= ' ';
    }
	aux[i++] = part;
	aux[i] = '\0';
	return aux;
}

int hashf(char *key){
    int sum=0;
    int len= min(5,(int)strlen(key));
    for(int i=0 ; i<len ; i++){
        sum+= key[i] * pow(10.0, (double)i);
    }
    return (sum % SIZE);
}

void print(tnode *aux, char *slice){ /// Recibe la posicion dentro del trie y la subcadena a imprimir
	if (aux == NULL){
        return;
	}
	if (aux->endWord){
        cout<< slice << "\n";
        node *auxNode= searchNode(slice);
        cout<< auxNode->phoneNumber << "\n";
	}
	for (int i = 0; i < 28; i++){
		if(aux->children[i] != NULL){
			print(aux->children[i], append(slice, i));
		}
	}
}

void autocomplete(tnode *aux, char *prefix){ /// Recibe el prefijo que va a buscar y el nodo desde el que buscar
	int len = strlen(prefix);
    for(int i = 0; i < len; i++){
        prefix[i]= toupper(prefix[i]);
        if(isEmpty(aux)!=true)
            aux = aux->children[index(prefix[i])];
    }
    if(aux!=NULL)
        print(aux, prefix);
}



void menu(){
    cout << "\n\nMenu de Opciones\n";
    cout << "1. Contacto nuevo\n";
    cout << "2. Buscar contacto\n";
    cout << "3. Autocompletar\n";
    cout << "4. Eliminar contacto\n";
    cout << "5. Leer archivo\n";
    cout << "0. SALIR\n\n";

    cout<< "\nIngrese una opcion: ";
}

void initialize(){
    for(int i=0 ; i<SIZE ; i++){
        arr[i]= NULL;
    }
}



void insertNode(node newNode){ /// Recibe un nodo y lo inserta en la tabla hash
    node *aux = (node*)malloc(sizeof(node));
    strcpy(aux->name, newNode.name);
    strcpy(aux->phoneNumber, newNode.phoneNumber);
    int index;
    index = hashf(aux->name);
    aux->next = arr[index];
    arr[index] = aux;
}


node* searchNode(char *key){ /// Recibe una llave y la busca dentro de la tabla hash
    int index;
    node *aux;
    index = hashf(key);
    aux = arr[index];
    while(aux != NULL){
        if(strcmp(key, aux->name) == 0){
            return aux;
        }
        else{
            aux = aux->next;
        }
    }
    return NULL;
}

void deleteNode(char *key){ /// Recibe una llave, la busca y en caso de encontrarse la elimina de la tabla hash
    int index;
    node *aux, *aux2;
    index = hashf(key);
    aux = arr[index];
    while(aux!=NULL){
        if(strcmp(key, aux->name)==0){
            break;
        }
        else{
            aux2= aux;
            aux= aux->next;
        }
    }
    if(aux==NULL){
        cout<< "\nNo existe ningun contacto con el nombre " << key;
    }
    else{
        if(arr[index]==aux){
            arr[index]=aux->next;
        }
        else{
            aux2->next=aux->next;
        }
        free(aux);
        printf("Contacto eliminado");
    }
}


void initializePhoneBook(){
    ifstream file("Guia_de_telefono.txt");
    string str;
    node aux;
    if(file.is_open()){
        while (getline(file, str)) {
            ///cout<< str << "\n";
            strcpy(aux.name, str.c_str());
            getline(file, str);
            strcpy(aux.phoneNumber, str.c_str());
            insertNode(aux);
            insertWord(aux.name);
        }
    }
}
