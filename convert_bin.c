#include <stdio.h>

int bit_reverse( int value );

int main (int argc, char** argv)
{
  FILE* filein;
  int nextval;
  int i;
  int value;

  if (argc != 2)
  {
    fprintf( stderr, "USAGE: %s <bin file>\n", argv[0] );
    return -1;
  }

  filein = fopen( argv[1], "r" );
  if (filein == NULL)
  {
    fprintf( stderr, "%s: ERROR, input file [%s] not found.\n", argv[0], argv[1] );
    return -1;
  }

  
  i = 0;
  while ((nextval = fgetc( filein )) != EOF)
  {
    if (i++ == 1)
    {
      i = 0;
      value = value + (nextval << 8);
      printf( "%0x\n", value );
    }
    else
      value = nextval;
  }

  return 0;
}

int bit_reverse( int value )
{
  int result;
  int i;

  result = 0;
  for (i=0; i<8; i++)
  {
    result <<= 1;
    result |= value & 1;
    value >>= 1;
  }

  return result;
}
