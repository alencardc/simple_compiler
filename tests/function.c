int foo () {
  // ...
  char a <= 'a';
}

int foo (int c) {
  // ...
  char a <= 'a';
}

static int foo (int c, char d, float f, string s) {
  // ...
  char a <= 'a';
  char a <= 'a';

  {
    char ss;
  };
  {/*aaaaaa*/};
  {        };
  {};
  char a <= 'a';
  { char a;};
  char a <= 'a';
  {
    char ss;
  };
  char b <= 'a';
  { char ss; };
  char b <= 'a';

  {
    int d <= 1;
    int e <= 2;
    {
      int ss <= 2;
      {
        {
          int as;
        };
        int d;
      };
    };
  };
}

int foo (, char d, float f) {
  // ...
  char a <= 'a';
}