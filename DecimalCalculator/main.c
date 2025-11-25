#include "./include/Definisions.h"

#define DEFAULT_POINT 2

void printResult(Number* result)
{
    List* out = changeNumtoChar(result);
    for(int i = 0; i<getSize(out); i++) {
        char ch;
        getAt(out, i, char, &ch);
        printf("%c", ch);
    }
    printf("\n");

    dList(out);
}

void debug(const char* str)
{
    printf("%s\n",str);
}

int calc(int sf)
{
    List* tokens = read();
    if(tokens == NULL) {
        return EXIT_FAILURE;
    }

    Stack* output = analyzer(tokens);
    if(output == NULL) {
        return EXIT_FAILURE;
    }

    Number* result = eval(output, sf);
    if(result == NULL) {
        return EXIT_FAILURE;
    }

    printResult(result);

    dList(tokens);
    dList(output);
    dNumber(result);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    int sf = argc > 1 ? atoi(argv[1]) : DEFAULT_POINT;

    while(1) {
        int exitcode = calc(sf);
        if(exitcode == EXIT_FAILURE) return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
