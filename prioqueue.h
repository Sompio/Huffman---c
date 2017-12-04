//Written by Lena Kallin Westin <kallin@cs.umu.se>.
//May be used in the course Datastrukturer och Algoritmer (C) and DV2: Algoritmer och
// problemlösning at Umeå University.
//Usage exept those listed above requires permission by the author.

#include "list_2cell.h"

/*
Datatypen Priokö enligt definitionen på sidan 293 i Lars-Erik Janlert,
Torbjörn Wiberg Datatyper och algoritmer 2., [rev.] uppl.,Lund,
Studentlitteratur, 2000, x, 387 s. ISBN 91-44-01364-7

Implementerar priokö med hjälp av en datatypen lista

Som standard så är användaren av datatypen ansvarig för att avallokera
minnet för datavärdena. Genom att anropa pqueue_setMemHandler och till denna
skicka med en funktion för att avallokera ett dataobjekt så kommer kön
ta över ansvaret för minneshanteringen och automatiskt avallokera värdena
då de tas bort från kön. OBS i detta fall så måste användaren kopiera
värdena om de ska finnas kvar efter att ha tagits bort från kön.
*/

#ifndef __PRIOQUEUE_H
#define __PRIOQUEUE_H  
#include <stdlib.h>
/* Type for values in the prioqueue */
#ifndef __VALUE
#define __VALUE
typedef void *VALUE;
#endif

/* Type for function comparing two values (see create for details)*/
#ifndef __COMPAREFUNC
#define __COMPAREFUNC
typedef int CompareFunction(VALUE,VALUE);
#endif

/*Types for memory deallocation functions */
#ifndef __MEMFREEDATAFUNC
#define __MEMFREEDATAFUNC
typedef void memFreeFunc(VALUE);
#endif

typedef struct MyPQ {
	list *pq;
	CompareFunction *cf;
} MyPQ;

typedef MyPQ pqueue;

/*
Syfte: Skapa en ny priokö
Parametrar: compare_function - Pointer to a function that is called for comparing
                     two priorities. The function should return 0 if the left
                     parameter has lower priority than the right parameter and >0 
                     otherwize
Returvärde: Den nyskapade priokö (pqueue *)
Kommentarer: Då man använt priokön färdigt så måste minnet för priokön
             avallokeras via funktionen pqueue_free
*/
pqueue *pqueue_empty(CompareFunction *compare_function);

/*
Syfte: Installera en minneshanterare för priokön så att den kan ta över
       ansvaret för att avallokera minnet för värdena då de ej finns kvar
       i priokön mer.
Parametrar: q - priokön (pqueue *)
            f - en funktionspekare till en funktion som tar in ett värde
                av typen data (void pekare) och avallokerar minnet för värdet
                den pekar på (ett av den typ av värden som lagras i priokön).
Kommentarer: Bör anropas direkt efter att priokön skapats om funktionaliteten
             ska användas. priokön funkar även utan att denna funktion anropas,
             men det är då upp till användaren av datatypen att avallokera allt
             minne för datat som lagras i priokön.
*/
void pqueue_setMemHandler(pqueue *q, memFreeFunc *f);

/*
Syfte: Ta bort första värdet i priokön
Parametrar: q - priokön (pqueue *)
Kommentarer: odefinierad för en tom priokö
*/
void pqueue_delete_first(pqueue *q);

/*
Syfte: Hämtar första värdet i priokön
Parametrar: q - priokön (pqueue *)
Returvärde: värdet som fanns längst ram i priokön
Kommentarer: ej definierad för tom priokö.
*/
data pqueue_inspect_first(pqueue *q);

/*
Syfte: Lägga till ett värde på rätt ställe utifrån sin prioritet i priokön
Parametrar: q - priokön (pqueue *)
            d- värdet som ska läggas till (data)
Kommentarer:
*/
void pqueue_insert(pqueue *q,data d);

/*
Syfte: Konrollera om priokön är tom
Parametrar: q - priokön (pqueue *)
Returvärde: true om priokön är tom false annars
Kommentarer:
*/
bool pqueue_isEmpty(pqueue *q);

/*
Syfte: Avallokera minne för priokön.
Parametrar: q - priokön (pqueue *)
Kommentarer: Minne för ev värden i priokön avallokeras bara om en funktion
             för detta är satt via pqueue_setMemFunc. Annars är det användarens
             uppgift att avallokera minnet för alla värden i priokön.
*/
void pqueue_free(pqueue *q);

#endif