#include <ctype.h>

int isNumber(char[]);
int mathClamp(int, int, int);

int isNumber(char s[]){
    for (int i = 0; s[i] != '\0'; i++){
        if (isdigit(s[i]) == 0){
            return 0;
        }
    }
    return 1;
}

int mathClamp(int val, int min, int max){
    if (val <= min)
        return min;
    else if (val >= max)
        return max;
    else
        return val;
}
