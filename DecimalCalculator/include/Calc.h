#ifndef CALC_H
#define CALC_H

#include "../class/include/List.h"
#include "../class/include/Number.h"

void prints(List*);

void digitalAlign(Number*, Number*);
Number* sum(Number*, Number*);
Number* sumOnly(Number*, Number*);
Number* sub(Number*, Number*);
Number* subOnly(Number*, Number*);
Number* multi(Number*, Number*);
Number* divide(Number*, Number*, int);
bool isZero(Number*);
void deleteZero(Number*);
Number* pointShift(Number*, Number*);
int compareAbs(Number*, Number*);

#endif /* CALC_H */
