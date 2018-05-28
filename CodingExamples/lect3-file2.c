#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void readfile( FILE* file );

int main( int argc, char** argv ) {
    if ( argc != 2 ) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }
    FILE* f = fopen( argv[1], "r");
    if ( f == NULL ) {
        printf("Unable to open file! %s is invalid name?\n", argv[1] );
        return -1;
    }
    readfile( f );
    fclose( f );
    return 0;
}

void readfile( FILE* file ) {
    char** line = malloc( sizeof( char* ) );
    ssize_t* length = malloc( sizeof( ssize_t ) );
    
    while ( 1 ) {
        int read = getline(line, length, file);
        if (read == -1 ) {
            break;
        }
        printf("%s", *line);
    }
    printf("\nEnd of File.\n");
    free( length );
    free( line );
}
