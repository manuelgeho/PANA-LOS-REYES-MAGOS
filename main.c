#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#define DEMANDA "demanda.bin"
#define RECETAS "recetas.bin"
#define STOCK "stockingredientes.bin"
#define PVENTAS "pventas.bin"
#define PRECIOS "precios.bin"
#define VENTAS "ventas.bin"
#define COSTOS "costos.bin"

#define N 100
#define UP 72
#define DOWN 80
#define ENTER 13
#define ESC 27


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
    int idVenta;
    PedidoPreparacion items_pedido[20]; ///puedo pedir hasta 20 items
    int cantItems;
    float valor_total; ///valor total a pagar
    int cancelado; ///si cancelado=1 , se cancelo el pedido
} Venta;

///////////////////PASO 1/////////////////////////////////////////
////Prototipados Paso 1
void obtenerStockIngredientes(StockIngrediente[],int*);
void mostrarUnIngrediente(StockIngrediente);
void mostrarStockIngredientes(StockIngrediente[],int);

/////////////////////PASO 2/////////////////////////////////////////
////Prototipados Paso 2
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
void mostrarUnaReceta(Receta);
void mostrarPreparadosVentas(PreparacionVenta[],int);

///////////////////PASO 3/////////////////////////////////////////
void registrarPrecios(PrecioPreparacion[],int);
void cargarPrecios(PreparacionVenta [],PrecioPreparacion [],int,StockIngrediente [],int,Receta [],int);
void mostrarPrecios(PrecioPreparacion[],int);
void modificarPrecios(PrecioPreparacion[],int);
void obtenerPrecios(PrecioPreparacion[],int*);
float costoPreparacion(char[],StockIngrediente[],int,Receta[],int);
int buscarStockPorNombre(char[],StockIngrediente[],int);
int buscarPrecioPorNombre(char[],PrecioPreparacion[],int);
void registrarVentas(PrecioPreparacion[],int,PreparacionVenta[],int);
void nuevaVenta(Venta* v,PrecioPreparacion[],int,PreparacionVenta preparacionesVenta[],int validosPV);
void ingresarPedidoPreparacion(PedidoPreparacion*,PreparacionVenta[],int);
int validarNombreProducto(char[],PreparacionVenta[],int);
void obtenerVentas(Venta[],int*);
void mostrarVentas();
void mostrarUnaVenta(Venta);
void mostrarPedidoPreparacion(PedidoPreparacion[],int);
int hayStockPreparado(char[],int,PreparacionVenta[],int);
void descontarStockPreparados(char[],int,PreparacionVenta[],int);
void modificarVentas();
int cantVentas();
void registrarStockRestante(StockIngrediente[],int);
int validarNombreReceta(char[],Receta[],int);
void centrarTexto(char[],int);

/////////////////// PASO 4 /////////////////////////////////////////
//prototipado menues
void gotoxy (int, int); /// func de ubicacion de cursor
void menuGeneral(Preparacion[],int*,Receta[],int*,StockIngrediente[],int*,Preparacion[],int*,PrecioPreparacion[],int*);
void menuResumenDelDia(StockIngrediente[],int,PrecioPreparacion[],int,PreparacionVenta[],int,Receta[],int,Venta[],int*);
void menuStockIngredientes(StockIngrediente[],int*);
void menuRecetas(Receta recetas[],int*);
void menuVentas(Preparacion[],int*,Receta[],int validosRec,StockIngrediente[],int,PreparacionVenta[],int*,PrecioPreparacion[],int*);
float ingresoTotalDia(Venta[],int);
float costoTotal(StockIngrediente[],int);

//Implementaciones PASO 1
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
    else if(strcmpi(ingrediente.nombre_ingrediente,"huevo") == 0)
    {
        printf("\nCantidad: %.2f u",ingrediente.cantidad);
        printf("\nTipo: %s",ingrediente.tipo);
        printf("\nCosto: $ %.2f /u",ingrediente.costo);
    }
    else
    {
        printf("\nCantidad: %.2f kg",ingrediente.cantidad);
        printf("\nTipo: %s",ingrediente.tipo);
        printf("\nCosto: $ %.2f /kg",ingrediente.costo);
    }

    printf("\n------------------------------------------\n");
}

