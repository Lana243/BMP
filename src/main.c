#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc != 8)
		exit(-1);
	Image a = load_bmp(argv[2]);
	a = crop(a, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
	a = rotate(a);
	save_bmp(argv[3], a);
	clean_matrix(a.data, a.info.biHeight);	
    return 0;
}
