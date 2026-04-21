// C program for linked list implementation of stack
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack_unbound.h"

// source: https://www.geeksforgeeks.org/stack-data-structure-introduction-program/
 
CStackNode* newNode(int data)
{
    CStackNode* stackNode = (CStackNode*) malloc(sizeof(CStackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}
 
int isEmpty(CStack* root)
{
    int r = !root->topStack;
    return r;
}
 
void conc_push(CStack* root, int data)
{
    CStackNode* stackNode = newNode(data);
    stackNode->next = root->topStack;
    root->topStack = stackNode;
    root->c_elms++;
}
 
int conc_pop(CStack* root)
{
    if (isEmpty(root))
        return INT_MIN;
    CStackNode* temp = root->topStack;
    root->topStack = (root->topStack)->next;
    int popped = temp->data;
    free(temp);
    root->c_elms--;
    return popped;
}
 
int conc_top(CStack* root)
{
    if (isEmpty(root))
        return INT_MIN;
    int r = root->topStack->data;
    return r;
}

void stackFree(CStack* root)
{
  // we free the stack just popping all the elements
  while(!isEmpty(root))
  {
	  conc_pop(root);
  }
}

CStack* cstack_init()
{
    CStack *stack = malloc(sizeof(CStack));
    stack->topStack = NULL;
    pthread_mutex_init(&(stack->lock), NULL);
    stack->c_elms = 0;
    return stack;
}
