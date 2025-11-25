#include<stdio.h>
#include<stdbool.h>
#include "../class/include/List.h"
#include "../class/include/Number.h"
#include "../class/include/Token.h"
#include "../class/include/Stack.h"

int precidence(ops op)
{
    switch(op) {
        case ast:
        case slush:
            return 2;
        case plus:
        case minus:
            return 1;
        default: return 0;
    }
    return 0;
}

int compareOp(Token* lhs, Token* rhs)
{
    ops l = getOp(lhs);
    ops r = getOp(rhs);
    return precidence(l) - precidence(r);
}

Stack* analyzer(List* tokens)
{
    Stack* stack = newStack();
    Stack* opstack = newStack();

    for(int i = 0; i < getSize(tokens); i++) {
        Token* t;
        LIST_ERROR err = getAt(tokens, i, Token*, &t);
        if(err != LIST_OK) {
            printListErr(err);
            return NULL;
        }

        switch(getType(t)) {

            case NUMBER:
                push(stack, t);
                break;

            case OPERATOR: {
                while(getSize(opstack) > 0) {
                    Token* top = peek(opstack);
                    if(top == NULL) {
                        return NULL;
                    }

                    if(getType(top) != OPERATOR) break;

                    if(compareOp(top, t) >= 0) {
                        pop(opstack);
                        push(stack, top);
                    } else break;
                }
                push(opstack, t);
                break;
            }

            case L_PAR:
                push(opstack, t);
                break;

            case R_PAR:
                while(getSize(opstack) > 0) {
                    Token* top = peek(opstack);
                    if(top == NULL) {
                        return NULL;
                    }
                    if(getType(top) == L_PAR) {
                        pop(opstack);
                        break;
                    }else {
                        pop(opstack);
                        push(stack, top);
                    }
                }
                break;

            case EQUAL:
                break;
        }
    }
    while(getSize(opstack) > 0) {
        Token* top = pop(opstack);
        if(top == NULL) {
            return NULL;
        }
        if(getType(top) == L_PAR || getType(top) == R_PAR) {
            fprintf(stderr, "Error: mismatched parentheses\n");
            dList(stack);
            dList(opstack);
            return NULL;
        }
        push(stack, top);
    }
    dList(opstack);
    return stack;
}
