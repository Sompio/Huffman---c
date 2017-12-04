
#include <stdio.h>
#include <string.h>
#include "prioqueue.h"
#include "tree_3cell.h"
#include "bitset.h"

/*This struct is used as labels in the Huffmantrees nodes.
 * It contains a frequency count and a char representation for the leafs*/
typedef struct{
    int frequency;
    unsigned char chr;
}character;

int LessThan(VALUE a, VALUE b);
void findSymbol(binary_tree *tree, binaryTree_pos pos, int path[],
                int path_len, bitset *bitarr[]);
void fileencode(FILE *read, FILE *write, bitset *bitarr[]);
void filedecode(FILE *read, FILE *write, binary_tree *tree);
int errorUse(void);

int main(int argc, char **argv)
{
    /*Making sure there are a correct number of arguments and a correct
    operationflag. Failiure results in usagehelp-output and exit*/
    if (argc != 5){
        
		return errorUse();
    }
    
    char encode[8];
	char decode[8];
    int operation = 0;
    
    strcpy(encode, "-encode");
	strcpy(decode, "-decode");
    
	if (!strcmp(argv[1], encode)){
        
		operation = 1;
        
	} else if(!strcmp(argv[1], decode)){
        
		operation = 2;
	}
    
    if (operation < 1 || operation > 2){
        
        return errorUse();
    }
    
	FILE *read;
    FILE *read2;
    FILE *write;
    /*making sure files to be opened exists while opening them*/
    if((read = fopen(argv[2], "r")) == NULL){
        
        printf("\nFile %s not found!\n", argv[2]);
        return 0;
    }
    if((read2 = fopen(argv[3], "r")) == NULL){
        
        printf("\nFile %s not found!\n", argv[3]);
        return 0;
    }
    write = fopen(argv[4], "w");
    
    pqueue *pq = pqueue_empty (LessThan);
    bitset *bitarr[256];
    binary_tree* tree;
    
    int ascii;
    int freq[256];
    int path[50];
    
    /*Set all the possible characters to at least 1 to prevent problems with the
    analysis-file not containing all the characters of the in-file. Existing
    characters will recieve 1 more weight. */
    for(int i = 0; i < 256; i++){
        
        freq[i] = 1;
    }
    
    /*read in the frequencyanalysisfile and store the number of chars at their
    ascii-code index.*/
    while((ascii = fgetc(read)) != EOF) {
        
        freq[ascii]++;
    }
    /*adding 1 to EOT*/
    freq[4]++;

    fclose(read);
    /*Create a tree for each character with an appropriate label.
     * The trees consists of only leafs and are queued according to weight*/
    for(int i = 0; i < 256; i++){
        
        tree = binaryTree_create();
        binaryTree_setMemHandler(tree, free);
        
        binaryTree_pos rootpos = binaryTree_root(tree);
            
        character *temp = malloc(sizeof(character));
        temp->chr = i;
        temp->frequency = freq[i];
            
        binaryTree_setLabel(tree, (void*)temp, rootpos);
        pqueue_insert(pq, tree);
            
    }
    /*Take from the queue the two first trees, wich are the 2 with the least 
     * weight, and make them left and right child to a new parent. 
     * The new parent have the combined weight of the 2 children. 
     * Repeat this until only 1 tree remains wich is the Huffman-tree*/
    while(!pqueue_isEmpty(pq)) {
        
        character *plabel = malloc(sizeof(character));
    
        binary_tree *top = binaryTree_create();
        binaryTree_setMemHandler(top, free);
        
        binary_tree *a = pqueue_inspect_first(pq);
        pqueue_delete_first(pq);
        binary_tree *b = pqueue_inspect_first(pq);
        pqueue_delete_first(pq);
    
        binaryTree_pos rootpos = binaryTree_root(top);
        
        /*Adding the weights of the 2 children to the new parentnode*/
        plabel->frequency = ((character*)(binaryTree_inspectLabel
                                         (a, binaryTree_root(a))))->frequency + 
                            ((character*)(binaryTree_inspectLabel
                                         (b, binaryTree_root(a))))->frequency;
                                         
        /*Connecting together the new tree*/
        rootpos->leftChild = binaryTree_root(a);
        rootpos->rightChild = binaryTree_root(b);
        binaryTree_root(a)->parent = rootpos;
        binaryTree_root(b)->parent = rootpos;
        
        binaryTree_setLabel(top, (void*)plabel, rootpos);
        
        free(a);
        free(b);
        
        if(!pqueue_isEmpty(pq)) {
            
            pqueue_insert(pq, top);
        }
        if(pqueue_isEmpty(pq)) {

            tree = top;
        }
    }
    
    findSymbol(tree, binaryTree_root(tree), path, 0, bitarr);
    
    switch (operation) {
        
        case 1:
            
            fileencode(read2, write, bitarr);
            
            long size1 = ftell(read2);
            long size2 = ftell(write);
            printf("%ld bytes read from %s.\n", size1, argv[3]);
            printf("%ld bytes used in encoded form.\n", size2);
            
            break;
        
        case 2:
        
            filedecode(read2, write, tree);
            break;
            
        default:
        
            return errorUse();
            
    }
    for(int i = 0; i < 256;i++){
        
        bitset_free(bitarr[i]);
    }
    
    binaryTree_free(tree);
    pqueue_free(pq);
    
    fclose(read2);
    fclose(write);
    
    return 0;
}

