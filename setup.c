#include "setup.h"
#include "computation.h"
#include "definitions.h"
#include "exponents.h"
#include "operands.h"
#include "structs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
extern const long long t_coefficients[];

// calculates encrypted powers of x shifted
// element_t **encrypt_alpha_taus(degree p) {
//     element_t **X = malloc(sizeof(element_t *) * p);
//     ForP { X[j] = encrypt(alpha * pow(tau, j)); }
//     return X;
// }

// element_t *encrypt_t(element_t **X, Degrees degrees) {
//     element_t *result = malloc(sizeof(element_t));
//     init_G1(result);
//     element_set1(*result);
//     VALUE(result) = 0;
//     element_t *ting;

//     for (uint i = 0; i < degrees.t; i++) {
//         printf("%lld*%lld ", t_coefficients[i],
//                VALUE(X[degrees.p - degrees.t + i]));
//         // (G^xi)^ct
//         ting = exponentiate(*X[degrees.p - degrees.t + i],
//         t_coefficients[i]); add(result, *ting); delete (ting);
//     }

//     return result;
// }

// // helper which logs all x's
// void log_X(element_t **X, uint length) {
//     for (uint i = 0; i < length; i++)
//         printf("x^%d=%lld|", i, VALUE(X[i]));
//     putchar('\n');
// }

// calculates encrypted powers of x
element_t **calculate_taus(/*element_t *(*encrypt)(long long), */ degree p) {
    element_t **X = malloc(sizeof(element_t *) * p);
    ForP { X[j] = encrypt(pow(tau, j)); }
    return X;
}
// element_t **encrypt_G1_taus(degree p) { return calculate_taus(&encrypt, p); }

// setup phase of the protocol
Setup setup(Degrees degrees) {
    element_t **encrypted_taus = calculate_taus(degrees.p);
    const subpolynomials lro = polynomials();
    display(lro);

    printf("Evaluations of verifier's subpolynomials at s = %lld:", tau);
    encrypted subpolynomials_v = Verifiers(lro);
    // element_t(*subpolynomials_v)[verifiers] =
    //     malloc(sizeof(element_t[verifiers]));

    printf("Evaluations of prover's subpolynomials at s = %lld:", tau);
    encrypted subpolynomials_p = evaluate(lro, NULL);

    printf("Shifted subpolynomial evaluations at x = %lld, α=%lld,%lld,%lld:",
           tau, alphas[0], alphas[1], alphas[2]);
    encrypted shifts = evaluate(lro, alphas);

    long long **T = malloc(sizeof(*T) * 3);
    const int Tx = tx(tau);
    for (int j = 0; j < 3; j++) {
        T[j] = malloc(sizeof(**T) * 3);
        ForLRO { T[j][i] = ut[j][i] * ros[i] * Tx; }
    }
    display_t(encrypted_taus, T, Tx);
    static const int beta = u_beta * u_gamma;

    element_t **enc_alphas = malloc(sizeof(element_t *) * 3);
    ForLRO enc_alphas[i] = encrypt(alphas[i]);

    /**
     * The Groth16 protocol
     * (σ,τ) <= Setup(R): Pick α,β,γ,δ,x <= Z^∗_p.Define τ = (α,β,γ,δ,x) and
     * compute σ = ([σ_1]_1, [σ_2]_2), where
     *
     * τ is the toxic waste - tau ceremony
     *
     * σ_1 = (( α, β, δ, {x^i}^{n−1}_{i=0},
     * { (βu_i(x)+αv_i(x)+w_i(x))/γ }^l_{i=0},
     * { (βu_i(x)+αv_i(x)+w_i(x))/δ }^m_{i=l+1},
     * { x^i*t(x)/δ }^{n−2}_{i=0} ))
     * is the proving key
     *
     * σ_2 = (( β, γ, δ, {x^i}^{n−1}_{i=0} ))
     * is the verification key
     */
    printf("\033[0;30m");

    element_t *enc_alpha = encrypt(u_alpha); // α
    element_t *enc_beta = encrypt(u_beta);   // β
    element_t *enc_gamma = encrypt(u_gamma); // γ
    element_t *enc_delta = encrypt(u_delta); // δ
    crypted prover_lro_evaluations = Provers(lro);
    printf("\033[0;37m"); // Show the text

    Setup keys = {{
                      // Pinocchio
                      .enc_tau = encrypted_taus,
                      .subpolynomials = subpolynomials_p,
                      .shifted = shifts,
                      .t = T,
                      .lro = lro,
                      // Groth16
                      .enc_alpha = enc_alpha,
                      .enc_beta = enc_beta,
                      .enc_gamma = enc_gamma,
                      .enc_delta = enc_delta,
                      .enc_tau_new = encrypted_taus,
                      .prover_lro_evaluations = prover_lro_evaluations,
                      .verifier_lro_evaluations = *subpolynomials_v,
                      .trho = *subpolynomials_v,
                  },
                  {// Pinocchio
                   .subpolynomials_v = subpolynomials_v,
                   .to = T[0][2],
                   .beta = beta,
                   .enc_alphas = enc_alphas,
                   // Groth16
                   .enc_beta = enc_beta,
                   .enc_gamma = enc_gamma,
                   .enc_delta = enc_delta,
                   .enc_tau = encrypted_taus}};
    return keys;
}
