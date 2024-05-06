#include "prover.h"
#include "definitions.h"
#include "delete.h"
#include "exponents.h"
#include "inits.h"
#include <stdio.h>
#include <stdlib.h>
extern const long long p_coefficients[];
extern const long long h_coefficients[];

// multiply two polynomials
long long *
multiply_polynomial(const long long A[], const long long B[], int m, int n) {
    long long *prod = malloc(sizeof(*prod) * (m + n - 1));
    for (int i = 0; i < m + n - 1; i++)
        prod[i] = 0;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            prod[i + j] += A[i] * B[j];

    return prod;
}
// add subpolynomials and their coefficients to the operand polynomials
void concat(const matrix variables,
            const long long coefficients[length],
            long long *operand) {
    ForOperations operand[k] = 0;
    ForVariables ForOperations operand[k] += variables[j][k] * coefficients[j];
}

void dispay_resulting_h(const long long *h) {
    for (int j = operations; j > -1; j--)
        printf("%0.1lld + ", h[j]);
    putchar('0');
}

void display_h(const long long *division, const majors bigger) {

    fputs("\nh(x) = (LR-O)/t + δr*L + δl*R + δlδr*t - δo = \n                  "
          "                  ",
          stdout);
    for (int k = operations - 2; k > -1; k--)
        printf("%0.1lld ", division[k]);
    fputs("+ \n                 ", stdout);
    ForOperationsBackwards {
        printf("%lld*%0.1lld=%0.1lld ", delta_r, bigger[L_major][k],
               delta_r * bigger[L_major][k]);
    }
    fputs("+ \n                  ", stdout);
    ForOperationsBackwards {
        printf("%lld*%0.1lld=%0.1lld ", delta_l, bigger[R_major][k],
               delta_l * bigger[R_major][k]);
    }
    fputs("+ \n", stdout);
    for (int k = operations; k > -1; k--) {
        printf("%lld*%lld*%0.1lld=%0.1lld ", delta_l, delta_r,
               t_coefficients[k], delta_l * delta_r * t_coefficients[k]);
    }
    printf("-\n                                      %lld = \n", delta_o);
}
void display_p(const long long *p) {
    printf("\np(x) = L*R-O = ");
    for (int k = (operations - 1) * 2; k > -1; k--)
        printf("%lld ", p[k]);
}

long long *divide_polynomial(const long long *dividend,
                             const long long divisor[operations + 1],
                             const int m,
                             const int n) {
    long long *quotient = malloc(sizeof(*quotient) * operations * 2 + length);
    for (int i = 0; i < m; i++)
        quotient[i] = dividend[i];

    long long normalizer = divisor[0];

    for (int i = 0; i < m - (n - 1); i++) {
        quotient[i] /= normalizer;
        long long coef = quotient[i];
        if (coef != 0)
            for (int j = 1; j < n; j++)
                quotient[i + j] += -divisor[j] * coef;
    }

    return quotient;
}

void display_polynomials(const subpolynomials lro, const majors bigger) {
    puts("Assign all values to the unencrypted variable polynomials:");
    ForLRO {
        long long operand[operations];
        printf("%c(x): ", opr[i]);
        ForOperationsBackwards {
            ForVariables {
                printf("%lld*%lld + ", lro[i][j][k], values[j]);
                operand[k] += lro[i][j][k] * values[j];
            }
            fputs("0 | ", stdout);
        }
        ForOperationsBackwards printf("%lld ", bigger[i][k]);
        putchar('\n');
    }
}
void zk_h(long long *division, const majors bigger) {

    ForOperationsBackwards { division[k] += delta_r * bigger[L_major][k]; }
    ForOperationsBackwards { division[k] += delta_l * bigger[R_major][k]; }
    for (int k = operations; k > -1; k--) {
        division[k] += delta_l * delta_r * t_coefficients[k];
    }
    division[0] -= delta_o;
}
const long long *get_h_indices(const subpolynomials lro) {
    majors bigger;

    ForLRO concat(lro[i], values, bigger[i]);
    display_polynomials(lro, bigger);

    long long *p = multiply_polynomial(bigger[L_major], bigger[R_major],
                                       operations, operations);
    for (int k = operations - 1; k > -1; k--)
        p[k] -= bigger[O_major][k];
    display_p(p);

    fputs("\nt(x) = ", stdout);
    for (int k = operations; k > -1; k--)
        printf("%lld ", t_coefficients[k]);

    long long *h_indices = divide_polynomial(
        p, t_coefficients, operations * 2 - 1, operations + 1);
    display_h(h_indices, bigger);
    zk_h(h_indices, bigger);
    dispay_resulting_h(h_indices);

    putchar('\n');

    free(p);
    return h_indices;
}

