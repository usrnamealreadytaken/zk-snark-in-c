#include "computation.h"
#include "definitions.h"
#include "exponents.h"
#include "inits.h"
#include "prover.h"
#include "setup.h"
#include "structs.h"
#include "verifier.h"
const char *__asan_default_options(void) { return "detect_leaks=0"; }

char *hash1 = "eorn errd an iron urr";
char *hash2 = "rrr nrrd ean on irurr";

void prepair(void) {
    pairing_init_set_str(pairing, pairing_parameters);

    init_G1(&G1_raw);
    element_from_hash(G1_raw, hash1, 21);
    element_pp_init(G1, G1_raw);
    pairing_pp_init(g1_pairing, G1_raw, pairing);
    // element_clear(G1_raw);
}

void clearup(void) {
    element_pp_clear(G1);
    pairing_pp_clear(g1_pairing);
    dea();
}

// the entire algorithm
int zksnark(void) {
    printf("s=%lld, αlro=%lld,%lld,%lld, β=%lld, γ=%lld, ρl=%lld, ρr=%lld, "
           "ρo=%lld\n",
           tau, alphas[0], alphas[1], alphas[2], u_beta, u_gamma, ro_l, ro_r,
           ro_o);
    static const Degrees degrees = {
        .p = P_DEGREE, .h = H_DEGREE, .t = T_DEGREE};

    prepair();
    init_a();
    // Setup
    puts("-------Setup-------");
    const Setup keys = setup(degrees);
    // Prover
    puts("\033[37m------Proving-------");
    const Proof proof = prover(keys.proving, degrees);
    // Verifier
    puts("\033[37m----Verification----");
    const bool result = verifier(proof, keys.verification);
    if (result) puts("\033[32mVerified successfully");
    else puts("\033[35false proof");
    clearup();
    return 0;
}

int main(void) {
    zksnark();
    return 0;
}
