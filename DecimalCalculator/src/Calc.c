#include "../include/Calc.h"
#include<stdio.h>

void prints(List* list)
{
    for(int i = 0; i<getSize(list); i++) {
        short ch;
        getAt(list, i, short, &ch);
        printf("%d ", ch);
    }
    printf("\n");
}

/**
 * 桁を合わせる
 */
void digitAlign(Number* lhs, Number* rhs)
{
    // 小数部分の桁合わせ
    int left = getPoint(lhs);
    int right = getPoint(rhs);
    if(left > right) {
        int diff = left - right;
        for(int i=0; i<diff; i++) {
            LIST_ERROR err = insertAt(getNum(rhs), 0, short, &(short){0});
            plerr(err, "in digitAlign, insert 0 to right");
        }
        setPoint(rhs, left);
    }
    else if(right > left) {
        int diff = right - left;
        for(int i=0; i<diff; i++) {
            LIST_ERROR err = insertAt(getNum(lhs), 0, short, &(short){0});
            plerr(err, "in digitAlign, insert 0 to left");
        }
        setPoint(lhs, right);
    }
    
    // 整数部分の桁合わせ
    left = getSize(getNum(lhs));
    right = getSize(getNum(rhs));
    if(left > right) {
        int diff = left - right;
        for(int i=0; i<diff; i++) {
            LIST_ERROR err = add(getNum(rhs), &(short){0});
            plerr(err, "in digitalign, add 0 to right");
        }
    }
    else if(right > left) {
        int diff = right - left;
        for(int i=0; i<diff; i++) {
            LIST_ERROR err = add(getNum(lhs), &(short){0});
            plerr(err, "in digitalign, add 0 to left");
        }
    }
}

Number* sum(Number* lhs, Number* rhs)
{
    if(getSign(lhs) ^ getSign(rhs)) {
        if(getSign(lhs)) {
            setSign(lhs, false);
            return sub(rhs, lhs);
        }
        else {
            setSign(rhs, false);
            return sub(lhs, rhs);
        }
    }
    Number* result = sumOnly(lhs, rhs);
    setSign(result, getSign(lhs));
    return result;
}

Number* sumOnly(Number* lhs, Number* rhs)
{
    Number* l_copy = cloneNumber(lhs);
    Number* r_copy = cloneNumber(rhs);
    Number* result = newNumber();
    digitAlign(l_copy, r_copy);

    short carry = 0;
    int size = getSize(getNum(l_copy));
    for(int i=0; i<size; i++) {
        short l, r;
        LIST_ERROR errl = getAt(getNum(l_copy), i, short, &l);
        plerr(errl, "in sumOnly, get from left");
        LIST_ERROR errr = getAt(getNum(r_copy), i, short, &r);
        plerr(errr, "in sumOnly, get from right");
        short sum = l + r + carry;
        short data = sum % 10;
        LIST_ERROR err = add(getNum(result), &data);
        plerr(err, "in sumOnly, add calcResult to result");
        carry = sum / 10;
    }

    if(carry > 0) {
        LIST_ERROR err = add(getNum(result), &(carry));
        plerr(err, "in sumOnly, add carry to result");
    }
    setSign(result, getSign(lhs));
    setPoint(result, getPoint(l_copy));

    dNumber(l_copy);
    dNumber(r_copy);

    return result;
}

Number* sub(Number* lhs, Number* rhs)
{
    if(getSign(lhs) ^ getSign(rhs)) {
        if(getSign(lhs)) {
            setSign(rhs, true);
            return sum(lhs, rhs);
        }
        else {
            setSign(rhs, false);
            return sum(lhs, rhs);
        }
    }

    Number* result = subOnly(lhs, rhs);

    if(compareAbs(lhs, rhs) < 0) 
        setSign(result, !getSign(lhs));
    else 
        setSign(result, getSign(lhs));

    return result;
}

Number* subOnly(Number* lhs, Number*rhs) 
{
    Number* l_copy = cloneNumber(lhs);
    Number* r_copy = cloneNumber(rhs);
    Number* result = newNumber();
    digitAlign(l_copy, r_copy);

    bool resultSign = false;

    if(compareAbs(l_copy, r_copy) < 0) {
        Number* tmp = l_copy;
        l_copy = r_copy;
        r_copy = tmp;
        resultSign = true;
    }

    short borrow = 0;
    int size = getSize(getNum(l_copy));

    for(int i=0; i<size; i++) {
        short l, r;
        LIST_ERROR errl = getAt(getNum(l_copy), i, short, &l);
        plerr(errl, "in subOnly, get from left");
        LIST_ERROR errr = getAt(getNum(r_copy), i, short, &r);
        plerr(errr, "in subOnly, get from right");
        short diff = l - r - borrow;
        if(diff < 0) {
            diff += 10;
            borrow = 1;
        }else borrow = 0;
        LIST_ERROR err = add(getNum(result), &(diff));
        plerr(err, "in subOnly, add calcResult to result");
    }

    deleteZero(result);

    setSign(result,resultSign);
    setPoint(result, getPoint(l_copy));

    dNumber(l_copy);
    dNumber(r_copy);
    return result;
}

