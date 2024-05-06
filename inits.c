#include "inits.h"

pairing_t pairing;
element_t G1_raw;
// G1 for exponentiation purposes
element_pp_t G1;
// pairing already initialized with G1 as an argument
pairing_pp_t g1_pairing;

void init_G1_valueless(element_t *element) {
    element_init_G1(*element, pairing);
}
void init_G1(element_t *element) {
    element_init_G1(*element, pairing);
    VALUE(element) = 0;
}
void init_GT(element_t *element) {
    element_init_GT(*element, pairing);
    VALUE(element) = 0;
}
