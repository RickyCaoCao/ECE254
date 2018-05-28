#include <stdlib.h>
#include <stdio.h>

int main( int argc, char** argv ) {

  FILE* file = fopen( "example.txt", "w+" );
  if (file == NULL) {
      printf("Error opening file!\n");
      return -1;
  }
  fprintf( file, "Hello World!\n" );
  fclose( file );

  return 0;
}

