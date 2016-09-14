#include<stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include<stdint.h>
#define MIN_ARG 2
enum encoding{UTF8=1, UTF16LE=2, UTF16BE=3, INVALIDENCODING,INVALIDEFLAG}; // define enum
void print_usage(); 
int get_file_argument(int argc, int optind);
enum encoding e_flag_command(int eflag, char*output_encoding);
int validate_file (const char * input_path, const char *output_path); // open and close file did not save file pointer 
enum encoding input_type_check( const char *input_path, const char *output_path);
void print_chars(uint8_t *input, int size, char *output);
void vflag_print_utf8(int vflag, int getcodePoint, char* read_in, char *write_out, int moreByteToRead,uint8_t*read_buffer);
void vflag_print_utf16le(int vflag, int getcodePoint, char * read_in, char *write_out, int moreByteToRead);
void print_info(const char * input_path, const char *output_path, enum encoding input_encoding_enum, enum encoding output_encoding_enum);

// for DCSE320
void bytesRead_UTF8( uint8_t * read_buffer, int *moreToRead);  // if input is UTF8 after read 1 byte check how many more bytes need to read for one conversion 
void getCodePointFromUFT8(uint8_t *read_buffer, int *moreToRead, int *codePoint); //for UTF8 after reading enough byte for converting convert the bytes to codePoint 
void codePointConvert(int getcodePoint, enum encoding output_encoding_enum, uint8_t * write_buffer, int*ByteToWrite, char *write_out); // from codepoint to the require output file
// call the following 3 helper function 
void codePointToUTF8(int getcodePoint,  uint8_t * write_buffer, int *ByteToWrite, char *write_out);
void codePointToUTF16LE(int getcodePoint,  uint8_t * write_buffer, int *ByteToWrite,char *write_out);
void codePointToUTF16BE(int getcodePoint,  uint8_t * write_buffer, int *ByteToWrite,char *write_out);
// for utf16
void checkIfSurrogate16LE (uint8_t * read_buffer, int *moreToRead); // THIS IS THE SAME AS CHECK HOW MANY MORE BYTES NEED TO READ 
void checkIfSurrogate16BE (uint8_t * read_buffer, int *moreToRead); 
void getCodePointFromUFT16LE(uint8_t *read_buffer, int *moreToRead, int *codePoint);
void getCodePointFromUFT16BE(uint8_t *read_buffer, int *moreToRead, int *codePoint);
// for utf16le and uft16be and utf8 after get the codepoint then call the codepointconver function to convert to the required type 
void print_array(void * array_name, int size);
int main (int argc, char*argv[]){
    int opt;
    int vflag=0;
    int eflag=0; 
    char output_encoding[20]="";
    const char *input_path=NULL;
    const char *output_path=NULL;
                                                           
    enum encoding output_encoding_enum=INVALIDENCODING; 
    enum encoding input_encoding_enum=INVALIDENCODING;
    FILE *input_file_pointer=NULL; 
    while((opt = getopt(argc, argv, "hve:")) != -1){
        switch (opt){
            case'h':
                print_usage();
                exit(EXIT_SUCCESS);
                break;  
            case'v':
                vflag++;
                break;
            case 'e':
                eflag=1;
                strncpy(output_encoding, optarg, 20);
                break;
            case '?':
            default:
                print_usage();
                exit(EXIT_FAILURE);
                break;
        }  
    } 
    // output_encoding is something we alreay get here. 
    if ((output_encoding_enum=e_flag_command(eflag, output_encoding))==INVALIDEFLAG){
        print_usage();
        exit(EXIT_FAILURE);
    }
    // check if there is input file and out put file; 
    if( get_file_argument(argc, optind) ==0 ){
        print_usage();
        exit(EXIT_FAILURE);
    }
    // if there is get the input file and output file here 
    input_path = argv[optind++];
    output_path = argv[optind++];  
    //validate the file arguments   checek if 
    if ( validate_file(input_path, output_path)==0){
        print_usage();
        exit(EXIT_FAILURE);
    } 
    // check what kind of input_encoding it is?
    if ( (input_encoding_enum =input_type_check(input_path,output_path))==INVALIDENCODING){
        print_usage();
        exit(EXIT_FAILURE);
    }

    // open and write the BOM to the output file 
    FILE *output_file_pointer=NULL;
    if ( (output_file_pointer = fopen(output_path,"w+"))==NULL){  // file is created if dose not exist 
        print_usage();
        exit(EXIT_FAILURE); 
    }
    else{
        printf("the file %s  was successfully created\n", output_path);
    }
    const char BOM_UTF8[3]={0xEF,0XBB,0XBF}; // UTF8 DO NOT NEED TO CONSIDER ABOUT LE OR BE write directly 
    const char BOM_UTF16LE[2]={0XFF,0XFE};   //
    const char BOM_UTF16BE[2]={0XFE,0XFF};  //
    //size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
    if(output_encoding_enum==UTF8){
        fwrite(BOM_UTF8,3,1,output_file_pointer);
    }
    else if(output_encoding_enum==UTF16LE){
        fwrite(BOM_UTF16LE,2,1,output_file_pointer);
    }
    else if(output_encoding_enum==UTF16BE){
        fwrite(BOM_UTF16BE,2,1,output_file_pointer);
    }
    else{
        print_usage();
        exit(EXIT_FAILURE);
    }  

    input_file_pointer = fopen(input_path,"r");   
    if(input_encoding_enum==UTF8){
        fseek(input_file_pointer,3,SEEK_CUR);
    }  
    else if((input_encoding_enum==UTF16LE) || (input_encoding_enum==UTF16BE)){
        fseek(input_file_pointer,2,SEEK_CUR);
    }
    uint8_t read_buffer[4]=""; 
    auto size_t filebytes_read=0;   
    if(vflag==1){   
        fprintf(stdout, "+--------------+-----------------+----------------+\n");
        fprintf(stdout, "|     ASCII    |    # of bytes   |    codepoint   |\n");
        fprintf(stdout, "+--------------+-----------------+----------------+\n");
    }
    else if(vflag==2){
        fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
        fprintf(stdout,"|      ASCII   |    # of bytes   |     codepoint   |      input    |\n");
        fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
    }
    else if(vflag>=3){
        fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
        fprintf(stdout,"|      ASCII   |     # of bytes  |    codepoint    |      input    |     output    |\n");
        fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
    }


    while(filebytes_read = fread(read_buffer,1,1,input_file_pointer) == 1){ // if can sucessfully read the data 
        read_buffer[1]=0;
        read_buffer[2]=0;
        read_buffer[3]=0; // clear 
        uint8_t write_buffer[4]={0,0,0,0};// clear
        int moreByteToRead=0;
        int *moreToRead=&moreByteToRead; // get the pointer 
        int getByteToWrite=0;
        int *ByteToWrite=&getByteToWrite;
        int getcodePoint =-1;
        int *codePoint = &getcodePoint; 
        // char readin_value=-1;
        char read_in[10] = " ";//
        // char writeout_value=-1;
        char write_out[10]=" ";//

        if(input_encoding_enum==UTF8){
            bytesRead_UTF8(read_buffer, moreToRead);   //1 check how many bytes do I need to read to the buffer
            fread((read_buffer+1), (*moreToRead),1,input_file_pointer);   //2 read the bytes to buffer; fread(buffer,n,1,pointer_holder);
            getCodePointFromUFT8(read_buffer,moreToRead, codePoint); //3 convert to code point
            if(moreByteToRead==0){
                print_chars(read_buffer, 1, read_in);
            }
            else if(moreByteToRead==1){
                print_chars(read_buffer, 2, read_in);
            }
            else if(moreByteToRead==2){
                print_chars(read_buffer, 3, read_in);        
            }
            else if(moreByteToRead==3){
                print_chars(read_buffer, 4, read_in);
            }
        }                                                         
        else if(input_encoding_enum==UTF16LE){
            fread((read_buffer+1), 1,1,input_file_pointer);     // 1, read another byte 
            checkIfSurrogate16LE (read_buffer,moreToRead);    // 2. check if surrogate? if it is read TWO more byte, so also passing the moretoread pointer
            fread((read_buffer+2), (*moreToRead),1,input_file_pointer);  // keep reading, if it is not surrogate then reading ZERO Byte
            getCodePointFromUFT16LE(read_buffer, moreToRead, codePoint);   //convert to code point
            if(moreByteToRead==0){
                print_chars(read_buffer, 2, read_in);
            }
            else if(moreByteToRead==2){
                print_chars(read_buffer, 4, read_in);
            }
        }
        else if(input_encoding_enum==UTF16BE){
            fread((read_buffer+1), 1,1,input_file_pointer);     // 1, read another byte 
            checkIfSurrogate16BE (read_buffer,moreToRead);    // 2. check if surrogate? if it is read TWO more byte, so also passing the moretoread pointer
            fread((read_buffer+2), (*moreToRead),1,input_file_pointer);  // keep reading, if it is not surrogate then reading ZERO Byte
            getCodePointFromUFT16BE(read_buffer, moreToRead, codePoint);    //convert to code point
            if(moreByteToRead==0){
                print_chars(read_buffer, 2, read_in);
            }
            else if(moreByteToRead==2){
                print_chars(read_buffer, 4, read_in);
            }
        }
        codePointConvert(getcodePoint, output_encoding_enum,write_buffer,ByteToWrite,write_out); // convert code point to the UTF8/UTF16LE/UTF16BE type output
        fwrite(write_buffer,getByteToWrite,1,output_file_pointer);
        if(input_encoding_enum==UTF8){
            vflag_print_utf8( vflag, getcodePoint, read_in, write_out, moreByteToRead,read_buffer);
        }
        else if(input_encoding_enum == UTF16LE || input_encoding_enum==UTF16BE)
            vflag_print_utf16le( vflag, getcodePoint, read_in, write_out, moreByteToRead);
    }
    // after everything is done close the inputfile and close the inputfile
    fclose(output_file_pointer);
    fclose(input_file_pointer); // point_holder is the inputfilepointer
#ifdef CSE320
print_info(input_path,output_path, input_encoding_enum, output_encoding_enum);
#else 
#endif
}

