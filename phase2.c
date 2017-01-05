#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


#include "display.h"
#include "debug.h"


/* Help dialog */
void usage(char *name){
	fprintf(stderr, "Usage:\n"
		"%s [ options ] file1 file2\n\n"
		"-f --debug-file <file> Use file as a debug output\n"
		"-? --help Print this dialog\n"
		, name);
}


int main(int argc, char* argv[]){
	
	/* Récupération des arguments */
	int opt;
	struct option longopts[] = {
		{ "debug-file", required_argument, NULL, 'f' },
		{ "help", no_argument, NULL, '?' },
		{ NULL, 0, NULL, 0 }
	};

	while ((opt = getopt_long(argc, argv, "f:?", longopts, NULL)) != -1) {
		switch(opt) {
		case 'f':			
			add_debug_to(optarg);
			break;
		case '?':
		default:
			usage(argv[0]);
			exit(1);
		}
	}
	
	/* Opening files */
	FILE *file1, *file2;
	if(argc - optind > 1){
		file1 = fopen(argv[optind++], "rb");
       	if(!file1){
			fprintf(stderr, "Erreur d'ouverture du fichier : %s\n", argv[optind]);
			return 0;
		}

		file2 = fopen(argv[optind++], "rb");
		if(!file2){
			fprintf(stderr, "Erreur d'ouverture du fichier : %s\n", argv[optind]);
			return 0;
		}
	}
	else{
		usage(argv[0]);
		return 0;
	}

	printf("FUSIOOOOOOOON\n");
	


	/* Memory free */
	fclose(file1);
	fclose(file2);


	return 0;
}
