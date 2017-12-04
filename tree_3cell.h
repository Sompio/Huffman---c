//Written by Johan Eliasson <johane@cs.umu.se>.
//May be used in the course Datastrukturer och Algoritmer (C) at Umeå University.
//Usage exept those listed above requires permission by the author.

#ifndef __BINARY_TREE_H
#define __BINARY_TREE_H

#include <stdbool.h>

#ifndef __DATA
#define __DATA
typedef void *data;
#endif

#ifndef __MEMFREEDATAFUNC
#define __MEMFREEDATAFUNC
typedef void memFreeFunc(data);
#endif

typedef struct tree_cell {
    struct tree_cell *parent;
    struct tree_cell *leftChild;
    struct tree_cell *rightChild;
    data label;
} node;

typedef node * binaryTree_pos;

typedef struct {
    binaryTree_pos root;
    memFreeFunc *freeFunc;
} binary_tree;


/*
Syfte: Skapa ett nytt binärt träd med en rotnod
Returvärde: pekare till det nyskapade trädet
Kommentarer:
*/
binary_tree *binaryTree_create();

/*
Syfte: Installera en minneshanterare för trädet så att den kan ta över
       ansvaret för att avallokera minnet för värdena då de ej finns kvar
       i trädet mer.
Parametrar: t - trädet
            f - en funktionspekare till en funktion som tar in ett värde
                av typen data (void pekare) och avallokerar minnet för värdet
                den pekar på (ett av den typ av värden som lagras i trädet).
Kommentarer: Bör anropas direkt efter att trädet skapats om funktionaliteten
             ska användas. Trädet funkar även utan att denna funktion anropas,
             men det är då upp till användaren av datatypen att avallokera allt
             minne för datat som lagras i trädet.
*/
void binaryTree_setMemHandler(binary_tree *l, memFreeFunc *f);

/*
Syfte: Hämta positionen för rotnoden
Parametrar: trädet
Returvärde: positionen för rotnden
Kommentarer:
*/
binaryTree_pos binaryTree_root(binary_tree *tree);

/*
Syfte: Kontrollera om en nod i trädet har ett vänsterbarn
Parametrar: tree - trädet
            n - positionen som man vill undersöka om den har ett vänsterbarn
Returvärde: true om et vänsterbarn fanns, false annars
Kommentarer:
*/
bool binaryTree_hasLeftChild(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Kontrollera om ett högerbarn finns till en given nod
Parametrar: tree - trädet
            n - positionen som man vill undersöka om den har ett högerbarn
Returvärde: true om et högerbarn fanns, false annars
Kommentarer:
*/
bool binaryTree_hasRightChild(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Hämta högra barnets position för en given nod
Parametrar: tree - trädet
            n - positionen vars högerbarn man vill hämta
Returvärde:  positionen för högerbarnet
Kommentarer: Ej definierad om ett högerbarn inte finns
*/
binaryTree_pos binaryTree_rightChild(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Hämta vänstra barnets position för en given nod
Parametrar: tree - trädet
            n - positionen vars vänsterbarn man vill hämta
Returvärde: positionen för vänsterbarnet
Kommentarer: Ej definierad om ett vänsterbarn inte finn
*/
binaryTree_pos binaryTree_leftChild(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Hämta positionen för föräldern
Parametrar: tree - trädet
            n - positionen vars förälder man vill hämta
Returvärde: positionen för föräldern
Kommentarer: Ej definierad för rotnoden
*/
binaryTree_pos binaryTree_parent(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Hämta en nods etikett
Parametrar: tree - trädet
            n - positionen för noden
Returvärde: värdet på etiketten
Kommentarer:
*/
data binaryTree_inspectLabel(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Kontrollera om en etikett finns i noden
Parametrar: tree - trädet
            n - positionen för noden
Returvärde: true om noden har en ettikett annars false
Kommentarer:
*/
bool binaryTree_hasLabel(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Sätta en etikett för en nod
Parametrar: tree - trädet
            label - etiketten för noden
            n - positionen för noden
Kommentarer:
*/
void binaryTree_setLabel(binary_tree *tree,data label,binaryTree_pos n);

/*
Syfte: Sätt in ett nytt barn till höger om en nod
Parametrar: tree - trädet
            n - positionen för föräldranoden
Returvärde: positionen för den nya noden
Kommentarer: Om ett högerbarn redan fanns kommer detta tas bort.
*/
binaryTree_pos binaryTree_insertRight(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Sätt in ett nytt barn till höger om en nod
Parametrar: tree - trädet
            n - positionen för föräldranoden
Returvärde: positionen för den nya noden
Kommentarer: Om ett vänsterbarn redan fanns kommer detta tas bort.
*/
binaryTree_pos binaryTree_insertLeft(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Ta bort en nod ur trädet
Parametrar: tree - trädet
            n - positionen för noden
Returvärde: position för föräldranoden till noden som togs bort
Kommentarer: Om noden som ska tas bort har barn kommer även dessa tas bort
*/
binaryTree_pos binaryTree_deleteNode(binary_tree *tree,binaryTree_pos n);

/*
Syfte: Avallokera minnet för trädet
Parametrar: tree - trädet
Kommentarer: efter anropet så är tree ej längre definierat
*/
void binaryTree_free(binary_tree *tree);

#endif