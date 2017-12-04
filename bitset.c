/*
 * Copyright 2015 Johan Eliasson (johane@cs.umu.se). TillÃ¥telse ges fÃ¶r anvÃ¤ndning pÃ¥ kurserna
 * i Datastrukturer och algoritmer vid UmeÃ¥ Universitet (inklusive DV1 och DV2). All annan anvÃ¤ndning krÃ¤ver 
 * fÃ¶rfattarens tillstÃ¥nd.
 */

/* Datatypen svarar mot en mÃ¤ngd (lexikon) av bitar. Inga av de matematiska mÃ¤ngdoperationerna finns med 
 * datatypen. NÃ¥gra extra operationer har lagts till fÃ¶r att datatypen ska bli lÃ¤ttare att anvÃ¤nda */

#include "bitset.h"
#include <stdlib.h>

//Create a new empty bitset of length 0.
bitset *bitset_empty() {
    bitset *b;
    b=malloc(sizeof(bitset));
    b->length=0;
    b->capacity=0;
    b->array=NULL;
    return b;
}

//Deallocate all memory used by a bitset
void bitset_free(bitset *b){
    free(b->array);
    free(b);
}



//Set bit bitno to value (true=1, false=0) in the bitset b.
//bitNo should be >= 0
//If bitNo >= bitset_size(b) the bitset will be extended up to that bit. Bits from bitset_size(b)
//to bitNo will be set to zero/false.
void bitset_setBitValue(bitset *b,int bitNo,bool value) {
    if(bitNo>=bitset_size(b)) {
        b->length=bitNo+1;
        while(b->capacity<b->length) {
            b->array=realloc(b->array,b->capacity/8+1);
            b->capacity+=8;
        }
    }
    int byteNo=bitNo/8;
    int bit=bitNo%8;
    char theByte=b->array[byteNo];
    if(value) {
        theByte=(theByte|1<<bit);
    }
    else {
        theByte=theByte& ~(1<<bit);
    }
    b->array[byteNo]=theByte;
}

//Get the value of bit bitNo in the bitset b. Undefined behaviour for bitNO >= bitset_size(b).
bool bitset_memberOf(bitset *b,int bitNo) {
    int byteNo=bitNo/8;
    int bit=bitNo%8;
    char theByte=b->array[byteNo];
    return ((theByte)&(1<<bit)) > 0;
}

//Convert this bitset to a byte array.
//The resulting array will be large enough to contain all bits up to bitset_size(b). if bitset_size is not
//a multiple of 8 bits the final byte is padded with bits of value 0.
//Memory is dynamicly allocated for the array. The user is responsible for deallocating the memory.
char *toByteArray(bitset *b) {
    char *res=calloc(b->capacity/8, sizeof(char));
    for (int i=0; i<b->capacity/8; i++) {
        res[i]=b->array[i];
    }
    return res;
    
}

//Returns the size of this bitset
int bitset_size(bitset *b) {
    return b->length;
}
