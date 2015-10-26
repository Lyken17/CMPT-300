#include "awesome.h"

typedef unsigned long long ll;

void getCurrentTime(char a[30]) {
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strcpy(a,asctime (timeinfo));
    int i = 0;
    while(a[i++] != '\n');
    a[i - 1] = '\0';
}

int value(char a) {
    int res;
    if (a >= 'a' && a <= 'z')
        return a - 'a' + 1;
    else {
        switch (a) {
            case ' ':
                return 0;
            case '#':
                return 27;
            case '.':
                return 28;
            case ',':
                return 29;
            case '\'':
                return 30;
            case '!':
                return 31;
            case '?':
                return 32;
            case '(':
                return 33;
            case ')':
                return 34;
            case '-':
                return 35;
            case ':':
                return 36;
            case '$':
                return 37;
            case '/':
                return 38;
            case '&':
                return 39;
            case '\\':
                return 40;
            default:
                return 0;
        }
    }
}

char reValue(int n) {
    if (n >= 1 && n <= 26)
        return n - 1 + 'a';
    else {
        switch (n) {
            case 0:
                return ' ';
            case 27:
                return '#';
            case 28:
                return '.';
            case 29:
                return ',';
            case 30:
                return '\'';
            case 31:
                return '!';
            case 32:
                return '?';
            case 33:
                return '(';
            case 34:
                return ')';
            case 35:
                return '-';
            case 36:
                return ':';
            case 37:
                return '$';
            case 38:
                return '/';
            case 39:
                return '&';
            case 40:
                return '\\';
            default:
                return ' ';
        }
    }
}

ll fastMod(ll base, ll exponent, unsigned long modulus) {
    ll result = 1;
    while (exponent > 0) {
        if (exponent & 1)
            result = (result * base) % modulus;
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

char* readString(char *readIn) {
    char *handle = (char*)malloc((strlen(readIn) * sizeof(char) + 20) * 2);
    memset(handle, 0, sizeof(strlen(readIn) * sizeof(char)));
    int i,j;
    for (i = 0, j = 0; i < strlen(readIn); ++i) {
        if ( (i + 1) % 8 != 0) {
            handle[j++] = readIn[i];
        }
    }
    handle[j] = '\0';
    return handle;
}

void handleString(FILE *fp, char *readIn,ll exponent, unsigned long modulus) {
    int length = strlen(readIn) / 6;
    ll *res = malloc((length * sizeof(ll) + 20) * 2);
    memset(res, 0, length * sizeof(ll));

    for (int i = 0 ; i < length ; ++i) {
        for (int j = 0 ; j < 6; ++j) {
            res[i] = res[i] * 41 + value(readIn[i * 6 + j]);
        }
    }

    for (int i = 0; i < length; ++i)
        res[i] = fastMod(res[i], exponent, modulus);

    for (int i = 0; i < length; ++i) {
        ll temp = res[i];
        int tempLength = log(modulus) / log(41) + 10;
            int arr[6];
        char tempRes[6];
        memset(arr,0, sizeof(arr));
        memset(tempRes, 0, sizeof(tempRes));

        //extract number
        int i = 0, j = 0;
        while (temp > 0) {
            arr[i++] = temp % 41;
            temp /= 41;
        }

        int __i = i;

        for (int ii = 0; ii < 6; ++ii) {
            fprintf(fp, "%c", reValue(arr[5 - ii]));
        }
    }
    fprintf(fp,"\n");
    //printf("%s\n",output);

    free(res);
}