void vflag_print_utf8(int vflag, int getcodePoint, char* read_in, char *write_out, int moreByteToRead,uint8_t*read_buffer){
    if(vflag==1){
        if (getcodePoint>=32 && getcodePoint<=126){
            fprintf(stdout, "|       %c     |        %d        |    U+%04x      |\n", *read_buffer, (moreByteToRead+1),getcodePoint);
            fprintf(stdout, "+--------------+-----------------+----------------+\n");
        }
        else {           
            fprintf(stdout, "|     NONE     |    %d           |   U+%04x       |\n", (moreByteToRead+1), getcodePoint);
            fprintf(stdout, "+--------------+-----------------+----------------+\n");
        }    
    }
    else if(vflag==2){   
        //  fprintf(stdout,"|  ASCII  | # of bytes | codepoint  |  input   |\n");    
        if (getcodePoint>=32 && getcodePoint<=126){
            fprintf(stdout,"|     %c       |        %d       |     U+%04x     |      0x%s      |\n", *read_buffer, (moreByteToRead+1),getcodePoint, read_in);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
        }
        else if(moreByteToRead>=1){
            fprintf(stdout,"|      NONE    |       %d        |       U+%04x    |      0x%s     |\n",  (moreByteToRead+1),getcodePoint, read_in);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
        }
    }
    else if(vflag>=3){
        if (getcodePoint>=32 && getcodePoint<=126){
            //fprintf(stdout,"|  ASCII  | # of bytes | codepoint  |  input   |  output  |\n");
            fprintf(stdout,"|      %c      |        %d        |    U+%04x      |     0x%s       |     0x%s      |\n", *read_buffer,(moreByteToRead+1), getcodePoint, read_in, write_out);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
        }
        else {
            fprintf(stdout,"|     NONE     |      %d         |    U+%04x      |    0x%s       |     0x%s      |\n",(moreByteToRead+1),getcodePoint, read_in, write_out);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
        }

    }
}


