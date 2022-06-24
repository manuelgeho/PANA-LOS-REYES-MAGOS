#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

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
        system("cls");
        gotoxy(40, 15);
        printf("Saliendo del programa...");
        Sleep(300);
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
    printf("\n3. Ver el ingreso total del dia");
    printf("\n4. Ver la ganancia del dia");
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
                system("cls");
                gotoxy(40, 15);
                printf("Cargando recetas...");
               // obtenerRecetas(recetas,&validosRec);


                break;

            case 2:
                printf("Ingrese la receta que desea busca:\t");
               // gets(recetaBuscada);
               // recetaEncontrada = buscarRecetaPorNombre(recetaBuscada, recetas, rVal)
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
    char icono, comprobante;
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
    printf("\t3) CARGAR PRECIOS\n");
    printf("\t4) MODIFICAR PRECIOS\n");
    printf("\t5) VOLVER AL MENU GENERAL\n\n");

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
                registrarVentas(precioPreparaciones,validosPP,preparacionesVenta,validosPV);
                nuevaVenta(&v, precios[],validosPP, preparacionesVenta[],validosPV);

                printf("Desea ver el comprobante de venta? (s/n)");
                scanf("%c", &comprobante);

                if (comprobante == 's' || comprobante == 'S')
                {
                    mostrarUnaVenta(venta);
                }


                break;

            case 2:

                modificarVentas();
                cantVentas();
                printf("Venta cancelada con éxito");

                break;

            case 3:
                /// ACA IRIA LA CARGA DE PRECIOS PARA ARTICULOS

                break;

            case 4:
                modificarPrecios(precios[],validosPP)

                break;


            case 5:
                menuGeneral();
                break;

            }

    }
    } while (teclaMov != ESC);


    }



    int main()
    {
        menuGeneral();
        return 0;
    }