Number* multi(Number* lhs, Number* rhs)
{
    bool resultSign = getSign(lhs) ^ getSign(rhs);

    Number* result = newNumber();

    makeNumList(result, "0");  

    for(int i=0; i<getSize(getNum(rhs)); i++) {
        short carry = 0;
        Number* tmp = newNumber();
        for(int j=0; j<getSize(getNum(lhs)); j++) {
            short l, r;
            LIST_ERROR errl = getAt(getNum(lhs), j, short, &l);
            plerr(errl, "in multi, get from left");
            LIST_ERROR errr = getAt(getNum(rhs), i, short, &r);
            plerr(errr, "in multi, get from right");
            short product = l * r + carry;
            short data = product % 10;
            carry = product / 10;
            LIST_ERROR err = add(getNum(tmp), &data);
            plerr(err, "in multi, add calcResult");
        }

        if(carry > 0) {
            LIST_ERROR err = add(getNum(tmp), &carry);
            plerr(err, "in multi, add carry to result");
        }

        for(int j=0; j<i; j++) {
            short zero = 0;
            LIST_ERROR err = insertAt(getNum(tmp), 0, short, &zero);
            plerr(err, "in multi, insert 0 to 下位桁");
        }
        Number* sum = sumOnly(result, tmp);
        dNumber(result);
        result = sum;
        dNumber(tmp);
    }

    setSign(result, resultSign);
    setPoint(result, getPoint(lhs)+getPoint(rhs));

    return result;
}

Number* divide(Number* lhs, Number* rhs, int sf)
{
    if(isZero(rhs)) return NULL;

    bool resultSign = getSign(lhs) ^ getSign(rhs);

    Number* l_copy = cloneNumber(lhs);
    Number* r_copy = cloneNumber(rhs);

    l_copy = pointShift(l_copy, r_copy);
    setPoint(r_copy, 0);

    Number* result = newNumber();
    Number* reminder = newNumber();

    int l_size = getSize(getNum(l_copy))-1;
    int times = l_size - getPoint(l_copy) + sf;
    int i = l_size;
    while(times >= 0) {
        short data = 0;
        if(i >= 0) {
            LIST_ERROR err = getAt(getNum(l_copy), i, short, &data);
            plerr(err, "in divide, get from left");   
        }
        LIST_ERROR err = insertAt(getNum(reminder), 0, short, &data);
        plerr(err, "in divide, insert 0 to reminder");

        short count = 0;
        while(compareAbs(reminder, r_copy) >= 0) {
            Number* tmp = subOnly(reminder, r_copy);
            dNumber(reminder);
            reminder = tmp;
            count++;
        }

        err = insertAt(getNum(result), 0, short, &count);
        plerr(err, "in divide, insert o to result");
        times--;
        i--;
    }
    
    deleteZero(result);

    setSign(result, resultSign);
    setPoint(result, sf);

    dNumber(reminder);
    dNumber(l_copy);
    dNumber(r_copy);

    return result;
}

/**
 * Numberが0かどうか確かめる
 */
bool isZero(Number* n)
{
    if(n == NULL) return true;
    
    List* num = getNum(n);
    if(num == NULL || getSize(num) == 0) return true;

    for(int i=0; i<getSize(getNum(n)); i++) {
        short data;
        LIST_ERROR err = getAt(getNum(n), i, short, &data);
        plerr(err, "in isZero, get from n");
        if(data != 0) return false;
    }
    return true;
}

/**
 * 不要なゼロを消す.
 */
void deleteZero(Number* num)
{
    while(getSize(getNum(num)) > 1) {
        int index = getSize(getNum(num))-1;
        short data;
        LIST_ERROR err = getAt(getNum(num), index, short, &data);
        plerr(err, "in deleteZero, get from n");
        if(data == 0) {
            printListErr(removeAt(getNum(num), index));
        }else break;
    }
}

/**
 * 小数点を合わせる.
 */
Number* pointShift(Number* lhs, Number* rhs)
{
    Number* l_copy = cloneNumber(lhs);
    Number* r_copy = cloneNumber(rhs);

    List* lc = getNum(l_copy);
    for(int i=getPoint(r_copy); i>0; i--) {
        int l_point = getPoint(l_copy);
        if(l_point > 0) {
            setPoint(l_copy, l_point-1);
        }else {
            LIST_ERROR err = insertAt(lc, 0, short, &(short){0});
            plerr(err, "in pointShift, insert 0 to left");
        }
    }
    setNum(l_copy, lc);

    //dList(lc);
    dNumber(r_copy);
    return l_copy;
}

/**
 * 二つの数を比べる
 * 等しければ0
 * lhsが大きければ正
 * rhsが大きければ負を返す
 */
int compareAbs(Number* lhs, Number* rhs)
{
    Number* l_copy = cloneNumber(lhs);
    Number* r_copy = cloneNumber(rhs);
    digitAlign(l_copy, r_copy);

    int size = getSize(getNum(l_copy));
    for(int i = size-1; i >= 0; i--) {
        short left, right;
        LIST_ERROR errl = getAt(getNum(l_copy), i, short, &left);
        plerr(errl, "in compareAbs, get from left");
        LIST_ERROR errr = getAt(getNum(r_copy), i, short, &right);
        plerr(errr, "in compareAbs, get from right");
        if(left != right) {
            dNumber(l_copy);
            dNumber(r_copy);
            return (int)left - right;    //左が大きければ正,右が大きければ負を返す
        }   
    }

    dNumber(l_copy);
    dNumber(r_copy);

    return 0;   //等しければ0をかえす
}
