#ifndef STRUCTS_H
#define STRUCTS_H
#include "computation.h"
#include <pbc/pbc.h>

enum {
    l_subpolynomial = 0,
    r_subpolynomial = 1,
    o_subpolynomial = 2,
    z_subpolynomial = 3,
    total_encrypted = 4
};

enum {
    L_major = 0,
    R_major = 1,
    O_major = 2,
};

typedef const unsigned int degree;

// contains provers evaluations at s
typedef element_t (*crypted)[length];
// encrypted is a pointer to an element pointer array
// conventionally, contains evaluations of lroz subpolynomials at s
typedef element_t *((*encrypted)[length]);
// the matrix of operand subpolynomials
typedef long long matrix[length][operations];
typedef long long **ptrmatrix;
// Polynomials LRO
typedef long long majors[3][operations];
// stores all subpolynomials, pointer to a matrix
typedef matrix(*subpolynomials);

typedef struct s_degrees {
    degree p;
    degree h;
    degree t;
} Degrees;

typedef struct s_proving {
    element_t **enc_tau; // right
    encrypted subpolynomials;
    encrypted shifted;
    long long **t;
    const subpolynomials lro;
    // new
    element_t *enc_alpha;    // alpha
    element_t *enc_beta;     // beta
    element_t **enc_tau_new; // x
    /* (bu(tau)+av(tau)+w(tau))/rho in l+1 to m*/
    crypted prover_lro_evaluations;
    /* (bu(tau)+av(tau)+w(tau))/gam in 0 to l*/
    element_t **verifier_lro_evaluations;
    /* x*t(tau)/rho in 0 to n-2*/
    element_t **trho;
} Proving;

typedef struct s_verification {
    encrypted subpolynomials_v;
    long long to;
    const int beta;
    element_t **enc_alphas;
} Verification;

typedef struct s_setup {
    Proving proving;
    Verification verification;
} Setup;

// Stores operand polynomials
typedef struct s_capitals {
    element_t L;
    element_t R;
    element_t O;
    element_t Z;
    element_t *P[3];
} Capitals;

typedef struct s_proof {
    element_t *H; // right
    Capitals *LROZ;
    Capitals *shifted;
} Proof;

// FIXME
// static const matrix evaluations[3] EVALUATIONS;
#endif // !STRUCTS_H
