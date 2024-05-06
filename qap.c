#include "computation.h"
#include "definitions.h"
// #include "definitions.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define len 256

#define SIGNED(i) (i * (minus ? -1 : 1))

// returns the index of a character. Or 0 if it wasn't found
int lookup(char *array, char character) {
    for (int i = 0; array[i]; i++)
        if (array[i] == character) return i;
    return 0;
}

// acquire all variable names from the r1cs
char *get_var_names(const char line[], u_long length) {
    char chars[len] = {'1'};
    memset(&chars[1], '\0', len - 1);
    int variables = 1;

    for (u_long i = 0; i < length; i++)
        if (isalpha(line[i]))
            if (!lookup(chars, line[i]))
                chars[variables++] = line[i]; // push a name

    // fitting all vars and the constant into a null terminated array
    char *array = malloc(variables + 1);
    memcpy(array, chars, variables);
    array[variables] = '\0';
    return array;
}
double ***
get_QAP_double(const char line[], size_t str_length, char *variables) {
    double ***vals;
    int length = strlen(variables);

    vals = malloc(sizeof(*vals) * 3);
    ForLRO {
        vals[i] = malloc(sizeof(**vals) * length);
        for (int j = 0; j < length; j++) {
            vals[i][j] = calloc(operations, sizeof(***vals));
        }
    }

    bool minus = false;
    int lro = 0;
    int op = 0;
    for (size_t i = 0; i < str_length - 1; i++) {

        if (line[i] == '\n') {
            op++;
            lro = 0;
            continue;
        }
        if (isspace(line[i])) {
            lro++;
            continue;
        }
        char *operand = calloc(128, sizeof(*operand)), *freeable = operand;
        for (int j = 0; line[i] && !isspace(line[i]); ++j, ++i)
            operand[j] = line[i];
        i--;
        // printf("%s\n", operand);// useful
        while (*operand) {
            minus = false;
            if (*operand == '+') {
                operand += 1;
            }
            if (*operand == '-') {
                operand += 1;
                minus = true;
            }

            if (isalpha(*operand)) { // if it's a letter without a mulpiplier
                vals[lro][lookup(variables, *operand)][op] = SIGNED(1);
            } else {
                double f = strtod(operand, &operand);

                // if it's a varaible
                if (isalpha(*operand)) {
                    double *p = &(vals[lro][lookup(variables, *operand)][op]);
                    if (*p) printf("variable %c taken\n", *operand);
                    *p = SIGNED(f);

                    operand += 1;
                } else {
                    if (vals[lro][lookup(variables, *operand)][op])
                        puts("int taken");
                    vals[lro][0][op] = SIGNED(f);
                }
            }
            operand += 1;
        }
        free(freeable);
    }
    return vals;
}

long long ***get_QAP(void) {
    char *variables = get_var_names(line, sizeof(line));
    int var_count = strlen(variables);
    printf("%s\n", variables);

    double ***vals = get_QAP_double(line, sizeof(line), variables);

    long long ***evals = malloc(sizeof(*evals) * 3);
    ForLRO {
        evals[i] = malloc(sizeof(*evals) * length);
        ForVariables {
            evals[i][j] = calloc(operations, sizeof(***evals));
            ForOperations { evals[i][j][k] = vals[i][j][k]; }
        }
    }

    printf("The letters are: \n");
    for (int j = 0; j < var_count; j++)
        printf("%c, ", variables[j]);
    puts("");
    free(variables);

    ForLRO {
        int j, k;
        for (j = 0; j < length; j++) {
            putchar('{');
            for (k = 0; k < operations; k++) {
                printf("%.0lf, ", vals[i][j][k]);
            }
            printf("}, ");
        }
        printf("},\n");
    }
    puts("");
    return evals;
}
