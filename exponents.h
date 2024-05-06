#ifndef EXPONENTS_H
#define EXPONENTS_H
#include <pbc/pbc.h>

void init_a(void);
void dea(void);
element_t *encrypt(long long y);
void encrypt_t(element_t e, long long value);
element_t *shift(element_t *element, long long value);
element_t *exponentiate(element_t element, long long value);
element_t *add2(element_t left, element_t right);
element_t *add(element_t *result, element_t multiplier);
#endif // !EXPONENTS_H
