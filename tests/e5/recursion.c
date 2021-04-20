int g;

int fatorial(int n) {
  if (n == 1) {
    return 1;
  }
  // int b <= 999;
  // int d;
  // int e <= a;
  // int c <= 998;
  return fatorial(n-1)*n;
}
int main () {
  g = fatorial(3);
  return 25;
}