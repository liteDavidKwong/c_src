#include <stdio.h>

int main ( int argc, char** argv )
{
  FILE* filein1;
  FILE* filein2;
  int charin1, charin2;

  if (argc != 3)
  {
    fprintf( stderr, "USAGE: %s <file1> <file2>\n", argv[0] );
    return -1;
  }

  if ((filein1 = fopen( argv[1], "r" )) == NULL)
  {
    fprintf( stderr, "%s: Could not open %s for reading.\n", argv[1] );
    return -1;
  }
  if ((filein2 = fopen( argv[2], "r" )) == NULL)
  {
    fprintf( stderr, "%s: Could not open %s for reading.\n", argv[1] );
    return -1;
  }
 
  while ( ((charin1 = fgetc( filein1 )) != EOF) &&
          ((charin2 = fgetc( filein2 )) != EOF) )
  {
    printf( "%c", charin1 );

    if (charin1 != charin2)
    {
      printf( "\nDifference found: %s = 8'h%02x %s = 8'h%02x\n", argv[1], charin1, argv[2], charin2 );
      return -1;
    }
  }

  return 0;
}
