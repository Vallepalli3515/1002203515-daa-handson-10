#include <stdio.h>
#include <stdlib.h>

// Define the structure for a node in the linked list
struct Node {
    int key;
    int value;
    struct Node* next;
    struct Node* prev;
};

// Define the structure for the doubly linked list
struct DoublyLinkedList {
    struct Node* head;
    struct Node* tail;
};

// Function to create a new node
struct Node* createNode(int key, int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// Function to create a new doubly linked list
struct DoublyLinkedList* createDoublyLinkedList() {
    struct DoublyLinkedList* list = (struct DoublyLinkedList*)malloc(sizeof(struct DoublyLinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

// Function to add a node to the doubly linked list
void addNode(struct DoublyLinkedList* list, int key, int value) {
    struct Node* newNode = createNode(key, value);
    if (list->head == NULL) {
        list->head = list->tail = newNode;
    } else {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
}

// Function to delete a node from the doubly linked list
void deleteNode(struct DoublyLinkedList* list, int key) {
    struct Node* current = list->head;
    while (current != NULL) {
        if (current->key == key) {
            if (current == list->head) {
                list->head = list->head->next;
                if (list->head) {
                    list->head->prev = NULL;
                }
                free(current);
                return;
            } else if (current == list->tail) {
                list->tail = list->tail->prev;
                list->tail->next = NULL;
                free(current);
                return;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                free(current);
                return;
            }
        }
        current = current->next;
    }
}

// Function to retrieve a value from the doubly linked list based on key
int retrieveValue(struct DoublyLinkedList* list, int key) {
    struct Node* current = list->head;
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return -1; // Key not found
}

// Function to check if the doubly linked list is empty
int isEmpty(struct DoublyLinkedList* list) {
    return list->head == NULL;
}

// Define the structure for the hash table
struct HashTable {
    int init_capacity;
    int load_factor;
    int capacity;
    int size;
    struct DoublyLinkedList** table;
};

// Function to create a new hash table
struct HashTable* createHashTable(int initialCapacity, int loadFactor) {
    struct HashTable* table = (struct HashTable*)malloc(sizeof(struct HashTable));
    table->init_capacity = initialCapacity;
    table->load_factor = loadFactor;
    table->capacity = initialCapacity;
    table->size = 0;
    table->table = (struct DoublyLinkedList**)malloc(initialCapacity * sizeof(struct DoublyLinkedList*));
    for (int i = 0; i < initialCapacity; i++) {
        table->table[i] = createDoublyLinkedList();
    }
    return table;
}

// Hash function using multiplication
int hash(int key, int capacity) {
    float golden_ratio = 0.6180339887;
    float scaled = key * golden_ratio;
    float fraction = scaled - (int)scaled;
    return (int)(capacity * fraction);
}

// Function to resize the hash table
void resizeHashTable(struct HashTable* table, int newCapacity) {
    struct DoublyLinkedList** newTable = (struct DoublyLinkedList**)malloc(newCapacity * sizeof(struct DoublyLinkedList*));
    for (int i = 0; i < newCapacity; i++) {
        newTable[i] = createDoublyLinkedList();
    }

    for (int i = 0; i < table->capacity; i++) {
        struct Node* current = table->table[i]->head;
        while (current != NULL) {
            int newIndex = hash(current->key, newCapacity);
            addNode(newTable[newIndex], current->key, current->value);
            current = current->next;
        }
    }

    for (int i = 0; i < table->capacity; i++) {
        free(table->table[i]);
    }
    free(table->table);

    table->table = newTable;
    table->capacity = newCapacity;
}

// Function to add a key-value pair to the hash table
void add(struct HashTable* table, int key, int value) {
    if (table->size >= table->capacity * table->load_factor) {
        resizeHashTable(table, table->capacity * 2);
    }
    int index = hash(key, table->capacity);
    addNode(table->table[index], key, value);
    table->size++;
}

// Function to delete a key-value pair from the hash table
void delete(struct HashTable* table, int key) {
    int index = hash(key, table->capacity);
    deleteNode(table->table[index], key);
    table->size--;

    if (table->size < table->capacity / 4) {
        resizeHashTable(table, table->capacity / 2);
    }
}

// Function to retrieve a value from the hash table based on key
int retrieve(struct HashTable* table, int key) {
    int index = hash(key, table->capacity);
    return retrieveValue(table->table[index], key);
}

// Function to check if the hash table is empty
int isEmptyHashTable(struct HashTable* table) {
    return table->size == 0;
}

// Function to print the hash table
void printHashTable(struct HashTable* table) {
    for (int i = 0; i < table->capacity; i++) {
        printf("Bucket %d: ", i);
        struct Node* current = table->table[i]->head;
        while (current != NULL) {
            printf("(%d, %d) ", current->key, current->value);
            current = current->next;
        }
        printf("\n");
    }
}

// Example usage
int main() {
    struct HashTable* hashTable = createHashTable(8, 4);

    add(hashTable, 2, 20);
    add(hashTable, 3, 30);
    add(hashTable, 25, 100);
    add(hashTable, 35, 150);

    printHashTable(hashTable);

    printf("Value for key 3 is: %d\n", retrieve(hashTable, 3));

    delete(hashTable, 3);

    printf("Value for key 3 after deletion is: %d\n", retrieve(hashTable, 3));

    printHashTable(hashTable);

    return 0;
}
