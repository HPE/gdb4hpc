// Test code for noargs. For a description of the test see noargs.exp

int
inc (int i)
{
  return i+1;
}

int
fib (int n)
{
  if (n <= 1)
    return n;

  return fib(n-2) + fib(n-1);
}

int
main (int argc, char* argv[])
{
  int i, j;

  for (i = 0; i < 10; i++)
    j += inc(i);

  j += fib(3); /* bp.1 */
  return j; /* bp.2 */
}

