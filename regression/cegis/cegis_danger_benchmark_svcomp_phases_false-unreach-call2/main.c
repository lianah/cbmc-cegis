int main(void) {
  unsigned int x = 1;
  unsigned int y;

  if (y <= 0) return 0;

  while (x < y) {
    if (x < y / x) {
      x *= x;
    } else {
      x++;
    }
  }

  __CPROVER_assert(x != y, "A");
  return 0;
}
