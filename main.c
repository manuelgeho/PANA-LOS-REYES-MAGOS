#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define DEMANDA "demanda.bin"
#define RECETAS "recetas.bin"
#define STOCK "stockingredientes.bin"
#define PVENTAS "pventas.bin"
#define PRECIOS "precios.bin"
#define VENTAS "ventas.bin"

#define N 100
#define UP 72
#define DOWN 80
#define ENTER 13
#define ESC 27


/////////////////// MENUES /////////////////////////////////////////
//prototipado menues
void gotoxy (int, int); /// func de ubicacion de cursor
void menuGeneral();
void menuResumenDelDia();
void menuStockIngredientes();
void menuRecetas();
void menuVentas();
void obtenerVentas(float[], int*); //pasa a un array las ventas del arch VENTAS
float ingresoDelDia(float [], int ); // sumatoria de ingresos x vta usando func anterior

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
void menuGeneral()
{
    system("COLOR E0");
    char icono, correrPrograma;
    int x, y, teclaMov, ingresoSubmenu, selector1;
    x = 1;
    y = 3;
    ingresoSubmenu = 1;


    icono = 16;


    gotoxy(40, 15);
    printf("Presione ENTER para ingresar al programa\n");
    scanf("%c", &correrPrograma);
    system("cls");



    gotoxy(0,0);
    printf("----- PANADERIA LOS REYES MAGOS ----- \n\n"); ///cabecera del menu

    gotoxy(x, y);
    printf("%c", icono);

    printf("\t1) \t Ingreso de mercader%ca.\n", 161);
    printf("\t2) \t Cocci%cn de preparados.\n", 162);
    printf("\t3) \t Venta al p%cblico.\n", 163);
    printf("\t4) \t Resumen del d%ca.\n",161);
    printf("\n");
    printf("\tPara salir presione la tecla Esc");
    printf("\n\n");



    do

    {
        teclaMov = getch();

        if (teclaMov == UP)
        {
            gotoxy(x, y);
            printf(" ");
            y--;
            gotoxy(x, y);
            printf("%c", icono);
            ingresoSubmenu--;

        }

        else if (teclaMov == DOWN)
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

                menuStockIngredientes();

                break;

            case 2:

                menuRecetas();


            case 3:

                menuVentas();
                break;

            case 4:

                menuResumenDelDia();
                break;



            }
        }


    }
    while (teclaMov != ESC);


        if (teclaMov == ESC)
    {
          for (int i = 0; i<5; i++)
        {
            gotoxy(40, 15);
            printf("Saliendo del programa...");
            Sleep(300);
            system("cls");
            Sleep(300);
        }
    }
    }
void menuResumenDelDia()
{

    char icono;
    int x, y, teclaMov, ingresoSubmenu;
    float  ingresoDia, gananciaDia;
    x = 1;
    y = 3;
    icono = 16;
    ingresoSubmenu = 0;
    ingresoDia = 0;
    gananciaDia=0;

    gotoxy(0,0);
    printf("\n----MENU RESUMEN DEL DIA\-----n");
    printf("\n1. Ver lista de remanentes de ingredientes en stock");
    printf("\n2. Ver lista de preparados y su cantidad que quedan en venta");
    printf("\n3. Ver el ingreso total del d%ca", 161);
    printf("\n4. Ver la ganancia del d%ca", 161);
    printf("\n5) Volver al Menu General");

    gotoxy(x, y);
    printf("%c", icono);

    do

    {
        teclaMov = getch();

        if (teclaMov == UP)
        {
            gotoxy(x, y);
            printf(" ");
            y--;
            gotoxy(x, y);
            printf("%c", icono);
            ingresoSubmenu--;

        }

        else if (teclaMov == DOWN)
        {
            gotoxy(x, y);
            printf(" ");
            y++;
            gotoxy(x, y);
            printf("%c", icono);
            ingresoSubmenu ++;
        }

        switch(ingresoSubmenu)
        {
        case 1:
            printf("REMANENTE STOCK INGREDIENTES:\n");

            mostrarStockIngredientes(stockIng[], validos);

            break;

        case 2:
            printf("PREPARADOS QUE QUEDAN PARA LA VENTA:");



            break;

        case 3:
            printf("INGRESO TOTAL DEL DIA\n");

            break;

        case 4:
            printf("GANANCIA DEL DIA\n");

            break:

        case 5:
            menuGeneral();
            break;
        }


    }
    while (teclaMov != ESC);
}
void menuStockIngredientes()
{
    gotoxy(40, 15);
    printf("Obteniendo ingredientes...\n");
    // obtenerStockIngredientes(stockIngredientes,&validosStock);
    Sleep(500);
    system("cls");
    gotoxy(20, 0);
    printf("----STOCK INGREDIENTES----\n\n");
    // mostrarStockIngredientes(stockIngredientes,validosStock);
}
void menuRecetas()
{
    char icono;
    int teclaMov, x, y, ingresoSubmenu, recetaEncontrada;
    char recetaBuscada[15];
    icono = 16;
    x = 1;
    y = 3;
    ingresoSubmenu = 1;

    gotoxy(0, 0);
    printf("----MENU RECETAS----");

    gotoxy(x, y);
    printf("%c", icono);

    printf("\t1) CARGAR RECETAS\n");
    printf("\t2) BUSCAR RECETAS\n");
    printf("\t3) VOLVER AL MENU GENERAL")




    do

    {
        teclaMov = getch();

        if (teclaMov == UP)
        {
            gotoxy(x, y);
            printf(" ");
            y--; //
            gotoxy(x, y);
            printf("%c", icono);
            ingresoSubmenu--;

        }

        else if (teclaMov == DOWN)
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
                for (int i = 0; i<5; i++)
        {
            gotoxy(40, 15);
            printf("Cargando recetas...");
            Sleep(300);
            system("cls");
            Sleep(300);
        }

               // obtenerRecetas(recetas,&validosRec);


                break;

            case 2:
                printf("Ingrese la receta que desea buscar:\t");
               // gets(recetaBuscada);
               // recetaEncontrada = buscarRecetaPorNombre(recetaBuscada, recetas, rVal)
               //system("cls");
              //  printf("%s", recetas[recetaEncontrada]);

                break;

            case 3:
                menuGeneral();

            }

        }
    } while (teclaMov != ESC);


    }
