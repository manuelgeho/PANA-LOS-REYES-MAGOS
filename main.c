#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //para cambios de color en consola

#define DEMANDA "demanda.bin"
#define RECETAS "recetas.bin"
#define STOCK "stockingredientes.bin"
#define PVENTAS "pventas.bin"
#define N 100

/**
Puntos de acuerdo:
Estructuras PamelCase
Variables y etc camelCase
despersistir = obtener
persistir = registrar
**/


///ESTRUCTURAS
typedef struct ///StockIngredientes
{
  char nombre_ingrediente[20];
  float cantidad;  //en kg o en ml segun el tipo de ingrediente
  char tipo[20]; /// "liquido" "solido"
  float costo; ///costo por kg o por litro según corresponda
}StockIngrediente;

typedef struct ///Preparacion
 {
   char nombre_preparacion[20];
   int cantidad; ///por unidad, no hay por peso
 }Preparacion;

 typedef struct ///Ingredientes Por Receta
 {
    char nombre_ingrediente[20];
    float cantidad; ///puede ser en ml o en kg
 }IngredienteXReceta;

typedef struct ///Recetas
 {
   char nombre_preparacion[20];
   IngredienteXReceta ingredientes[20]; ///Puede tener hasta 20 ingredientes
   int cantIngredientes; ///los validos de ingredientes
 }Receta;



 typedef struct ///Preparacion Venta
 {
   char nombre_preparacion[20];
   int cantidad; ///por unidad, no hay por peso
 }PreparacionVenta;

 typedef struct ///Precio Preparación
 {
   char nombre_preparacion[20];
   float precio_venta; ///precio por unidad
 }PrecioPreparacion;

 typedef struct /// Pedido Preparación
{
  char nombre_preparacion[20];
  int cantidad;
}PedidoPreparacion;

 typedef struct ///Venta
{
  PedidoPreparacion items_pedido[20]; ///puedo pedir hasta 20 items
  int cantItems;
  float valor_total; ///valor total a pagar
}Venta;

void obtenerStockIngredientes(StockIngrediente[],int*);
void menuResumenDelDia();

void obtenerStockIngredientes(StockIngrediente stockIng[],int* validosIng)
{
    FILE* fp;
    StockIngrediente aux;
    int i=0; ///cargo el arreglo desde cero

    fp = fopen(STOCK,"rb");

    if( fp != NULL)
    {
        while( fread(&aux,sizeof(StockIngrediente),1,fp) > 0 )
        {
            stockIng[i] = aux;
            i++;
        }

        *validosIng = i;
    }
    else
        printf("No se pudo abrir el archivo StockIngredientes");
}

void menuResumenDelDia()
{
    printf("\nRESUMEN DEL DIA\n");
    printf("\n1. Ver lista de remanentes de ingredientes en stock");
    printf("\n2. Ver lista de preparados y su cantidad que quedan en venta");
    printf("\n3. Ver el ingreso total del dia");
    printf("\n4. Ver la ganancia del dia");


}

///////////////////PASO 2/////////////////////////////////////////
//Prototipados Paso 2
void obtenerRecetas(Receta[],int* );
void obtenerDemanda(Preparacion[],int*);
void obtenerStockIngredientes(StockIngrediente[],int*);
int validarIngredientes(Receta,StockIngrediente [],int);
void descontarIngredientesPorReceta(IngredienteXReceta[],int,StockIngrediente[],int);
void descontarUnIngrediente(IngredienteXReceta,StockIngrediente[],int);
int buscarRecetaPorNombre(char[],Receta[],int);
void prepararDemandas(Preparacion[],int,Receta[],int,StockIngrediente[],int,PreparacionVenta[],int*);
void registrarPreparadosVenta(PreparacionVenta[],int);
void mostrarArregloDemandas(Preparacion[],int);
void mostrarArregloRecetas(Receta[],int);
void mostrarPreparadosVentas(PreparacionVenta[],int);

//Implementaciones Paso 2

void obtenerRecetas(Receta r[],int* rValidos)
{
    FILE* fp;
    int i=0;
    fp = fopen(RECETAS,"rb");
    if(fp != NULL)
    {
        while(fread(&r[i],sizeof(Receta),1,fp) >0)
        {
            i++;
        }
        *rValidos = i;
        fclose(fp);
    }else
        printf("No se pudo abrir el archivo recetas.bin");
}
void obtenerDemanda(Preparacion d[],int* dValidos)
{
    FILE* fp;
    int i=0;
    fp = fopen(DEMANDA,"rb");
    if(fp != NULL)
    {
        while(fread(&d[i],sizeof(Preparacion),1,fp) >0)
        {
            i++;
        }
        *dValidos = i;
        fclose(fp);
    }else
        printf("No se pudo abrir el archivo demanda.bin");
}
void obtenerPreparadosVentas(PreparacionVenta pv[],int* val)
{
    FILE* fp;
    fp = fopen(PVENTAS,"rb");
    int i=0;
    while(i<*val && fread(&pv[i],sizeof(PreparacionVenta),1,fp) >0)
    {
        i++;
    }

    *val = i;

    fclose(fp);
}

