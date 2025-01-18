#include <stdio.h>
#include <stdlib.h>

#define FILCOLS 10

struct acceso_rapido
{
    long nro_fila;
    long nro_col;
    float datos_fila [FILCOLS];
    float datos_col [FILCOLS];
};

float datos_sparse [FILCOLS*FILCOLS] = {0.0F};

float dato_accedido (long f,long c);
// Devuelve anterior valor.
float almacenar_dato (long f,long c,float valor);

struct acceso_rapido acceso_1;

// CARGA EL acc CON LA FILA f Y LA COLUMNA c
void limpiar_acceso (struct acceso_rapido *acceso);
void cargar_acceso_rapido (struct acceso_rapido *acc,long f,long c);
void cargar_acceso_rapido_fila (struct acceso_rapido *acc,long f);

int main()
{
    float anterior;
    struct acceso_rapido acceder1;

    almacenar_dato(4,2,10.0);
    printf ("%f\n",dato_accedido(4,2));
    anterior = almacenar_dato(4,2,40.0);
    printf ("Anterior : %f . Actual : %f\n",anterior,dato_accedido(4,2));
    printf ("///////////////////////////////////////////\n");

    // Uso de taccessos
    limpiar_acceso(&acceder1);
    cargar_acceso_rapido_fila(&acceder1,4);
    printf ("%ld : %ld\n",acceder1.nro_fila,acceder1.nro_col);
    for (int i=0;i<FILCOLS;i++)
    {
        printf ("DATO FILA %d ? : %f\n",i,acceder1.datos_fila[i]);
        printf ("DATO COLS %d ? : %f\n",i,acceder1.datos_col[i]);
    }
}

float dato_accedido (long f,long c)
{
   return datos_sparse [(f-1)*FILCOLS+(c-1)];
}

float almacenar_dato (long f,long c,float valor)
{
    float anterior;
    anterior = datos_sparse[(f-1)*FILCOLS+(c-1)];
    datos_sparse[(f-1)*FILCOLS+(c-1)] = valor;
    return anterior;
}

// CARGA EL acc CON LA FILA f Y LA COLUMNA c
void cargar_acceso_rapido (struct acceso_rapido *acc,long f,long c)
{
    float aux;
    long idx;
    acc->nro_fila = f;
    acc->nro_col = c;

    // Primero cargamos la fila
    idx = (f-1) * FILCOLS;
    for (int i=0;i<FILCOLS;i++)
    {
        acc->datos_fila[i]=datos_sparse[idx+i];
    }

    for (int i=0;i<FILCOLS;i++)
    {
        aux = dato_accedido(i,c);
        acc->datos_col[i]=aux;
    }

}

void cargar_acceso_rapido_fila (struct acceso_rapido *acc,long f)
{
    long idx;
    acc->nro_fila = f;

    // Primero cargamos la fila
    idx = (f-1) * FILCOLS;
    for (int i=0;i<FILCOLS;i++)
    {
        acc->datos_fila[i]=datos_sparse[idx+i];
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
