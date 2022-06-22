#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //para cambios de color en consola

#define DEMANDA "demanda.bin"
#define RECETAS "recetas.bin"
#define STOCK "stockingredientes.bin"
#define PVENTAS "pventas.bin"
#define PRECIOS "precios.bin"
#define VENTAS "ventas.bin"

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
} StockIngrediente;

typedef struct ///Preparacion
{
    char nombre_preparacion[20];
    int cantidad; ///por unidad, no hay por peso
} Preparacion;

typedef struct ///Ingredientes Por Receta
{
    char nombre_ingrediente[20];
    float cantidad; ///puede ser en ml o en kg
} IngredienteXReceta;

typedef struct ///Recetas
{
    char nombre_preparacion[20];
    IngredienteXReceta ingredientes[20]; ///Puede tener hasta 20 ingredientes
    int cantIngredientes; ///los validos de ingredientes
} Receta;



typedef struct ///Preparacion Venta
{
    char nombre_preparacion[20];
    int cantidad; ///por unidad, no hay por peso
} PreparacionVenta;

typedef struct ///Precio Preparación
{
    char nombre_preparacion[20];
    float precio_venta; ///precio por unidad
} PrecioPreparacion;

typedef struct /// Pedido Preparación
{
    char nombre_preparacion[20];
    int cantidad;
} PedidoPreparacion;

typedef struct ///Venta
{
    PedidoPreparacion items_pedido[20]; ///puedo pedir hasta 20 items
    int cantItems;
    float valor_total; ///valor total a pagar
    int cancelado; ///si cancelado=1 , se cancelo el pedido
} Venta;



///////////////////PASO 1/////////////////////////////////////////

void obtenerStockIngredientes(StockIngrediente[],int*);
void menuResumenDelDia();
void mostrarUnIngrediente(StockIngrediente);
void mostrarStockIngredientes(StockIngrediente[],int);


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

void mostrarStockIngredientes(StockIngrediente stockIng[],int validos)
{
    printf("\nStock de Ingredientes\n");
    for(int i=0 ; i<validos ; i++)
    {
        mostrarUnIngrediente(stockIng[i]);
    }
}

void mostrarUnIngrediente(StockIngrediente ingrediente)
{
    printf("\n------------------------------------------\n");
    printf("Ingrediente: %s",ingrediente.nombre_ingrediente);
    if(strcmpi("liquido",ingrediente.tipo) == 0)
    {
        printf("\nCantidad: %.2f ml",ingrediente.cantidad);
        printf("\nTipo: %s",ingrediente.tipo);
        printf("\nCosto: $ %.2f /l",ingrediente.costo);
    }
    else
    {
        printf("\nCantidad: %.2f kg",ingrediente.cantidad);
        printf("\nTipo: %s",ingrediente.tipo);
        printf("\nCosto: $ %.2f /kg",ingrediente.costo);
    }

    printf("\n------------------------------------------\n");
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
    }
    else
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
    }
    else
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

void mostrarArregloDemandas(Preparacion arreglo[],int validos)
{

    printf("\nDemandas:");

    for(int i=0 ; i<validos ; i++)
    {
        printf("\n----------------------------------------\n");
        printf("Producto: %s",arreglo[i].nombre_preparacion);
        printf("\nCantidad: %i",arreglo[i].cantidad);
        printf("\n----------------------------------------\n");
    }




}
void mostrarArregloRecetas(Receta arreglo[],int validos)
{
    printf("\nRecetas:");

    for(int i=0 ; i<validos ; i++)
    {
        printf("\n----------------------------------------------------\n");
        printf("Nombre receta: %s\n",arreglo[i].nombre_preparacion);
        printf("\nIngredientes receta:\n");

        for(int j=0 ; j<arreglo[i].cantIngredientes ; j++)
        {
            printf("\t- %s ",arreglo[i].ingredientes[j].nombre_ingrediente);
            printf("--> Cantidad: %.2f\n",arreglo[i].ingredientes[j].cantidad);
        }
        printf("\n----------------------------------------------------\n");
    }

}
void mostrarPreparadosVentas(PreparacionVenta arreglo[],int validos)
{
    printf("\nPreparados Ventas:");

    for(int i=0 ; i<validos ; i++)
    {
        printf("\n----------------------------------------\n");
        printf("Producto: %s",arreglo[i].nombre_preparacion);
        printf("\nCantidad: %i",arreglo[i].cantidad);
        printf("\n----------------------------------------\n");
    }


}

