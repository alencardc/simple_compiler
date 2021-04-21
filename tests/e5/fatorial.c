int globalzona;

int fatorial (int n){
   if(n == 1){
       return 1;
   }
   
   return fatorial(n - 1) * n;
   
}

int main(){
    globalzona = fatorial(3);
}