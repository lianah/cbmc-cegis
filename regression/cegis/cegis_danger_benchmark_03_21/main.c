int main(void) {
  unsigned int x = 0;
  unsigned int y = 1;

  //x = 0;
  //y = 1;

  while (x < 1000000) {
    x++;

    int nondet_0;
    if (nondet_0) {
      y++;
    }
  }

  __CPROVER_assert(x != y, "A");
  return 0;
}