//Retorna un FALSO(0) si el stock no es suficiente para cubrir los ingredientes necesarios, en caso contrario devuelve un VERDADERO(1)
int validarIngredientes(Receta receta,StockIngrediente stock[],int stockValidos)
{
    for(int ingredienteReceta=0; ingredienteReceta<receta.cantIngredientes; ingredienteReceta++)
    {
        for(int ingredienteStock=0; ingredienteStock<stockValidos; ingredienteStock++)
        {
            if(strcmpi(receta.ingredientes[ingredienteReceta].nombre_ingrediente,stock[ingredienteStock].nombre_ingrediente)==0 && receta.ingredientes[ingredienteReceta].cantidad > stock[ingredienteStock].cantidad)
            {
                return 0;
            }
        }
    }
    return 1;
}

//Descuenta del stock, todos los ingredientes que se le pasen en un arreglo de ingredientes.
void descontarIngredientesPorReceta(IngredienteXReceta ingredientesDeReceta[],int iValidos,StockIngrediente stock[],int sValidos)
{
    for(int ingrediente=0; ingrediente < iValidos; ingrediente++)
    {
        descontarUnIngrediente(ingredientesDeReceta[ingrediente],stock,sValidos);
    }
}

//Descuenta un ingrediente del stock
void descontarUnIngrediente(IngredienteXReceta ingrediente,StockIngrediente stock[],int stockValidos)
{
    for(int ingStock=0; ingStock < stockValidos; ingStock++)
    {
        if(strcmpi(ingrediente.nombre_ingrediente,stock[ingStock].nombre_ingrediente)==0)
        {
            stock[ingStock].cantidad -= ingrediente.cantidad;
        }
    }
}

//Retorna el indice donde se encuentra la receta que tiene el mismo nombre que la demanda
int buscarRecetaPorNombre(char nombreReceta[],Receta recetas[],int rVal)
{
    int indice=0;
    while(indice < rVal && strcmpi(recetas[indice].nombre_preparacion,nombreReceta)!=0)
    {
        indice++;
    }
    return indice;
}
//Prepara las demandas
void prepararDemandas(Preparacion demandas[],int dValidos,Receta recetas[],int rValidos,StockIngrediente stock[],int sValidos,PreparacionVenta pVentas[],int* pvValidos)
{
    int indicePV=0;
    int n=0;
    int indiceReceta=0;
    for(int i=0; i<dValidos; i++)
    {
        indiceReceta= buscarRecetaPorNombre(demandas[i].nombre_preparacion,recetas,rValidos);
            strcpy(pVentas[indicePV].nombre_preparacion,demandas[i].nombre_preparacion);
            while(validarIngredientes(recetas[indiceReceta],stock,sValidos) && n < demandas[i].cantidad)
            {
                descontarIngredientesPorReceta(recetas[indiceReceta].ingredientes,recetas[indiceReceta].cantIngredientes,stock,sValidos);

                pVentas[indicePV].cantidad++;
                n++;
            }
            n=0;
        indicePV++;
    }
    *pvValidos = indicePV;
}
//Registra las preparaciones listas para la venta en el archivo "preparadosventa.bin"
void registrarPreparadosVenta(PreparacionVenta pv[],int validosPV)
{
    FILE* fp;
    fp = fopen(PVENTAS,"wb");

    if(fp!=NULL)
    {
        for(int i=0; i<validosPV; i++)
        {
            fwrite(&pv[i],sizeof(PreparacionVenta),1,fp);
        }
        fclose(fp);
    }
}
int main()
{
    Preparacion demandas[N];
    int validosDem=0;
    Receta recetas[N];
    int validosRec=0;
    StockIngrediente stockIngredientes[N];
    int validosStock=0;
    PreparacionVenta preparacionesVenta[N];
    int validosPV=0;

    obtenerStockIngredientes(stockIngredientes,&validosStock);

    //Inicio del paso 2
    obtenerDemanda(demandas,&validosDem);
    obtenerRecetas(recetas,&validosRec);
    prepararDemandas(demandas,validosDem,recetas,validosRec,stockIngredientes,validosStock,preparacionesVenta,&validosPV);

    registrarPreparadosVenta(preparacionesVenta,validosPV);
    //Fin del paso 2

    return 0;
}


