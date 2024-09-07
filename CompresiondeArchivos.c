#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

// Estructura para los nodos del árbol de Huffman
typedef struct MinHeapNode {
    unsigned char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
} MinHeapNode;

// Estructura para la cola de prioridad
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    MinHeapNode **array;
} MinHeap;

// Crea un nuevo nodo del árbol de Huffman
MinHeapNode* crearNodo(unsigned char data, unsigned freq) {
    MinHeapNode* nodo = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    nodo->left = nodo->right = NULL;
    nodo->data = data;
    nodo->freq = freq;
    return nodo;
}

// Crea una cola de prioridad de tamaño `capacity`
MinHeap* crearMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode*));
    return minHeap;
}

// Intercambia dos nodos de la cola de prioridad
void intercambiarNodos(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify para mantener el árbol en la forma de un heap mínimo
void heapify(MinHeap* minHeap, int idx) {
    int menor = idx;
    int izquierda = 2 * idx + 1;
    int derecha = 2 * idx + 2;

    if (izquierda < minHeap->size && minHeap->array[izquierda]->freq < minHeap->array[menor]->freq) {
        menor = izquierda;
    }
    if (derecha < minHeap->size && minHeap->array[derecha]->freq < minHeap->array[menor]->freq) {
        menor = derecha;
    }

    if (menor != idx) {
        intercambiarNodos(&minHeap->array[menor], &minHeap->array[idx]);
        heapify(minHeap, menor);
    }
}

// Verifica si la cola de prioridad tiene un solo nodo
int esTamanoUno(MinHeap* minHeap) {
    return minHeap->size == 1;
}

// Extrae el nodo con menor frecuencia de la cola de prioridad
MinHeapNode* extraerMin(MinHeap* minHeap) {
    MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    heapify(minHeap, 0);
    return temp;
}

// Inserta un nuevo nodo en la cola de prioridad
void insertarMinHeap(MinHeap* minHeap, MinHeapNode* nodo) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && nodo->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = nodo;
}

// Construye el árbol de Huffman
MinHeapNode* construirHuffmanTree(unsigned char data[], int freq[], int size) {
    MinHeapNode *left, *right, *top;
    MinHeap* minHeap = crearMinHeap(size);

    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = crearNodo(data[i], freq[i]);
    }
    minHeap->size = size;
    heapify(minHeap, 0);

    while (!esTamanoUno(minHeap)) {
        left = extraerMin(minHeap);
        right = extraerMin(minHeap);
        top = crearNodo('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertarMinHeap(minHeap, top);
    }
    return extraerMin(minHeap);
}

// Imprime el código de Huffman recursivamente
void imprimirCodigo(MinHeapNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        imprimirCodigo(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        imprimirCodigo(root->right, arr, top + 1);
    }

    if (!(root->left) && !(root->right)) {
        printf("%c: ", root->data);
        for (int i = 0; i < top; ++i) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}

int main() {
    // Ejemplo de frecuencias de caracteres
    unsigned char datos[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    int frecuencia[] = {5, 9, 12, 13, 16, 45, 30};
    int tamano = sizeof(datos) / sizeof(datos[0]);

    MinHeapNode* raiz = construirHuffmanTree(datos, frecuencia, tamano);
    
    int arr[MAX_TREE_HT];
    int top = 0;

    printf("Codigos de Huffman:\n");
    imprimirCodigo(raiz, arr, top);

    return 0;
}
