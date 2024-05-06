#ifndef OPERANDS_H
#define OPERANDS_H
#include "structs.h"

encrypted Verifiers(subpolynomials lro);
encrypted evaluate(subpolynomials lro, const long long *powers);
crypted Provers(subpolynomials lro);
void display(subpolynomials lro);
void display_t(element_t **powers, long long *T[3], long long Tx);
subpolynomials polynomials(void);
#endif // OPERANDS_H
