#include <stdio.h>

int main( int argc, char** argv )
{
  FILE* filein;
  char fifo[2];
  int indent;
  int valid;
  int newline;
  int charin;
  int i;

  if (argc != 2)
  {
    fprintf( stderr, "USAGE: %s <input file>\n", argv[0] );
    return -1;
  }

  if ((filein = fopen( argv[1], "r" )) == NULL)
  {
    fprintf( stderr, "%s: Could not open %s for reading.\n", argv[0], argv[1] );
    return -1;
  }

  indent = 0;
  valid = 0;
  while ((charin = fgetc( filein )) != EOF)
  {
    valid++;
    fifo[1] = fifo[0];
    fifo[0] = charin;

    if (valid >= 2)
    {
      if ((fifo[1] == '-') && (fifo[0] == '>'))
      {
        indent += 2;
        valid = 0;
      }
      else if ((fifo[1] == '<') && (fifo[0] == '-'))
      {
        indent -= 2;
        valid = 0;
      }
    }

    if (valid >= 2)
    {
      valid--;
      if ((fifo[1] == ';') || (fifo[1] == '\n'))
      {
        newline = 1;
      }
      else
      {
        if (newline == 1)
        {
          newline = 0;
          printf( "\n" );
          for (i=0; i<indent; i++)
            printf( " " );
        }
        printf( "%c", fifo[1] );
      }
    }
  }

  for (;valid>0;valid--)
  {
    if (newline == 1)
    {
      newline = 0;
      printf( "\n" );
      for (i=0; i<indent; i++)
        printf( " " );
    }
    printf( "%c", fifo[valid-1] );
  }

  return 0;
}
