#include<stdio.h>
#include<stdbool.h>
#include "../class/include/List.h"
#include "../class/include/Number.h"
#include "../class/include/Token.h"


#define MAX_LEN 1024

ops convertc2o(char c)
{
    ops op;
    switch(c) {
        case '+':   op = plus; break;
        case '-':   op = minus; break;
        case '*':   op = ast; break;
        case '/':   op = slush; break; 
        default:    op = max_op; break;
    }
    return op;
}

List* tokenize(const char* line)
{
    if(line == NULL) return NULL;
    int idx = 0;
    List* tokens = newList(Token*);

    while(line[idx] != '\0') {
        char c = line[idx];

        // 空白は無視
        if(c == ' ' || c == '\t') {idx++; continue;}

        // 数字
        if((c >= '0' && c <= '9') || c == '+' || c == '-') {

            // 単項処理
            bool isUnary = false;
            if(idx == 0 || line[idx-1] == '(' || line[idx-1] == '+' || line[idx-1] == '-' || line[idx-1] == '*' || line[idx-1] == '/' ) {
                if(c == '+' || c == '-') isUnary = true;
            }

            // 単項じゃないなら二項演算子
            if((c == '+' || c == '-') && !isUnary) {
                Token* t = newOpToken(convertc2o(c));
                LIST_ERROR err = add(tokens, &t);
                if(err != LIST_OK) {
                    fprintf(stderr, "in tokenize op\n");
                    printListErr(err);
                    return NULL;
                }
                idx++;
                continue;
            }

            Number* n = newNumber();
            int start = idx;
            if(isUnary) idx++;
            while((line[idx] >= '0' && line[idx] <= '9') || line[idx] == '.') idx++;
            int len = idx - start;
            char buf[MAX_LEN];
            strncpy(buf, &line[start], len);
            buf[len] = '\0';

            makeNumList(n, buf);

            Token* t = newNumberToken(n);

            LIST_ERROR err = add(tokens, &t);
            if(err != LIST_OK) {
                fprintf(stderr, "in tokenize number\n");
                printListErr(err);
                return NULL;
            }
            continue;
        }

        // 括弧
        if(c == '(' || c == ')') {
            Token* t = c == '(' ? newLParToken() : newRParToken();
            LIST_ERROR err = add(tokens, &t);
            if(err != LIST_OK) {
                fprintf(stderr, "in tokenize ()\n");
                printListErr(err);
                return NULL;
            }
            idx++;
            continue;
        }

        // 演算子
        if(c == '+' || c == '-' || c == '*' || c == '/') {
            Token* t = newOpToken(convertc2o(c));
            LIST_ERROR err = add(tokens, &t);
            if(err != LIST_OK) {
                fprintf(stderr, "in tokenize op\n");
                printListErr(err);
                return NULL;
            }
            idx++;
            continue;
        }

        // =
        if(c == '=') {
            Token* t = newEqualToken();
            LIST_ERROR err = add(tokens, &t);
            if(err != LIST_OK) {
                fprintf(stderr, "in tokenize =\n");
                printListErr(err);
                return NULL;
            }
            idx++;
            break;
        }

        fprintf(stderr, "Error: Invalied character: %c\n", c);
        dList(tokens);
        return NULL;
    }

    return tokens;
}


List* read()
{
    List* tokens = newList(Token*);
    char str[MAX_LEN];

    while(1) {
        printf(">");
        if(fgets(str, MAX_LEN, stdin) == NULL) return NULL;

        int len = strlen(str);
        if(len > 0 && str[len-1] == '\n') str[len-1] = '\0';

        List* lineTokens = tokenize(str);

        if(lineTokens == NULL) {
            dList(lineTokens);
            fprintf(stderr, "Error: tokenize return NULL.\n");
            return NULL;
        }

        for(int i=0; i<getSize(lineTokens); i++) {
            Token* t;
            LIST_ERROR err = getAt(lineTokens, i, Token*, &t);
            if(err != LIST_OK) {
                fprintf(stderr, "in read loop\n");
                printListErr(err);
                dList(lineTokens);
                return NULL;
            }

            Token* copy = cloneToken(t);
            if(copy == NULL) {
                fprintf(stderr, "Error: cloneToken return NULL.\n");
                return NULL;
            }

            add(tokens, &copy);
        }

        dList(lineTokens);


        if(getSize(tokens) == 0) {
            fprintf(stderr, "Error: No tokens found.\n");
            return NULL;
        }

        Token* last;
        LIST_ERROR err = getAt(tokens, getSize(tokens)-1, Token*, &last);
        if(err != LIST_OK) {
            fprintf(stderr, "in read last\n");
            printListErr(err);
            return NULL;
        }
        if(getType(last) == EQUAL) break;
    }
    return tokens;
}
