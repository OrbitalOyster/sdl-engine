// obj/utils/lfsr.o
// 0

#include "utils/lfsr.h"

#include "utils/debug.h"

#define TAPS 61680 // 1111000011110000
#define INITIAL_STATE 12345

int main() {
  LFSR_TYPE state = INITIAL_STATE;
  DTEST_UNIT_START("LFSR");
  DTEST_EXPECT_UINT(lfsr(&state, TAPS), (unsigned int)2618032128);
  DTEST_EXPECT_UINT(lfsr(&state, TAPS), (unsigned int)71304191);
  DTEST_EXPECT_UINT(lfsr(&state, TAPS), (unsigned int)4248912269);
  DTEST_EXPECT_UINT(lfsr(&state, TAPS), (unsigned int)1077936304);
  DTEST_EXPECT_UINT(lfsr(&state, TAPS), (unsigned int)1013546096);
  DTEST_UNIT_END;
}
