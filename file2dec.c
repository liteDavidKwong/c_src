#include <stdio.h>

int main (int argc, char** argv)
{
  if (argc != 3)
  {
    fprintf( stderr, "Usage: %0s <file in> <file out>\n", argv[0] );
    return -1;
  }

  FILE* filein;
  FILE* fileout;

  filein = fopen( argv[1], "r" );
  if (filein == NULL)
  {
    fprintf( stderr, "%0s: Error, unable to open %0s for reading.\n", argv[0], argv[1] );
    return -1;
  } 
  fileout = fopen( argv[2], "w" );
  if (fileout == NULL)
  {
    fprintf( stderr, "%0s: Error, unable to open %0s for writing.\n", argv[0], argv[2] );
    return -1;
  }

  int charin;
  int i;
 
  i = 0;
  while ((charin = fgetc(filein)) != EOF)
  {
    i = i + 1;
    fprintf(fileout, "%03d", charin );
    if (i%20 == 0)
      fprintf(fileout, "\n");
  }

  fclose( filein );
  fclose( fileout );

  return 0;
}
