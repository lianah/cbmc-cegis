int main(void) {
  int x;

  x = 1000003;

  while (x > 0) {
    x -= 2;
  }

  assert(x >= 0);
  return 0;
}
