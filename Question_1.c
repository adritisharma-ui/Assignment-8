#include <stdio.h>
#include <stdlib.h>
struct Node {
    char ch;
    int freq;
    struct Node *left, *right;
};
struct MinHeap {
    int size;
    int capacity;
    struct Node** array;
};
struct Node* newNode(char ch, int freq) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->ch = ch; node->freq = freq; node->left = node->right = NULL;
    return node;
}
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0; minHeap->capacity = capacity;
    minHeap->array = (struct Node**)malloc(capacity * sizeof(struct Node*));
    return minHeap;
}
void swapNodes(struct Node** a, struct Node** b) {
    struct Node* t = *a; *a = *b; *b = t;
}
void minHeapify(struct MinHeap* heap, int idx) {
    int smallest = idx;
    int l = 2*idx + 1, r = 2*idx + 2;
    if (l < heap->size && heap->array[l]->freq < heap->array[smallest]->freq) smallest = l;
    if (r < heap->size && heap->array[r]->freq < heap->array[smallest]->freq) smallest = r;
    if (smallest != idx) {
        swapNodes(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}
struct Node* extractMin(struct MinHeap* heap) {
    struct Node* temp = heap->array[0];
    heap->array[0] = heap->array[--heap->size];
    minHeapify(heap, 0);
    return temp;
}
void insertMinHeap(struct MinHeap* heap, struct Node* node) {
    int i = heap->size++;
    while (i && node->freq < heap->array[(i-1)/2]->freq) {
        heap->array[i] = heap->array[(i-1)/2];
        i = (i-1)/2;
    }
    heap->array[i] = node;
}
void buildMinHeap(struct MinHeap* heap) {
    for (int i = (heap->size-1)/2; i >= 0; i--)
        minHeapify(heap, i);
}
int isLeaf(struct Node* node) {
    return !node->left && !node->right;
}
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* heap = createMinHeap(size);
    for (int i = 0; i < size; i++)
        heap->array[i] = newNode(data[i], freq[i]);
    heap->size = size;
    buildMinHeap(heap);
    return heap;
}
struct Node* buildHuffmanTree(char data[], int freq[], int size) {
    struct Node *left, *right, *top;
    struct MinHeap* heap = createAndBuildMinHeap(data, freq, size);
    while (heap->size > 1) {
        left = extractMin(heap);
        right = extractMin(heap);
        top = newNode('$', left->freq + right->freq);
        top->left = left; top->right = right;
        insertMinHeap(heap, top);
    }
    return extractMin(heap);
}
void printCodes(struct Node* root, int arr[], int top) {
    if (!root) return;
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (isLeaf(root)) {
        printf("%c: ", root->ch);
        for (int i = 0; i < top; i++) printf("%d", arr[i]);
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter number of characters: ");
    scanf("%d", &n);
    char* arr = (char*)malloc(n * sizeof(char));
    int* freq = (int*)malloc(n * sizeof(int));
    printf("Enter characters and their frequencies:\n");
    for (int i = 0; i < n; i++) {
        printf("Char %d: ", i + 1);
        scanf(" %c", &arr[i]);  // space before %c to consume newline
        printf("Freq %d: ", i + 1);
        scanf("%d", &freq[i]);
    }
    struct Node* root = buildHuffmanTree(arr, freq, n);
    int code[100];
    printCodes(root, code, 0);
    free(arr);
    free(freq);
    return 0;
}
}
