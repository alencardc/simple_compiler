int globalzona;

int foo(int n) {
  int a;
  a = n + 1;
  return a;
}

int main() {
  int a;
  a = 123;
  globalzona = foo(999);
  return 25;
}