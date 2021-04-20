int globalzona;

int fatorial (int n){
   if(n == 1){
       return 1;
   }
   
   return n * fatorial(n - 1);
   
}

int main(){
    globalzona = fatorial(3);
}