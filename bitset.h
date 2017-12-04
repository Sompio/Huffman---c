/*
 * Copyright 2015 Johan Eliasson (johane@cs.umu.se). TillÃ¥telse ges fÃ¶r anvÃ¤ndning pÃ¥ kurserna
 * i Datastrukturer och algoritmer vid UmeÃ¥ Universitet (inklusive DV1 och DV2). All annan anvÃ¤ndning krÃ¤ver
 * fÃ¶rfattarens tillstÃ¥nd.
 */

#ifndef __Huffman__BitSet__
#define __Huffman__BitSet__

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int length;
    int capacity;
    char *array;
} bitset;

//Create a new empty bitset of length 0.
bitset *bitset_empty();

//Set bit bitno to value (true=1, false=0) in the bitset b.
//bitNo should be >= 0
//If bitNo >= bitset_size(b) the bitset will be extended up to that bit. Bits from bitset_size(b)
//to bitNo will be set to zero/false.
void bitset_setBitValue(bitset *b,int bitNo,bool value);

//Get the value of bit bitNo in the bitset b. Undefined behaviour for bitNO >= bitset_size(b).
bool bitset_memberOf(bitset *b,int bitNo);

//Convert this bitset to a byte array.
//The resulting array will be large enough to contain all bits up to bitset_size(b). if bitset_size is not
//a multiple of 8 bits the final byte is padded with bits of value 0.
//Memory is dynamicly allocated for the array. The user is responsible for deallocating the memory.
char *toByteArray(bitset *b);

//Returns the size of this bitset
int bitset_size(bitset *b);

//Deallocate all memory used by a bitset
void bitset_free(bitset *b);

#endif /* defined(__Huffman__BitSet__) */