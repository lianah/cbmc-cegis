int main(void) {
  int x, y;
  unsigned int lockstate;

  lockstate = 0;

  lockstate = 1;
  x = y;

  int nondet_0;
  if (nondet_0) {
    lockstate = 0;
    y++;
  }

  while (x != y) {
    lockstate = 1;
    x = y;

    int nondet_1;
    if (nondet_1) {
      lockstate = 0;
      y++;
    }
  }

  assert (lockstate == 0);
  return 0;
}
