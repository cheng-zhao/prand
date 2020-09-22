/*******************************************************************************
* randms.c: this file is part of the randms library.
 
* randms: C library for generating random numbers with multiple streams.

* Github repository:
        https://github.com/cheng-zhao/randms

* Copyright (c) 2020 Cheng Zhao <zhaocheng03@gmail.com>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*******************************************************************************/

#include "randms.h"
#include "mrg32k3a.h"
#include "mt19937.h"
#include <stdlib.h>

/******************************************************************************
Function `randms_init`:
  Initialisation of the interface for the selected random number generator.
Arguments:
  * `type`:     the ID of the pre-defined random number generator;
  * `seed`:     an integer for initalisation the generator;
  * `nstream`:  total number of streams;
  * `step`:     step size for jumping ahead;
  * `err`:      an integer for storing the error message.
Return:
  A universal interface of the random number generator.
******************************************************************************/
randms_t *randms_init(const randms_rng_enum type, const uint64_t seed,
    const unsigned int nstream, const uint64_t step, int *err) {
  *err = 0;
  switch (type) {
    case RANDMS_RNG_MRG32K3A:
      return mrg32k3a_init(seed, nstream, step, err);
    case RANDMS_RNG_MT19937:
      return mt19937_init(seed, nstream, step, err);
    default:
      *err = RANDMS_ERR_UNDEF_RNG;
      return NULL;
  }
}

/******************************************************************************
Function `randms_errmsg`:
  Produce error message for a given error code.
Arguments:
  * `err`:      the error code
Return:
  A string with error message.
******************************************************************************/
char *randms_errmsg(const int err) {
  switch (err) {
    case 0:
      return "no error";
    case RANDMS_ERR_MEMORY:
      return "failed to allocate memory for the random number generator";
    case RANDMS_ERR_MEMORY_JUMP:
      return "failed to allocate memory for jumping ahead";
    case RANDMS_ERR_STEP:
      return "the step size for jumping ahead is too large";
    case RANDMS_ERR_UNDEF_RNG:
      return "the type of the random number generator is undefined";
    case RANDMS_WARN_SEED:
      return "invalid seed value";
    default:
      return "undefined error code";
  }
}

/******************************************************************************
Function `randms_destroy`:
  Release memory allocated for the random number generator interface.
Arguments:
  * `rng`:      the instance of the random number generator.
******************************************************************************/
void randms_destroy(randms_t *rng) {
  free(rng->state);
  if (rng->nstream > 1) free(rng->state_stream);
  free(rng);
}

