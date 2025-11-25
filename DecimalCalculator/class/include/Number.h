#ifndef NUMBER_H
#define NUMBER_H

#include "List.h"
#include<stdbool.h>

typedef struct _number Number;

Number* newNumber();
void dNumber(Number*);
int makeNumList(Number*, const char*);
List* changeNumtoChar(Number*);
Number* cloneNumber(Number*);
List* getNum(Number*);
void setNum(Number*, List*);
bool getSign(Number*);
void setSign(Number*, bool);
int getPoint(Number*);
void setPoint(Number*, int);

#endif /* NUMBER_H */
