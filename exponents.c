#include "exponents.h"
#include "inits.h"
#include <stdio.h>
mpz_t a;

void init_a(void) { mpz_init(a); }

void set_a(long long v) {
    if (v >= 0) {
        mpz_set_si(a, v);
    } else {
        mpz_set_str(a, "730750818665451621361119245571504901405976559617",
                    10); // pairing a
        // mpz_set_str(a, "208617601094290618684641029477488665211553761021",
        // 10);//pairing d
        mpz_sub_ui(a, a, -v);
    }
}

element_t *shift(element_t *element, long long value) {
    set_a(value);
    element_pow_mpz(*element, *element, a);
    VALUE(element) *= value;

    return element;
}
// does not modify either, creates an element
element_t *add2(element_t left, element_t right) {
    element_t *result = malloc(sizeof(element_t));
    element_init_same_as(*result, left);
    element_mul(*result, left, right);
    VALUE(result) = left[0].value + right[0].value;

    return result;
}

// modifies the left operand
element_t *add(element_t *result, element_t multiplier) {
    element_mul(*result, *result, multiplier);
    VALUE(result) += multiplier[0].value;
    return result;
}
// exponentiate element by value
element_t *exponentiate(element_t element, const long long value) {
    element_t *result = malloc(sizeof(element_t));
    init_G1_valueless(result);
    set_a(value);
    element_pow_mpz(*result, element, a);
    VALUE(result) = element[0].value * value;

    return result;
}

// encrypts y by adding g to itself y times, elliptic curve
element_t *encrypt(long long value) {
    element_t *result = malloc(sizeof(element_t));
    init_G1_valueless(result);
    set_a(value);
    element_pp_pow(*result, a, G1);
    VALUE(result) = value;

    return result;
}
// encrypts y by adding g to itself y times, elliptic curve
void encrypt_t(element_t e, long long value) {
    element_init_G1(e, pairing);
    set_a(value);
    element_pp_pow(e, a, G1);
    e[0].value = value;
    printf("%lld", e[0].value);
    return;
}
void dea(void) { mpz_clear(a); }
