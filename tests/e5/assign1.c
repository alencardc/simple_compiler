int a;
int b;
int c;

int abc(int a, int b, int c){
    return 15;
}

int foo(int rfp12){
    int rfp20;
    int rfp24;
    int rfp28;
    
    int buffer;

    buffer = rfp12;
    buffer = rfp24;
    buffer = rfp20;

    abc(a + b, b + c, a + rfp20);
    // {
    //     int rfp16;
    //     buffer = rfp16;
    // };

    // int rfp20;
    // buffer = rfp20;
    // buffer = rfp20;
    // buffer = rfp20;
    // buffer = rfp20;
}