#ifndef _DEFINISIONS_H_
#define _DEFINISIONS_H_

#include<stdio.h>
#include<stdlib.h>
#include "../class/include/List.h"
#include "../class/include/Number.h"
#include "./Calc.h"
#include "../class/include/Stack.h"
#include "../class/include/Token.h"

// analyzer
int precidence(ops);
int compareOp(Token*, Token*);
Stack* analyzer(List*);

// evaluate
Number* popVal(List*);
Number* eval(Stack*, int);

// reader
ops convertc2o(char);
List* tokenize(const char*);
List* read();

#endif /* _DEFINISIONS_H_ */
