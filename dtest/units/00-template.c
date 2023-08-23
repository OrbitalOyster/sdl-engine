//
// 0

int main() {
  DTEST_UNIT_START("Template test");
  DTEST_EXPECT_TRUE(2 + 3 == 5);
  DTEST_EXPECT_TRUE(1 + 1 == 3); // Supposed to fail
  DTEST_EXPECT_FALSE(1 * 2 == 3);
  DTEST_UNIT_END
  return 0;
}
