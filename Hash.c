#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include <stdint.h>
// para int32_t, uint32_t, etc.


//          Esta versión sólo soporta llaves (keys) enteras positivas.


enum{ EMPTY_CELL = -2, DELETED_CELL = -1 };

typedef struct
{
   int32_t bar_code;
   char    name[ 32 ];
   float   price;
} Product;

void Product_Print( Product* p )
{
   printf( 
      "Barcode: %d\n"
      "   Name: %s\n"
      "  Price: $%0.2f\n\n",
      p->bar_code, p->name, p->price );
}


typedef struct
{
   /**
    * Guarda la llave de indexado. En este ejemplo dicha llave es el código de
    * barras (bar_code). Lo necesitamos para cuando toque resolver colisiones.
    * Aunque en este ejemplo no aplica, la key podría ser negativa, por eso está
    * declarada como entero sin signo.
    */
   int32_t key;

   /**
    * Guarda el índice del producto en la tabla de productos del cliente.
    * EMPTY_CELL indica que la entrada está libre, y DELETED_CELL que la entrada fue borrada. 
    * Aunque los índices siempre son positivos, vamos a utilizar valores
    * negativos como centinelas, por eso está declarada como entero sin signo.
    */
   int32_t index;
} Table_entry;


typedef struct
{
	Table_entry* table; ///< Es la tabla hash
	size_t  size;       ///< Es el número máximo de elementos en la tabla
	size_t  len;        ///< Es el número actual de elementos en la tabla
} Hash_table;


/**
 * @brief Crea una tabla hash.
 *
 * @param size El número de elementos en la tabla hash
 *
 * @return Referencia a una nueva tabla hash.
 */
Hash_table* HT_New( size_t size )
{
   Hash_table* ht = ( Hash_table* )malloc( sizeof( Hash_table ) );
   if( NULL != ht ) {

      ht->len = 0;
      ht->size = size;

      ht->table = ( Table_entry* ) malloc( size * sizeof( Table_entry ) );
      if( NULL == ht->table ){

         free( ht );
         ht = NULL;

      } else {
         for( int i = 0; i < ht->size; ++i ) {

            ht->table[ i ].index = EMPTY_CELL;
            ht->table[ i ].key = 0;
         }
      }
   }

   return ht;
   // el cliente es responsable de verificar que efectivamente la tabla se creó
}

/**
 * @brief Destruye una tabla hash
 *
 * @param ht Referencia a una tabla hash
 *
 * @post La referencia ht es puesta a NULL
 */
void HT_Delete( Hash_table** ht )
{
   assert( ht );

   free( (*ht)->table );
   free( *ht );
   *ht = NULL;
}

// Es la función hash
int h( int key, int m )
{
   return key % m;
}

static void print_table_hash( const Hash_table* ht )
{
   fprintf( stderr, "\nSize: %ld\n", ht->size );
   fprintf( stderr, "Len: %ld\n", ht->len );
   for( size_t i = 0; i < ht->size; ++i ){
      fprintf( stderr, "[%02ld](%d, %d)\n", i, ht->table[ i ].key, ht->table[ i ].index );
   }
   fprintf( stderr, "\n" );
}

// es la función de resolución de colisiones
int probe( int key, int i )
{
   return i + 1;
}

bool HT_Insert( Hash_table* ht, int32_t key, int32_t idx )
{
   assert( ht );
   assert( ht->len < ht->size );

   int pos;
   // índice que se actualizará en cada colisión

   int home = pos = h( key, ht->size );
   // calcula una hash key base

   fprintf( stderr, "Calculé el valor hash: %d para la llave: %d\n", pos, key );
   // información de depuración


   int i = 0;

   // si el slot está desocupado, se salta el while;
   // en caso contrario entra a buscar uno:
   while( ht->table[ pos ].index >= 0 ){

      // no se aceptan duplicados:
      if( ht->table[ pos ].key == key ){
         fprintf( stderr, "Error: Llave duplicada\n" );
         return false;
      }

      pos = ( home + probe( key, i ) ) % ht->size;
      fprintf( stderr, "Colisión. Recalculé el valor hash: %d para la llave: %d\n", pos, key );

      ++i;
      // el incremento de esta variable depende del método de resolución de
      // colisiones utilizado
   }

   ht->table[ pos ].key = key;
   ht->table[ pos ].index = idx;

   ++ht->len;

   return true;
}

/**
 * @brief Devuelve el índice idx en la tabla de productos correspondiente al
 * código de barras, key.
 *
 * @param ht Tabla hash
 * @param key El código de barras
 *
 * @return El índice |idx| en la tabla de productos, o\n
 *         -1 si la celda calculada en la tabla hash está vacía, o\n
 *         -2 no se encontró
 *
 * @pre La tabla existe.
 * @pre La tabla no está vacía.
 */
int32_t HT_Search( const Hash_table* ht, int32_t key )
{
   assert( ht );
   assert( ht->len > 0 );

   int home = h( key, ht->size );
   int pos = home;

   fprintf( stderr, "Calculé el valor hash: %d para la llave: %d\n", pos, key );

   int i = 0;

   if( ht->table[ pos ].index == EMPTY_CELL ) return -1;
   // celda vacía: el producto no existe en la tabla hash

   // si la key en la celda coincide con la key buscada, se salta el while;
   // en caso contrario entra a buscar una coincidencia:
   while( ht->table[ pos ].key != key ){    

      pos = ( home + probe( key, i ) ) % ht->size;
      fprintf( stderr, "Celda ocupada: Recalculé el valor hash: %d para la llave: %d\n", pos, key );
      
      ++i;
   }

   return( ht->table[ pos ].key == key ? ht->table[ pos ].index : -2 );
}

bool HT_IsEmpty( const Hash_table* ht )
{
   return ht->len == 0;
}

bool HT_IsFull( const Hash_table* ht )
{
   return ht->len == ht->size;
}

#define MAX_PRODUCTS 5

int division(int key, int m){
    return key%=m;
}

int msd(int key, int m){

int z = pow(10,(3/2));

key = pow(key,2);

int num = key%z;
int den = pow(10,(3/2));
int msd= (num/den)%m;

return msd;
}

//----------------------------------------------------------------------
// Driver program 
//----------------------------------------------------------------------
int main()
{
   Product productos[ MAX_PRODUCTS ] =
   {
      { 1000, "Gansito", 9.0 },     // [0]
      { 2000, "Crema", 16.5 },      // [1]
      { 3000, "Arroz", 28.5 },      // ...
      { 4000, "Papitas", 14.0 },
      { 5000, "Detergente", 25.0 },
   };

   Hash_table* tabla = HT_New( 17 );

   assert( tabla );
   // el programa se detiene si la tabla no se pudo crear

   assert( HT_IsEmpty( tabla ) == true );
   // la tabla recién se creó, debe estar vacía

//   print_table_hash( tabla );


   for( size_t i = 0; i < MAX_PRODUCTS && !HT_IsFull( tabla ); ++i ){

      HT_Insert( 
         tabla,                   // tabla hash
         productos[ i ].bar_code, // key
         i );                     // idx
   }

   print_table_hash( tabla );

   int32_t idx = HT_Search( tabla, 5000 );
   assert( idx == 4 ); // esperamos el índice 4
   Product_Print( &productos[ idx ] );


   HT_Delete( &tabla );
   assert( tabla == NULL );
}