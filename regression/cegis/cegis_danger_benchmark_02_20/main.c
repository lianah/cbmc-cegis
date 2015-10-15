int main(void) {
  unsigned int x, y;

  x = 0;
  y = 0;

  while (x < 1000003) {
    x++;

    int nondet_0;
    if (nondet_0) {
      y++;
    }
  }

  assert(x != y);
  return 0;
}
