#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 256
#define FILENAME "storage"

int listSize = 1;

typedef struct {
    char nombre[STR_LEN];
    char apellido[STR_LEN];
    char email[STR_LEN];
    char telefono[STR_LEN];
    char celular[STR_LEN];
    char direccion[STR_LEN*4];
} tcontacto;

typedef struct elementoLista {
    tcontacto contacto;
    struct elementoLista* next;
} elemento;
elemento *inicioLista;


void agregarContacto(); // Solicita y guarda datos de un contacto
void printMenu(); // Muestra las opciones del menu principal
tcontacto crearContacto(); // Solicita datos de un contacto
void guardarContacto(tcontacto contacto); // Guarda un contacto
void swap(elemento* node1, elemento* node2);
void sort(elemento* start);
void saveToFile();

int validateList(){
    if(inicioLista == NULL){
        printf("Aún no hay contactos, ¿desea agregar uno ahora?");
        printf("\n1.- Si\n2.- No");

        int opt;
        scanf("%i", &opt);

        if(opt == 1){
            agregarContacto();
        }

        return 0;
    }
    return 1;
}

void printMenu(){
    for(int i = 0; i < 30; i++)
        printf("\n");
    printf("%i contactos guardados\n", listSize-1);
    printf("1.- Agregar contacto\n");
    printf("2.- Buscar contacto\n");
    printf("3.- Eliminar contacto\n");
    printf("4.- Mostrar todos los contactos\n");
    printf("5.- Salir\n");
    printf("Ingrese la opcion deseada: ");
}

tcontacto crearContacto(){
    tcontacto auxContacto;

    printf("Nombre: ");
    scanf("%s", &(auxContacto.nombre));
    printf("Apellido: ");
    scanf("%s", &(auxContacto.apellido));
    printf("Celular: ");
    scanf("%s", &(auxContacto.celular));
    printf("Telefono de casa: ");
    scanf("%s", &(auxContacto.telefono));
    printf("Email: ");
    scanf("%s", &(auxContacto.email));
    printf("Direccion: ");
    scanf("%s", &(auxContacto.direccion));

    return auxContacto;
}

void mostrarDatos(tcontacto contacto){
    printf("\n\n");
    printf("%s %s", contacto.nombre, contacto.apellido);
    printf("\nEmail: %s", contacto.email);
    printf("\nTelefono\n\t|_ Casa: %s\n\t|_ Celular: %s", contacto.telefono, contacto.celular);
    printf("\nDireccion: %s", contacto.direccion);
    printf("\n\n");
}

void guardarContacto(tcontacto contacto){

    if(inicioLista == NULL){
        inicioLista = (elemento*) malloc(sizeof(elemento));
    }

    elemento *listaAux = inicioLista;


    while(listaAux != NULL && listaAux->next != NULL){
        listaAux = listaAux->next;
    }

    if(listaAux == NULL){
        listaAux = (elemento*)malloc(sizeof(elemento));
    }

    listaAux->contacto = contacto;
    listaAux->next = (elemento*) malloc(sizeof(elemento));
    strcpy(listaAux->next->contacto.nombre, "zzzzzzzzzzz");

    mostrarDatos(listaAux->contacto);
    listSize++;

    printf("¿Desea agregar otro contacto?\n");
    printf("1.- Si\n2.- No");

    int opt;
    scanf("%i", &opt);

    if(opt == 1){
        agregarContacto();
    }else{
        saveToFile();
    }


}

void agregarContacto(){
    printf("Agregar contacto\n\n");

    tcontacto contacto;
    contacto = crearContacto();
    guardarContacto(contacto);

    sort(inicioLista);
}

void mostrarTodo(){
    validateList();

    elemento* listaAux = inicioLista;

    int i = 1;

    while (listaAux->next != NULL){
        printf("#%i\n", i);
        mostrarDatos(listaAux->contacto);
        listaAux = listaAux->next;
        i++;
    }

    printf("\n\n1.- Mostrar contactos de nuevo\n2.- Volver al menu principal\n");
    int a;
    scanf("%i", &a);
    if(a == 1){
        mostrarTodo();
    }
};

elemento* buscarContacto(char query[STR_LEN]){
    elemento* listaAux = inicioLista;
    elemento* coincidencias = (elemento*) malloc(sizeof(elemento));
    elemento* coincidenciasAux = coincidencias;
    while(listaAux->next != NULL){
        if(strcmp(listaAux->contacto.nombre, query) == 0){
            coincidenciasAux->contacto = listaAux->contacto;
            coincidenciasAux->next = (elemento*) malloc(sizeof(elemento));
            coincidenciasAux = coincidenciasAux->next;
        }

        listaAux = listaAux->next;
    }

    return coincidencias;
}

void sort(elemento* start){
    int swapped, i;
    elemento* nodeAux;
    elemento* leftNodeAux = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do
    {
        swapped = 0;
        nodeAux = start;

        while (nodeAux->next != leftNodeAux)
        {
            if (nodeAux->next != NULL)
            {
                if(strcmp(nodeAux->contacto.nombre, "zzzzzzzzzzz") == 0) return;

                if(strcmp(nodeAux->contacto.nombre, nodeAux->next->contacto.nombre) > 0){
                    swap(nodeAux, nodeAux->next);
                    swapped = 1;
                }
            }
            nodeAux = nodeAux->next;
        }
        leftNodeAux = nodeAux;
    }
    while (swapped);
}

