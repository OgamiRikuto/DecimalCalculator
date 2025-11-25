#include "../include/Stack.h"

Stack* newStack()
{
    return newList(Token*);
}

void push(Stack* stack, Token* t)
{
    if(stack == NULL || t == NULL) return;
    add(stack, &t);
}

Token* pop(Stack* stack)
{
    Token* token = NULL;
    if(isEmpty(stack)) return NULL;

    int index = getSize(stack) - 1;
    getAt(stack, index, Token*, &token);
    removeAt(stack, index);

    return token;
}

Token* peek(Stack* stack)
{
    Token* token = NULL;
    if(isEmpty(stack)) return NULL;

    getAt(stack, getSize(stack) - 1, Token*, &token);
    return token;
}

bool isEmpty(Stack* stack)
{
    return stack == NULL || getSize(stack) == 0;
}
