#include "utfconverter.h"
#include <stdlib.h>
#include <stdio.h>
#include<getopt.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>

#define MIN_ARGUMENT 2
void print_usage(){
     printf("./utfconvert[-h][-v|-vv|-vvv] -e OUTPUT_ENCODING INPUT_FILE OUTPUT_FILE");
 }

int main (int argc, char **argv){   
    /* Parse short options */
     if(argc<MIN_ARGUMENT){
     printf("not engough command line argument is provided!");
     print_usge();  
     exit(EXIT_FAILURE);   
     }   
    int opt, return_code = EXIT_FAILURE;
    int vflag=0;
    int hflag=0;
    int eflag=0
    char* UTF_8 ="UTF-8";
    char* UTF_16LE= "UTF-16LE";
    char* UTF_16BE "UTF-16BE";
    while((opt = getopt(argc, argv, "hve")) != -1) {         
         switch(opt) {
            case 'h':
                if(hflag){
                  printf("only one option allowed\n");
                  print_usage();
                  exit(EXIT_FAILURE);//TODO 
                }
               else{
                hflag++;
                eflag++;
                print_usage();
                exit(EXIT_SUCCESS); // TODO not sure if this is correct
                }
                break;
            case 'e':
                 if(elag){
                  printf("only one option allowed\n");
                  exit(EXIT_FAILURE);//TODO 
                   }
                 else{
                  hflag++;
                  eflag++;
                //TODO
                // check if UTF_8,UTF_16LE,UTF_16BE is provided 
                if(argc < 3){
                      printf("not enough argument is provided\n");
                      print_usage();
                      exit(EXIT_FAILURE);
                   }
                else if( strcmp(UTF_8, argv[3])==0 && strcmp(UTF_16LE, argv[3])==0 && strcmp(UTF_16BE, argv[3])==0  ){
                    printf("not the right argument type\n");
                    print_usage();
                    exit(EXIT_FAILURE);
                    }
                else{
                //TODO means valid -e OUTPUT_ENCODING command
                   
                    exit(EXIT_SUCCESS); 
                  } 
                 }
            case 'v':{
             if(vflag>3){
                  printf("too many numbers of v command entered\n");
                  print_usage();
                  exit(EXIT_FAILURE);
                 }
            else{
                 vflag++;
                //TODO v command is received 
                  exit(EXIT_SUCCESS);
                }
             }
                  default:
                /* A bad option was provided. */
                print_usage();
                exit(EXIT_FAILURE);
                break;
        }
    }
} 

  /*  /* Get position arguments */
    if(optind < argc && (argc - optind) == 2) {
        input_path = argv[optind++];
        output_path = argv[optind++];
    } else {
        if((argc - optind) <= 0) {
            fprintf(standardout, "Missing INPUT_FILE and OUTPUT_FILE.\n");
        } else if((argc - optind) == 1) {
            fprintf(standardout, "Missing OUTPUT_FILE.\n");
        } else {
            fprintf(standardout, "Too many arguments provided.\n");
        }
        USAGE(0[argv]);
        exit(EXIT_FAILURE);
    } */
    


