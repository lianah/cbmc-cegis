int main(void) {
  unsigned int x, y;

  x = 0;
  y = 1;

  while (x < 1000000) {
    x++;

    int nondet_0;
    if (nondet_0) {
      y++;
    }
  }

  assert(x != y);
  return 0;
}
