
int fibo(int a) 
{
  if (a==1) return 1;
  return a*fibo(a-1);
}
int main() 
{
  int m,n,i;
  m = read();
  i = 1;
  write(m);
  while(i<=m)
  {
    n = fibo(i);
    write(n);
    i=i+1;
  }
  return 1;
}