void vflag_print_utf16le(int vflag, int getcodePoint, char * read_in, char *write_out, int moreByteToRead){
    if(vflag==1){
        if (getcodePoint>=32 && getcodePoint<=126){
            fprintf(stdout, "|      %c    |         %d       |    U+%04x      |\n",  getcodePoint,(moreByteToRead+2), getcodePoint);
            fprintf(stdout, "+--------------+-----------------+----------------+\n");
        }
        else {           
            fprintf(stdout, "|     NONE     |        %d        |       U+%04x   |\n",(moreByteToRead+2), getcodePoint);
            fprintf(stdout, "+--------------+-----------------+----------------+\n");
        }    
    }
    else if(vflag==2){   
        //  fprintf(stdout,"|  ASCII  | # of bytes | codepoint  |  input   |\n");    
        if (getcodePoint>=32 && getcodePoint<=126){
            fprintf(stdout,"|      %c    |       %d         |       U+%04x    |     0x%s      |\n", getcodePoint,(moreByteToRead+2), getcodePoint,read_in);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
        }
        else {
            fprintf(stdout,"|     NONE     |          %d      |     U+%04x      |      0x%s     |\n",  (moreByteToRead+2),getcodePoint,read_in);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
        }
    }
    else if(vflag>=3){
        if (getcodePoint>=32 && getcodePoint<=126){
            //fprintf(stdout,"|  ASCII  | # of bytes | codepoint  |  input   |  output  |\n");
            fprintf(stdout,"|      %c     |        %d        |     U+%04x      |       0x%s    |     0x%s      |\n",  getcodePoint, (moreByteToRead+2),getcodePoint, read_in, write_out);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
        }
        else{
            fprintf(stdout,"|     NONE     |       %d         |    U+%04x     |      0x%s     |      0x%s     |\n", (moreByteToRead+2),getcodePoint, read_in, write_out);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
        }

    }
}






