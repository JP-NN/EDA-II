/*Copyright (C)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * 2022 - francisco dot rodriguez at ingenieria dot unam dot mx
 */


#include <stdio.h>

#include <stdlib.h>
// malloc and friends

#include <string.h>

#include <assert.h>

#include <stdbool.h>

#include <stdint.h>

#define DBG_HELP      1
#define STR_MAX_LEN   32

#if DBG_HELP == 1
#define DBG_PRINT( ... ) \
   do{\
      fprintf( stderr, "DBG:" __VA_ARGS__ ); \
   } while( 0 )
#else
#define DBG_PRINT( ... ) ;
#endif

typedef enum
{
   BST_PREORDER,
   BST_INORDER,
   BST_POSTORDER,
} BST_Dir;

typedef struct
{
   int barcode;
   char name[ STR_MAX_LEN ];
} Product;

typedef struct Node_
{
   Product product;

   struct Node_* left;
   struct Node_* right;
} Node;

static Node* new_node( Product* p )
{
   Node* n = (Node*) malloc( sizeof( Node ) );
   if( n != NULL )
   {
      n->left = NULL;
      n->right = NULL;

      n->product.barcode = p->barcode;
      strncpy( n->product.name, p->name, STR_MAX_LEN );
   }

   DBG_PRINT( "new_node():Creating the node: %p (Key=%d)\n", n, n->product.barcode );
   return n;
}

static Node* insert_node( Node* parent, Product* p )
{
   if (parent==NULL)
   {
      parent=new_node(p);

   }
   else if (p->barcode<parent->product.barcode)
   {
      parent->left=insert_node(parent->left,p);
   }
   else{parent->right=insert_node(parent->right,p);}

   return parent;
   
}

typedef struct
{
   Node* root;
   size_t len;
} BST;

/**
 * @brief Crea un nuevo árbol.
 *
 * @return Un nuevo árbol BST.
 */
BST* BST_New()
{
   BST* t = (BST*) malloc( sizeof( BST ) );
   if( t != NULL ){

      t->root = NULL;
      t->len = 0;
   }
   return t;
}

// borra todos los nodos (utiliza el recorrido postorder para eliminar primero
// a los hijos y luego al padre):
static void clear_all( Node* parent )
{ 
   if (parent==NULL)  return ;
   clear_all(parent->left);
   clear_all(parent->right);
   DBG_PRINT("Clear_all:Deleting the node %p(key=%d)\n",parent,parent->product.barcode);
   free(parent);
}

/**
 * @brief Destruye al árbol.
 *
 * @param bst Un árbol BST.
 */
void BST_Delete( BST** bst )
{
   assert( *bst != NULL );

   DBG_PRINT( "Delete:Deleting all nodes...\n" );

   clear_all( (*bst)->root );
   // borra todos los nodos

   free( *bst );
   // borra el árbol

   *bst = NULL;
}

/**
 * @brief Inserta una copia del elemento en el árbol.
 *
 * @param bst Un árbol BST.
 * @param p Referencia al elemento a insertar.
 *
 * @return true si el elemento pudo ser insertado; false si falló la asignación de memoria, (o si se encontró un duplicado (cuando se incorpore dicha funcionalidad)).
 }
 */
bool BST_Insert( BST* bst, Product* p )
{
   assert(bst);  //Arbol existe
   assert(p);     //Producto existe
   Node* tmp=insert_node(bst->root,p);
   //En cualquier nodo no raiz es decartado(se usa para la recursion)

   if (bst->root==NULL&& tmp!=NULL)    
      bst->root=tmp;

   return tmp!=NULL;
}

/**
 * @brief Borra todos los nodos del árbol, sin borrar al árbol
 *
 * @param bst Un árbol BST.
 */
void BST_Delete_all( BST* bst )
{
   if (!bst){
     return;
   }
   

}

