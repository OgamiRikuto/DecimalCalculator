#include<stdio.h>
#include "../class/include/List.h"
#include "../class/include/Number.h"
#include "../class/include/Token.h"
#include "../class/include/Stack.h"
#include "../include/Calc.h"

Number* popVal(List* stack)
{
    Number* num = NULL;
    if(isEmpty(stack)) return NULL;

    int index = getSize(stack) - 1;
    getAt(stack, index, Number*, &num);
    removeAt(stack, index);

    return num;
}

Number* eval(Stack* output, int sf)
{
    List* stack = newList(Number*);

    for(int i=0; i<getSize(output); i++) {
        Token* t;
        LIST_ERROR err = getAt(output, i, Token*, &t);
        if(err != LIST_OK) {
            printListErr(err);
            return NULL;
        }

        if(getType(t) == NUMBER) {
            Number* n = cloneNumber(getNumber(t));
            add(stack, &n);
        }
        else if(getType(t) == OPERATOR) {
            if(getSize(stack) < 2) {
                fprintf(stderr, "Error: insufficient operands\n");
                dList(stack);
                return NULL;
            }

            Number* rhs = popVal(stack);
            Number* lhs = popVal(stack);

            if(lhs == NULL || rhs == NULL) {
                fprintf(stderr, "Error: num = null\n");
                return NULL;
            }

            Number* res = NULL;
            switch(getOp(t)) {
                case plus:  res = sum(lhs, rhs); break;
                case minus: res = sub(lhs, rhs); break;
                case ast:   res = multi(lhs, rhs); break;
                case slush: res = divide(lhs, rhs, sf); break;
                default: break;
            }

            if(res == NULL) {
                fprintf(stderr, "Error: divide by zero\n");
                dNumber(lhs);
                dNumber(rhs);
                res = newNumber();
                makeNumList(res, "0");
                return res;
            }
            
            add(stack, &res);

            dNumber(lhs);
            dNumber(rhs);
        }
    }

    if(getSize(stack) != 1) {
        fprintf(stderr, "Error: invalid RPN expression\n");
        dList(stack);
        return NULL;
    }

    Number* result = popVal(stack);
    Number* finalResult = cloneNumber(result);
    dNumber(result);
    dList(stack);

    return finalResult;
}
