#include "verifier.h"
#include "definitions.h"
#include "delete.h"
#include "exponents.h"
#include "inits.h"
#include "structs.h"
#include <pbc/pbc_field.h>
#include <stdio.h>
#include <stdlib.h>

bool variables_consistent(Capitals *capitals, const int beta) {
    Capitals *multiplied = malloc(sizeof(*capitals));

    ForLRO multiplied->P[i] = exponentiate(*capitals->P[i], beta);
    element_t *sum =
        add(add2(*multiplied->P[0], *multiplied->P[1]), *multiplied->P[2]);
    printf("βγ*(L(s)+R(s)+O(s)) = %d*(%lld+%lld+%lld) = %lld\n", beta,
           capitals->L[0].value, capitals->R[0].value, capitals->O[0].value,
           VALUE(sum));

    element_t *Z_gamma = exponentiate(capitals->Z, u_gamma);
    printf("Z(s)*γ = %lld*%lld = %lld\n", capitals->Z[0].value, u_gamma,
           VALUE(Z_gamma));

    bool result = !element_cmp(*Z_gamma, *sum);
    delete (sum);
    delete (Z_gamma);
    return result;
}

Capitals *get_LRO(const encrypted evaluations, const Capitals regular) {
    Capitals *capitals = malloc(sizeof(*capitals));
    capitals->P[0] = &capitals->L;
    capitals->P[1] = &capitals->R;
    capitals->P[2] = &capitals->O;
    element_t *temp;

    ForLRO {
        init_G1(capitals->P[i]);
        ForVerifier {
            temp = exponentiate(*evaluations[i][j], values[j]);
            add(capitals->P[i], *temp);
            delete (temp);
        }
        add(capitals->P[i], *regular.P[i]);
    }

    return capitals;
}

void display_resulting(Capitals capitals,
                       encrypted evaluations,
                       Capitals regular) {
    ForLRO {
        printf("%c(s) = %cv(s) + %cp(s) = ", opr[i], opr[i], opr[i]);
        ForVerifier {
            printf("%lld*%lld + ", VALUE(evaluations[i][j]), values[j]);
            // *capitals->P[i] += evaluations.p[i][j] * values[j];
        }
        // *capitals->P[i] += *regular.P[i];
        printf("%lld = %lld\n", VALUE(regular.P[i]), VALUE(capitals.P[i]));
    }
}

void display_right(const element_t h,
                   const long long t,
                   const element_t O,
                   const element_t right) {
    printf("E(th+O) = E(%lld*%lld+%lld) = E(%lld)\n", t, h[0].value, O[0].value,
           right[0].value);
}

bool shifts_respected(Capitals *LROZ,
                      Capitals *shifted,
                      element_t **enc_alphas) {
    element_t enc_shifted, enc_alpha;
    bool respected = true;
    init_GT(&enc_shifted);
    init_GT(&enc_alpha);

    ForLRO { //
        printf("%c*%lld = %c`\n", opr[i], alphas[i], opr[i]);
        element_pairing(enc_alpha, *enc_alphas[i], *LROZ->P[i]);
        pairing_pp_apply(enc_shifted, *shifted->P[i], g1_pairing);
        respected &= !element_cmp(enc_alpha, enc_shifted);
    }
    element_clear(enc_shifted);
    element_clear(enc_alpha);

    return respected;
}

bool validate_computation(Proof proof, Verification keys) {
    // LRO for all variables
    element_t *left_pp = malloc(sizeof(element_t));
    init_GT(left_pp);
    Capitals *LRO = get_LRO(keys.subpolynomials_v, *proof.LROZ);
    element_pairing(*left_pp, LRO->R, LRO->L);
    VALUE(left_pp) = VALUE(LRO->P[R_major]) * VALUE(LRO->P[L_major]);
    display_resulting(*LRO, keys.subpolynomials_v, *proof.LROZ);
    printf("E(ρl*L*ρr*R)=E(%lld)\n\n", VALUE(left_pp));

    printf("E(h) = E(%lld)\n", VALUE(proof.H));

    element_t *th = exponentiate(*proof.H, keys.to), th_pp, O_pp;
    init_GT(&th_pp);
    init_GT(&O_pp);
    pairing_pp_apply(th_pp, *th, g1_pairing);
    th_pp[0].value = VALUE(th);
    delete (th);

    pairing_pp_apply(O_pp, LRO->O, g1_pairing);
    O_pp[0].value = LRO->O[0].value;

    element_t *right_pp = add2(th_pp, O_pp);
    display_right(*proof.H, keys.to, LRO->O, *right_pp);
    const bool evaluated = !element_cmp(*right_pp, *left_pp);
    delete (right_pp);
    delete (left_pp);
    return evaluated;
}

int verifier(Proof proof, Verification keys) {
    const bool respected =
        shifts_respected(proof.LROZ, proof.shifted, keys.enc_alphas);
    if (respected) puts("All shifts respected\n");
    else puts("Shift comparison not passed\n");

    const bool checksum = variables_consistent(proof.LROZ, keys.beta);
    if (checksum) puts("Variable consistency passed\n");
    else puts("Variable consistency not passed\n");

    const bool evaluated = validate_computation(proof, keys);
    if (evaluated) puts("Evaluation polynomials match\n");
    else puts("Evaluation polynomials don't match\n");
    // if (evaluated) puts("\nE(ρlρr*LR) == E(ρlρr*th+ρlρr*O)\n");

    free(proof.LROZ);
    free(proof.shifted);
    return respected && checksum && evaluated;
}