void swap(elemento* nodo1, elemento* nodo2){
    tcontacto temp;
    temp = nodo1->contacto;
    nodo1->contacto = nodo2->contacto;
    nodo2->contacto = temp;
}

void buscar(){
    if(validateList() == 0){
        return;
    }

    printf("Nombre:");
    char query[STR_LEN];

    scanf("%s", &query);

    elemento* coincidencias = buscarContacto(query);

    while(coincidencias->next != NULL){
        mostrarDatos(coincidencias->contacto);
        coincidencias = coincidencias->next;
    }

    printf("¿Deseas hacer otra busqueda?");
    printf("\n1.-Si\n2.-No\n");
    int opt;
    scanf("%i", &opt);
    if(opt==1){
        buscar();
    }
}

void saveToFile(){
    FILE* file = fopen(FILENAME, "wb");
    if(file == NULL){
        printf("Error al guardar cambios en archivo");
        exit(1);
    }

    elemento* listAux = inicioLista;
    int i = 0;
    while(i < listSize && listAux->next != NULL){
        fwrite(&(listAux->contacto), sizeof(tcontacto), 1, file);

        listAux = listAux->next;
        i++;
    }

    fclose(file);
    printf("Todos los cambios guardados");
}

void syncWithFile(){
    FILE* file = fopen(FILENAME, "rb");
    if(file == NULL){
        file = fopen(FILENAME, "wb");
        if(file == NULL){
            printf("No se puede crear el archivo");
            exit(1);
        }else{
            return;
        }
    }

    tcontacto auxNode;
    elemento* listAux = inicioLista = (elemento*)malloc(sizeof(elemento));

    fseek(file, 0L, SEEK_END);
    int sz = ftell(file);

    fseek(file, 0L, SEEK_SET);

    int i = 0;

    while(i < (sz/sizeof(tcontacto))){
        fread(&auxNode, sizeof(tcontacto), 1, file);
        mostrarDatos(auxNode);

        listAux->contacto = auxNode;
        listAux->next = (elemento*)malloc(sizeof(elemento));
        listAux = listAux->next;

        i++;
        listSize++;
        fseek(file, sizeof(tcontacto)*i, SEEK_SET);
    }

    fclose(file);
}

int isEq(tcontacto contact1, tcontacto contact2){
    if(
        strcmp(contact1.nombre, contact2.nombre) == 0 &&
        strcmp(contact1.apellido, contact2.apellido) == 0 &&
        strcmp(contact1.direccion, contact2.direccion) == 0 &&
        strcmp(contact1.email, contact2.email) == 0 &&
        strcmp(contact1.telefono, contact2.telefono) == 0 &&
        strcmp(contact1.celular, contact2.celular) == 0
    ){
        return 1;
    }else{
        return 0;
    }
}

void delete(){
    if(validateList() == 0){
        return;
    }

    printf("Buscar contacto a eliminar\n");

    printf("Nombre:");
    char query[STR_LEN];

    scanf("%s", &query);

    elemento* coincidencias = buscarContacto(query);
    elemento* coincidenciasAux = coincidencias;

    int i = 0;

    while(coincidenciasAux->next != NULL){
        printf("Contacto %i", i+1);
        mostrarDatos(coincidenciasAux->contacto);
        coincidenciasAux = coincidenciasAux->next;
        i++;
    }

    int opt = -1;
    while(opt < 0 || opt > (i)){
        printf("Introduce el número de contacto que desee eliminar (0-%i), introduce 0 para cancelar: ", i);
        scanf("%i", &opt);
    }

    if(opt == 0) return;

    i = 1;
    coincidenciasAux = coincidencias;
    tcontacto toDelete;
    while(coincidenciasAux->next != NULL){
        if(i == opt){
            toDelete = coincidenciasAux->contacto;
            break;
        }
        coincidenciasAux = coincidenciasAux->next;
        i++;
    }

    elemento* temp = inicioLista, *prev;

    if (temp != NULL && (isEq(temp->contacto, toDelete)==1))
    {
        inicioLista  = temp->next;
        temp = (elemento*)malloc(sizeof(elemento));

        listSize--;
        saveToFile();
        return;
    }

    while (temp != NULL && (isEq(temp->contacto, toDelete)==0))
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;

    temp = (elemento*)malloc(sizeof(elemento));

    listSize--;
    saveToFile();
}

int main(){
    syncWithFile();

    while(1){

        printMenu();

        int opt;
        opt = getchar()-'0';

        switch (opt){
            case 1:
                agregarContacto();
                break;

            case 2:
                buscar();
                break;

            case 3:
                delete();
                break;

            case 4:
                mostrarTodo();
                break;

            case 5:
                printf("Hasta luego");
                return 0;
        }

    }
}
