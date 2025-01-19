#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILCOLS 10
#define NROACCESOS_RAPIDOS 10

#define TAM_BUFFER 200

struct acceso_rapido
{
    long nro_fila;
    long nro_col;
    float datos_fila [FILCOLS];
    float datos_col [FILCOLS];
};

//
struct acceso_rapido_grupo
{
    struct acceso_rapido accesos[NROACCESOS_RAPIDOS];
    int con_columna [NROACCESOS_RAPIDOS];
    // AL GUARDAR EL ACCESO RAPIDO SE LE DA UN VALOR
    // DE ACCESOS_INICIALES_PARA_ACCESO_RAPIDO  3
    int nro_accesos[NROACCESOS_RAPIDOS];
} tgestoraccesos;

float datos_sparse [FILCOLS*FILCOLS] = {0.0F};
int cargar_datos_sparse (char *nombre_fichero, float datos[]);

// USO DE LA MATRIZ
float dato_accedido (long f,long c,float sparse[]);
// Devuelve anterior valor.
float almacenar_dato (long f,long c,float valor,float sparse[]);

// ACCESOS RAPIDOS.
// CARGA EL acc CON LA FILA f Y LA COLUMNA c
void limpiar_acceso (struct acceso_rapido *acceso);
void cargar_acceso_rapido (struct acceso_rapido *acc,long f,long c,float sparse[]);
void cargar_acceso_rapido_fila (struct acceso_rapido *acc,long f,float sparse[]);

// ACCESOS RAPIDOS GUARDADOS.
//void limpiar_acceso_mem (struct acceso_rapido *acceso);
//void cargar_acceso_rapido_mem (struct acceso_rapido *acc,long f,long c);
//void cargar_acceso_rapido_fila (struct acceso_rapido *acc,long f);

struct acceso_rapido acceso_1;

int main(int argc,char *argv[])
{

    // PARTE SIN FICHEROS.

    float anterior;
    struct acceso_rapido acceder1;

    // VARIABLES PARA FICHEROS
    char *n_fichero = {"\0"};

    // CODIGO SIN FICHEROS

    almacenar_dato(4,2,10.0,datos_sparse);
    printf ("%f\n",dato_accedido(4,2,datos_sparse));
    anterior = almacenar_dato(4,2,40.0,datos_sparse);
    printf ("Anterior : %f . Actual : %f\n",anterior,dato_accedido(4,2,datos_sparse));
    printf ("///////////////////////////////////////////\n");

    // Uso de taccessos
    limpiar_acceso(&acceder1);
    cargar_acceso_rapido_fila(&acceder1,4,datos_sparse);
    for (int i=0;i<FILCOLS;i++)
    {
        printf ("DATO FILA %d ? : %f\n",i,acceder1.datos_fila[i]);
        printf ("DATO COLS %d ? : %f\n",i,acceder1.datos_col[i]);
    }

    // PARTE CON FICHEROS.
    if (argc==2)
    {
        n_fichero = argv[1];
        printf ("%s cargado.\n",n_fichero);
        cargar_datos_sparse(n_fichero, datos_sparse);
        // Ver una lectura
        printf ("ACCESO : 2.2 = %f\n",dato_accedido(2,2,datos_sparse));
    }
}

int cargar_datos_sparse (char *nombre_fichero, float datos[])
{
    char buffer[TAM_BUFFER];
    char *ptr_tok;
    FILE *fichero;
    long nro_datos = 0;
    long f_aux = 1;
    long c_aux = 1;

    fichero = fopen (nombre_fichero,"r");
    if (fichero==NULL) {
        fputs ("Error al abrir el fichero.\n",stderr);
        exit (1);
    }

    while (fgets(buffer,TAM_BUFFER,fichero) != NULL)
    {
        if ((ptr_tok = strtok (buffer,"; \n")) != NULL)
        {
            almacenar_dato(f_aux,c_aux,atof(ptr_tok),datos);
            c_aux++;
            nro_datos++;
            while (ptr_tok != NULL)
            {
                ptr_tok = strtok (NULL, "; \n");
                if (ptr_tok != NULL)
                {
                    almacenar_dato(f_aux,c_aux,atof(ptr_tok),datos);
                    c_aux++;
                    nro_datos++;
                }
            }
        }
        c_aux = 1;
        f_aux++;
    }
    printf ("OK transferidos. %ld datos.\n",nro_datos);
    return 0;
}

// FUNCIONES SIMPLES PARA ACCEDER Y MODIFICAR LOS DATOS.
float dato_accedido (long f,long c,float sparse[])
{
   return sparse [(f-1)*FILCOLS+(c-1)];
}

float almacenar_dato (long f,long c,float valor,float sparse[])
{
    float anterior;
    anterior = sparse[(f-1)*FILCOLS+(c-1)];
    sparse[(f-1)*FILCOLS+(c-1)] = valor;
    return anterior;
}

// CARGA EL acc CON LA FILA f Y LA COLUMNA c
void cargar_acceso_rapido (struct acceso_rapido *acc,long f,long c,float sparse[])
{
    float aux;
    long idx;
    acc->nro_fila = f;
    acc->nro_col = c;

    // Primero cargamos la fila
    idx = (f-1) * FILCOLS;
    for (int i=0;i<FILCOLS;i++)
    {
        acc->datos_fila[i]=sparse[idx+i];
    }

    for (int i=0;i<FILCOLS;i++)
    {
        aux = dato_accedido(i,c,sparse);
        acc->datos_col[i]=aux;
    }

}

void cargar_acceso_rapido_fila (struct acceso_rapido *acc,long f,float sparse[])
{
    long idx;
    acc->nro_fila = f;

    // Primero cargamos la fila
    idx = (f-1) * FILCOLS;
    for (int i=0;i<FILCOLS;i++)
    {
        acc->datos_fila[i]=sparse[idx+i];
    }
}

void limpiar_acceso (struct acceso_rapido *acceso)
{
    for (int i=0;i<FILCOLS;i++)
    {
        acceso->datos_col[i] = 0.0F;
        acceso->datos_fila[i] = 0.0F;
    }
}