void menuVentas()
{
    char icono, comprobante, confirmacion;
    int teclaMov, x, y, ingresoSubmenu;
    icono = 16;
    x = 1;
    y = 3;
    ingresoSubmenu = 1;

    gotoxy (0, 0);
    printf("----MENU VENTAS----");

    gotoxy(x, y);
    printf("%c", icono);

    printf("\t1)INGRESAR NUEVA VENTA\n");
    printf("\t2)CANCELAR VENTA\n");
    printf("\t3) VER LISTA DE PRECIOS\n");
    printf("\t4) CARGAR PRECIOS\n");
    printf("\t5) MODIFICAR PRECIOS\n");
    printf("\t6) VOLVER AL MENU GENERAL\n\n");

        do

    {
        teclaMov = getch();

        if (teclaMov == UP)
        {
            gotoxy(x, y);
            printf(" ");
            y--; //
            gotoxy(x, y);
            printf("%c", icono);
            ingresoSubmenu--;

        }

        else if (teclaMov == DOWN)
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
                obtenerDemanda(demandas,&validosDem);
                prepararDemandas(demandas,validosDem,recetas,validosRec,stockIngredientes,validosStock,preparacionesVenta,&validosPV);
                nuevaVenta(&v, precios[],validosPP, preparacionesVenta[],validosPV);
                registrarVentas(precioPreparaciones,validosPP,preparacionesVenta,validosPV);
                mostrarUnaVenta(venta);



                break;

            case 2:

                modificarVentas();
                cantVentas();

                for (int i = 0; i<5; i++)
        {
            gotoxy(40, 15);
            printf("Venta cancelada con exito");
            Sleep(300);
            system("cls");
            Sleep(300);
        }


                break;

            case 3:

                mostrarPrecios(precios[], validos);

                break;

            case 4:
                cargarPrecios(preparacionesVenta,precioPreparaciones,validosPV,stockIngredientes,validosStock,recetas,validosRec);
                for (int i = 0; i<5; i++)
        {
            gotoxy(40, 15);
            printf("Precios cargados con %cxito", 130);
            Sleep(300);
            system("cls");
            Sleep(300);
        }

                break;


            case 5:
                gotoxy(40, 15);
                printf("Esta seguro que desea modificar los precios?(s/n)\n");
                scanf("%c", &confirmacion)

                if (confirmacion == 's' || confirmacion == 'S')
                {
                    modificarPrecios(precios[], validosPP)
                }

                break;

            case 6:
                menuGeneral();
                break;

            }

    }
    } while (teclaMov != ESC);


    }
void obtenerVentas(float registroVentas[], int* validosRV)
{
    FILE* fp;
    int aux, i=0;

    fp = fopen(VENTAS,"rb");

    if( fp != NULL)
    {
        while( i < N &&fread(&aux,sizeof(float),1,fp) > 0 )
        {
            registroVentas[i] = aux;
            i++;
        }

        *validosRV = i;
    }
    else
    {
        printf("NO");
    }


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
        menuGeneral();
        return 0;
    }
