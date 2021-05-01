int resultado;

int fib(int n){
    if( n == 0 || n == 1){
        return n;
    }

    return fib(n - 1) + fib(n - 2);
}

int main(){
    resultado = fib(9);
}
//0,1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597
