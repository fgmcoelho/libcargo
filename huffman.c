#include <stdlib.h>

// TODO: REMOVE LATER
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "huffman.h"
#include "heap.h"

/*
    Huffman Tree internal structs.
*/
struct huffman_node_st{
    unsigned weight, size;
    unsigned char byte, code[4];
    struct huffman_node_st *left, *right, *parent;
};

typedef struct huffman_node_st huffmanNode;

static huffmanNode* createHuffmanNode(unsigned weight){
    huffmanNode* newHuffmanNode = malloc(sizeof(huffmanNode));
    if (newHuffmanNode == NULL){
        return NULL;
    }
    newHuffmanNode->weight = weight;
    newHuffmanNode->parent = NULL;

    return newHuffmanNode;
}

static void clearHuffmanNode(huffmanNode* node){
    if (node != NULL){
        if (node->left != NULL){
            clearHuffmanNode(node->left);
        }
        if (node->right != NULL){
            clearHuffmanNode(node->right);
        }
        free(node);
    }
}

/*
    Huffman output
*/
struct huffman_output_st{
    unsigned originalSize, compressedSize;
    unsigned char* buffer;
};

void huffmanOutputClear(huffmanOutput** output){
    if (output != NULL && *output != NULL){
        free((*output)->buffer);
        free(*output);
        *output = NULL;
    }
}

/*
    Huffman encoder/decoder.
*/
struct huffman_table_st {
    unsigned count;
    huffmanNode* node;
};

typedef struct huffman_table_st huffmanTable;

struct huffman_st {
    huffmanTable table[__HUFFMAN_TABLE_SIZE];
    huffmanNode* root;
    int finished, maxSize;
};

static void print_byte(unsigned char byte){
    int i;
    for(i = 7; i >= 0; --i){
        if (byte & (1 << i)){
            printf("1");
        }
        else{
            printf("0");
        }
    }
    printf("\n");
}

static void print_full_code(unsigned char* code, unsigned char byte){
    printf("Code for \"%c\" is: ", byte);
    int i, j;
    for (i = 3; i >= 0; i--){
        for (j = 7; j >= 0; j--){
            if (code[i] & (1 << j)){
                printf("1");
            }
            else{
                printf("0");
            }
        }
    }

    printf("\n");
}


static void print_code(unsigned char* code, int size, unsigned char byte){
    printf("Code for \"%c\" is: ", byte);
    int i, j, first = 1;
    for (i = 31 - size; i > 0; i--){
        printf("-");
    }

    for (i = size/8; i >= 0; i--){
        if (first){
            j = size % 8;
            first = 0;
        }
        else{
            j = 7;
        }
        for (; j >= 0; j--){
            if (code[i] & (1 << j)){
                printf("1");
            }
            else{
                printf("0");
            }
        }
    }
    printf("\n");
}

static int compare(unsigned char* bytes, unsigned char* uncompressed, huffmanOutput* output){
    int i;
    for (i = 0; i < output->originalSize; ++i){
        if (bytes[i] != uncompressed[i]){
            printf("%c different from %c\n", bytes[i], uncompressed[i]);
            return i;
        }
    }
    return 0;
}

static void fillHuffmanByNode(huffman* h, huffmanNode* node, int isLeft){
    // root
    if (node->parent == NULL){
        node->code[3] = node->code[2] = node->code[1] = node->code[0] = 0;
    }
    else{
        if (node->parent == h->root){
            node->size = 0;
        }
        else{
            node->size = node->parent->size + 1;
            if (h->maxSize < node->size){
                h->maxSize = node->size;
            }
        }
        node->code[3] = node->parent->code[3];
        node->code[2] = node->parent->code[2];
        node->code[1] = node->parent->code[1];
        node->code[0] = node->parent->code[0];
        if (isLeft == 0){
            unsigned codeIndex = node->size/8;
            unsigned codeBit = (node->size % 8);
            node->code[codeIndex] |= 1 << codeBit;
        }
    }

    if (node->left == NULL && node->right == NULL){
        h->table[node->byte].node = node;
    }
    else{
        fillHuffmanByNode(h, node->left, 1);
        fillHuffmanByNode(h, node->right, 0);
    }
}

static void fillHuffmanTable(huffman* h){
    printf("Filling huffman table!\n");
    if (h == NULL || h->root == NULL || h->finished == 0){
        printf("Error filling huffman table!\n");
        return;
    }
    fillHuffmanByNode(h, h->root, 0);
}

