//Written by Lena Kallin Westin <kallin@cs.umu.se>.
//May be used in the course Datastrukturer och Algoritmer (C) and DV2: Algoritmer och
// problemlösning at Umeå University.
//Usage exept those listed above requires permission by the author.

#include <stdio.h>
#include "prioqueue.h"

/*
priokö implementerad med hjälp av riktad lista. Kräver implementation av lista som överenstämmer med gränssninttet för listan som används på kursen för att fungera.
*/

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
pqueue *pqueue_empty(CompareFunction *compare_function){
    MyPQ *prioq = calloc(sizeof (MyPQ),1);
    if (!prioq)
        return NULL;
    prioq->pq=list_empty();
    prioq->cf = compare_function;
    return (pqueue *)prioq;
}

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
void pqueue_setMemHandler(pqueue *q, memFreeFunc *f) {
    MyPQ *prioq = (MyPQ*)q;
    list_setMemHandler(prioq->pq,f);
}

/*
Syfte: Ta bort första värdet i priokön
Parametrar: q - priokön (pqueue *)
Kommentarer: odefinierad för en tom priokö
*/
void pqueue_delete_first(pqueue *q){
    MyPQ *prioq = (MyPQ*)q;
    list_remove(prioq->pq,list_first(prioq->pq));
}

/*
Syfte: Hämtar första värdet i priokön
Parametrar: q - priokön (pqueue *)
Returvärde: värdet som fanns längst ram i priokön
Kommentarer: ej definierad för tom priokö.
*/
data pqueue_inspect_first(pqueue *q){
    MyPQ *prioq = (MyPQ*)q;
    return list_inspect(prioq->pq,list_first(prioq->pq));
}

/*
Syfte: Lägga till ett värde på rätt ställe utifrån sin prioritet i priokön
Parametrar: q - priokön (pqueue *)
            d- värdet som ska läggas till (data)
Kommentarer:
*/
void pqueue_insert(pqueue *q,data d){
    MyPQ *prioq = (MyPQ*)q;
    int placed = 0;
    list_position pos =list_first(prioq->pq);

    if (list_isEmpty(prioq->pq)){
        list_insert(prioq->pq, d, list_first(prioq->pq));
    }else{
        while(!placed && pos!=list_end(prioq->pq)){
            if(prioq->cf(d, list_inspect(prioq->pq, pos))){
                    placed = 1;
                    list_insert(prioq->pq, d, pos);
            }
            pos = list_next(prioq->pq, pos);
        }
        if (!placed){
            list_insert(prioq->pq, d, pos);
        }
    }
}

/*
Syfte: Konrollera om priokön är tom
Parametrar: q - priokön (pqueue *)
Returvärde: true om priokön är tom false annars
Kommentarer:
*/
bool pqueue_isEmpty(pqueue *q){
    MyPQ *prioq = (MyPQ*)q;
    return list_isEmpty(prioq->pq);
}

/*
Syfte: Avallokera minne för priokön.
Parametrar: q - priokön (pqueue *)
Kommentarer: Minne för ev värden i priokön avallokeras bara om en funktion
             för detta är satt via pqueue_setMemFunc. Annars är det användarens
             uppgift att avallokera minnet för alla värden i priokön.
*/
void pqueue_free(pqueue *q){
    MyPQ *prioq = (MyPQ*)q;
    list_free(prioq->pq);
    free(prioq);
}