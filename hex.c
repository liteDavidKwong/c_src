#include <stdio.h>

int main (int argc, char** argv)
{
  int value;
  short int value2;

  if (argc != 2)
  {
    fprintf( stderr, "USAGE: %s <hexadecimal value>\n", argv[0] );
    return -1;
  }

  sscanf( argv[1], "%0x", &value );
  value2 = value;
  printf( "Hex: %0x Dec: %0d Dec16: %0d\n", value, value, value2 );

  return 0;
}
