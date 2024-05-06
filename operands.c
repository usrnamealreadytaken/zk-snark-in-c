#include "definitions.h"
#include "delete.h"
#include "exponents.h"
#include "inits.h"
#include "math.h"
#include "qap.h"
#include "structs.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// array [3] - c + a1*x + a2*x^2
// multiplies polynomial by x - n
void mul_n(long long *p, const int n) {
    for (int i = n; i-- > 0;)
        p[i + 1] = p[i];
    p[0] = 0;
    for (int i = 0; i < n; i++)
        p[i] -= p[i + 1] * n;
    return;
}
// finds a subpolynomial of a variable at operand, la, for example
void suspolynomial(long long polynomial[operations],
                   const long long evaluations[operations],
                   const long long fact) {
    // stores fractions
    long long frac[operations + 1];
    // multiplication polynomial
    long long mul_p[operations + 1];
    // integer which mul_p is multiplied by
    long long multiplier;
    memset(polynomial, 0, operations * sizeof(*polynomial));
    memset(frac, 0, (operations + 1) * sizeof(*frac));
    memset(mul_p, 0, (operations + 1) * sizeof(*mul_p));
    mul_p[0] = 1;

    // fact=1;
    for (unsigned int k = 1; k < operations + 1; k++) {
        multiplier = 0;
        if (k > 1) mul_n(mul_p, k - 1);

        for (long long i = 1; i < k + 1; i++) {
            frac[i] = 1;
            for (long long j = 1; j < k + 1; j++) {
                if (j == i) continue;
                frac[i] *= (i - j);
            }
            multiplier += evaluations[i - 1] * fact / frac[i];
        }
        for (unsigned int i = 0; i < k; i++)
            polynomial[i] += mul_p[i] * multiplier;
    }
}

// Calculating one of the three operand polynomials
void operand(matrix polynomial,           // array of subpolynomials l/r/o
             const ptrmatrix evaluations, // array of evaluations
             char sub,
             long long fact) {
    ForVariables {
        printf("%c%c: ", sub, var[j]);
        ForOperations printf("%lld ", evaluations[j][k]);
        putchar('\n');

        suspolynomial(polynomial[j], evaluations[j], fact);
    }
    putchar('\n');
    return;
}
void display_t(element_t **powers, long long *T[3], long long Tx) {

    printf("t(%lld)= ", tau);
    for (int k = operations; k > -1; k--)
        printf("%lld*%lld + ", t_coefficients[k], VALUE(powers[k]));
    printf("0 = %lld\n", Tx);
    for (int j = 0; j < 3; j++)
        ForLRO {
            printf("ρ%c%s*t = %lld * %lld * %lld = %lld\n", sub[i], bao[j],
                   ut[j][i], ros[i], Tx, T[j][i]);
        }
}
void display(subpolynomials lro) {
    puts("Coefficients of subpolynomials for:");
    puts("Input/output variables:");
    ForVerifier {
        ForLRO {
            printf("%c%c: ", sub[i], var[j]);
            ForOperationsBackwards printf("%lld ", lro[i][j][k]);
            putchar('\n');
        }
        putchar('\n');
    }
    puts("Prover:");
    ForProver {
        ForLRO {
            printf("%c%c = ", sub[i], var[j]);
            ForOperationsBackwards printf("%lld ", lro[i][j][k]);
            putchar('\n');
        }
        putchar('\n');
    }
}

// Return all polynomials with their sums
subpolynomials polynomials(void) {
    puts("Evaluations of operand subpolynomials (variable coefficients at a "
         "particular computation):");
    puts("x:  1 2 3 4 5");
    const subpolynomials lro = malloc(sizeof(matrix[3]));
    long long ***evaluations = get_QAP();

    // precomputation
    // factor by wich to multiply the thing
    long long fact = 1;
    for (unsigned int k = 2; k < operations; k++)
        fact *= k;
    // computation
    ForLRO operand(lro[i], evaluations[i], sub[i], i == 2 ? fact * fact : fact);
    return lro;
}

void display_evaluations_v(const int i,
                           const long long evals[verifiers],
                           const matrix p) {
    printf("%lld*( | ", ros[i]);
    ForVerifier {
        ForOperationsBackwards printf("%lld*%lld + ", p[j][k], ipow(tau, k));
        printf("0 = %lld | ", evals[j]);
    }
    printf(") = ");
}

void display_evaluations_prover(const long long evals[provers],
                                const matrix p,
                                const int powers) {
    char s[11 + 20 * provers * operations];
    memset(s, 0, sizeof(s));
    sprintf(s + strlen(s), "%d*( | ", powers);
    ForProver {
        ForOperationsBackwards sprintf(s + strlen(s), "%lld*%lld + ", p[j][k],
                                       ipow(tau, k));
        s[strlen(s) - 3] = '\0';
        sprintf(s + strlen(s), " = %lld | ", evals[j]);
    }
    sprintf(s + strlen(s), ") = ");
    printf("%s", s);
    return;
}

