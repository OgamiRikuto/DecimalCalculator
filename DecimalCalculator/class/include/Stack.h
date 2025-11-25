#ifndef STACK_H
#define STACK_H

#include "List.h"
#include "Token.h"
#include "Number.h"
#include<stdbool.h>

typedef List Stack;

Stack* newStack();
void push(Stack*, Token*);
Token* pop(Stack*);
Token* peek(Stack*);
bool isEmpty(Stack*);

#endif /* STACK_H */