void print_info(const char * input_path, const char *output_path, enum encoding input_encoding_enum, enum encoding output_encoding_enum){
    char hostname[20]=" ";
    gethostname(hostname,100);
    printf("CSE320: Host: %s\n",hostname);
    struct stat sb;
    int sample_st_dev=0;
    int sample_st_info=0;
    int sample_st_size=0;
    //struct stat sb;
    stat(input_path, &sb);
    printf("CSE320: Input: %s  %lu %lu %ldbyte(s)\n", input_path, sb.st_ino, sb.st_dev, sb.st_size);
    printf("CSE320: Output: %s\n", output_path);
    if(input_encoding_enum==UTF8){
        printf("CSE320: Input encoding: %s\n","UTF-8"); //TODO need to find out the input encoding!
    }
    else if(input_encoding_enum==UTF16LE){
        printf("CSE320: Input encoding: %s\n","UTF-16LE"); 
    }
    else if(input_encoding_enum==UTF16BE){
        printf("CSE320: Input encoding: %s\n","UTF-16BE"); 
    }
    if(output_encoding_enum==UTF8){
        printf("CSE320: Output encoding: %s\n","UTF-8"); //TODO need to find out the input encoding!
    }
    else if(output_encoding_enum==UTF16LE){
        printf("CSE320: Output encoding: %s\n","UTF-16LE"); 
    }
    else if(output_encoding_enum==UTF16BE){
        printf("CSE320: Output encoding: %s\n","UTF-16BE"); 
    }
    printf("The file %s was successfully created!\n",output_path);
}

enum encoding e_flag_command(int eflag, char*output_encoding){   
    if (eflag==0){
        printf("missing the '-e' flag, this flag is required\n");
        return INVALIDEFLAG;
    }
    else if(eflag ==1 ){
        if (output_encoding==NULL){
            printf("you should enter an OUT PUT ENCODING\n");
            return INVALIDEFLAG;
        } 
        else if(strlen(output_encoding)>8){
            printf("the flag '-e' has an incorrect value %s!\n", output_encoding);
            return INVALIDEFLAG;
        } 
        else if( (strcmp("UTF-8",output_encoding)==0) ){
            return UTF8;
        }  
        else if ( (strcmp ("UTF-16LE",output_encoding)==0) ){
            return UTF16LE;
        }
        else if ( (strcmp ("UTF-16BE",output_encoding)==0)){
            return UTF16BE; 
        }
        else {
            printf("the flag '-e' has an incorrect value %s!\n",output_encoding);
            return INVALIDEFLAG;
        }
    }
}

int get_file_argument(int argc, int optind){
    if(optind < argc && (argc - optind) == 2) {
        return 1; 
    } else {
        if((argc - optind) <= 0) {
            fprintf(stdout, "Missing INPUT_FILE and OUTPUT_FILE.\n");
            return 0;
        } else if((argc - optind) == 1) {
            fprintf(stdout, "Missing OUTPUT_FILE.\n");
            return 0;
        } else {
            fprintf(stdout, "Too many arguments provided.\n");
            return 0;
        }        
    }
}

int validate_file (const char * input_path, const char *output_path){
    //return 0 if there is any error 
    if (input_path==NULL){
        printf("input file is NULL!\n");
        return 0; 
    } 
    if(output_path==NULL){
        printf("output file is NULL!\n");
        return 0; 
    }
    if( (output_path != NULL) && ( strcmp(input_path,output_path)==0)){
        printf("the file %s was not created! same as input file\n",output_path);
        return 0;
    }
    struct stat sb_output;
    stat(output_path, &sb_output);
    struct stat sb;
    stat(input_path,&sb);
    if ((sb.st_ino)==(sb_output.st_ino)) { // device number
        printf("the output file and input file has the same inode number, so they are still the same file\n");
        return 0;
    }  
    if (stat(input_path, &sb)==-1){
        perror("stat");
        printf("file does not exists\n");
        return 0;
    }
    FILE *input_file_pointer;
    if (  (input_file_pointer = fopen(input_path,"r"))==NULL){    
        printf("file accisle right prohibited\n");
        return 0;
    }
    fclose(input_file_pointer); 
    return 1;
}