void display_evaluations_p(const int i,
                           const long long evals[provers],
                           const matrix p,
                           const int powers) {
    char s[11];
    sprintf(s, powers ? "*%d*" : "*", powers);
    printf("%lld%s( | ", ros[i], s);
    ForProver {
        ForOperationsBackwards printf("%lld*%lld + ", p[j][k], ipow(tau, k));
        printf("0 = %lld | ", evals[j]);
    }
    fputs(") = ", stdout);
}

/**
 * compute subpolynomials lro
 */
encrypted Verifiers(subpolynomials lro) {
    encrypted results = malloc(sizeof(*results) * total_encrypted * length);
    putchar('\n');
    ForLRO {
        long long evals[length];
        ForVerifier { evals[j] = poly_eval(lro[i][j], operations, tau); }
        printf("ρ%c*%c(s) = ", sub[i], sub[i]);
        display_evaluations_v(i, evals, lro[i]);
        ForVerifier {
            element_t *result =
                encrypt(ros[i] * poly_eval(lro[i][j], operations, tau));
            results[i][j] = result;
            printf("%lld ", VALUE(results[i][j]));
        }
        putchar('\n');
    }

    return results;
}
/**
 * compute lroz
 */
encrypted evaluate(subpolynomials lro, const long long *powers) {
    encrypted results = malloc(sizeof(*results) * total_encrypted * length);
    /**
     * compute subpolynomials lro
     */
    putchar('\n');
    ForLRO {
        long long evals[length];
        ForProver { evals[j] = poly_eval((lro[i])[j], operations, tau); }
        printf("ρ%c%s*%c(s) = ", sub[i], (powers ? "*α" : ""), sub[i]);
        display_evaluations_p(i, evals, lro[i], powers ? powers[i] : 0);

        ForProver {
            element_t *result =
                encrypt(ros[i] * evals[j] * (powers ? powers[i] : 1));
            results[i][j] = result;
            printf("%lld ", VALUE(results[i][j]));
        }
        putchar('\n');
    }
    /**
     * compute subpolynomials z
     * as we have already multiplied lro by ρlro
     * we don't need to do it again
     */
    if (!powers) {
        printf("%c(s) = ", sub[3]);
        ForProver printf("| %lld*(%lld+%lld+%lld) ", u_beta,
                         VALUE(results[l_subpolynomial][j]),
                         VALUE(results[r_subpolynomial][j]),
                         VALUE(results[o_subpolynomial][j]));
        fputs("| = ", stdout);
        ForProver {
            element_t *temp;
            temp = add2(*results[l_subpolynomial][j],
                        *results[r_subpolynomial][j]);
            add(temp, *results[o_subpolynomial][j]);
            results[z_subpolynomial][j] = exponentiate(*temp, u_beta);
            delete (temp);
            printf("%lld ", VALUE(results[z_subpolynomial][j]));
        }
        putchar('\n');
    }

    return results;
}

/**
 * compute polynomial with which the Prover computes
 */
crypted Provers(subpolynomials lro) {
    crypted results = malloc(sizeof(*results));
    /**
     * compute subpolynomials lro
     */

    long long evals[3][length];
    ForLRO {
        ForVariables {
            evals[i][j] = poly_eval(lro[i][j], operations, tau);
            // printf("|%lld|", evals[i][j]);
        }
    }
    ForVariables { encrypt_t(*results[j], 0); }
    ForLRO {
        printf("%c%s(s) = ", sub[i], bao[i]);
        display_evaluations_prover(evals[i], lro[i], baos[i]);

        ForProver {
            element_t *result = encrypt(evals[i][j] * baos[i]);
            add(results[j], *result);
            printf("%lld ", results[i][j][0].value);
        }
        putchar('\n');
    }
    // putchar('\n');
    // ForLRO {
    //     long long evals[length];
    //     ForProver { evals[j] = poly_eval(lro[i][j], operations, tau); }
    //     printf("ρ%c%s*%c(s) = ", sub[i], (powers ? "*α" : ""), sub[i]);
    //     display_evaluations_p(i, evals, lro[i], powers ? powers[i] : 0);

    //     ForProver {
    //         element_t *result =
    //             encrypt(ros[i] * evals[j] * (powers ? powers[i] : 1));
    //         results[i][j] = result;
    //         printf("%lld ", VALUE(results[i][j]));
    //     }
    //     putchar('\n');
    // }
    /**
     * compute subpolynomials z
     * as we have already multiplied lro by ρlro
     * we don't need to do it again
     */
    // if (!powers) {
    //     printf("%c(s) = ", sub[3]);
    //     ForProver printf("| %lld*(%lld+%lld+%lld) ", u_beta,
    //                      VALUE(results[l_subpolynomial][j]),
    //                      VALUE(results[r_subpolynomial][j]),
    //                      VALUE(results[o_subpolynomial][j]));
    //     fputs("| = ", stdout);
    //     ForProver {
    //         element_t *temp;
    //         temp = add2(*results[l_subpolynomial][j],
    //                     *results[r_subpolynomial][j]);
    //         add(temp, *results[o_subpolynomial][j]);
    //         results[z_subpolynomial][j] = exponentiate(*temp, u_beta);
    //         delete (temp);
    //         printf("%lld ", VALUE(results[z_subpolynomial][j]));
    //     }
    //     putchar('\n');
    // }

    return results;
}
