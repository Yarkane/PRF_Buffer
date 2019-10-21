/*
 * Random Number Generator
 */

#include <stdio.h>
#include <math.h>

#define RNG_M   2147483647     /* 2^31-1 */
#define RNG_A   16807          /* 7^5 */

/*
 * MINSTD, a Park-Miller random number generator
 *
 * It's a linear congruential generator of pseudorandom sequences
 * of numbers, uniformly distributed in [1,RNG_M], with relation
 * X(n+1) = RNG_A * X(n) % RNG_M
 *
 * Implementation note: to compute x = (7^5 * x) mod (2^31 - 1)
 * wihout overflowing 31 bits, we exploit the fact that
 * (2^31 - 1) = 127773 * (7^5) + 2836
 *
 * From "Random number generators: good ones are hard to find",
 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
 * October 1988, p. 1195
*/

long rng(long x) {
  long hi, lo;

  // can't be initialized with 0, so use another value.
  if(x == 0) x = 123456789;
  hi = x / 127773;
  lo = x % 127773;
  x = RNG_A * lo - 2836 * hi;
  if(x < 0) x += RNG_M;
  return(x);
}

/*
 * Uniform distribution in [0,1]
 *
 * range [0,1]
 * f(x) = 1
 * F(x) = x
 */
double uniform(long *seed) {
	double u;

	*seed = rng(*seed);
	u = (double)(*seed) / RNG_M;
	return(u);
}

/*
 * Uniform distribution in [a,b]
 *
 * range [a,b]
 * f(x) = 1/(b-a)
 * F(x) = (x-a)/(b-a)
 */
double uniform2(double a, double b, long *seed) {
	double u;

	u = uniform(seed);
	u = a + u * (b-a);
	return(u);
}

/*
 * Exponential distribution with parameter lambda
 *
 * range [0,inf]
 * f(x) = lambda * exp(-lambda*x)
 * F(x) = 1 - exp(-lambda*x)
 */
double exponential(double lambda, long *seed) {
	double u;

	u = uniform(seed);
	u = -(1.0/lambda) * log(1.0-u);
	return(u);
}