enum encoding input_type_check( const char *input_path, const char *output_path){
    auto unsigned char read_value[5]=""; // initialize every element to be zero  
    auto size_t bytes_read;
    enum encoding input_encoding;
    FILE *input_file_pointer=NULL;
    if (  (input_file_pointer = fopen(input_path,"r"))==NULL){    
        input_encoding= INVALIDENCODING; 
    }
    if ((bytes_read = fread(read_value,2,1,input_file_pointer)) != 2){   
        input_encoding =   INVALIDENCODING;  
    }
    if (read_value[0] == 0xFF && read_value[1] == 0XFE){
        input_encoding= UTF16LE;  
    }
    else if(read_value[0]==0xFE && read_value[1]==0xFF){
        input_encoding= UTF16BE;   
    }
    else if (read_value[0]==0XEF && read_value[1]==0xBB) {
        // if it is not UTF16LE or UTF16BE keep reading 
        if(  bytes_read = fread((read_value+2),1,1,input_file_pointer) != 1){
            input_encoding= INVALIDENCODING;
        }

        if (read_value[2]==0xBF){
            input_encoding=  UTF8; // it is UTF-8 BOM
        }
        else{
            printf("the input file %s dose not have a valid BOM\n",input_path);
            printf("the file %s was not created\n",output_path);
            input_encoding= INVALIDENCODING; 
        }
    }
    else {
        printf("the input file %s does not have a valie BOM\n",input_path);
        printf("the file %s was not created\n",output_path);
        input_encoding= INVALIDENCODING; 
    }
    fclose(input_file_pointer); // file is closed here 
    return input_encoding;
}   

void bytesRead_UTF8( uint8_t * read_buffer, int *moreToRead){
    unsigned int bytevalue= (unsigned int )(*read_buffer); // so that I can do bitwise operation does it matter here if I use int 
    unsigned int MSB =(bytevalue>>7)&0x1;
    if (MSB==0){
        *moreToRead=0;
    } 
    else if(MSB==1){
        unsigned int value1 = ((bytevalue & 0xE0)>>5)&0x7; // masking the unecessary bits
        unsigned int value2 = ((bytevalue & 0xF0)>>4)&0xF;
        unsigned int value3 = ((bytevalue & 0xF8)>>3)&0x1F;
        if(value1==0b110){ // binaray 
            *moreToRead=1;
        }
        else if(value2==0b1110){
            *moreToRead=2; // need to read 2 more bytes
        }
        else if(value3==0b11110){
            *moreToRead=3;; // need to read 3 more bytes
        }
        else {
            printf("not a valid type, can not figure out how many more bytes need to read for this UTF8\n");
        }
    }
}

