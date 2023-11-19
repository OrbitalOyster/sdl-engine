/*
 * Linear-feedback shift register implementation
 */

#ifndef LFSR_H
#define LFSR_H

#include <stdint.h>

#define LFSR_TYPE uint32_t

LFSR_TYPE getNextLFSR(LFSR_TYPE *state, LFSR_TYPE taps);

#endif /* LFSR_H */
