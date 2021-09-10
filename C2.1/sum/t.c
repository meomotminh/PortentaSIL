int g;                          // un-initialized global

int main()
{
    int a, b, c, d, e, f;       // local variables
    a = b = c = d = e = f = 1;  // values do not matter
    g = sum(a,b,c,d,e,f);       // call sum(), passing a,b,c,d,e,f
}



