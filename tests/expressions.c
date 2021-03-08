int testExpressions() {
  test = 123;
  test = 123.45;
  //test = 'a'; // n pode atribuir char?
  //test = "aa"; // n pode atribuir string?
  //test = false; // n pode atribuir boolean?

  test2 = test;

  test3 = (test);
  test3 = (12345);

  test4 = -test;
  test4 = -12345;
  test4 = -(test);
  test4 = -(*test);
  test4 = #123;
  test4 = &test;
  test4 = &(?test);

  test5 = !!test;
  test5 = *&test; 

  test6 = 123 * 2;
  test6 = 123 / 2;
  test6 = 123 % 2;
  test6 = 123 % test5;
  test6 = 123 % -test5;
  test6 = 123 % -(*test5);
  test6 = &test % -(*test5);
  

  // Nesse exemplo: ele reconhece -test3, dps 123 e 2, entao 123 * 2 e
  // junta tudo fazendo  -test3 - 123 * 2
  // Depois reconhece &test, então *test5, daí -(*test5) e só entao (&test % -(*test5))
  // Termina reconhecedno a soma de  -test3 - 123 * 2 com (&test % -(*test5))
  // E por fim a declaração da variavel
  test6 = -test3 - 123 * 2 + (&test % -(*test5));

  test6 = -test3 * 2 + 1 <= test5; 
  
  // equational vem por ultimo 
  test6 = test > test5 != test3; 
  test6 = test / test5 < 2 == test3;
  //A exponenciacao vem primeiro amiguinho
  capitaoKirk = 2 * 2 ^ 3; 
  //é o mesmo que (69 ^69) * (69 ^69)
  viihTubeCospeGato = 69 ^ 69 * 69 ^ 69; 
  //erradao = 29 ^;

  //Testes bitwise
  //Primeiro equidade dps bitwise
  bitwise = 63 & 9 == 7;
  //Primeiro and dps or
  bitwise_or_and = 63 | 9 & 7;

  //Testes com logical and e or
   logicaland = 63 || 9 && 7;
   logicalandBitwiseOr = 63 | 9 && 7;
   logicalandBitwiseOr = 63 & 8 | 9 && 7;

  // Testes com o operador ternario
  ternary = 3 || 1+2 ? 42*2 : 1|2; 
  ternary = isMarried ? (sheIsHot ? youRock : youFuckedUp) : itsOK; 
  ternary = isMarried ? sheIsHot ? youRock : youFuckedUp : itsOK;

  //int ternary <= isMarried ? sheIsHot ? youRock : youFuckedUp : haveSex ? isOK : youNerdola;

  
   //int vetorzaoBrabo <= abc[vetorzaoBrabo[2] ^ 2] - 24;
   //int vetorzaumBrabissimo <= a[2 ^2] - 26 + 32;
  ternary = isMarried ? sheIsHot ? youRock : youFuckedUp : haveSex ? isOK : youNerdola;

  
   vetorzaoBrabo = abc[vetorzaoBrabo[2] ^ 2] - 24;
  vetorzaumBrabissimo = a[2 ^2] - 26 + 32;

  testFunction = incVar(test) * 2;
}