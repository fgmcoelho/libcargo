#include <stdlib.h>

// TODO: REMOVE LATER
#include <stdio.h>
#include <string.h>

#include "huffman.h"
#include "heap.h"

/*
    Huffman Tree internal structs.
*/

struct huffman_node_st{
    unsigned weight;
    unsigned char byte;
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
    Huffman encoder/decoder.
*/

struct huffman_table_st {
    unsigned count, size;
    unsigned char code[4];
};

typedef struct huffman_table_st huffmanTable;

struct huffman_st {
    huffmanTable table[__HUFFMAN_TABLE_SIZE];
    huffmanNode* root;
    int finished;
};

static void fillHuffmanByNode(huffman* h, huffmanNode* node, unsigned char* code, unsigned size){
    printf("Recursive call %d!\n", size);
    if (node->left == NULL && node->right == NULL){
        h->table[node->byte].code[0] = code[0];
        h->table[node->byte].code[1] = code[1];
        h->table[node->byte].code[2] = code[2];
        h->table[node->byte].code[3] = code[3];
        h->table[node->byte].size = size;
        printf("Setting final node %c to %x %x %x %x.\n", node->byte,
            h->table[node->byte].code[3],
            h->table[node->byte].code[2],
            h->table[node->byte].code[1],
            h->table[node->byte].code[0]
        );
    }
    else{
        fillHuffmanByNode(h, node->left, code, size + 1);
        unsigned codeIndex = size/8;
        unsigned codeByte = 7 - (size%8);
        code[codeIndex] |= 1 << codeByte;
        //printf("Setting code index %u to %x.\n", codeIndex, code[codeIndex]);
        fillHuffmanByNode(h, node->right, code, size + 1);
    }
}

static void fillHuffmanTable(huffman* h){
    printf("Filling huffman table!\n");
    if (h == NULL || h->root == NULL || h->finished == 0){
        printf("Error filling huffman table!\n");
        return;
    }
    unsigned char code[4];
    code[0] = 0;
    code[1] = 0;
    code[2] = 0;
    code[3] = 0;
    fillHuffmanByNode(h, h->root, code, 0);
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
        newHuffman->table[i].code[0] = 0;
        newHuffman->table[i].code[1] = 0;
        newHuffman->table[i].code[2] = 0;
        newHuffman->table[i].code[3] = 0;
    }
    newHuffman->root = NULL;
    newHuffman->finished = 0;

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
    printf("Starting the Queue search! %u\n", heapGetSize(firstQueue));
    while(heapGetSize(firstQueue) + heapGetSize(secondQueue) > 1){
        printf("Seeking the queues %u %u\n", heapGetSize(firstQueue), heapGetSize(secondQueue));
        huffmanNode* firstTry = heapGetFirstElement(firstQueue);
        if (firstTry == NULL){
            first = heapPopFirstElement(secondQueue);
            second = heapPopFirstElement(secondQueue);
        }
        else{
            huffmanNode* secondTry = heapGetFirstElement(secondQueue);
            if (secondTry == NULL){
                first = firstTry;
                second = heapPopFirstElement(firstQueue);
            }
            else{
                if (firstTry->weight < secondTry->weight){
                    first = heapPopFirstElement(firstQueue);
                    huffmanNode* thirdTry = heapGetFirstElement(firstQueue);
                    if (secondTry->weight < thirdTry->weight){
                        second = heapPopFirstElement(secondQueue);
                    }
                    else {
                        second = heapPopFirstElement(firstQueue);
                    }
                }
                else{
                    first = heapPopFirstElement(secondQueue);
                    huffmanNode* thirdTry = heapGetFirstElement(secondQueue);
                    if (firstTry->weight < thirdTry->weight){
                        second = heapPopFirstElement(firstQueue);
                    }
                    else{
                        second = heapGetFirstElement(secondQueue);
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

unsigned char* huffmanEncode(huffman* h, unsigned char* data, unsigned dataSize, unsigned* resultSize){
    if (h == NULL || h->finished == 0 || data == NULL || dataSize == 0){
        return NULL;
    }

    // TODO: Add an max code control, alloc data * max code.
    unsigned char* resultBuffer = malloc(dataSize);
    if (resultBuffer == NULL){
        return NULL;
    }

    unsigned i, j = 0;;
    int bitIndex = 7;
    for (i = 0; i < dataSize; i++){
        if (h->table[data[i]].count == 0){
            free(resultBuffer);
            return NULL;
        }
        int size = h->table[data[i]].size;
        while (size > 0){
            resultBuffer[j] |= 1 << (bitIndex);
            bitIndex = (bitIndex - 1) % 8;
            if (bitIndex == 7){
                j++;
            }
            size--;
        }
    }

    resultBuffer = realloc(resultBuffer, j);
    *resultSize = j;
    return resultBuffer;
}

unsigned char* huffmanDecode(huffman* h, unsigned char* data, unsigned dataSize, unsigned* resultSize){
    if (h == NULL || h->finished == 0 || data == NULL || dataSize == 0){
        return NULL;
    }


}

static void printTree(huffmanNode* node, int level){
    if (node->left == NULL && node->right == NULL){
        int i;
        for (i = 0; i < level; i++){
            printf("-");
        }
        printf("Node data: \"%c\" \"%02x\"\n", (unsigned char)node->byte, (unsigned char)node->byte);
    }
    else{
        //printf("Connect Node!\n");
        printTree(node->left, level + 1);
        printTree(node->right, level + 1);
    }

}

static void printHuffman(huffman* h){
    printf("\n\nPRINTING TREE!\n\n");
    printTree(h->root, 0);
    printf("\n\nPRINTING TABLE!\n\n");
    int i;
    for (i = 0; i < __HUFFMAN_TABLE_SIZE; i++){
        if (h->table[i].count != 0){
            printf("%c -> Count %04d | code: %02x %02x %02x %02x\n",
                (unsigned char)i, h->table[i].count,
                (unsigned char)h->table[i].code[3],
                (unsigned char)h->table[i].code[2],
                (unsigned char)h->table[i].code[1],
                (unsigned char)h->table[i].code[0]);
        }
    }

}

int main(){
    char testString[] = "j'aime aller sur le bord de l'eau les jeudis ou les jours impairs";
    huffman* huff = huffmanCreate();
    if (huff == NULL){
        printf("Error creating the huffman struct!\n");
        return 1;
    }
    if(huffmanAddElements(huff, testString, strlen(testString)) == 0){
        printf("Error adding the elements!\n");
        return 1;
    }
    if(huffmanFinish(huff) == 0){
        printf("Error finishing the huffman struct!\n");
        return 1;
    }
    printHuffman(huff);
    huffmanClear(&huff);

    return 0;
}
