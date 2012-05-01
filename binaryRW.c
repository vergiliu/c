#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SECTOR_SIZE 512
// offset 8 | 8 | text

void prettyPrint(char *buffer, int length) {
    int i;
    char c;
    for (i=0;i<length;i++) {
        printf("%02X ", 0x000000FF & buffer[i]);
        if (i == 7) printf("  ");
    }

    if ( length != 16) for ( i=0; i < 16-length; i++ ) printf("__ ");
    printf(" | ");
    for ( i=0; i < length; i++) {
        c = buffer[i];
        if ( c > 0x1F &&  c < 0x7F) printf("%c",  c);
        else printf(".");
    }
}

int main(int argc, char *argv[]) {
    FILE *f;
    FILE *output, *input;
    char *p, myOption, myFile[128];
    long myOffset;
    long mySize;
    char buf[128];
    unsigned long offset = 0;
    unsigned long readElements, i;

    //printf("%s %s %s \n", argv[1], argv[2], argv[3], argv[4]);
    if (5 != argc) {
        printf("Usage: %16s %4s %8s %8s %16s\n", argv[0], "r/w",  "offset", "length", "file");
        exit(1);
    }
    myOffset = strtol(argv[2], &p, 16);
    mySize = strtol(argv[3], &p, 16);
    myOption = argv[1][0];
    strcpy(myFile, argv[4]);
    printf("%16s %4s %8s %8s %16s\n", "<run>", "r/w",  "offset", "length", "file");
    printf("%16s %4c %8lX %8lX %16s\n", argv[0], myOption, myOffset, mySize, myFile);

    if ( 'r' == myOption ) {
        f = fopen(myFile, "rb");
        if ( NULL == f ) {
            printf("file %s cannot be opened\n", myFile);
            exit(1);
        }
        fseek(f, myOffset, SEEK_SET);
        output = fopen("output.bin", "w+");
        offset = myOffset;
        while ( !feof(f) && ( ftell(f) < myOffset + mySize ) )  {
            readElements = fread(buf, 1, 16, f);
            fwrite(buf, 1, readElements, output);
            printf("%08lX ", offset);
            prettyPrint(buf, readElements);
            offset += readElements;
            if (0 == offset % 512 ) printf(" <<< [%04ld]", offset / SECTOR_SIZE);
            printf("\n");
        }
        printf("\n");
        fclose(f);
        fclose(output);
    }
    else
    if ( 'w' == myOption ) {
        input = fopen("input.bin", "rb");
        if ( NULL == input ) {
            printf("input file %s cannot be opened\n", "input.bin");
            exit(1);
        }
        output = fopen(myFile, "w+");
        if ( NULL == output ) {
            printf("output file %s cannot be opened\n", myFile);
            exit(1);
        }
        offset = fseek(output, myOffset, SEEK_SET);
        while ( !feof(input) && ( ftell(input) < mySize ) )  {
            printf("@ %08lX offset", offset);
            readElements = fread(buf, 1, 16, input);
            fwrite(buf, 1, readElements, output);
            offset += readElements;
            printf("\r");
            if (0 == offset % 0x1000) printf("@ %08lX offset\n", offset);
        }
        printf("\n");
        fclose(output);
        fclose(input);
        printf("written input.bin file to %s [%8ld]\n", myFile, offset);
    }
}
