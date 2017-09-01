/* PPM helper library
 * Author: Chance Nelson
 */

#include <stdio.h>
#include "ppm.c"


FILE * create_ppm_p3(char * name, int width, int height, int max);  // creates and prepares a new p3 file
FILE * create_ppm_p6(char * name, int width, int height, int max);  // creates and prepares a new p6 file
void write_pixel(int r, int g, int b, FILE * file);                 // writes a pixel to a ppm file
FILE * convert_p3_to_p6(char * name, FILE * p3)                     // convert from p3 to p6
FILE * convert_p3_to_p3(char * name, FILE * p6)                     // convert from p6 to p3
