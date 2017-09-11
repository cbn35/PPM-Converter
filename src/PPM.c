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
    FILE * out = fopen(name, "w");
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
        fputc((unsigned char) r, file);
        fputc((unsigned char) g, file);
        fputc((unsigned char) b, file);
    }
}


int * get_ppm_file_information(FILE * ppm) {
    /* Desc: Reads through a PPM file, and grabs it's header information
     * Args:
     *      ppm (FILE*): PPM file to examine
     * Returns (int*): array of the structure [type, width, height, max]
     */
    char * buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int * info = malloc(sizeof(int) * 4);

    // Get the PPM type
    fgets(buffer, BUFFER_SIZE, ppm);
    while(buffer[0] == '#') fgets(buffer, BUFFER_SIZE, ppm);
    char temp[1];          // Create a temporary string for atoi
    temp[0] = buffer[1];   // Stick the char representing the file type into temp
    info[0] = atoi(temp);  // Finally, grab the actual number
    
    // Get the width and height
    fgets(buffer, BUFFER_SIZE, ppm);
    while(buffer[0] == '#') fgets(buffer, BUFFER_SIZE, ppm);
    char dimensionBuffer[255];
    int i = 0;
    while(buffer[i] != ' ' && buffer[i] != '\n') {
        dimensionBuffer[i] = buffer[i];
        i++;
    }

    info[1] = atoi(dimensionBuffer);

    if(buffer[i] == '\n') {
        fgets(buffer, BUFFER_SIZE, ppm);
        i = 0;
    } else {
        i++;
    }

    int j = i;
    while(buffer[i] != '\n') {
        dimensionBuffer[i - j] = buffer[i];
        i++;
    }

    for(i = 0; i < 255; i++) printf("%c\n", dimensionBuffer[i]);

    info[2] = atoi(dimensionBuffer);


    // Get the max RGB value
    fgets(buffer, BUFFER_SIZE, ppm);
    while(buffer[0] == '#') fgets(buffer, BUFFER_SIZE, ppm);
    info[3] = atoi(buffer);

    return info;
}


int * read_image(FILE * image) {
    /* Reads in a PPM file, and creates an array of RGB values representing the pixels
     * Args:
     *      image (FILE*): File pointer to PPM image.
     * Returns (int*): 1D array of lenth width * height * 3, with RGB values for each pixel
     */
    int * metadata = get_ppm_file_information(image); // Grab the metadata

    // Set up the pixel array to be returned
    int * pixels = malloc(sizeof(int) * metadata[1] * metadata[2] * 3);  // width * height * 3 vals each
    
    char * buffer = malloc(sizeof(char) * BUFFER_SIZE);

    // If it's a P3 file, we have to read in chars one at a time separated by spaces, then atoi them
    if(metadata[0] == 3) {
        int bufferIndex = 0;
        fgets(buffer, BUFFER_SIZE, image);
        while(buffer != NULL) {
            for(int i = 0; i < BUFFER_SIZE; i++) {
                char pixBuffer[BUFFER_SIZE];
                int j = i
                while(buffer[i] != ' ' && buffer[i] != '\n') {
                    pixBuffer[i - j] = buffer[i];
                }
                
                pixels[bufferIndex] = atoi(pixBuffer);
                bufferIndex++;
            }
        }
    }

    // If it's a P6 file, read in one char at a time, then cast that value to an int
    if(metadata[0] == 6) {
        int bufferIndex = 0;
        fgets(buffer, BUFFER_SIZE, image);
        while(buffer != NULL) {
            for(int i = 0; i < BUFFER_SIZE; i++) {
                char pixBuffer = buffer[i];
                pixels[bufferIndex] = (int) picBuffer;
                bufferIndex++;
            }
        }
    }

    return pixels;
}


/*
FILE * convert_p3_to_p6(char * name, FILE * p3) {
    rewind(p3);
    int * info = get_ppm_file_information(p3);

    FILE * output = create_ppm_p6(name, info[1], info[2], info[3]);
    char * buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int r, g, b = 0;

    fgets(buffer, 3, p3);
    while(buffer != NULL) {
        r = (int) buffer[0];
        g = (int) buffer[1];
        b = (int) buffer[2];
        write_pixel(r, g, b, output);
    }

    return output;
}

FILE * convert_p6_to_p3(char * name, FILE * p6) {
    rewind(p6);
    int * info = get_ppm_file_information(p6);

    FILE * output = create_ppm_p6(name, info[1], info[2], info[3]);         // create output file
    char * buffer = malloc(sizeof(char) * BUFFER_SIZE);                     // create reading buffer
    char charToIntBuffer;                                                   // create second buffer for grabbing individual RGB values
    int * pixArray = malloc(sizeof(int) * atoi(info[1]) * atoi(info[2]));   // create array for storing pixel values
    int pixel[3];                                                           // temp vars for RGB pixel values

    /* Run through the width and height of the image,
     * grabbing three numbers and assign these to the
     * rgb variables, then send that and the file pointer
     * to write_pixel
     */ /*
    for(int i = 0; i < atoi(info[1]) * atoi(info[2]); i++) {
        for(int k = 0; k < 2; k++) {
            fgetc(charToIntBuffer, p6);
            int i = 0;
            while(charToIntBuffer != '\n' || charToIntBuffer != ' ' || i < 3) {
                buffer[i] = charToIntBuffer 
            }
            
            
        }

    }

    

    fgets(buffer, BUFFER_SIZE, p6);
}

*/
