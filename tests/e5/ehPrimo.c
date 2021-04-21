int resto(int numerador, int denominador){
    int res_div;

    res_div = numerador / denominador;
    res_div = res_div * denominador;

    return numerador - res_div;
}

int isNumeroPrimo(int a){
    int i;
    for( i = 2: i < a : i = i + 1){
        if(resto(a, i) == 0){
            return 0;
        }
    }
    return 1;
}

int ehPrimo;
int main(){
    ehPrimo = isNumeroPrimo(5);
}