/* Implementation of PPM.h
 * Handles the creation and editing of ppm files for other programs
 * Author: Chance Nelson
 */


#define BUFFER_SIZE 255


FILE * create_ppm_p3(char * name, int width, int height, int max) {
    /* Desc: Creates a PPM file with required header
     * Args:
     *      name (char*): name/path of ppm file
     *      width  (int): width of file
     *      height (int): height of file
     *      max    (int): max value of color value in RGB (eg: 255)
     * Returns (FILE*): File pointer to the P3 file
     */
    FILE * out = fopen(name, "w");
    fprintf(out, "P3\n");
    fprintf(out, "%d\n", width);
    fprintf(out, "%d\n", height);
    fprintf(out, "%d\n", max);
    
    return out;    
}


FILE * create_ppm_p6(char * name, int width, int height, int max) {
    /* Desc: Creates a PPM file with a P6 header
     * Args:
     *      name (char*): name/path of file
     *      width  (int): width of image
     *      height (int): height of image
     *      max    (int): max rgb value for each pixel (eg: 255)
     * Returns (FILE*): File pointer to the P6 file 
     */
    FILE * out = fopen(name, 'w');
    fprintf(out, "P6\n");
    fprintf(out, "%d %d\n", width, height);
    fprintf(out, "%d", max);
    
    return out;
}


void write_pixel(int r, int g, int b, FILE* file) {
    /* Desc: Writes a pixel to a PPM file
     * Args:
     *      r, g, b (int)  : RGB values for the pixel
     *      file    (FILE*): File to write to
     */

    // Get the filetype. Currently P3 and P6 have support
    char type[2];
    rewind(file);
    fgets(type, 2, file);

    fseek(file, 0, SEEK_END);

    if(type == "P3") {
        fprintf(file, "%d %d %d\n", r, g, b);
    }

    if(type == "P6") {
        fputc((unsigned char) r);
        fputc((unsigned char) g);
        fputc((unsigned char) b);
    }
}


FILE * convert_p3_to_p6(char * name, FILE * p3, int width, int height, int max) {
    FILE * output = create_ppm_p6(width, height, max)
}


FILE * get_ppm_file_information(FILE * ppm) {
    int width, height, max = 0;
}
