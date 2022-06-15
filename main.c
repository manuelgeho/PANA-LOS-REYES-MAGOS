#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //para cambios de color en consola
#define DEMANDA "demanda.bin"
#define RECETAS "recetas.bin"

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

typedef struct ///Recetas
 {
   char nombre_preparacion[20];
   IngredienteXReceta ingredientes[20]; ///Puede tener hasta 20 ingredientes
   int cantIngredientes; ///los validos de ingredientes
 }Receta;

typedef struct ///Ingredientes Por Receta
 {
    char nombre_ingrediente[20];
    float cantidad; ///puede ser en ml o en kg
 }IngredienteXReceta;

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


int main()
{
    printf("Hello world!\n");
    return 0;
}
