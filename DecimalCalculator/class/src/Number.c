#include "../include/Number.h"
#include<stdio.h>

typedef struct _number {
    List* num;  //数値
    bool sign;  // true: 負, false: 正
    int point;  // 実際の数値 * 10^point = 数値
} Number;

// コンストラクタ
Number* newNumber()
{
    Number* this = NULL;

    this = (Number*)malloc(sizeof(Number));
    if(this == NULL) return NULL;

    this->num = newList(short);
    this->sign = false;
    this->point = 0;

    return this;
}

// デストラクタ
void dNumber(Number* n)
{
    dList(n->num);
    free(n);
}

/**
 * 与えられた文字列からNumber型の数字に変換する
 */
int makeNumList(Number* n, const char* str)
{
    if(str == NULL || str[0] == '\0') return -1;
    int i=0;
    bool hasSign = false;
    if((str[i] < '0' || str[i] > '9') && (str[i] != '+' && str[i] != '-')) return -1;
    switch(str[i]) {    // 符号処理
        case '-':
            hasSign = true;
            n->sign = true;
            i++;
            break;
        case '+':
            hasSign = true;
            n->sign = false;
            i++;
            break;
        default:
            n->sign = false;
            break;
    }

    if(str[i] < '0' || str[i] > '9') return -1;

    int pointIndex = 0;
    bool isPointExist = false;
    bool isZeroFirst = true;
    while(str[i] != '\0') {
        if((str[i] < '0' || str[i] > '9') && ((str[i] != '.') || isPointExist)) return -1;
        if(str[i] == '.') {
            if(str[i+1] == '\0') return -1; //「123.」のような例外を省く
            isPointExist = true;
            i++;
            continue;
        }
        short digit = str[i] - '0';
        if(isZeroFirst) {   //先頭の0はすべて削除(pointで辻褄は合わせることができる
            if(digit != 0) {
                isZeroFirst =false;
                printListErr(add(n->num, &digit));
            }
        }else {
            printListErr(add(n->num, &digit));
        }
        if(!isPointExist) pointIndex++;
        i++;
    }
    if(!isPointExist) n->point = 0;
    else {
        int charNum = i-1;  //ナル文字を省く
        if(hasSign) charNum--;  //符号文字を省く
        n->point = charNum - pointIndex;
    }
    if(getSize(n->num) == 0) {
        short zero = 0;
        printListErr(add(n->num, &zero));
        n->point = 0;
    }
    printListErr(reverse(n->num));
    return 0;
}

/**
 * 与えられたNumber型から文字列のリストに変換してそのリストを返す.
 */
List* changeNumtoChar(Number* n)
{
    List* numList = newList(char);
    int size = getSize(n->num);
    for(int i = 0; i < size; i++) {
        short data;
        LIST_ERROR err = getAt(n->num, i, short, &data);
        if(err != LIST_OK) {
            printListErr(err);
            return NULL;
        }
        char ch = data + '0';
        printListErr(add(numList, &ch));
    }

    while(getSize(numList)-1 < n->point) {
        char zero = '0';
        printListErr(add(numList, &zero));
    }
    printListErr(reverse(numList));

    if(n->point > 0) {
        int dotIndex = getSize(numList) - n->point;
        char dot = '.';
        printListErr(insertAt(numList, dotIndex, char, &dot));
    }
    if(n->sign) {
        char minus = '-';
        printListErr(insertAt(numList, 0, char, &minus));
    }
    return numList;   
}

/**
 * コピーする関数
 */
Number* cloneNumber(Number* n)
{
    if(n == NULL) {
        return NULL;
    }
    Number* copy = newNumber();

    copy->sign = n->sign;
    copy->point = n->point;
    LIST_ERROR err_code = clone(copy->num, n->num);
    if(err_code != LIST_OK) {
        printListErr(err_code);
        dNumber(copy);
        return NULL;
    }
    return copy;
}

// 以下ゲッター, セッター

List* getNum(Number* n)
{
    return n->num;
}

void setNum(Number* n, List* list)
{
    printListErr(clone(n->num, list));
}

bool getSign(Number* n)
{
    return n->sign;
}

void setSign(Number* n ,bool sign)
{
    n->sign = sign;
}

int getPoint(Number* n)
{
    return n->point;
}

void setPoint(Number* n, int p)
{
    n->point = p;
}

