#include <stdio.h>

int main ( int argc, char** argv )
{
  FILE* filein;
  int charin;
  int convert_ascii;
  int ascii_state;
  int ascii_value;

  char line[2048];
  int cursor;

  if (argc != 2)
  {
    fprintf( stderr, "USAGE: %s <input file>\n", argv[0] );
    return -1;
  }

  filein = fopen( argv[1], "r" );
  if (filein == NULL)
  {
    fprintf( stderr, "%s: Could not open %s for reading.\n", argv[0], argv[1] );
    return -1;
  }

  convert_ascii = 0;
  ascii_state = 0;
  cursor = 0;
  while ((charin = fgetc(filein)) != EOF)
  {
    printf( "%c", charin );  // Echo input
    if (convert_ascii == 1)
    {
      if (((charin >= '0') && (charin <= '9')) ||
          ((charin >= 'A') && (charin <= 'F')) ||
          ((charin >= 'a') && (charin <= 'f')))
      {
        if (charin >= 'a')
          ascii_value += (charin - 'a') + 10;
        else if (charin >= 'A')
          ascii_value += (charin - 'A') + 10;
        else
          ascii_value += charin - '0';

        if (ascii_state == 1)
        {
          ascii_state = 0;
          printf( "\"%c\"", ascii_value );  // Display ASCII character
          if (cursor < 2047)
          {
            line[cursor++] = (char) ascii_value;
            if (ascii_value == 0x0d)
              line[cursor++] = 10;
          } else {
            fprintf( stderr, "%s: Full ASCII line size too small, program needs modifications.", argv[0] );
            return -1;
          }
          ascii_value = 0; 
        }
        else
        {
          ascii_state = 1;
          ascii_value <<= 4;
        }
      }
    }

    if (charin == '[')
    {
      convert_ascii = 1;
      ascii_state = 0;
    }
    else if (charin == ']')
    {
      convert_ascii = 0;
    }
  }

  line[cursor] = '\0';
  printf( "\nFull ASCII output:\n\"%s\"\n", line );

  return;
};
