int g;

int fib(int n) {
   if (n <= 1) {
      return n;
   }
   return fib(n-1) + fib(n-2);
}
 
int main () {
  int n <= 12;
  g = fib(n);
  return 0;
}