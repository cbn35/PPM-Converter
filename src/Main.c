/* Main file for checking program arguments, and converting files
 * Author: Chance Nelson
 */


#include <stdlib.h>
#include <stdio.h>
#include "PPM.h"

int main(int argc, char ** argv) {
    char * errMsg = "ppmrw: convert between the PPM P3 and P6 formats\nUSAGE: ppmrw 3/6 <IMPUT FILE> <OUTPUT FILE>\n";
   
    // Check for wrong number of args 
    if(4 > argc || argc < 4) {
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
    FILE * testOpenOutput = fopen(argv[3], "r");
    if(testOpenOutput != NULL) {
        printf("ERROR: Output file already exists\n");
        printf(errMsg);
        fclose(testOpenOutput);
        return 0;
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

    int count = 0;

    // Get a count of the number of RGB values in the file
    if(metadata[0] == 3) {
        char last = 0;
        char buffer = fgetc(input);
        while(buffer != EOF) {
            if(metadata[0] == 3) {
                if(buffer == '\n' && last != ' ') count++;
                
                if(buffer == ' ') count++;
            }
 
            last = buffer;
            buffer = fgetc(input);
        }
    }

    if(metadata[0] == 6) {
        unsigned char buffer[BUFFER_SIZE];
        count = fread(buffer, sizeof(unsigned char), sizeof(unsigned char) * BUFFER_SIZE, input);
        int temp = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, input);
        while(temp > 0) {
            count += temp;
            temp = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, input);
        }
    }

    // If the pixmap array doesn't match the number of actual RGB values, abort
    if(count != (metadata[1] * metadata[2] * 3)) {
        printf("WIDTH: %d\nHEIGHT: %d\n HEADER: %d ACTUAL: %d\n", metadata[1], metadata[2], (metadata[1] * metadata[2] * 3), count);
        printf("ERROR: Image dimensions do not match header values\n");
        printf(errMsg);
        fclose(input);
        free(pixmap);
        return 0;
    }

    FILE * out;
    if(atoi(argv[1]) == 3) out = create_ppm_p3(argv[3], metadata[1], metadata[2], metadata[3]);
    if(atoi(argv[1]) == 6) out = create_ppm_p6(argv[3], metadata[1], metadata[2], metadata[3]);

    for(int i = 0; i < metadata[1] * metadata[2] * 3; i += 3) {
        //printf("%d %d %d\n", pixmap[i], pixmap[i + 1], pixmap[i + 2]);  
        
        if (write_pixel(pixmap[i], pixmap[i + 1], pixmap[i + 2], out) == -1) {
            printf("ERROR: Fatal error writing to output file. Do you have permission to write?\n");
            printf(errMsg);
            fclose(out);
            fclose(input);
            free(pixmap);    
        }
    }
}
