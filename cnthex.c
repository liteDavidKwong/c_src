#include <stdio.h>

int main ( int argc, char** argv )
{
  int i, start, end, incr;
  int j, rep;

  if ((argc < 3) || (argc > 5))
  {
    fprintf( stderr, "USAGE: %s <start> <end> [increment] [repetition]\n", argv[0] );
    return -1;
  }

  start = atoi( argv[1] );
  end = atoi( argv[2] );
  if (start < end)
    incr = 1;
  else
    incr = -1;
  rep = 1;

  if (argc == 4)
  {
    incr = atoi( argv[3] );
  }

  if (argc == 5)
  {
    rep = atoi( argv[4] );
  }

  if ( ((start <= end) && (incr <= 0)) ||
       ((start >= end) && (incr >= 0)) )
  {
    fprintf( stderr, "%s: ERROR, increment using wrong sign.\n", argv[0] );
    return -1;
  }

  for (j=0; j<rep; j++)
  {
    if (start < end)
    {
      for (i=start; i<=end; i+=incr)
      {
        if (rep != 1)
          printf( "%04x, ", j );
        printf( "%04x\n", i );
      }
    }
    else
    {
      for (i=start; i>=end; i+=incr)
      {
        if (rep != 1)
          printf( "%04x, ", j );
        printf( "%04x\n", i );
      }
    }
  }

  return 0;  
}  
