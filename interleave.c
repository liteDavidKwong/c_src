#include <stdio.h>

int main ( int argc, char** argv )
{
  FILE* filein[32];
  int i;
  int charin;
  int not_empty;

  if ((argc < 2) || (argc > 33))
  {
    fprintf( stderr, "USAGE: %s <file 1> <file 2> [<file 3> .. <file 32>]\n", argv[0] );
    return -1;
  }

  for (i=0; i<(argc-1); i++)
  {
    filein[i] = fopen( argv[i+1], "r" );
    if (filein[i] == NULL)
    {
      fprintf( stderr, "%s: ERROR, could not open %s for reading.\n", argv[0], argv[i+1] );
      return -1;
    }
  }

  not_empty = 1;
  while (not_empty == 1)
  {
    not_empty = 0;
    for (i=0; i<(argc-1); i++)
    {
      do
      {
        charin = fgetc( filein[i] );
        if (charin != EOF)
        {
          not_empty = 1;
          printf( "%c", charin );
        }
      }
      while ((charin != EOF) && (charin != '\n'));
    }
  }

  return 0;
}
