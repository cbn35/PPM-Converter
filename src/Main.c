/* Main file for checking program arguments, and converting files
 * Author: Chance Nelson
 */


#include <stdlib.h>
#include <stdio.h>
#include "PPM.h"

int main(int argc, char ** argv) {
    char * errMsg = "ppmrw: convert between the PPM P3 and P6 formats\nUSAGE: ppmrw 3/6 <IMPUT FILE> <OUTPUT FILE>\n";
   
    // Check for wrong number of args 
    if(4 > argc < 4) {
        printf("ERROR: incorrect number of args\n");
        printf(errMsg);
        return 0;
    }

    // Check that input file exists
    FILE * testOpenInput = fopen(argv[2], "r");
    if(testOpenInput == NULL) {
        printf("ERROR: Input file could not be opened\n");
        printf(errMsg);
        return 0;
    } else {
        fclose(testOpenInput);
    }

    // Check that output file doesn't exist
    FILE * testOpenOutput = fopen(argv[2], "r");
    if(testOpenOutput != NULL) {
        printf("ERROR: Output file already exists\n");
        printf(errMsg);
    }

    FILE * input = fopen(argv[2], "r");
    int * metadata = get_ppm_file_information(input);
    
    // If the header has an incorrect magic number, abort
    if(metadata[0] != 3 && metadata[0] != 6) {
        printf("ERROR: Invalid PPM format in header\n");
        printf(errMsg);
        fclose(input);
        return 0;
    }

    // If the image dimensions don't make any sense, abort
    if(metadata[1] < 1 || metadata[2] < 1) {
        printf("ERROR: Invalid file size in header\n");
        printf(errMsg);
        fclose(input);
        return 0;
    }

    // If the max RGB value doesn't make any sense, abort
    if(1 > metadata[3]  || metadata[3] > 255) {
        printf("ERROR: Invalid Max RGB value in header\n");
        printf(errMsg);
        fclose(input);
        return 0;
    }

    int * pixmap = read_image(input);  // Get the pixmap for the file
    
    // Move the file pointer for input to the beginning of the pixel data
    rewind(input);
    get_ppm_file_information(input);

    // Get a count of the number of RGB values in the file
    int count = 1;
    buffer[0] = fgetc(input);
    while(buffer[0] != EOF) {
        if(buffer[0] == ' ') count++;
        buffer[0] = fgetc(input);
    }

    // If the pixmap array doesn't match the number of actual RGB values, abort
    if(count != (metadata[1] * metadata[2] * 3)) {
        printf("ERROR: Image dimensions do not match header values\n");
        printf(errMsg);
        fclose(input);
        free(pixmap);
    }
}