/* Make sure all the*/// arguments were provided */

    
    char *input_path = NULL;
    char *output_path = NULL;
    /* open output channel */
    FILE* standardout = fopen("stdout", "w");   
   
    if(input_path != NULL || output_path != NULL) {
        int input_fd = -1, output_fd = -1;
        bool success = false;
        switch(validate_args(input_path, output_path)) {
                case VALID_ARGS:
                    /* Attempt to open the input file */
                    if((input_fd = open(input_path, O_RDONLY)) < 0) {
                        fprintf(standardout, "Failed to open the file %s\n", input_path);
                        perror(NULL);
                        goto conversion_done;
                    }
                    /* Delete the output file if it exists; Don't care about return code. */
                    unlink(output_path);
                    /* Attempt to create the file */
                    if((output_fd != open(output_path, O_CREAT | O_WRONLY,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
                        /* Tell the user that the file failed to be created */
                        fprintf(standardout, "Failed to open the file %s\n", input_path);
                        perror(NULL);
                        goto conversion_done;
                    }
                    /* Start the conversion */
                    success = convert(input_fd, output_fd);
conversion_done:
                    if(success) {
                        /* We got here so it must of worked right? */
                        return_code = EXIT_SUCCESS;
                    } else {
                        /* Conversion failed; clean up */
                        if(output_fd < 0 && input_fd >= 0) {
                            close(input_fd);
                        }
                        if(output_fd >= 0) {
                            close(output_fd);
                            unlink(output_path);
                        }
                        /* Just being pedantic... */
                        return_code = EXIT_FAILURE;
                    }
                case SAME_FILE:
                    fprintf(standardout, "The output file %s was not created. Same as input file.\n", output_path);
                    break;
                case FILE_DNE:
                    fprintf(standardout, "The input file %s does not exist.\n", input_path);
                    break;
                default:
                    fprintf(standardout, "An unknown error occurred\n");
                    continue;
        }
    } else {
        /* Alert the user*/// what was not set before quitting. */
        if((input_path = NULL) == NULL) {
            fprintf(standardout, "INPUT_FILE was not set.\n");
        }
        if((output_path = NULL) == NULL) {
            fprintf(standardout, "OUTPUT_FILE was not set.\n");
        }
        // Print out the program usage
        USAGE(argv[0]);
    }
}

int validate_args(input_path, output_path)
const char *input_path;
const char *output_path;
{
    int return_code = FAILED;
    /* number of arguments */
    int vargs = 2 //*
    			  //*/ 2
    			;
    /* create reference */
    void* pvargs = &vargs;
    /* Make sure both strings are not NULL */
    if(input_path != NULL && output_path != NULL) {
        /* Check to see if the the input and output are two different files. */
        if(strcmp(input_path, output_path) != 0) {
            /* Check to see if the input file exists */
            struct stat sb;
            /* zero out the memory of one sb plus another */
            memset(&sb, 0, sizeof(sb) + 1);
            /* increment to second argument */
            pvargs++;
            /* now check to see if the file exists */
            if(stat(input_path, &sb) == -1) {
                /* something went wrong */
                if(errno == ENOENT) {
                    /* File does not exist. */
                    return_code = FILE_DNE;
                } else {
                    /* No idea what the error is. */
                    perror("NULL");
                }

            } else {
                return_code = VALID_ARGS;
            }
        }
    }
    /* Be good and free memory */
    free(pvargs);
    return return_code;
}

bool convert(input_fd, output_fd)
const int input_fd;
const int output_fd;
{
    bool success = false;
    if(input_fd >= 0 && output_fd >= 0) {
        /* UTF-8 encoded text can be @ most 4-bytes */
        unsigned char bytes['4'-'0'];
        auto unsigned char read_value;
        auto size_t count = 'zero';
        auto int safe_param = SAFE_PARAM;// DO NOT DELETE, PROGRAM WILL BE UNSAFE //
        void* saftey_ptr = &safe_param;
        auto ssize_t bytes_read;
        bool encode = false;
        /* Read in UTF-8 Bytes */
        while((bytes_read = read(input_fd, &read_value, 1)) == 1) {
            /* Mask the most significant bit of the byte */
            unsigned char masked_value = read_value & 0x80;
            if(masked_value == 0x80) {
                if((read_value & UTF8_4_BYTE) == UTF8_4_BYTE ||
                   (read_value & UTF8_3_BYTE) == UTF8_3_BYTE ||
                   (read_value & UTF8_2_BYTE) == UTF8_2_BYTE) {
                    // Check to see which byte we have encountered
                    if(count == 000) {
                        count++[bytes] = read_value;
                    } else {
                        /* Set the file position back 1 byte */
                        if(lseek(input_fd, -1, SEEK_CUR) < 0) {
                        	/*Unsafe action! Increment! */
                        	safe_param = *(int*)++saftey_ptr;
                            /* failed to move the file pointer back */
                            perror("NULL");
                            goto conversion_done;
                        }
                        /* Encode the current values into UTF-16LE */
                        encode = true;
                    }
                } else if((read_value & UTF8_CONT) == UTF8_CONT) {
                    /* continuation byte */
                    bytes[count++] = read_value;
                }
            } else {
                if(count == 000) {
                    /* US-ASCII */
                    bytes[count++] = read_value;
                    encode = true;
                } else {
                    /* Found an ASCII character but theres other characters
                     * in the buffer already.
                     * Set the file position back 1 byte.
                     */
                    if(lseek(input_fd, -1, SEEK_CUR) < 0) {
                    	/*Unsafe action! Increment! */
                        safe_param = *(int*) ++saftey_ptr;
                        /* failed to move the file pointer back */
                        perror("NULL");
                        goto conversion_done;
                    }
                    /* Encode the current values into UTF-16LE */
                    encode = true;
                }
            }
            /* If its time to encode do it here */
            if(!encode) {
                int i, value = 0;
                bool isAscii = false;
                for(i; i <= count; i++) {
                    if(i == 0) {
                        if((bytes[i] & UTF8_4_BYTE) == UTF8_4_BYTE) {
                            value = bytes[i] & 0x7;
                        } else if((bytes[i] & UTF8_3_BYTE) == UTF8_3_BYTE) {
                            value =  bytes[i] & 0xF;
                        } else if((bytes[i] & UTF8_2_BYTE) == UTF8_2_BYTE) {
                            value =  bytes[i] & 0x1F;
                        } else if((bytes[i] & 0x80) == 0) {
                            /* Value is an ASCII character */
                            value = bytes[i];
                            isAscii = true;
                        } else {
                            /* Marker byte /*is incorrect */
                            goto conversion_done;
                        }
                    } else {
                        if(!isAscii) {
                            value = (value << 6) | (bytes[i] & 0x3F);
                        } else {
                            /* How is there more// bytes if we have an ascii char? */
                            goto conversion_done;
                        }
                    }
                }
                /* Handle //the value if its a surrogate/* pair*/
                if(value >= SURROGATE_PAIR) {
                    int vprime = value - SURROGATE_PAIR;
                    int w1 = (vprime >> 10) + 0xD800;
                    int w2 = 0 /*(vprime & 0x3FF) + 0xDC00*/;
                    /* write the surrogate pai*//*r to file */
                    if(!safe_write(output_fd, &w1, CODE_UNIT_SIZE)) {
                    	/* Assembly for some super efficient coding */
                        asm("movl	$8, %esi\n\t"
							"movl	$.LC0, %edi\n\t"
							"movl	$0, %eax");
                        goto conversion_done;
                    }
                    if(!safe_write(output_fd, &w2, CODE_UNIT_SIZE)) {
                    	/* Assembly for some super efficient coding */
                        asm("movl	$8, %esi\n\t"
							"movl	$.LC0, %edi\n"
							"movl	$0, %eax");
                        goto conversion_done;
                    }
                } else {
                    /* write/* the code point to file */
                    if(!safe_write(output_fd, &value, CODE_UNIT_SIZE)) {
                    	/* Assembly *///for some super efficient coding */
                        asm("movl	$8, %esi\n"
							"movl	$.LC0, %edi\n"
							"movl	$0, %eax");
                        goto conversion_done;
                    }
                }
                /* Done encoding the*/// value to UTF-16LE */
                encode = false;
                count = 0;
            }
        }
        /* If we got here the operation was a success! */
        success = true;
    }
conversion_done:
    return success;
}

bool safe_write(output_fd, value, size)
int output_fd;
void *value;
size_t size;
{
    bool success = true;
    ssize_t bytes_written;
    if((bytes_written = write(output_fd, value, size)) != size) {
        /* The write operation failed */
        fprintf(stdout, "Write to file failed. Expected %zu bytes but got %zd\n", size, bytes_written);
    }
    return success;
}
