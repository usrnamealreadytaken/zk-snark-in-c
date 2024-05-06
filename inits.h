#ifndef INITS_H
#define INITS_H
#include <pbc/pbc.h>
#define VALUE(element) (*element)[0].value

extern pairing_t pairing;
extern element_t G1_raw;
extern element_pp_t G1;
extern pairing_pp_t g1_pairing;

void init_G1(element_t *element);
void init_GT(element_t *element);
void init_G1_valueless(element_t *element);
#endif // INITS_H
