#include "../include/Token.h"

Token* newNumberToken(Number* n)
{
    Token* self = NULL;

    self = (Token*)malloc(sizeof(Token));
    if(self == NULL) return NULL;

    self->type = NUMBER;
    self->num = cloneNumber(n);
    if(self->num == NULL) return NULL;
    return self;
}


Token* newOpToken(ops op)
{
    Token* self = NULL;

    self = (Token*)malloc(sizeof(Token));
    if(self == NULL) return NULL;

    self->type = OPERATOR;
    self->op = op;
    return self;
}

Token* newEqualToken()
{
    Token* self = NULL;

    self = (Token*)malloc(sizeof(Token));
    if(self == NULL) return NULL;

    self->type = EQUAL;
    self->op = equal;
    return self;
}

Token* newLParToken()
{
    Token* self = (Token*)malloc(sizeof(Token));
    if(self == NULL) return NULL;

    self->type = L_PAR;
    return self;
}

Token* newRParToken()
{
    Token* self = (Token*)malloc(sizeof(Token));
    if(self == NULL) return NULL;

    self->type = R_PAR;
    return self;
}

void dToken(Token* self)
{
    if(self == NULL) return;

    if(self->type == NUMBER && self->num != NULL) dNumber(self->num);
    free(self);
}

Token* cloneToken(Token* self)
{
    if(self == NULL) return NULL;

    Token* copy;
    switch(self->type) {
        case NUMBER:
            copy = newNumberToken(self->num);
            break;
        case OPERATOR:
            copy = newOpToken(self->op);
            break;
        case L_PAR:
            copy = newLParToken();
            break;
        case R_PAR:
            copy = newRParToken();
            break;
        case EQUAL:
            copy = newEqualToken();
            break;
        default:
            copy = NULL;
            break;
    }
    return copy;
}


TokenType getType(Token* self)
{
    return self->type;
}

Number* getNumber(Token* self)
{
    return self->num;
}

ops getOp(Token* self)
{
    return self->op;
}
