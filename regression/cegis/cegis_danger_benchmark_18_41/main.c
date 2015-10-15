int main(void) {
  unsigned int x, y;

  x = 0;
  y = 0;

  while (x < 1000003) {
    unsigned int nondet_0;
    if (nondet_0) {
      x++;
    }

    unsigned int nondet_1;
    if (nondet_1) {
      y++;
    }
  }

  assert(x != y);
  return 0;
}