static void inorder( Node* parent, void (*visit)( Product* ) )
{
   if( parent == NULL ) return;

   inorder( parent->left, visit );
   visit( &( parent->product ) );
   inorder( parent->right, visit );
}

static void preorder( Node* parent, void (*visit)( Product* ) )
{
   if (!parent)
   {
      visit(parent->product.barcode);
      preorder(parent->left,visit);
      preorder(parent->right,visit);
   }
   
}

static void postorder( Node* parent, void (*visit)( Product* ) )
{
   if (!parent)
   {
      postorder(parent->left,visit);
      postorder(parent->right,visit);
      visit(&parent->product.barcode);
   }
}

void BST_Traverse( BST* bst, BST_Dir dir, void (*visit)( Product* ) )
{
   assert( bst );


   switch( dir )
   {
      case BST_PREORDER:  preorder(  bst->root, visit ); break;
      case BST_INORDER:   inorder(   bst->root, visit ); break;
      case BST_POSTORDER: postorder( bst->root, visit ); break;
      default: assert( 0 );
   }
}

static Node* seek( Node* parent, int key )
{
   if( parent == NULL || parent->product.barcode == key ) return parent;

   return seek(
            key < parent->product.barcode ? parent->left : parent->right,
            key );
}

/**
 * @brief Busca un elemento en la lista.
 *
 * @param bst Un árbol BST.
 * @param key El valor que se está buscando (en este ejemplo, el código de barras).
 * @param p Si el elemento existe, entonces devuelve una copia en este argumento.
 *
 * @return true si el elemento existe; false en caso contrario, con lo cual el argumento
 *         p no es válido.
 */
bool BST_Find( BST* bst, int key, Product* p )
{
   assert(bst);
   Node* v=seek(bst->root,key);
   if (v)
   {
      p->barcode=v->product.barcode;
      strncpy(p->name,v->product.name,STR_MAX_LEN);
   }
   
}

static Node* find_succ( Node* node )
{
   while( node->left != NULL )
   {
      node = node->left;
   }

   return node;
}

static Node* delete_node( Node* parent, int key )
{
   if( parent == NULL ) return parent;

   if( key < parent->product.barcode ) parent->left = delete_node( parent->left, key );
   // busca el nodo en la rama izquierda

   else if( key > parent->product.barcode ) parent->right = delete_node( parent->right, key );
   // busca el nodo en la rama derecha

   else  // key == parent->product.barcode:
   { 
      // el nodo es hoja o tiene 1 hijo:

      if( parent->left == NULL )
      {
         Node* tmp = parent->right;
         free( parent );
         return tmp;
      }

      else if( parent->right == NULL )
      {
         Node* tmp = parent->left;
         free( parent );
         return tmp;
      }

      // el nodo tiene dos hijos:

      else
      {
         Node* tmp = find_succ( parent->right );

         parent->product = tmp->product;

         parent->right = delete_node( parent->right, tmp->product.barcode );
      }
   }

   return parent;
}

void BST_Erase( BST* bst, int key )
{
}

void print( Product* p )
{
   printf( "Barcode: %d\n"
           "   Name: %s\n",
           p->barcode, p->name );
}

void BST_Min( BST* bst, int key, Product* p )
{
   while (bst->root->left!=NULL)
   {
      bst->root->left++;
      printf("\nEcontranding menoring\n");
   }
}

void BST_Max( BST* bst, int key, Product* p )
{
}

bool BST_Predecessor( BST* bst, int key, Product* p )
{
}

bool BST_Succesor( BST* bst, int key, Product* p )
{
}

//----------------------------------------------------------------------
// Generador de la gráfica
//----------------------------------------------------------------------
void print_bst_rec_labels( Node* parent, FILE* f )
{
   if( parent == NULL ) return;

   print_bst_rec_labels( parent->left, f );

   int barcode = parent->product.barcode;
   fprintf( f, "node_%d[label=\"<f0>|<f1>%d|<f2>\"];\n", barcode, barcode );

   //fprintf( f, "node_%d[label=\"<f0>|<f1>(%d,%s)|<f2>\"];\n", barcode, barcode, parent->product.name );
   // por si quieren imprimir el nombre del producto

   print_bst_rec_labels( parent->right, f );
}

