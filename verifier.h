#ifndef VERIFIER_H
#define VERIFIER_H
#include "structs.h"
#include <pbc/pbc.h>
#include <stdbool.h>
extern pairing_t pairing;
extern pairing_pp_t g1_pairing;
extern element_pp_t G1;
extern element_t G1_raw;
int verifier(Proof proof, Verification keys);
#endif // !VERIFIER_H