void findSymbol(binary_tree *tree, binaryTree_pos pos, 
                int path[], int path_len, bitset *bitarr[]){
    
    /*Recursively traverse the tree until a leaf is reached, saving the 
     * binary sequence taken on the way there in a bitset. 
     * the bitset is saved in the bitset array at the acsii-code index.
     * (taken from leaf-label)*/
    if(binaryTree_hasLeftChild(tree, pos)){

        path[path_len] = 0;
        /*add 1 to the length of the path and start over with leftchild as root.
         * saving 0 in the patharray*/
        findSymbol(tree, binaryTree_leftChild(tree, pos), 
                   path, path_len+1, bitarr);
    }

    if(binaryTree_hasRightChild(tree, pos)){
        
        path[path_len] = 1;
        /*save 1 for rightchild in the patharray*/
        findSymbol(tree, binaryTree_rightChild(tree, pos), 
                   path, path_len+1, bitarr);
    }
    
    if(!binaryTree_hasLeftChild(tree, pos) &&
       !binaryTree_hasRightChild(tree, pos)){
        
        /*A leaf is reached at these conditions and the patharray can be saved
        into a bitset and sent to the bitsetarray*/
        bitset *traverse = bitset_empty();
        unsigned char symbol = ((character*)(binaryTree_inspectLabel
                                            (tree, pos)))->chr;
            
        for(int i = 0; i<path_len; i++) {
            
            bitset_setBitValue(traverse, i, path[i]);

        }
        
        bitarr[(int)symbol] = traverse;
    }
}

void fileencode(FILE *read, FILE *write, bitset *bitarr[]){
    
    int ascii;
    int offset = 0;
    char *buffer;
    bitset *temp;
    bitset *complete = bitset_empty();
    
    /*For each character in the infile, that characters bitset is copied into
     * a large bitset containing the entire file in binary code.*/
    while((ascii = fgetc(read)) != EOF){
        
        temp = bitarr[ascii];
        
        for(int i = 0; i < bitset_size(temp); i++){
            
            bitset_setBitValue(complete, offset, bitset_memberOf(temp, i));
            offset++;
        }
    }
    
    /*Adds the EOT-character at the end*/
    for(int i = 0; i < bitset_size(bitarr[4]); i++){ 
        
        bitset_setBitValue(complete, offset, bitset_memberOf(bitarr[4], i));
        offset++;
    }
    
    /*Convert the bitset into a byte array and write the chars to the outfile*/
    buffer = toByteArray(complete);

    for(int i=0; i < bitset_size(complete) / 8; i++) {
        
        fputc((unsigned char)buffer[i], write);
    }
    
    free(buffer);
    bitset_free(complete);
}

void filedecode(FILE *read, FILE *write, binary_tree *tree){
    
    int index;
	int size;
	int traverse = 0;
	unsigned char symbol;
    bool *decode;
	binaryTree_pos pos = binaryTree_root(tree);
	
	/* Get all the characters from the text into an array
     * by first getting the length of the text to set the array*/
    fseek(read, 0, SEEK_END);
	size = ftell(read);
	fseek(read, 0, SEEK_SET);
	int textcode[size];
	
	for (index = 0 ; index < size; index++){
        /*fill the array with binary representations for each char*/
		textcode[index] = fgetc(read);
	}
    /*Fill an array with binary code from the infile. The boolarray is 
     * inizialized with only 0's*/
    decode = (bool*)calloc(size * 8,sizeof(bool));
    
    for (index = 0; index < size; index++){
        
        for (int i = 0; i < 8; i++){
            /*Replace with 1 in the boolarray if the char in the infile
             * contains 1, for each bit in the current index*/
            decode[index * 8 + i] = (textcode[index] >> i) & 1;
        }
    }
    
	/*Traverse the binarytree using the binary sequence from the decode-array*/
	while(traverse < size * 8){
        
		if(binaryTree_hasLeftChild(tree, pos) || 
           binaryTree_hasRightChild(tree, pos)){
            
			if(decode[traverse] == 0){
                
				pos = binaryTree_leftChild(tree, pos);
				traverse++;
			}
			else if(decode[traverse] == 1){
                
				pos = binaryTree_rightChild(tree, pos);
				traverse++;
			}
		}
		else{
            /*Get the symbol from the leaf and put it in the out-text*/
			symbol = ((character*)(binaryTree_inspectLabel(tree, pos)))->chr;
            
            if(symbol == '\4'){
                
                break;
            }
            
			fputc(symbol, write);
			pos = binaryTree_root(tree);
            
		}
	}
	free(decode);
	printf("File decoded successfully.\n");
}

/*Comparefunction for the priorityqueue*/
int LessThan(VALUE a, VALUE b){
    /*Makes VALUE a and b compare 2 binarytrees rootfrequencies*/
    return ((character*)(binaryTree_inspectLabel
                        (a, binaryTree_root(a))))->frequency <
           ((character*)(binaryTree_inspectLabel
                        (b, binaryTree_root(b))))->frequency;
}

int errorUse(void){
    
	printf("\nUSAGE:\nhuffman [OPTION] [FILE0] [FILE1] [FILE2]");
	printf("\nOptions:");
    printf("\n-encode encodes FILE1 acording to the frequence ");
    printf("analysis done on FILE0.");
	printf(" Stores the result in FILE2");
	printf("\n-decode decodes FILE1 acording to the frequence ");
    printf("analysis done on FILE0.");
	printf(" Stores the result in FILE2\n");
    
	return 0;
}