void print_bst_rec_relations( Node* parent, FILE* f )
{
   if( parent == NULL ) return;

   int barcode_parent = parent->product.barcode;

   if( parent->left )
   {
      int barcode_left = parent->left->product.barcode;
      fprintf( f, "\"node_%d\":f0 -> \"node_%d\":f1;\n", barcode_parent, barcode_left );
   }

   if( parent->right )
   {
      int barcode_right = parent->right->product.barcode;
      fprintf( f, "\"node_%d\":f2 -> \"node_%d\":f1;\n", barcode_parent, barcode_right );
   }

   print_bst_rec_relations( parent->left, f );
   print_bst_rec_relations( parent->right, f );
}

void print_bst( BST* bst, char* file_name )
{
   FILE* file = fopen( file_name, "w" );
   if( file == NULL )
   {
      fprintf( stderr, "Error creating the file.\n" );
      return;
   }

   fprintf( file, "digraph G {\n" "node [shape=record];\n" );

   print_bst_rec_labels( bst->root, file );

   print_bst_rec_relations( bst->root, file );

   fprintf( file, "}\n" );

   fclose( file );
}

//----------------------------------------------------------------------
// driver program
//----------------------------------------------------------------------
int main()
{
   BST* b = BST_New();
   assert( b != NULL );

   Product p = { .barcode = 1000, .name="NA" };

   assert( BST_Insert( b, &p ) == true );

   p.barcode = 500;
   assert( BST_Insert( b, &p ) == true );

   p.barcode = 1500;
   assert( BST_Insert( b, &p ) == true );

   p.barcode = 250;
   assert( BST_Insert( b, &p ) == true );

   p.barcode = 750;
   assert( BST_Insert( b, &p ) == true );

   p.barcode = 1750;
   assert( BST_Insert( b, &p ) == true );

   p.barcode = 1250;
   assert( BST_Insert( b, &p ) == true );

   printf( "Preorder:\n" );
   BST_Traverse( b, BST_PREORDER, print );

   printf( "Inorder:\n" );
   BST_Traverse( b, BST_INORDER, print );

   printf("Min value encontnra sdiagf");
   BST_Min(b,500, &p);

   printf( "Postorder:\n" );
   BST_Traverse( b, BST_POSTORDER, print );

   print_bst( b, "my_bst.dot" );
   // para dibujar al árbol debes tener instalado el programa "dot", el cual
   // viene en el paquete graphviz:
   //
   //       $ sudo apt install graphviz
   //
   // print_graph() genera un archivo mi_grafo.dot el cual tienes que renderizar
   // con el programa "dot". Para obtener una imagen del grafo en formato png:
   //
   //       $ dot -Tpng my_bst.dot -omy_bst.png
   //
   // Pero si la quisieras como svg (más pequeña (en tamaño) y editable):
   //
   //       $ dot -Tsvg my_bst.dot -omy_bst.svg
   //

   Product dummy;
   assert( BST_Find( b, 750, &dummy ) == true );
   print( &dummy );

   printf( "\nTesting erasing the element with key %d\n", 1000 );
   BST_Erase( b, 1000 );
   BST_Traverse( b, BST_PREORDER, print );

   printf( "\nTesting erasing the element with key %d\n", 1750 );
   BST_Erase( b, 1750 );
   BST_Traverse( b, BST_PREORDER, print );

   printf( "\nTesting erasing the element with key %d\n", 250 );
   BST_Erase( b, 250 );
   BST_Traverse( b, BST_PREORDER, print );

   print_bst( b, "my_bst_with_elements_deleted.dot" );

   BST_Delete( &b );
   assert( b == NULL );
}