static int huffmanHeapCmp(void* a, void* b){
	return ((huffmanNode*)a)->weight < ((huffmanNode*)b)->weight;
}

huffman* huffmanCreate(){
    huffman* newHuffman = malloc(sizeof(huffman));
    if (newHuffman == NULL){
        return NULL;
    }
    int i;
    for (i = 0; i < __HUFFMAN_TABLE_SIZE; ++i){
        newHuffman->table[i].count = 0;
        newHuffman->table[i].node = 0;
    }
    newHuffman->root = NULL;
    newHuffman->finished = newHuffman->maxSize = 0;

    return newHuffman;
}

int huffmanAddElement(huffman* h, unsigned char byte){
    if (h == NULL || h->finished == 1){
        return 0;
    }

    h->table[byte].count++;
    return 1;
}

int huffmanAddElements(huffman* h, unsigned char* bytes, unsigned size){
    if (bytes == NULL || size == 0){
        return 0;
    }

    unsigned i;
    for (i = 0; i < size; i++){
        if(huffmanAddElement(h, bytes[i]) == 0){
            return 0;
        }
    }

    return 1;
}

int huffmanFinish(huffman* h){
    if (h == NULL || h->finished == 1){
        return 0;
    }

    heap* firstQueue = heapCreate(huffmanHeapCmp);
    int i;
    for (i = 0; i < __HUFFMAN_TABLE_SIZE; i++){
        if (h->table[i].count > 0){
            huffmanNode* node = createHuffmanNode(h->table[i].count);
            if (node == NULL){
                heapClear(&firstQueue, free);
                return 0;
            }
            node->byte = (unsigned char)i;
            node->left = node->right = NULL;
            heapInsert(firstQueue, node);
        }
    }

    heap* secondQueue = heapCreate(huffmanHeapCmp);
    huffmanNode* first, *second;
    while(heapGetSize(firstQueue) + heapGetSize(secondQueue) > 1){
        huffmanNode* firstTry = heapGetFirstElement(firstQueue);
        if (firstTry == NULL){
            first = heapPopFirstElement(secondQueue);
            second = heapPopFirstElement(secondQueue);
        }
        else{
            huffmanNode* secondTry = heapGetFirstElement(secondQueue);
            if (secondTry == NULL){
                first = heapPopFirstElement(firstQueue);
                second = heapPopFirstElement(firstQueue);
            }
            else{
                if (firstTry->weight < secondTry->weight){
                    first = heapPopFirstElement(firstQueue);
                    huffmanNode* thirdTry = heapGetFirstElement(firstQueue);
                    if (thirdTry == NULL){
                        second = heapPopFirstElement(secondQueue);
                    }
                    else{
                        if (secondTry->weight < thirdTry->weight){
                            second = heapPopFirstElement(secondQueue);
                        }
                        else {
                            second = heapPopFirstElement(firstQueue);
                        }
                    }
                }
                else{
                    first = heapPopFirstElement(secondQueue);
                    huffmanNode* thirdTry = heapGetFirstElement(secondQueue);
                    if (thirdTry == NULL){
                        second = heapPopFirstElement(firstQueue);
                    }
                    else{
                        if (firstTry->weight < thirdTry->weight){
                            second = heapPopFirstElement(firstQueue);
                        }
                        else{
                            second = heapPopFirstElement(secondQueue);
                        }
                    }
                }
            }
        }
        huffmanNode* node = createHuffmanNode(first->weight + second->weight);
        if (node == NULL){
            heapClear(&firstQueue, free);
            heapClear(&secondQueue, free);
            return 0;
        }
        first->parent = second->parent = node;
        node->left = first;
        node->right = second;
        heapInsert(secondQueue, node);
    }
    if (heapGetSize(firstQueue) == 1){
        h->root = heapPopFirstElement(firstQueue);
    }
    else{
        h->root = heapPopFirstElement(secondQueue);
    }
    h->finished = 1;
    heapClear(&firstQueue, NULL);
    heapClear(&secondQueue, NULL);

    fillHuffmanTable(h);

    return 1;
}

void huffmanClear(huffman** h){
    if(h != NULL && *h != NULL){
        clearHuffmanNode((*h)->root);
        free(*h);
    }
}