element_t *evaluate_H(const long long *h, element_t **powers) {
    element_t *result = malloc(sizeof(element_t));
    element_t *temp;
    init_G1(result);
    printf("h(s) = ");
    for (int k = operations; k > -1; k--) {
        printf("%lld*%lld + ", h[k], VALUE(powers[k]));
        fflush(stdout);
        temp = exponentiate(*powers[k], h[k]);

        add(result, *temp);
        delete (temp);
    }
    printf("0 = %lld\n", VALUE(result));
    return result;
}

Capitals *calculate_LRO(const encrypted subpolynomials, const long long T[3]) {
    Capitals *capitals = malloc(sizeof(*capitals));
    init_G1(&capitals->Z); /*G1*/
    capitals->P[0] = &capitals->L;
    capitals->P[1] = &capitals->R;
    capitals->P[2] = &capitals->O;

    element_t *temp;

    ForLRO {
        init_G1(capitals->P[i]);
        ForProver {
            add(capitals->P[i],
                *(temp = exponentiate(*subpolynomials[i][j], values[j])));
            free(temp);
        }
        add(capitals->P[i], *encrypt(T[i] * deltas[i]));
    }

    return capitals;
}

void display_LRO(const Capitals *capitals,
                 const encrypted evaluations,
                 const long long T[3],
                 const char *s) {
    ForLRO {
        printf("t*δ%c+%s%c(s) = ", sub[i], s, opr[i]);
        printf("(%lld*%lld = %lld)", T[i], deltas[i], T[i] * deltas[i]);
        ForProver {
            printf(" + %lld*%lld", VALUE(evaluations[i][j]), values[j]);
        }
        printf(" = %0.1lld\n", VALUE(capitals->P[i]));
    }
}

void get_Z(Capitals *capitals,
           const encrypted evaluations,
           const long long T[3]) {
    element_t *temp;
    ForProver {
        temp = exponentiate(*evaluations[z_subpolynomial][j], values[j]);
        add(&capitals->Z, *temp);
        delete (temp);
    }
    temp = encrypt(T[0] * delta_l + T[1] * delta_r + T[2] * delta_o);
    add(&capitals->Z, *temp);
    delete (temp);
}
void display_z(const Capitals *capitals,
               const encrypted evaluations,
               const long long T[3]) {
    printf("t*δ%c+t*δ%c+t*δ%c+%c = (%lld*%lld+%lld*%lld+%lld*%lld = %lld)",
           sub[0], sub[1], sub[2], opr[3], T[0], delta_l, T[1], delta_r, T[2],
           delta_o, T[0] * delta_l + T[1] * delta_r + T[2] * delta_o);
    ForProver {
        printf(" + %lld*%lld", VALUE(evaluations[z_subpolynomial][j]),
               values[j]);
    }
    printf(" = %lld\n", capitals->Z[0].value);
}

// generating a proof
Proof prover(const Proving keys, const Degrees degrees) {
    // indices of h
    const long long *h_indices = get_h_indices(keys.lro);

    // actual evaluation of h
    element_t *H = evaluate_H(h_indices, keys.enc_tau);

    Capitals *LRO = calculate_LRO(keys.subpolynomials, keys.t[0]);
    display_LRO(LRO, keys.subpolynomials, keys.t[0], "");

    get_Z(LRO, keys.subpolynomials, keys.t[2]);
    display_z(LRO, keys.subpolynomials, keys.t[2]);

    Capitals *shifted = calculate_LRO(keys.shifted, keys.t[1]);

    putchar('\n');
    display_LRO(shifted, keys.shifted, keys.t[1], "α");
    putchar('\n');

    ForProver {
        for (uint i = 0; i < total_encrypted; i++)
            delete (keys.subpolynomials[i][j]);
        for (uint i = 0; i < z_subpolynomial; i++)
            delete (keys.shifted[i][j]);
    }

    for (uint i = 0; i < degrees.p; i++)
        delete (keys.enc_tau[i]);

    free(keys.enc_tau);
    free(keys.shifted);
    free(keys.subpolynomials);

    Proof proof = {H, LRO, shifted};
    return proof;
}
