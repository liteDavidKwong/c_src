#include <stdio.h>

int main (int argc, char** argv)
{
  int dec;

  if (argc != 2)
  {
    fprintf( stderr, "USAGE: %s <decimal value>\n", argv[0] );
    return -1;
  }

  dec = atoi( argv[1] );
  printf( "%0x\n", dec );

  return 0;
}
