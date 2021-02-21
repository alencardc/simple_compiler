int testExpressions() {
  /*int test <= 123;
  float test <= 123.45;
  char test <= 'a';
  string test <= "aa";
  bool test <= false;

  int test2 <= test;

  int test3 <= (test);
  int test3 <= (12345);

  int test4 <= -test;
  int test4 <= -12345;
  int test4 <= -(test);
  int test4 <= -(*test);
  int test4 <= #123;
  int test4 <= &test;
  int test4 <= &(?test);

  int test5 <= !!test;
  int test5 <= *&test; */

  /*int test6 <= 123 * 2;
  int test6 <= 123 / 2;
  int test6 <= 123 % 2;
  int test6 <= 123 % test5;
  int test6 <= 123 % -test5;
  int test6 <= 123 % -(*test5);
  int test6 <= &test % -(*test5);
  */

  // Nesse exemplo: ele reconhece -test3, dps 123 e 2, entao 123 * 2 e
  // junta tudo fazendo  -test3 - 123 * 2
  // Depois reconhece &test, então *test5, daí -(*test5) e só entao (&test % -(*test5))
  // Termina reconhecedno a soma de  -test3 - 123 * 2 com (&test % -(*test5))
  // E por fim a declaração da variavel
  //int test6 <= -test3 - 123 * 2 + (&test % -(*test5));

  //int test6 <= -test3 * 2 + 1 <= test5; 
  
  // equational vem por ultimo 
  //int test6 <= test > test5 != test3; 
  //int test6 <= test / test5 < 2 == test3;
  //A exponenciacao vem primeiro amiguinho
  //int capitaoKirk <= 2 * 2 ^ 3; 
  //é o mesmo que (69 ^69) * (69 ^69)
  //int viihTubeCospeGato <= 69 ^ 69 * 69 ^ 69; 
  //int erradao <= 29 ^;

  //Testes bitwise
  //Primeiro equidade dps bitwise
  //int bitwise <= 63 & 9 == 7;
  //Primeiro and dps or
  //int bitwise_or_and <= 63 | 9 & 7;

  //Testes com logical and e or
   //int logicaland <= 63 || 9 && 7;
   //int logicalandBitwiseOr <= 63 | 9 && 7;
   //int logicalandBitwiseOr <= 63 & 8 | 9 && 7;

  
}