huffmanOutput* huffmanEncode(huffman* h, unsigned char* data, unsigned dataSize){
    if (h == NULL || h->finished == 0 || data == NULL || dataSize == 0){
        return NULL;
    }

    huffmanOutput* output = malloc(sizeof(huffmanOutput));
    if (output == NULL){
        return NULL;
    }

    unsigned char* resultBuffer = malloc(dataSize * (1 + (h->maxSize/8)));
    if (resultBuffer == NULL){
        free(output);
        return NULL;
    }

    unsigned inputIndex, outputIndex = 0, codeSize;
    int bitIndex = 7;
    resultBuffer[0] = 0;
    for (inputIndex = 0; inputIndex < dataSize; inputIndex++){
        if (h->table[data[inputIndex]].count == 0){
            free(output);
            free(resultBuffer);
            return NULL;
        }
        for (codeSize = 0; codeSize <= h->table[data[inputIndex]].node->size; codeSize++){
            unsigned codeIndex = codeSize/8;
            unsigned codeBit = (codeSize % 8);
            if (h->table[data[inputIndex]].node->code[codeIndex] & (1 << codeBit)){
                resultBuffer[outputIndex] |= 1 << bitIndex;
            }
            if (bitIndex == 0){
                bitIndex = 7;
                outputIndex++;
                resultBuffer[outputIndex] = 0;
            }
            else{
                bitIndex--;
            }
        }
    }

    output->buffer = realloc(resultBuffer, outputIndex + 1);
    if (output->buffer == NULL){
        free(resultBuffer);
        free(output);
        return NULL;
    }

    output->compressedSize = outputIndex;
    output->originalSize = dataSize;

    return output;
}

unsigned char* huffmanDecode(huffman* h, huffmanOutput* output, unsigned* resultSize){
    if (h == NULL || h->finished == 0 || output == NULL || output->compressedSize == 0
            || output->originalSize ==0 || resultSize == NULL){
        return NULL;
    }

    unsigned char* resultBuffer = malloc(output->originalSize);
    if (resultBuffer == NULL){
        return NULL;
    }

    int i = 0, bitIndex = 7, compressedIndex = 0;
    huffmanNode* node = h->root;
    while(i < output->originalSize){
        if (node->left == NULL && node->right == NULL){
            resultBuffer[i] = node->byte;
            i++;
            node = h->root;
        }
        else{
            if (output->buffer[compressedIndex] & (1 << bitIndex)){
                node = node->right;
            }
            else{
                node = node->left;
            }
            if (bitIndex == 0){
                bitIndex = 7;
                compressedIndex++;
            }
            else{
                bitIndex--;
            }
        }
    }

    *resultSize = output->originalSize;
    return resultBuffer;
}

static void printTree(huffmanNode* node, int level){
    if (node->left == NULL && node->right == NULL){
        print_full_code(node->code, node->byte);
    }
    else{
        printTree(node->left, level + 1);
        printTree(node->right, level + 1);
    }

}

static void printHuffman(huffman* h){
    printf("\n\nPRINTING TREE!\n\n");
    printTree(h->root, 0);
}

int main(int argc, const char* argv[]){

    if (argc != 2){
        printf("Error, you need to pass a file as an argument.\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL){
        printf("Error opening file.\n");
        return 1;
    }

    struct stat st;
    stat(argv[1], &st);

    unsigned char* bytes = malloc(st.st_size);
    fread(bytes, st.st_size, 1, fp);
    fclose(fp);


    huffman* huff = huffmanCreate();
    if (huff == NULL){
        printf("Error creating the huffman struct!\n");
        return 1;
    }
    if(huffmanAddElements(huff, bytes, st.st_size) == 0){
        printf("Error adding the elements!\n");
        return 1;
    }
    if(huffmanFinish(huff) == 0){
        printf("Error finishing the huffman struct!\n");
        return 1;
    }
    printHuffman(huff);

    huffmanOutput* output = huffmanEncode(huff, bytes, st.st_size);
    if (output == NULL){
        printf("Error compressing data!");
        return 1;
    }
    unsigned resSize;
    unsigned char* res = huffmanDecode(huff, output, &resSize);
    if (res == NULL){
        printf("Error decompressing data!");
    }
    printf("Successfully compressed the string. From %u bytes to %u bytes.\n",
        output->originalSize, output->compressedSize);
    printf("Testing, is same: %d.\n", compare(bytes, res, output));
    /*printf("Original:\n%s\n", testString);
    printf("Decompressed:\n");
    int i;
    for (i = 0; i < resSize; ++i){
        printf("%c", res[i]);
    }
    printf("\n");*/

    free(res);
    free(bytes);
    huffmanOutputClear(&output);
    huffmanClear(&huff);

    return 0;
}
