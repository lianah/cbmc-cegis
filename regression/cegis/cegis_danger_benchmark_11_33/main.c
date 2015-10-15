int main(void) {
  int x;

  if (x < 100 || x > 200) {
    return 0;
  }

  while (x > 0) {
    x -= 2;
  }

  assert(x >= 0);
  return 0;
}