void getCodePointFromUFT8(uint8_t *read_buffer, int *moreToRead, int *codePoint){
    unsigned int value_byte1 =(unsigned int) (*read_buffer);
    if ((*moreToRead)==0){
        *codePoint = value_byte1; 
    }
    else if((*moreToRead)==1){
        unsigned int value_byte2= (unsigned int) (*(read_buffer+1));         
        unsigned int A = value_byte1 & 0x1F; //mask to get x part in  110xxxxx
        unsigned int A_p= (A<<6)&0x3E0; // mask the unecessary bits
        unsigned int B = value_byte2 & 0x3F; // mask to get x part in 10xxxxxx 
        unsigned int Z = A_p | B; // get the code point
        *codePoint=Z;  // save the code point to the address 
    }
    else if ((*moreToRead)==2){
        unsigned int value_byte2=(unsigned int)(*(read_buffer+1));  
        unsigned int value_byte3=(unsigned int) (*(read_buffer+2)); 
        unsigned int A = value_byte1 & 0xF;  // get the x part of the first byte 1110xxxx
        unsigned int B = value_byte2 & 0x3F; // get the x part of the second byte 10xxxxxx
        unsigned int C = value_byte3 & 0x3F; // get the x part of the third byte 10xxxxxx 
        unsigned int A_p = (A<<12)&0xF000; // mask the not needed bits  1111 0000 0000 0000
        unsigned int B_p=(B<<6)&0xFC0;     // mask the not needed bits 1111 1100 0000
        unsigned int Z = ( (A_p|B_p)|C); // get the code point 
        *codePoint=Z;
    }  
    else if((*moreToRead)==3){
        unsigned int value_byte2=(unsigned int)(*(read_buffer+1));  
        unsigned int value_byte3=(unsigned int) (*(read_buffer+2)); 
        unsigned int value_byte4=(unsigned int) (*(read_buffer+3)); 
        unsigned int A = value_byte1 & 0x7;  // get the x part of the first byte 11110xxx  mask with 0x111==7
        unsigned int B = value_byte2 & 0x3F; // get the x part of the second byte 10xxxxxx 
        unsigned int C = value_byte3 & 0x3F; // get the x part of the third byte 10xxxxxx 
        unsigned int D = value_byte4 & 0x3F; // get the x part of the fourth byte 10xxxxxx
        unsigned int A_p=(A<<18)&0x1C0000;    // mask the not needed bits 1 1100 0000 0000 0000 0000 
        unsigned int B_p=(B<<12)&0x3F000;              // 11 1111 0000 0000 0000 
        unsigned int C_p=(C<<6)&0xFC0;                         // 1111 1100 0000 
        unsigned int Z= (((A_p|B_p)|C_p)|D); // get the code point 
        *codePoint=Z;
    }
}
void codePointConvert(int getcodePoint, enum encoding output_encoding_enum, uint8_t * write_buffer, int*ByteToWrite, char *write_out){
    if(output_encoding_enum==UTF8){
        codePointToUTF8(getcodePoint, write_buffer, ByteToWrite, write_out);
    }
    else if(output_encoding_enum==UTF16LE){
        codePointToUTF16LE(getcodePoint, write_buffer, ByteToWrite,write_out);
    }
    else if(output_encoding_enum==UTF16BE){
        codePointToUTF16BE(getcodePoint, write_buffer, ByteToWrite,write_out);
    }
}
// convert condepoint to UTF8 
void codePointToUTF8(int getcodePoint,  uint8_t * write_buffer, int *ByteToWrite,char *write_out){
    if(getcodePoint>=0x0 && getcodePoint<=0x7F){
        write_buffer[0]=getcodePoint;
        *ByteToWrite=1;
    }
    else if(getcodePoint>=0x80 && getcodePoint<=0x7FF){
        uint8_t A = (getcodePoint & 0x7C0)>>6; // get the most significant 5 bit
        uint8_t B = getcodePoint & 0x3F;        // get the less significant 6 bits
        write_buffer[0]= 0xC0|A;  // cocate natete the most significant 5 bits with 110,  A|1100 0000 = A|0xC0
        write_buffer[1]= 0x80|B;         // concatenat the less significant 6 bits with 10, B|10000000 = B|0x80
        *ByteToWrite=2; 

    }
    else if(getcodePoint>=0x800 && getcodePoint<=0xFFFF){
        uint8_t A= (getcodePoint & 0xF000)>>12; // get the most significant 4 bits
        uint8_t B= (getcodePoint & 0xFC0)>>6;   // get the less significant 6 bits 
        uint8_t C= (getcodePoint & 0x3F);       // get the less significant 6 btis
        write_buffer[0]=0xE0|A;                   // A concatenate with 1110  A|11100000 = A|0XE0
        write_buffer[1]=0X80|B;                     // B concate with 10        B|10000000=B|0x80
        write_buffer[2]=0X80|C;                     // C concatenate with 10    C|10000000=C|0x80
        *ByteToWrite=3;

    }
    else if(getcodePoint>=0x10000 && getcodePoint<=0x1FFFFF){
        uint8_t A =(getcodePoint & 0x1C0000)>>18; // get the most significant 3 bits
        uint8_t B=(getcodePoint & 0x3F000)>>12;  // get the less significant 6 bits
        uint8_t C=(getcodePoint & 0xFC0)>>6;     // get the less significant 6 bits
        uint8_t D=getcodePoint & 0x3F;          // get the less significant 6 bits
        write_buffer[0]=0xF0|A;     // A concatenate with 1110 A|11110000 = A|0xF0 
        write_buffer[1]=0x80|B;     // B concatenate with 10   
        write_buffer[2]=0X80|C;     // C concatenate with 10
        write_buffer[3]=0X80|D;     // D concatenate with 10 
        *ByteToWrite=4; 

    }
    print_chars(write_buffer, (*ByteToWrite), write_out);
}
// convert codepoint to UTF16LE
void codePointToUTF16LE(int getcodePoint,  uint8_t * write_buffer, int *ByteToWrite, char *write_out){
    if ((getcodePoint>=0X0000 && getcodePoint<=0XD7FF)|| (getcodePoint>=0xE000 && getcodePoint<=0xFFFF)){
        // seperate code point in 2 bytes put them in write_buff; 
        uint8_t A = (getcodePoint>>8)&0xff; // get the most significant 8 btis the first byte; 
        uint8_t B = getcodePoint & 0xFF; // get the less significant 8 bit the secodn byte; 
        // because it is little inden so reverse the position 
        write_buffer[0]=B;
        write_buffer[1]=A;
        *ByteToWrite=2;
    }  
    else if(getcodePoint>=0x10000){
        unsigned int V= getcodePoint-0x10000;
        // only need 16 bit to hold the whole thing ....
        uint16_t Vh=(V>>10)&0X3FF;     // give us the highest 10 bits of the  1111 1111 1100 0000 0000 0xFFC00
        uint16_t Vl=V&0x3FF;
        uint16_t W1=0xD800+Vh;
        uint16_t W2=0xDC00+Vl;
        // get each byte       
        uint8_t A=(W1>>8)&0xFF; // get 8 bytes  1111 1111 0xFF;
        uint8_t B=(W1&0xFF);
        uint8_t C=(W2>>8)&0xFF; // get the 8 bytes mask to double make sure 
        uint8_t D=(W2&0xFF);
        // little endian 
        write_buffer[0]=B;
        write_buffer[1]=A;
        write_buffer[2]=D;
        write_buffer[3]=C;
        *ByteToWrite=4; 

    }
    else{
        printf("not a valie code point can not convert to UTF16LE\n");
    }
    print_chars(write_buffer, ( *ByteToWrite), write_out);  
}
// convert codePoint to UTF16BE
void codePointToUTF16BE(int getcodePoint, uint8_t *write_buffer, int*ByteToWrite,char*write_out){
    if ((getcodePoint>=0X0000 && getcodePoint<=0XD7FF)|| (getcodePoint>=0xE000 && getcodePoint<=0xFFFF)){
        // seperate code point in 2 bytes put them in write_buff; 
        uint8_t A = ((getcodePoint>>8) & 0xff); // get the most significant 8 btis the first byte;  
        uint8_t B = getcodePoint & 0xFF; // get the less significant 8 bit the secodn byte; 
        *write_buffer=A;
        *(write_buffer+1)=B;  
        *ByteToWrite=2;
    }  
    else if(getcodePoint>=0x10000){
        // we will get a 4 bytes UTF16BE;
        unsigned int V= getcodePoint-0x10000;
        // only need 16 bit to hold the whole thing ....
        uint16_t Vh=(V>>10)&0x3FF;      // give us the highest 10 bits of the  11 1111 1111 0x3FF
        uint16_t Vl=V&0x3FF;
        uint16_t W1=0xD800+Vh;
        uint16_t W2=0xDC00+Vl;
        // get each byte       
        uint8_t A=(W1>>8)&0xFF;
        uint8_t B=(W1&0xFF);
        uint8_t C=(W2>>8)&0xFF;
        uint8_t D=(W2&0xFF);
        // BIG endian 
        write_buffer[0]=A;
        write_buffer[1]=B;
        write_buffer[2]=C;
        write_buffer[3]=D;
        *ByteToWrite=4;   
    }
    else{
        printf("not a valid code point can not convert to UTF16LE\n");
    }
    print_chars(write_buffer, ( *ByteToWrite), write_out);
}

