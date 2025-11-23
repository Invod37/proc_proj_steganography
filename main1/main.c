#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>

#include "src/steg_encode.h"
#include "src/steg_decode.h"
#include "src/bmp_utils.h"


void print_usage(const char *prog_name) {
    printf("Usage:\n");
    printf("  %s -h | --help\n", prog_name);
    printf("  %s -v | --version\n", prog_name);
    printf("  %s -e -i <input.bmp> -t <text_file> -o <output.bmp>\n", prog_name);
    printf("  %s -d -i <input.bmp> -t <text_file>\n", prog_name);
}

int main(int argc, char *argv[]) {
    bool encode = false;
    bool decode = false;
    bool help_flag = false;
    bool version_flag = false;

    char *input_bmp_path = NULL;
    char *output_bmp_path = NULL;
    char *msg_file_path = NULL;


    struct option long_options[] = {
        {"help",    no_argument,       0, 'h'},
        {"version", no_argument,       0, 'v'},
        {"encode",  no_argument,       0, 'e'},
        {"decode",  no_argument,       0, 'd'},
        {"text",    required_argument, 0, 't'},
        {"input",   required_argument, 0, 'i'},
        {"output",  required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "hvedt:i:o:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h': help_flag = true; break;
            case 'v': version_flag = true; break;
            case 'e': encode = true; break;
            case 'd': decode = true; break;
            case 't': msg_file_path = optarg; break;
            case 'i': input_bmp_path = optarg; break;
            case 'o': output_bmp_path = optarg; break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }


    if (help_flag) {
        print_usage(argv[0]);
        return 0;
    }

    if (version_flag) {
        printf("Steganography Tool v1.0\n");
        return 0;
    }


    if ((encode && decode) || (!encode && !decode)) {
        fprintf(stderr, "Error: Must specify exactly one mode: -e (encode) or -d (decode).\n");
        print_usage(argv[0]);
        return 1;
    }

    if (encode) {
        if (!input_bmp_path || !msg_file_path || !output_bmp_path) {
            fprintf(stderr, "Error: Encode mode requires -i, -t, and -o arguments.\n");
            return 1;
        }
    }

    
    if (decode) {
        if (!input_bmp_path || !msg_file_path) {
            fprintf(stderr, "Error: Decode mode requires -i and -t arguments.\n");
            return 1;
        }
        
        if (output_bmp_path) {
            printf("Warning: Output file (-o) is ignored in decode mode based on strict rules.\n");
        }
    }

    
    unsigned int inpfsize = 0;
    unsigned int msgfsize = 0;
    struct stat st;

    
    if (stat(input_bmp_path, &st) == -1) {
        printf("Opening input file : %s\n", input_bmp_path);
        perror("");
        return 1;
    }
    inpfsize = st.st_size;

    if (encode) {
        if (stat(msg_file_path, &st) == -1) {
            printf("Opening message file : %s\n", msg_file_path);
            perror("");
            return 1;
        }
        msgfsize = st.st_size;
    }

    
    FILE *file;
    BmpFileHeader fileHeader;
    BmpInfoHeader infoHeader;

    
    file = fopen(input_bmp_path, "rb"); 
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    
    if (fread(&fileHeader, sizeof(BmpFileHeader), 1, file) != 1) {
        fprintf(stderr, "Error reading BMP File Header\n");
        fclose(file);
        return 1;
    }

    
    if (fread(&infoHeader, sizeof(BmpInfoHeader), 1, file) != 1) {
        fprintf(stderr, "Error reading BMP Info Header\n");
        fclose(file);
        return 1;
    }

    fclose(file);

    
    if (encode || decode) {
        if (fileHeader.bfType != 0x4D42 || 
            fileHeader.bfReserved1 != 0 || 
            fileHeader.bfReserved2 != 0 || 
            fileHeader.bfSize != inpfsize) 
        {
            printf("Input file : %s is not in BMP format or corrupted!\n", input_bmp_path);
            if (fileHeader.bfType != 0x4D42) printf("(Invalid Type: 0x%X)\n", fileHeader.bfType);
            if (fileHeader.bfSize != inpfsize) printf("(Size mismatch: Header %u vs File %u)\n", fileHeader.bfSize, inpfsize);
            return 1;
        }
    }

    if (encode) {
        if (infoHeader.biBitCount < 24) {
            printf("The image in the file : %s has a color depth of less than 24 bits!\n", input_bmp_path);
            return 1;
        }

        if (msgfsize > 0 && (infoHeader.biSizeImage / msgfsize < 8)) {
            printf("The text message in the file : %s is too big!\n", msg_file_path);
            return 1;
        }
    }

    
    int fresult;
    if (encode) {
        fresult = steg_encode(input_bmp_path, msg_file_path, output_bmp_path); 
    }

    if (decode) {
        fresult = steg_decode(input_bmp_path, msg_file_path);
    }

    return fresult;
}
