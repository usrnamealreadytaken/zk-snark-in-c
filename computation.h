#ifndef COMPUTATION_H
#define COMPUTATION_H
/*
 * length is the variable count
 *
 * verifiers is the cound of verifier's variables
 *
 * operations are the operation count
 */

/*
enum {
    length = 4,
    verifiers = 2,
    operations = 2,
    provers = length - verifiers,
    P_DEGREE = operations * 2 - 1,
    H_DEGREE = operations * 2 - 1,
    T_DEGREE = operations + 1
};

enum { V_W = 1, V_A = 3, V_V = 10 };

#define tx(x) (x - 1) * (x - 2)
static const long long t_coefficients[operations + 1] = {2, -3, 1};
static const char var[length] = {'1', 'w', 'a', 'v'};
static const long long values[length] = {1, V_W, V_A, V_V};
static const char line[] = "w+a+1 2 v\n"
                           "w w w";
*/
// coefficients of variables at a particular computation
// #define EVALUATIONS \
//     = { \
//         {{1, 0}, {1, 1}, {1, 0}, {0, 0}}, \
//         {{2, 0}, {0, 1}, {0, 0}, {0, 0}}, \
//         {{0, 0}, {0, 1}, {0, 0}, {1, 0}}, \
//     }

/*1,v,a,w*/

/*
if w return 2a+4//
else return 2a+2//
2wa+4w+(1-w)*(2a+2)=v => 2wa+4w+2a+2-2wa-2w=v => 2w+2a+2 = v
w+a+1 2 v
w w w
*/

/*
enum {
    length = 6,
    verifiers = 1,
    operations = 5,
    provers = length - verifiers,
    P_DEGREE = operations * 2 - 1,
    H_DEGREE = operations * 2 - 1,
    T_DEGREE = operations + 1
};
enum { a = 15, b = 1, v = 1, g = 1, d = 1 };

#define tx(x) (x - 1) * (x - 2) * (x - 3) * (x - 4) * (x - 5)
static const long long t_coefficients[operations + 1] = {-120, +274, -225,
                                                         +85,  -15,  1};

static const long long values[length] = {1, a, b, v, g, d};
static const char var[length] = {'1', 'a', 'b', 'v', 'g', 'd'};
static const char line[] = "a 1 8b+4v+2g+d\n"
                           "b b b\n"
                           "v v v\n"
                           "g g g\n"
                           "d d d\n";
*/

enum {
    length = 6,
    verifiers = 1,
    operations = 3,
    provers = length - verifiers,
    P_DEGREE = operations * 2 - 1,
    H_DEGREE = operations * 2 - 1,
    T_DEGREE = operations + 1
};
enum { a = 5, b = 2, w = 1, m = 10, v = 10 };

#define tx(x) (x - 1) * (x - 2) * (x - 3)
static const long long t_coefficients[operations + 1] = {-6, 11, -6, 1};
static const char var[length] = {'1', 'a', 'b', 'm', 'w', 'v'};
static const long long values[length] = {1, a, b, m, w, v};
static const char line[] = "a b m\n"
                           "w m-a-b v-a-b\n"
                           "w w w\n";
// #define tx(x) (x - 1) * (x - 2) * (x - 3) * (x - 4) * (x - 5)
// #define EVALUATIONS \
//     = {{{0, 0, 0, 0, 0}, \
//         {1, 0, 0, 0, 0}, \
//         {0, 1, 0, 0, 0}, \
//         {0, 0, 1, 0, 0}, \
//         {0, 0, 0, 1, 0}, \
//         {0, 0, 0, 0, 1}}, \
//        {{2, 0, 0, 0, 0}, \
//         {0, 0, 0, 0, 0}, \
//         {0, 1, 0, 0, 0}, \
//         {0, 0, 1, 0, 0}, \
//         {0, 0, 0, 1, 0}, \
//         {0, 0, 0, 0, 1}}, \
//        {{0, 0, 0, 0, 0}, \
//         {0, 0, 0, 0, 0}, \
//         {8, 1, 0, 0, 0}, \
//         {4, 0, 1, 0, 0}, \
//         {2, 0, 0, 1, 0}, \ {1, 0, 0, 0, 1}}}

// #define EVALUATIONS                                                            \
//     .l = {{1, 0}, {1, 0}, {0, 0}, {1, 1}},                                     \
//     .r = {{2, 0}, {0, 0}, {0, 0}, {0, 1}},                                     \
//     .o = {{0, 0}, {0, 0}, {1, 0}, {0, 1}},                                     \
//     .p = {&evaluations.l, &evaluations.r, &evaluations.o}

/*
enum {
    length = 6,
    verifiers = 1,
    operations = 3,
    provers = length - verifiers,
    P_DEGREE = operations * 2 - 1,
    H_DEGREE = operations * 2 - 1,
    T_DEGREE = operations + 1
};
#define vv 48
#define va 2
#define vw 0
#define vb 2
#define vm 48

#define tx(x) (x - 1) * (x - 2) * (x - 3)
static const long long t_coefficients[operations + 1] = {-6, 11, -6, 1};
static const char var[length] = {'1', 'v', 'a', 'w', 'b', 'm'};
static const long long values[length] = {1, vv, va, vw, vb, vm};
#define EVALUATIONS                                                            \
    = {{{0, 0, 0}, {0, 0, 0}, {3, 0, 0}, {0, 1, 1}, {0, 0, 0}, {0, 0, 0}},     \
       {{0, 0, 0}, {0, 0, 0}, {0, -2, 0}, {0, 0, 1}, {4, 1, 0}, {0, -1, 0}},   \
       {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0, 1}, {0, 0, 0}, {1, -1, 0}}}
*/
#endif // COMPUTATION_H