void checkIfSurrogate16LE (uint8_t * read_buffer, int *moreToRead){
    uint8_t byte1= (*(read_buffer+1));
    uint8_t byte2= (*read_buffer);
    uint16_t value= ((byte1<<8)&0xff00) | (byte2);
    if((value>=0x0000 && value<=0xD7FF)|| (value>=0xE000 && value<=0xFFFF) ){
        *moreToRead=0; // it is not surrogate pair, do not need to read more
    }
    else {
        *moreToRead=2; // it is surrogate pair, need to read TWO more bytes 
    }

}

void checkIfSurrogate16BE (uint8_t * read_buffer, int *moreToRead){
    uint8_t byte1= (*read_buffer);
    uint8_t byte2= (*(read_buffer+1));
    uint16_t value= ((byte1<<8)&0xff00) | (byte2); // TODO CAN I DO IT THIS WAY??????
    if((value>=0x0000 && value<=0xD7FF)|| (value>=0xE000 && value<=0xFFFF) ){
        *moreToRead=0;// it is not surrogate pair;
    }
    else {
        *moreToRead=2; // it is surrogate pair;
    }

}
void getCodePointFromUFT16LE(uint8_t *read_buffer, int *moreToRead, int *codePoint){
    if((*moreToRead)==0){
        // codePoint is the same as the value of the read buffer
        uint8_t byte1= *(read_buffer+1);
        uint8_t byte2= *read_buffer;
        uint16_t value= ((byte1<<8)&0xff00) | (byte2); // TODO CAN I DO IT THIS WAY?????? 
        *codePoint = value;
    }
    else if ((*moreToRead)==2){
        uint8_t byte1= *(read_buffer+1);     // byte1 and byte2 together is w1, for LE w1=byte2byte1
        uint8_t byte2= *read_buffer;
        uint16_t w1= ((byte1<<8)&0xff00)|byte2; // w1 is one byte
        uint16_t vh=w1&0x3FF;   // vh can only have 10 byte  
        uint8_t byte3 =*(read_buffer+3); // byte3 and byte4 together is w2, for LE w2=byte4byte3 
        uint8_t byte4 =*(read_buffer+2);
        uint16_t w2= ((byte3<<8)&0xff00)|byte4; 
        uint16_t vl=w2&0x3FF; //
        uint32_t v= (((vh<<10)&0xFFC00)|vl)+0x10000;  // 
        *codePoint=v; 
    }

}
void getCodePointFromUFT16BE(uint8_t *read_buffer, int *moreToRead, int *codePoint){
    if((*moreToRead)==0){
        // codePoint is the same as the value of the read buffer
        uint8_t byte1= *(read_buffer);
        uint8_t byte2= *(read_buffer+1);
        uint16_t value= ((byte1<<8)&0xff00)| (byte2); // TODO CAN I DO IT THIS WAY?????? 
        *codePoint = value;
    }
    else if ((*moreToRead)==2){
        uint8_t byte1= *read_buffer;     // byte1 and byte2 together is w1, for BE w1=byte1byte2
        uint8_t byte2= *(read_buffer+1);
        uint16_t w1= ((byte1<<8)&0xff00)|byte2; 
        uint16_t vh=w1-0xD800;
        uint8_t byte3 =*(read_buffer+2); // byte3 and byte4 together is w2, for LE w2=byte3byte4 
        uint8_t byte4 =*(read_buffer+3);
        uint16_t w2= ((byte3<<8)&0xff00)|byte4; 
        uint16_t vl=w2-0xDC00;
        uint32_t v= (((vh<<10)&0xFFC00)|vl)+0x10000;  // vh 10    1111 1111 1100 0000 0000 0xFFC00
        *codePoint=v; 
    }
}
void print_usage(){
    printf("Usage: ./utfconvert[-h][-v|-vv|-vvv] -e OUTPUT_ENCODING INPUT_FILE OUTPUT_FILE\n");
    printf("Option arguments:\n");
    printf("-h  Display the usage menu\n");
    printf("-v Enables verbose output. This argument can be used up to three times for a noticable effect.\n");
    printf("-e OUTPUT_ENCODING   Format to encode the output file.\n");
    printf("       Accepted values:\n");
    printf("                     UTF-8\n");
    printf("                     UTF-16LE\n");
    printf("                     UTF-16BE\n");
    printf("If this flag is not provided or an invalid value is given the program should exit with the EXIT_FAILURE return code\n");
    printf("Positional Arguments:\n");
    printf("INPUT_FILE     File to convert.Must contain a valid BOM. If it does not contain a valid BOM the program should exit with the EXit_FAILURE return code\n");
    printf("OUTPUT_FILE  Outputfile to create. If the file already exists and its not the same as the input file, it should be overwritten\n");
    printf("If the OUTPUT_FILE is the same as the INPUT_FILE the program should exit with the EXIT_FAILURE return code\n");

}
//  read_buffer, size, read_in 
void print_chars(uint8_t *input, int size, char *output) {
    for(int i=0; i<size;i++){
        sprintf(output+2*i, "%02x",*(input+i)); 
    }
}


