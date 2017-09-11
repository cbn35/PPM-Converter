/* Unit test for checking if get_ppm_file_information is working correctly
 *
 * Author: Chance Nelson
 */


#include <stdio.h>
#include "../src/PPM.h"


int main(int argc, char ** argv) {
    char * data;
    if(argc > 1) {
        data = argv[1];
    } else {
        data = "P3\n2 23\n255\n255 255 255 255 255 255\n255 255 255 255 255 255";
    }

    FILE * out = fopen("temp.ppm", "w");
    fputs(data, out);
    fclose(out);

    int * info = get_ppm_file_information(fopen("temp.ppm", "r"));

    printf("Type: %d\nWidth: %d\n Height: %d\n Max: %d", info[0], info[1], info[2], info[3]);
    free(info);

    return 0;
}
