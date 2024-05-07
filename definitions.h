#ifndef DEFINITIONS_H
#define DEFINITIONS_H

static const long long tau = 8;   // x
static const long long alpha = 3; // shift

static const long long alpha_l = 1;
static const long long alpha_r = 2;
static const long long alpha_o = 3;
static const long long alphas[3] = {alpha_l, alpha_r, alpha_o}; // α - shifts

static const long long delta_l = 2;
static const long long delta_r = 3;
static const long long delta_o = 5;
static const long long deltas[3] = {delta_l, delta_r, delta_o}; // δ - shifts

static const long long u_alpha = 2;                 // α
static const long long u_beta = 7;                  // β - shifts
static const long long u_gamma = 6;                 // γ
static const long long u_delta = 4;                 // δ
static const long long ro_l = 4;                    // ρl
static const long long ro_r = 3;                    // ρr
static const long long ro_o = ro_l * ro_r;          // ρo
static const long long ros[3] = {ro_l, ro_r, ro_o}; // ρl
static const long long ut[3][3] = {
    {1, 1, 1},
    {alpha_l, alpha_r, alpha_o},
    {u_beta, u_beta, u_beta},
}; // ρl

static const long long baos[3] = {u_beta, u_alpha, 1};
static char *const sht[3] = {"", "*α", "*β"};
static char *const bao[3] = {"*β", "*α", ""};
static const char sub[4] = {'l', 'r', 'o', 'z'};
static const char opr[4] = {'L', 'R', 'O', 'Z'};
#define newEncrypted                                                           \
    malloc(sizeof(*results));                                                  \
    results->p[0] = results->l;                                                \
    results->p[1] = results->r;                                                \
    results->p[2] = results->o;

#define ForLRO for (unsigned int i = 0; i < 3; i++)

#define ForP for (unsigned int j = 0; j < p; j++)
#define ForPBackwards for (unsigned int j = p - 1; j-- > 0;)

#define ForVariables for (unsigned int j = 0; j < length; j++)
#define ForVariablesBackwards for (unsigned int j = length - 1; j-- > 0;)
// for (unsigned int j = length - 1; j > -1; j--)

#define ForVerifier for (unsigned int j = 0; j < verifiers; j++)
#define ForProver for (unsigned int j = verifiers; j < length; j++)

#define ForOperationsn for (unsigned int n = 0; n < operations; n++)
#define ForOperations for (unsigned int k = 0; k < operations; k++)
#define ForOperationsBackwards for (unsigned int k = operations; k-- > 0;)
// for (int k = operations - 1; k > -1; k--)

// static const unsigned int P_DEGREE = 4; // degree of p
// static const unsigned int H_DEGREE = 2; // degree of h
#define pairing_parameters                                                     \
    "type a\n\
q 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\n\
h 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\n\
r 730750818665451621361119245571504901405976559617\n\
exp2 159\n\
exp1 107\n\
sign1 1\n\
sign0 1\n"
#endif // !DEFINITIONS_H
