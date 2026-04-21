#ifndef STACK_UNBOUND_H
#define STACK_UNBOUND_H

#include <stdlib.h>
#include <pthread.h>

// A structure to represent a stack
// here we use a linked list to represent the unbound stack
typedef struct _StackNode {
    int data;
    struct StackNode* next;
} CStackNode;

typedef struct _CStack 
{
    CStackNode* topStack;
    pthread_mutex_t lock;
    int c_elms;
} CStack;

CStackNode* newNode(int data);
 
int isEmpty(CStack* root);
 
void conc_push(CStack* root, int data);
 
int conc_pop(CStack* root);
 
int conc_top(CStack* root);

void stackFree(CStack* root);

CStack* cstack_init();

#endif /* CONCURRENT_STACK_H */