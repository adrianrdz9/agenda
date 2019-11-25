#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 256
#define FILENAME "storage"

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
    struct elementoLista *next;
} elemento;
elemento *inicioLista;


void agregarContacto(); // Solicita y guarda datos de un contacto
void printMenu(); // Muestra las opciones del menu principal
tcontacto crearContacto(); // Solicita datos de un contacto
void guardarContacto(tcontacto contacto); // Guarda un contacto
void swap(elemento* node1, elemento* node2);
void sort(elemento* start);
void saveToFile(elemento* start);

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
    printf("1.- Agregar contacto\n");
    printf("2.- Buscar contacto\n");
    printf("3.- Eliminar contacto\n");
    printf("4.- Editar contacto\n");
    printf("5.- Mostrar todos los contactos\n");
    printf("6.- Salir\n");
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


    while(listaAux->next != NULL){
        listaAux = listaAux->next;
    }

    listaAux->contacto = contacto;
    listaAux->next = (elemento*) malloc(sizeof(elemento));
    strcpy(listaAux->next->contacto.nombre, "zzzzzzzzzzz");

    mostrarDatos(listaAux->contacto);

    printf("¿Desea agregar otro contacto?\n");
    printf("1.- Si\n2.- No");

    int opt;
    scanf("%i", &opt);

    if(opt == 1){
        agregarContacto();
    }else{
        saveToFile(inicioLista);
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

    printf("\n\n1.- Mostrar contactos de nuevo\n2.- Volver al menu principal");
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
            if ((strcmp(nodeAux->contacto.nombre, nodeAux->next->contacto.nombre) > 0))
            {
                swap(nodeAux, nodeAux->next);
                swapped = 1;
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
    printf("\n1.-Si\n2.-No");
    int opt;
    scanf("%i", &opt);
    if(opt==1){
        buscar();
    }
}

void saveToFile(elemento* start){
    FILE* file = fopen(FILENAME, "wb");
    if(file == NULL){
        printf("Error al guardar cambios en archivo");
        exit(1);
    }

    elemento* listAux = inicioLista;

    while(listAux->next != NULL){
        fwrite(&(listAux->contacto), sizeof(tcontacto), 1, file);

        mostrarDatos(listAux->contacto);

        listAux = listAux->next;
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
        fseek(file, sizeof(tcontacto)*i, SEEK_SET);
    }

    fclose(file);
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

            case 5:
                mostrarTodo();
                break;

            case 6:
                printf("Hasta luego");
                return 0;
        }

    }
}