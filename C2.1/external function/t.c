/************t.c file*************/
extern int g(int x, int y); // an external function

int func(int a, int b, int c, int d, int e, int f)
{
    int x,y,z;  // local variable
    x = 1; y = 2; z = 3;
    g(x,y);     // call g(x,y)
    return a + e;
}