///////////////////PASO 3/////////////////////////////////////////
void registrarPrecios(PrecioPreparacion[],int);
void cargarPrecios(PreparacionVenta [],PrecioPreparacion [],int,StockIngrediente [],int,Receta [],int);
void mostrarPrecios(PrecioPreparacion[],int);
void modificarPrecios(PrecioPreparacion[],int);
void obtenerPrecios(PrecioPreparacion[],int*);
float costoPreparacion(char[],StockIngrediente[],int,Receta[],int);
int buscarStockPorNombre(char[],StockIngrediente[],int);
int buscarPrecioPorNombre(char[],PrecioPreparacion[],int);


///Cada venta debe registrarse en el archivo “ventas.bin”
void registrarVentas();
void ingresarVentas(Venta[],int*,PrecioPreparacion[],int,PreparacionVenta[],int);
void nuevaVenta(Venta* v,PrecioPreparacion[],int,PreparacionVenta preparacionesVenta[],int validosPV);
void ingresarPedidoPreparacion(PedidoPreparacion*,PreparacionVenta[],int);
int validarNombreProducto(char[],PreparacionVenta[],int);
void obtenerVentas(Venta[],int*);

///Por cada venta, se debe descontar del stock de preparados, hay que tener en
///cuenta que puede quedarse sin stock de algún preparado.
int hayStockPreparado(char[],int,PreparacionVenta[],int);
void descontarStockPreparados(char[],int,PreparacionVenta[],int);

///El usuario se puede arrepentir de una compra, por lo tanto deberíamos poder
///eliminar una venta generada, esto implica que en el archivo de ventas se pueda
///hacer una “baja lógica”, por lo tanto deberia agregar un campo en la estructura de Venta.
void actualizarVentas(Venta[],int);
void mostrarVentas(Venta[],int);
void mostrarUnaVenta(Venta);
void mostrarPedidoPreparacion(PedidoPreparacion[],int);