////Implementaciones Paso 2

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
////Retorna el indice donde se encuentra la receta que tiene el mismo nombre que la demanda
int buscarRecetaPorNombre(char nombreReceta[],Receta recetas[],int rVal)
{
    int indice=0;
    while(indice < rVal && strcmpi(recetas[indice].nombre_preparacion,nombreReceta)!=0)
    {
        indice++;
    }
    if(indice>=rVal)
    {
        indice=-1;
    }

    return indice;
}

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
        mostrarUnaReceta(arreglo[i]);
    }

}
void mostrarUnaReceta(Receta r)
{
    printf("\n----------------------------------------------------\n");
    printf("Nombre receta: %s\n",r.nombre_preparacion);
    printf("\nIngredientes receta:\n");

    for(int j=0 ; j<r.cantIngredientes ; j++)
    {
        printf("\t- %s ",r.ingredientes[j].nombre_ingrediente);
        printf("--> Cantidad: %.2f\n",r.ingredientes[j].cantidad);
    }
    printf("\n----------------------------------------------------\n");
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
////implementaciones Paso 3//////////////////////////
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

void registrarVentas(PrecioPreparacion precios[],int validosPP,PreparacionVenta preparacionesVenta[],int validosPV)
{
    FILE* fp;
    Venta v;
    int i=0;
    char op;

    fp = fopen(VENTAS,"ab");  ///agrego cada venta al final

    i = cantVentas() + 1; ///para saber que valor va a tener el id del pedido


    if(fp!=NULL)
    {

        nuevaVenta(&v,precios,validosPP,preparacionesVenta,validosPV);

        v.idVenta = i;

        mostrarUnaVenta(v);

        printf("Desea confirmar el pedido? s/n \n");
        fflush(stdin);
        scanf("%c",&op);

        if(op != 's' && op != 'S') ///si no confirma el pedido, cancelado=1
        {
            v.cancelado = 1;
        }

        fwrite(&v,sizeof(Venta),1,fp);

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

    printf("\nIngrese el pedido:\n");

    while(i<20 && (op == 's' || op == 'S'))
    {
        ingresarPedidoPreparacion(&((*v).items_pedido[i]),preparacionesVenta,validosPV);

        if((*v).items_pedido[i].cantidad != 0) ///si en cantidad tengo 0, es porque se anulo el pedido especifico de un producto,
        {
            /// asi que no incremento i
            indice = buscarPrecioPorNombre((*v).items_pedido[i].nombre_preparacion,precios,validosPP);

            total += (*v).items_pedido[i].cantidad * precios[indice].precio_venta;

            descontarStockPreparados((*v).items_pedido[i].nombre_preparacion,(*v).items_pedido[i].cantidad,preparacionesVenta,validosPV);

            i++;
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

    while((*pedido).cantidad < 0)
    {
        printf("\nIngrese una cantidad valida:  ");
        scanf("%i",&((*pedido).cantidad));
    }

    while(!hayStockPreparado((*pedido).nombre_preparacion,(*pedido).cantidad,preparacionesVenta,validosPV))
    {
        printf("\nIngrese una nueva cantidad o 0 para anular este pedido:  ");
        scanf("%i",&((*pedido).cantidad));
    }
}


void obtenerVentas(Venta ventas[],int* validosV)
{
    FILE* fp;
    int i=0;
    fp = fopen(VENTAS,"rb");
    if(fp!=NULL)
    {
        while(fread(&ventas[i],sizeof(Venta),1,fp) > 0)
        {
            if(ventas[i].cancelado == 0) ///guardo en el arreglo si no esta cancelado
            {
                i++;
            }
        }
        *validosV = i;

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
int validarNombreReceta(char nombre[],Receta recetas[],int validosRec)
{
    int existe=0,i=0;

    while(i<validosRec && strcmpi(nombre,recetas[i].nombre_preparacion) != 0)
    {
        i++;
    }

    if(i<validosRec)
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

int cantVentas()
{
    FILE* fp;
    int cant=0;

    fp = fopen(VENTAS,"rb");

    if(fp != NULL)
    {
        fseek(fp,0,SEEK_END); ///me posiciono al final del archivo;

        cant = ftell(fp)/sizeof(Venta);

        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo en modo lectura");

    return cant;
}

void modificarVentas()
{
    FILE* fp;
    int id=0,i=0,validos=0;
    Venta v;

    fp = fopen(VENTAS,"r+b");

    printf("\nIngrese el id del pedido a cancelar:  ");
    scanf("%i",&id);

    validos = cantVentas();

    if(fp != NULL)
    {
        fread(&v,sizeof(Venta),1,fp);

        while( (i < validos) && v.idVenta != id)///busco el id
        {
            fread(&v,sizeof(Venta),1,fp);
            i++;
        }

        if(v.cancelado){
          printf("\nEl id ingresado ya esta dado de baja.");
        }else if(i < validos)
        {
            v.cancelado = 1;

            fseek(fp,sizeof(Venta)*(-1),SEEK_CUR); /// me posicion antes de esa variable

            fwrite(&v,sizeof(Venta),1,fp);///sobreescribo

            for (int i = 0; i<3; i++)
                    {
                        gotoxy(40, 15);
                        printf("Venta cancelada con exito");
                        Sleep(300);
                        system("cls");
                        Sleep(300);
                    }
        }
        else
            printf("\nNo existe el id ingresado");
        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo");

}

void mostrarVentas()
{
    FILE* fp;
    Venta v;

    fp = fopen(VENTAS,"rb");

    if(fp != NULL)
    {
        while( fread(&v,sizeof(Venta),1,fp) > 0)
        {
            if(v.cancelado == 0)
                mostrarUnaVenta(v);
        }

        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo en modo lectura");

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
    printf("Id venta: %i",v.idVenta);
    printf("\nItems pedido:\n");
    mostrarPedidoPreparacion(v.items_pedido,v.cantItems);
    printf("\nTotal: $ %.2f",v.valor_total);
    printf("\n---------------------------------------------\n");

}

void registrarStockRestante(StockIngrediente stockIngredientes[],int validosIng)
{
    FILE* fp;

    fp=fopen(STOCK,"wb");

    if(fp!=NULL)
    {
        fwrite(stockIngredientes,sizeof(StockIngrediente),validosIng,fp);

        fclose(fp);
    }
    else
        printf("No se pudo abrir el archivo");

}


//implementacion menues
void gotoxy(int x, int y) //recibe coordenadas de colocacion de cursor
{
    HANDLE manipulador; /// identificador de consola
    COORD coordenada; //(struct que contiene variables x y de coord)
    manipulador=GetStdHandle(STD_OUTPUT_HANDLE); //obtiene manipulador de dispo salida en ejecución y toma control sobre esa salida
    coordenada.X = x;
    coordenada.Y = y;
    SetConsoleCursorPosition (manipulador, coordenada); //recibe manipulador de buffer pantalla  y nvas coord del cursor
}
float ingresoTotalDia(Venta ventas[],int validosV)
{
    float total=0;

    for(int i=0; i<validosV ; i++)
    {
        total += ventas[i].valor_total;
    }

    return total;

}

float costoTotal(StockIngrediente stockIngredientes[],int validosStock)
{
    float costo=0;

    for(int i=0 ; i<validosStock ; i++)
    {
        costo += stockIngredientes[i].cantidad * stockIngredientes[i].costo;
    }

    return costo;
}

void centrarTexto(char texto[],int y)
{
    int tam = strlen(texto);

    gotoxy(84 - tam/2,y);

    printf("%s",texto);
}
void menuResumenDelDia(StockIngrediente stockIngredientes[],int validosIng,PrecioPreparacion precioPreparaciones[],int validosPP,PreparacionVenta preparacionesVenta[],int validosPV,Receta recetas[],int validosRec,Venta ventas[],int* validosV)
{

    char icono,op;
    int x, y, teclaMov, ingresoSubmenu,salir;
    float  ingresoDia, gananciaDia,costosDia;
    x = 1;
    y = 3;
    icono = 16;
    ingresoSubmenu = 1;
    ingresoDia = 0;
    gananciaDia=0;

    gotoxy(0,0);
    do
    {
        //posicionamiento y muestra de menu


       printf("\n----MENU RESUMEN DEL DIA-----\n\n");

        gotoxy(x, y);
        printf("%c", icono);
        printf("\t1. Ver lista de remanentes de ingredientes en stock\n");
        printf("\t2. Ver lista de preparados y su cantidad que quedan en venta\n");
        printf("\t3. Ver el ingreso total del d%ca\n", 161);
        printf("\t4. Ver la ganancia del d%ca\n", 161);
        printf("\n");
        printf("\tPara salir presione la tecla Esc");

        //codigo para utilizar teclado para navegabilidad

        ingresoSubmenu = 1;
        salir=0;

        do
        {
            teclaMov = getch();

            if (teclaMov == UP && y>3)
            {
                gotoxy(x, y);
                printf(" ");
                y--;
                gotoxy(x, y);
                printf("%c", icono);
                ingresoSubmenu--;

            }

            else if (teclaMov == DOWN && y<6)
            {
                gotoxy(x, y);
                printf(" ");
                y++;
                gotoxy(x, y);
                printf("%c", icono);
                ingresoSubmenu ++;
            }

            if(teclaMov == ENTER)
            {


                system("cls");


                switch(ingresoSubmenu)
                {

                case 1:
                    system("cls");


                    printf("REMANENTE STOCK INGREDIENTES:\n");

                    mostrarStockIngredientes(stockIngredientes, validosIng);

                    printf("Desea persistir el Stock Remanente? ");
                    fflush(stdin);
                    scanf("%c",&op);
                    if(op=='s' || op=='S')
                        registrarStockRestante(stockIngredientes,validosIng);

                    x = 1;
                    y = 3;
                    salir=1;
                    system("pause");
                    system("cls");


                    break;

                case 2:

                    printf("PREPARADOS QUE QUEDAN PARA LA VENTA:\n");

                    mostrarPreparadosVentas(preparacionesVenta,validosPV);

                    x = 1;
                    y = 3;
                    salir=1;
                    system("pause");
                    system("cls");

                    break;

                case 3:

                    printf("INGRESO TOTAL DEL DIA\n");

                    obtenerVentas(ventas,validosV);

                    ingresoDia = ingresoTotalDia(ventas,*validosV);

                    printf("\nEl ingreso total del dia fue de $ %.2f\n",ingresoDia);

                    x = 1;
                    y = 3;
                    salir=1;
                    system("pause");
                    system("cls");
                    break;

                case 4:

                    printf("GANANCIA DEL DIA\n");

                    ingresoDia = ingresoTotalDia(ventas,*validosV);
                    costosDia = costoTotal(stockIngredientes,validosIng);

                    gananciaDia = ingresoDia - costosDia;

                    printf("\nLa ganancia del dia fue de $ %.2f\n",gananciaDia);

                    x = 1;
                    y = 3;
                    salir=1;


                    system("pause");
                    system("cls");
                    break;


                }
            }
        }
        while (teclaMov != ESC && salir!=1);
    }
    while(teclaMov!=ESC);
}
void menuStockIngredientes(StockIngrediente stockIngredientes[],int* validosStock)
{
    gotoxy(40, 15);
    printf("Obteniendo ingredientes...\n");
    Sleep(500);
    system("cls");
    gotoxy(20, 0);
    printf("----STOCK INGREDIENTES----\n\n");
    obtenerStockIngredientes(stockIngredientes,validosStock);
    mostrarStockIngredientes(stockIngredientes,*validosStock);
}

void menuRecetas(Receta recetas[],int* validosRec)
{
    char icono;
    int teclaMov, x, y, ingresoSubmenu, recetaEncontrada,salir=0;
    char recetaBuscada[20];
    icono = 16;
    x = 1;
    y = 3;
    ingresoSubmenu = 1;
    gotoxy(0, 0);

    do
    {

        printf("----MENU RECETAS----");

        gotoxy(x, y);
        printf("%c", icono);

        printf("\t1) CARGAR RECETAS\n");
        printf("\t2) BUSCAR RECETAS\n");
        printf("\n");
        printf("\tPara salir presione la tecla Esc");
        printf("\n\n");
        ingresoSubmenu = 1;
        salir=0;


        do
        {
            teclaMov = getch();

            if (teclaMov == UP && y>3)
            {
                gotoxy(x, y);
                printf(" ");
                y--;
                gotoxy(x, y);
                printf("%c", icono);
                ingresoSubmenu--;

            }

            else if (teclaMov == DOWN && y<4)
            {
                gotoxy(x, y);
                printf(" ");
                y++;
                gotoxy(x, y);
                printf("%c", icono);
                ingresoSubmenu ++;
            }

            if (teclaMov == ENTER)
            {
                system("cls");

                switch (ingresoSubmenu)
                {
                case 1:

                    gotoxy(40, 15);
                    for (int i = 0; i<3; i++)
                    {
                        gotoxy(40, 15);
                        printf("Cargando recetas...");
                        Sleep(300);
                        system("cls");
                        Sleep(300);
                    }

                    obtenerRecetas(recetas,validosRec);
                    mostrarArregloRecetas(recetas,*validosRec);

                    salir=1;
                    x = 1;
                    y = 3;
                    system("pause");
                    system("cls");

                    break;

                case 2:

                    printf("Ingrese la receta que desea buscar:");
                    fflush(stdin);
                    gets(recetaBuscada);

                    while(!validarNombreReceta(recetaBuscada,recetas,*validosRec))
                    {
                        printf("El nombre ingresado no existe. Ingrese la receta que desea buscar:");
                        fflush(stdin);
                        gets(recetaBuscada);
                    }

                    recetaEncontrada = buscarRecetaPorNombre(recetaBuscada,recetas,*validosRec);
                    mostrarUnaReceta(recetas[recetaEncontrada]);

                    system("pause");
                    system("cls");

                    salir=1;
                    x = 1;
                    y = 3;

                    break;

                }

            }
        }
        while(teclaMov!=ESC && salir!=1);

    }
    while (teclaMov != ESC);


}
void menuVentas(Preparacion demandas[],int* validosDem,Receta recetas[],int validosRec,StockIngrediente stockIngredientes[],int validosStock,PreparacionVenta preparacionesVenta[],int* validosPV,PrecioPreparacion precioPreparaciones[],int* validosPP )
{
    char icono, comprobante, confirmacion;
    int teclaMov, x, y, ingresoSubmenu,salir=0;
    float agarrarCosto;
    icono = 16;
    x = 1;
    y = 3;
    ingresoSubmenu = 1;

    gotoxy (0, 0);

    do
    {
        printf("----MENU VENTAS----");

        gotoxy(x, y);
        printf("%c", icono);

        printf("\t1) LISTAR DEMANDAS\n");
        printf("\t2) PREPARAR DEMANDAS PARA LA VENTA\n");
        printf("\t3) CARGAR PRECIOS\n");
        printf("\t4) MODIFICAR PRECIOS\n");
        printf("\t5) AGREGAR UNA VENTA\n");
        printf("\t6) CANCELAR UNA VENTA\n");
        printf("\t7) MOSTRAR VENTAS\n");
        printf("\n");
        printf("\tPara salir presione la tecla Esc");

        ingresoSubmenu = 1;
        salir=0;

        do

        {
            teclaMov = getch();

            if (teclaMov == UP && y>3)
            {
                gotoxy(x, y);
                printf(" ");
                y--;
                gotoxy(x, y);
                printf("%c", icono);
                ingresoSubmenu--;

            }

            else if (teclaMov == DOWN && y<9)
            {
                gotoxy(x, y);
                printf(" ");
                y++;
                gotoxy(x, y);
                printf("%c", icono);
                ingresoSubmenu ++;
            }

            if (teclaMov == ENTER)
            {
                system("cls");

                switch(ingresoSubmenu)
                {
                case 1:
                    obtenerDemanda(demandas,validosDem);
                    mostrarArregloDemandas(demandas,*validosDem);
                    salir=1;
                    x = 1;
                    y = 3;
                    system("pause");
                    system("cls");
                    break;

                case 2:
                    prepararDemandas(demandas,*validosDem,recetas,validosRec,stockIngredientes,validosStock,preparacionesVenta,validosPV);
                    registrarPreparadosVenta(preparacionesVenta,*validosPV);
                    mostrarPreparadosVentas(preparacionesVenta,*validosPV);
                    salir=1;
                    x = 1;
                    y = 3;
                    system("pause");
                    system("cls");

                    break;

                case 3:

                    *validosPP = *validosPV;

                    cargarPrecios(preparacionesVenta,precioPreparaciones,*validosPV,stockIngredientes,validosStock,recetas,validosRec);
                    registrarPrecios(precioPreparaciones,*validosPP);
                    for (int i = 0; i<3; i++)
                    {
                        gotoxy(40, 15);
                        printf("Precios cargados con %cxito", 130);
                        Sleep(300);
                        system("cls");
                        Sleep(300);
                    }


                    salir=1;
                    x = 1;
                    y = 3;
                    mostrarPrecios(precioPreparaciones,*validosPP);
                    system("pause");

                    system("cls");

                    break;

                case 4:
                    mostrarPrecios(precioPreparaciones,*validosPP);

                    printf("Esta seguro que desea modificar los precios?(s/n)\n");
                    scanf("%c", &confirmacion);

                    if (confirmacion == 's' || confirmacion == 'S')
                    {
                        modificarPrecios(precioPreparaciones, *validosPP);
                    }
                    system("cls");
                    mostrarPrecios(precioPreparaciones,*validosPP);
                    salir=1;
                    x = 1;
                    y = 3;
                    system("pause");

                    system("cls");

                    break;


                case 5:
                    registrarVentas(precioPreparaciones,*validosPP,preparacionesVenta,*validosPV);
                    mostrarVentas();
                    salir=1;
                    x = 1;
                    y = 3;
                    system("pause");
                    system("cls");

                    break;

                case 6:

                    modificarVentas();

                    mostrarVentas();
                    system("pause");
                    system("cls");

                    salir=1;
                    x = 1;
                    y = 3;
                    break;

                case 7:
                    mostrarVentas();
                    system("pause");
                    system("cls");

                    salir=1;
                    x = 1;
                    y = 3;
                    break;

                }

            }
        }
        while (teclaMov != ESC && salir !=1);
    }
    while(teclaMov != ESC);
}

float ingresoDelDia(float registroVentas[], int validosRV)
{
    float sumatoriaIngresos = 0;


    for(int i = 0; i < validosRV; i++)
    {
        sumatoriaIngresos = sumatoriaIngresos + registroVentas[i];
    }
    printf("Ingreso del dia: %.2f\n", sumatoriaIngresos);



    return sumatoriaIngresos;
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
    float registroVentasTotal[N];
    int validosRV = 0;
    Venta ventas[N];
    int validosV = 0;

    system("cls");

    system("COLOR E0"); //color pantalla (fue random)
    char icono, correrPrograma;
    int x, y, teclaMov, ingresoSubmenu, selector1,salir;
    x = 1;
    y = 3;
    ingresoSubmenu = 1;
    salir=0;
    icono = 16;

    gotoxy(40, 15);
    printf("Presione ENTER para ingresar al programa\n"); //presentacion
    scanf("%c", &correrPrograma);
    system("cls");

    gotoxy(0,0);

    ///codigo para seleccionar conflechas y pulsar enter

    do
    {
        printf("----- PANADERIA LOS REYES MAGOS ----- \n\n"); ///cabecera del menu

        gotoxy(x, y);
        printf("%c", icono);
        ///MENU

        printf("\t1) \t Ingreso de mercader%ca.\n", 161);
        printf("\t2) \t Cocci%cn de preparados.\n", 162);
        printf("\t3) \t Venta al p%cblico.\n", 163);
        printf("\t4) \t Resumen del d%ca.\n",161);
        printf("\n");
        printf("\tPara salir presione la tecla Esc");
        printf("\n\n");

        ingresoSubmenu = 1;
        salir = 0;
        do
        {

            teclaMov = getch();

            if (teclaMov == UP && y>3)
            {


                gotoxy(x, y);
                printf(" ");
                y--;
                gotoxy(x, y);
                printf("%c", icono);
                ingresoSubmenu--;


            }

            else if (teclaMov == DOWN && y<6)
            {
                gotoxy(x, y);
                printf(" ");
                y++;
                gotoxy(x, y);
                printf("%c", icono);
                ingresoSubmenu++;
            }

            ///codigo para que al pulñsar eenterse limpie la pantalla y salga sólo
            //la opcion elegida
            if (teclaMov == ENTER)
            {
                system("cls");

                switch (ingresoSubmenu)
                {
                case 1:
                    //muestra stock ingredientes
                    //despersiste ingredientes

                    menuStockIngredientes(stockIngredientes,&validosStock);

                    salir = 1;
                    x = 1;
                    y = 3;
                    system("pause");
                    system("cls");
                    break;

                case 2:
                    //ingreso al submenu recetas

                    menuRecetas(recetas,&validosRec);

                    salir = 1;
                    x = 1;
                    y = 3;
                    system("cls");


                    break;

                case 3:
                    //ingreso al submenu ventass
                    menuVentas(demandas,&validosDem,recetas,validosRec,stockIngredientes,validosStock,preparacionesVenta,&validosPV,precioPreparaciones,&validosPP);


                    salir = 1;
                    x = 1;
                    y = 3;
                    system("cls");

                    break;

                case 4:
                    //ingreso al submenu resumen del dia
                    menuResumenDelDia(stockIngredientes,validosStock,precioPreparaciones,validosPP,preparacionesVenta,validosPV,recetas,validosRec,ventas,&validosV);
                    salir = 1;
                    x = 1;
                    y = 3;
                    system("cls");

                    break;

                }
            }
        }
        while(teclaMov !=ESC && salir != 1);
    } //codigo para salir
    while (teclaMov !=ESC);


    for (int i = 0; i<3; i++)
    {
        gotoxy(40, 15);
        printf("Saliendo del programa...");
        Sleep(300);
        system("cls");
        Sleep(300);

    }

    return 0;
}


