#ifndef TOKEN_H
#define TOKEN_H

#include "Number.h"

typedef enum {
    equal = 0,
    plus,
    minus,
    ast,
    slush,
    l_par,
    r_par,
    max_op
}ops;

typedef enum {
    NUMBER,
    EQUAL,
    OPERATOR,
    L_PAR,
    R_PAR
} TokenType;

typedef struct _token {
    TokenType type;
    union {
        Number* num;
        ops op;
    };
}Token;

Token* newNumberToken(Number*);
Token* newOpToken(ops);
Token* newEqualToken();
Token* newLParToken();
Token* newRParToken();
void dToken(Token*);

Token* cloneToken(Token*);
TokenType getType(Token*);
Number* getNumber(Token*);
ops getOp(Token*);

#endif /* TOKEN_H */