/////////////////////implementaciones Paso 3//////////////////////////
void registrarPrecios(PrecioPreparacion precios[],int validosPP)
{
    FILE* fp;

    fp = fopen(PRECIOS,"wb");

    if(fp!=NULL)
    {
        fwrite(precios,sizeof(PrecioPreparacion),validosPP,fp);

        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo en modo escritura");
}
void cargarPrecios(PreparacionVenta preparacionesVentas[],PrecioPreparacion precios[],int validosPV,StockIngrediente stockIngredientes[],int validosStock,Receta recetas[],int validosRec)
{
    float costo;

    for(int i=0 ; i<validosPV; i++)
    {
        strcpy(precios[i].nombre_preparacion,preparacionesVentas[i].nombre_preparacion);
        costo = costoPreparacion(precios[i].nombre_preparacion,stockIngredientes,validosStock,recetas,validosRec);
        precios[i].precio_venta = 1.5 * costo;
    }
}
void mostrarPrecios(PrecioPreparacion precios[],int validos)
{
    printf("\nLISTA DE PRECIOS\n");
    for(int i = 0 ; i<validos ; i++)
    {
        printf("\n----------------------------------------\n");
        printf("Preparacion: %s",precios[i].nombre_preparacion);
        printf("\nPrecio: $ %.2f",precios[i].precio_venta);
        printf("\n----------------------------------------\n");
    }
}

int buscarStockPorNombre(char nombreIngrediente[],StockIngrediente stockIngredientes[],int validosStock)
{
    int i=0;
    while(i < validosStock && strcmpi(stockIngredientes[i].nombre_ingrediente,nombreIngrediente)!=0)
    {
        i++;
    }
    return i;
}
float costoPreparacion(char nombre[],StockIngrediente stockIngredientes[],int validosStock,Receta recetas[],int validosRec)
{
    int indiceReceta=0,indiceStock=0;
    float costo=0;

    indiceReceta = buscarRecetaPorNombre(nombre,recetas,validosRec);

    for(int j=0 ; j<recetas[indiceReceta].cantIngredientes ; j++)
    {
        indiceStock = buscarStockPorNombre(recetas[indiceReceta].ingredientes[j].nombre_ingrediente,stockIngredientes,validosStock);

        costo += recetas[indiceReceta].ingredientes[j].cantidad * stockIngredientes[indiceStock].costo;
    }


    return costo;
}
void modificarPrecios(PrecioPreparacion precios[],int validosPP)
{
    char op;
    char nombre[30];
    int indice=0;

    do
    {
        printf("Ingrese el nombre de la producto a modificar el precio:  ");
        fflush(stdin);
        gets(nombre);


        indice = buscarPrecioPorNombre(nombre,precios,validosPP);

        if(indice != -1)
        {
            printf("\nIngrese el nuevo precio:  ");
            scanf("%f",&(precios[indice].precio_venta));
        }
        else
            printf("No existe un producto con tal nombre");


        printf("\nDesea modificar otro precio? s/n \n");
        fflush(stdin);
        scanf("%c",&op);

    }
    while(op == 's' || op == 'S');

    registrarPrecios(precios,validosPP);
}

int buscarPrecioPorNombre(char nombre[],PrecioPreparacion precios[],int validosPP)
{
    int i=0;

    while(i<validosPP && strcmpi(nombre,precios[i].nombre_preparacion) != 0)
    {
        i++;
    }

    if(i >= validosPP)
        i=-1;

    return i;
}


void obtenerPrecios(PrecioPreparacion precios[],int* validosPP)
{
    FILE* fp;
    int i=0;

    fp = fopen(PRECIOS,"rb");

    if( fp != NULL)
    {
        while(fread(&precios[i],sizeof(PrecioPreparacion),1,fp) > 0)
        {
            i++;
        }

        *validosPP = i;

        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo en modo lectura");
}

void registrarVentas(Venta ventas[],int validosV)
{
    FILE* fp;

    fp = fopen(VENTAS,"wb");

    if(fp!=NULL)
    {

        fwrite(ventas,sizeof(Venta),validosV,fp);

        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo en modo escritura");
}
void nuevaVenta(Venta* v,PrecioPreparacion precios[],int validosPP,PreparacionVenta preparacionesVenta[],int validosPV)
{

    int i=0,indice=0;
    char op='s';
    float total=0;

    while(i<20 && (op == 's' || op == 'S'))
    {
        ingresarPedidoPreparacion(&((*v).items_pedido[i]),preparacionesVenta,validosPV);

        if((*v).items_pedido[i].cantidad != 0) ///si en cantidad tengo 0, es porque se anulo el pedido especifico de un producto,
        {                                   /// asi que no incremento i
            indice = buscarPrecioPorNombre((*v).items_pedido[i].nombre_preparacion,precios,validosPP);

            total += (*v).items_pedido[i].cantidad * precios[indice].precio_venta;

            i++;

            descontarStockPreparados((*v).items_pedido[i].nombre_preparacion,(*v).items_pedido[i].cantidad,preparacionesVenta,validosPV);
        }


        printf("\nDesea comprar otro producto? s/n \n");
        fflush(stdin);
        scanf("%c",&op);
    }

    (*v).cantItems = i;
    (*v).valor_total = total;
    (*v).cancelado=0;


}
void ingresarPedidoPreparacion(PedidoPreparacion* pedido,PreparacionVenta preparacionesVenta[],int validosPV)
{


        printf("\nIngrese el nombre del producto:  ");
        fflush(stdin);
        gets((*pedido).nombre_preparacion);

        while(!validarNombreProducto((*pedido).nombre_preparacion,preparacionesVenta,validosPV))
        {
            printf("\nNo existe el producto ingresado. Ingrese el nombre del producto:  ");
            fflush(stdin);
            gets((*pedido).nombre_preparacion);
        }

        printf("\nIngrese la cantidad:  ");
        scanf("%i",&((*pedido).cantidad));

        while(!hayStockPreparado((*pedido).nombre_preparacion,(*pedido).cantidad,preparacionesVenta,validosPV))
        {
            printf("\nIngrese una nueva cantidad o 0 para anular este pedido:  ");
            scanf("%i",&((*pedido).cantidad));
        }
}

void ingresarVentas(Venta ventas[],int* validosV,PrecioPreparacion precios[],int validosPP,PreparacionVenta preparacionesVenta[],int validosPV)
{
    int i=*validosV; ///puedo agregar ventas
    char op='s';

    printf("\n\nIngrese su pedido\n\n");

    while(i<N && (op == 's' || op == 'S' ))
    {
        nuevaVenta(&ventas[i],precios,validosPP,preparacionesVenta,validosPV);
        mostrarUnaVenta(ventas[i]);

        printf("Desea confirmar el pedido? s/n \n");
        fflush(stdin);
        scanf("%c",&op);

        if(op != 's' && op != 'S') ///si no confirma el pedido, cancelado=1
        {
            ventas[i].cancelado = 1;
        }

        i++;

        printf("Desea agregar un nuevo pedido? s/n \n");
        fflush(stdin);
        scanf("%c",&op);
    }

    *validosV = i;
}
void obtenerVentas(Venta ventas[],int* validosV)
{
    FILE* fp;
    int i=0;
    Venta aux;

    fp = fopen(VENTAS,"rb");

    if(fp!=NULL)
    {
        system("PAUSE");

        while(fread(&ventas[i],sizeof(Venta),1,fp) > 0)
        {
            if(ventas[i].cancelado == 0) ///guardo en el arreglo si no esta cancelado
            {
                i++;
            }
        }

        *validosV = i;

        printf("Validos dentro de obtener: %i",*validosV);

        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo en modo lectura");

}

int hayStockPreparado(char nombre[],int cantidad,PreparacionVenta preparacionesVenta[],int validosPV)  ///devuelve 1 si hay stock
{
    int i=0,hay=0;

    while(i<validosPV && strcmpi(nombre,preparacionesVenta[i].nombre_preparacion) != 0)
    {
        i++;
    }

    if(cantidad <= preparacionesVenta[i].cantidad)
        hay = 1;
    else
        printf("\nNo hay Stock disponible. Disponible: %i",preparacionesVenta[i].cantidad);


    return hay; ///si no hay stock devuelve 0
}

int validarNombreProducto(char nombre[],PreparacionVenta preparacionesVenta[],int validosPV)
{
    int existe=0,i=0;

    while(i<validosPV && strcmpi(nombre,preparacionesVenta[i].nombre_preparacion) != 0)
    {
        i++;
    }

    if(i<validosPV)
        existe=1;

    return existe;

}
void descontarStockPreparados(char nombre[],int cantidad,PreparacionVenta preparacionesVenta[],int validosPV)
{
    int i=0;

    while(i<validosPV && strcmpi(nombre,preparacionesVenta[i].nombre_preparacion) != 0)
    {
        i++;
    }

    preparacionesVenta[i].cantidad -= cantidad;
}

void actualizarVentas(Venta ventas[],int validosV)
{
    FILE* fp;

    fp = fopen(VENTAS,"wb");

    if(fp != NULL)
    {

        fwrite(ventas,sizeof(Venta),validosV,fp);

        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo");

}

void mostrarVentas(Venta ventas[],int validosV)
{
    for(int i=0 ; i<validosV ; i++)
    {
        mostrarUnaVenta(ventas[i]);
    }
}
void mostrarPedidoPreparacion(PedidoPreparacion pedidos[],int validos)
{
    for(int i=0 ; i<validos ; i++)
    {
        printf("\t%15s --> Cantidad: %i\n",pedidos[i].nombre_preparacion,pedidos[i].cantidad);
    }
}

void mostrarUnaVenta(Venta v)
{
        printf("\n---------------------------------------------\n");
        printf("Items pedido:\n");
        mostrarPedidoPreparacion(v.items_pedido,v.cantItems);
        printf("\nTotal: $ %.2f",v.valor_total);
        printf("\n---------------------------------------------\n");
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
    PrecioPreparacion precioPreparaciones[N];
    int validosPP=0;
    Venta ventas[N];
    int validosV=0;


    ///Paso 1
    obtenerStockIngredientes(stockIngredientes,&validosStock);
    mostrarStockIngredientes(stockIngredientes,validosStock);

    //Inicio del paso 2
    obtenerDemanda(demandas,&validosDem);
    obtenerRecetas(recetas,&validosRec);
    prepararDemandas(demandas,validosDem,recetas,validosRec,stockIngredientes,validosStock,preparacionesVenta,&validosPV);

    registrarPreparadosVenta(preparacionesVenta,validosPV);

    mostrarArregloDemandas(demandas,validosDem);
    mostrarArregloRecetas(recetas,validosRec);
    mostrarPreparadosVentas(preparacionesVenta,validosPV);

    printf("\nStock restante:\n");
    mostrarStockIngredientes(stockIngredientes,validosStock);

    //Fin del paso 2

    ///Paso 3


    //cargarPrecios(preparacionesVenta,precioPreparaciones,validosPV,stockIngredientes,validosStock,recetas,validosRec);
    // validosPP = validosPV;
    //registrarPrecios(precioPreparaciones,validosPP);
    obtenerPrecios(precioPreparaciones,&validosPP);
    mostrarPrecios(precioPreparaciones,validosPP);

    //modificarPrecios(precioPreparaciones,validosPP);

    //  mostrarPrecios(precioPreparaciones,validosPP);


    ///ingreso las ventas en un arreglo
    ingresarVentas(ventas,&validosV,precioPreparaciones,validosPP,preparacionesVenta,validosPV);
    //mostrarVentas(ventas,validosV);

    ///guardo en un archivo
    registrarVentas(ventas,validosV);

    ///en caso de agregar una venta, sobreescribo el archivo con la nueva venta
    //ingresarVentas
    //registrarVentas

    ///actualizo el archivo en caso de que haya pedidos cancelados
    obtenerVentas(ventas,&validosV);
    actualizarVentas(ventas,validosV);

    mostrarVentas(ventas,validosV);


    return 0;
}


