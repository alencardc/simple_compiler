int a;

int relacionais(int a){
    if(a < 2 && a > 0){
        return 1;
    }
    else{
        return 2;
    }
}

int main(){
    int b;
    int c;

    b = relacionais(0); //2
    c = relacionais(1); //1
}