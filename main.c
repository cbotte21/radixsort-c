#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef char* Payload;

typedef struct {
    Payload* element;
    struct Node* left;
    struct Node* right;
} Node;

Node* radix_init_default(void);
void radix_insert(Node* base, int key, Payload* payload);
Payload* radix_get(Node* base, int key);

int main() {
    Node* base = radix_init_default();
    for (int i = 0; i < 500; i++) {
        Payload* payload = (Payload*)malloc(sizeof(Payload));
        if (!payload) {
            printf("Failed to initialize payload\n");
            exit(1);
        }
        *payload = "Found.\n";
        radix_insert(base, 300, payload);
    }

    for (int i = 0; i < 500; i++) {
        printf("%s", (char*)*radix_get(base, i));
    }
    return 0;
}

Node* radix_init_default(void) {
    Node* base = (Node*)malloc(sizeof(Node));
    if (!base) {
        printf("Could not initialize node.\n");
        exit(1);
    }
    base->element = NULL;
    base->left = NULL;
    base->right = NULL;
    return base;
}

void radix_insert_recur(Node* base, int key, Payload* payload, int depth) {
    int offsetValue = (bool)((2 << depth) & key);
    if (depth) {
        Node* next = (Node*)(offsetValue ? base->right : base->left);
        if (!next) {
            next = (Node*)malloc(sizeof(Node));
            if (!next) {
                printf("Could not initialize Node.\n");
                exit(1);
            }
            next->left = NULL;
            next->right = NULL;
            next->element = NULL;
            if (offsetValue) {
                base->right = (struct Node*)next;
            } else {
                base->left = (struct Node*)next;
            }
        }
        radix_insert_recur(next, key, payload, depth - 1);
    }
    base->element = payload;
}

void radix_insert(Node* base, int key, Payload* payload) {
    radix_insert_recur(base, key, payload, sizeof(int)*8);
}

Payload* radix_get_recur(Node* base, int key, int depth) {
    int offsetValue = (bool)((2 << depth) & key);
    if (depth) {
        Node* next = (Node*)(base+1+offsetValue);
        if (!next) {
            if (offsetValue) {
                base->left = (struct Node *) malloc(sizeof(Node));
                next = (Node*)base->left;
            } else {
                base->right = (struct Node*) malloc(sizeof(Node));
                next = (Node*)base->right;
            }
        }
        radix_get_recur(next, key, depth - 1);
    }
    return base->element;
}

Payload* radix_get(Node* base, int key) {
    return radix_get_recur(base, key, sizeof(int)*8);
}