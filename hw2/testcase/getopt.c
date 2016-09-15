#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <fcntl.h>           
#include <sys/stat.h>
#include <stdint.h>

void Usage();
void ConvertFiles(char* input_file, char* input_format, char*output_file, char*output_format,int*vflags);
void CheckHowmanyMoreBytesUTF8(uint8_t* read_buffer, int* morebyte_toread);
void CheckHowmanyMoreBytesUTF16LE(FILE* input,uint8_t*read_buffer,int* morebyte_toread);
void CheckHowmanyMoreBytesUTF16BE(FILE* input,uint8_t*read_buffer,int* morebyte_toread);
void GetCodePointFromUTF8(uint8_t*read_buffer,int*morebyte_toread, unsigned int * unicode);
void GetCodePointFromUTF16LE(uint8_t* read_buffer, int * morebyte_toread, unsigned int *unicode);
void GetCodePointFromUTF16BE(uint8_t* read_buffer, int * morebyte_toread, unsigned int *unicode);
void CodePointToUTF(unsigned int unicode, char * output_format,uint8_t* write_buffer,int* byte_towrite);
void printfTheInformation(int*vflags,int codepoint, int total_byte, uint8_t * read_buffer, uint8_t*write_buffer,char*string_format);
int main (int argc, char*argv[]){
	int opt;
	int hflags=0; int vflags=0;int eflags=0;
	char output_format[15]; char input_file[50]; char output_file[50];char input_format[15];
	unsigned char input_formatCheck[10]={0};
	while( (opt = getopt(argc, argv, "hve:"))!=-1){
		switch(opt){
		case 'h':
			hflags++;
			Usage();
			break;
		case 'v':
			vflags++;
			break;
		case 'e':
			eflags++;
			strncpy(output_format,optarg,15);
			break;
		case '?':
		default:
			Usage();
			exit(EXIT_FAILURE);
		}
	}
	if((argc-optind)==0){
		printf("there is no input file and output file\n");
		Usage();
		exit(EXIT_FAILURE);
	}
	else if((argc-optind)==1){
		printf("there is no output file\n");
		Usage();
		exit(EXIT_FAILURE);
	}
	strcpy(input_file,argv[optind]);
	strcpy(output_file,argv[++optind]);
	//missing -e flags
	if( (vflags!=0 && eflags==0) || (hflags==0 && eflags==0)){
		printf("missing the '-e' flag this flag is required\n");
		printf("the file %s was not created\n",output_file);
		Usage();
		exit(EXIT_FAILURE);
	}
	// bad argument passed to -e check if output_format is of 
	if(strcmp(output_format,"UTF-8")!=0 && strcmp(output_format,"UTF-16LE")!=0 && strcmp(output_format,"UTF-16BE")!=0){
		printf("not a valid format to encode the input file, should be UTF-8,UTF-16LE or UTF-16BE\n");
		Usage();
		exit(EXIT_FAILURE);	
	}
	//validate the file
	struct stat output_stat; struct stat input_stat;
	stat(input_file, &input_stat);
	stat(output_file,&output_stat);
	if(input_stat.st_ino==output_stat.st_ino){
		printf("the output file and input file are the same\n");
		exit(EXIT_FAILURE);
	}
	//open the file to see what kind of encodind it is 
	FILE* input;
	if((input = fopen(input_file, "r")) ==NULL) {
		printf("can't open the input_file %s\n", input_file);
		exit(EXIT_FAILURE);
	}
	size_t twobytes;
	if((twobytes=fread(input_formatCheck,1,2,input))!=2){
		printf("can't read from the file \n");
		exit(EXIT_FAILURE);
	}
	char UTF_8[10]="UTF-8"; char UTF_16LE[10]="UTF-16LE";char UTF_16BE[10]="UTF-16BE";
	if(input_formatCheck[0]==0xFF && input_formatCheck[1]==0xFE){
		strcpy(input_format,UTF_16LE);
    }		
	else if(input_formatCheck[0]==0xFE && input_formatCheck[1]==0xFF){
		strcpy(input_format,UTF_16BE);
	}
	else if(input_formatCheck[0]==0xEF && input_formatCheck[1]==0xBB){
		size_t thirdbyte;
		if((thirdbyte=fread( (input_formatCheck+2),1,1,input))!=1){
			printf("failed to read the third byte\n");
			printf("not a valid encoding input format\n");
			exit(EXIT_FAILURE);
		}
		if(input_formatCheck[2]==0xBF){
			strcpy(input_format, UTF_8);
		}
   }
	else{
		printf("not a valid encoding input format\n");
		exit(EXIT_FAILURE);
	}
	if(fclose(input)!=0){
		printf("failed to close the file\n");
		exit(EXIT_FAILURE);
	}
	ConvertFiles(input_file, input_format, output_file, output_format,&vflags);
    exit(EXIT_SUCCESS);
}	

void ConvertFiles(char* input_file, char* input_format, char*output_file, char*output_format,int*vflags){
	FILE* input; FILE*output;
	input = fopen(input_file, "r"); output=fopen(output_file,"w+");
	if(strcmp(input_format,"UTF-8")==0) fseek(input, 3, SEEK_SET);
	else if(strcmp(input_format,"UTF-16LE")==0)fseek(input, 2, SEEK_SET);
	else if(strcmp(input_format,"UTF-16BE")==0)fseek(input, 2, SEEK_SET);
	unsigned char UTF8[3]={0xEF,0XBB,0XBF}; unsigned char UTF16LE[2]={0xFF,0XFE}; unsigned char UTF16BE[2]={0xFE,0XFF};
	if(strcmp(output_format,"UTF-8")==0)  fwrite(UTF8, 1, 3, output);
    else if(strcmp(output_format,"UTF-16LE")==0) fwrite(UTF16LE,1,2,output);
	else if(strcmp(output_format,"UTF-16BE")==0) fwrite(UTF16BE,1,2,output);
	uint8_t read_buffer[4]={0}; 
	if(*vflags==1){
			fprintf(stdout, "+--------------+-----------------+----------------+\n");
        	fprintf(stdout, "|     ASCII    |    # of bytes   |    codepoint   |\n");
        	fprintf(stdout, "+--------------+-----------------+----------------+\n");
	 }
	else if(*vflags==2){
			fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
        	fprintf(stdout,"|      ASCII   |    # of bytes   |     codepoint   |      input    |\n");
        	fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");   
	}
	else if (*vflags==3){
			fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
        	fprintf(stdout,"|      ASCII   |     # of bytes  |    codepoint    |      input    |     output    |\n");
        	fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
	}
	while( fread(read_buffer, 1, 1, input)!=0){
	read_buffer[1]=0; read_buffer[2]=0; read_buffer[3]=0; uint8_t write_buffer[4]={0};
	int morebyte_toread=0; unsigned int unicode=0; int byte_towrite=0;
	char string_format[10]=""; int total_byte=0;
			if(strcmp(input_format,"UTF-8")==0){
				CheckHowmanyMoreBytesUTF8(read_buffer,&morebyte_toread);
				fread(read_buffer+1,1,morebyte_toread,input);
				GetCodePointFromUTF8(read_buffer,&morebyte_toread, &unicode);
				CodePointToUTF(unicode, output_format,write_buffer,&byte_towrite);
				fwrite(write_buffer,1,byte_towrite,output);
				total_byte=1+morebyte_toread;
				int i;
				for(i=0;i<total_byte;i++)
					sprintf(string_format+i*2, "%02x",(int)read_buffer[i]);
				}
			else if(strcmp(input_format,"UTF-16LE")==0){
				fread(read_buffer+1,1,1,input); //read one more byte first
				CheckHowmanyMoreBytesUTF16LE(input,read_buffer,&morebyte_toread);
				fread(read_buffer+2,1,morebyte_toread,input);
				GetCodePointFromUTF16LE(read_buffer, &morebyte_toread, &unicode);
				CodePointToUTF(unicode, output_format,write_buffer,&byte_towrite);
				fwrite(write_buffer,1,byte_towrite,output);	
				total_byte=2+morebyte_toread;
				int i;
				for(i=0;i<total_byte;i++)
					sprintf(string_format+i*2, "%02x",(int)read_buffer[i]);
			}
			else if(strcmp(input_format,"UTF-16BE")==0){
				fread(read_buffer+1,1,1,input);
				CheckHowmanyMoreBytesUTF16BE(input,read_buffer,&morebyte_toread);
				fread(read_buffer+2,1,morebyte_toread,input);
				GetCodePointFromUTF16BE(read_buffer, &morebyte_toread, &unicode);
				CodePointToUTF(unicode, output_format,write_buffer,&byte_towrite);
				fwrite(write_buffer,1,byte_towrite,output);	
				total_byte=2+morebyte_toread;
				int i;
				for(i=0;i<total_byte;i++)
					sprintf(string_format+i*2, "%02x",(int)read_buffer[i]);
			}
	 printfTheInformation(vflags,unicode,total_byte, read_buffer,write_buffer,string_format);
	}
	fclose(input);
	fclose(output);
}
void CheckHowmanyMoreBytesUTF8(uint8_t* read_buffer, int* morebyte_toread){
		if((read_buffer[0]>>7) ==0) *morebyte_toread=0;
		else if( (read_buffer[0]>>5)==0x6) *morebyte_toread=1;
		else if((read_buffer[0]>>4)==0xE) *morebyte_toread=2;
		else if((read_buffer[0]>>3)==0X1E) *morebyte_toread=3;
		else printf("this is not a valid UTF-8 encoding\n");
}
void CheckHowmanyMoreBytesUTF16LE(FILE* input,uint8_t*read_buffer,int* morebyte_toread){
		uint16_t value=((read_buffer[1]<<8)&0xff00)|read_buffer[0];
		if((value>=0x0 && value <= 0xD7FF)||(value>=0xE000 && value<=0xFFFF)) *morebyte_toread=0;
		else *morebyte_toread=2; 
}
void CheckHowmanyMoreBytesUTF16BE(FILE* input,uint8_t*read_buffer,int* morebyte_toread){
		//fread((read_buffer+1),1,1,input);
		uint16_t value = ((read_buffer[0]<<8)&0xff00)|read_buffer[1];
		if((value>=0x0 && value <= 0xD7FF)||(value>=0xE000 && value<=0xFFFF)) *morebyte_toread=0;
		else *morebyte_toread=2;
}
void GetCodePointFromUTF8(uint8_t*read_buffer,int*morebyte_toread, unsigned int * unicode){
	if(*morebyte_toread==0) {
		unsigned int value = (unsigned int) (read_buffer[0]&0x7F);
		*unicode = value;
	}
	else if(*morebyte_toread==1){
		uint8_t byte1=read_buffer[0]&0x1F;
		uint8_t byte2=read_buffer[1]&0x3F;
		unsigned int value1=(unsigned int) (byte1<<6);
		*unicode = value1|byte2;
	}
	else if (*morebyte_toread==2){
		uint8_t byte1=read_buffer[0]&0xF;
		uint8_t byte2=read_buffer[1]&0x3F;
		uint8_t byte3=read_buffer[2]&0x3F;
		unsigned int value1 = (unsigned int) (byte1<<12);
		unsigned int value2 = (unsigned int) (byte2<<6);
		*unicode = value1|value2|byte3;
	}
	else if (*morebyte_toread==3){
		uint8_t byte1=read_buffer[0]&0x7;
		uint8_t byte2=read_buffer[1]&0x3F;
		uint8_t byte3=read_buffer[2]&0x3F;
		uint8_t byte4=read_buffer[3]&0x3F;
		unsigned int value1 = (unsigned int) (byte1<<18);
		unsigned int value2= (unsigned int) (byte2<<12);
		unsigned int value3 =(unsigned int) (byte3<<6);
		*unicode = value1|value2|value3|byte4;
	}
}

void GetCodePointFromUTF16LE(uint8_t* read_buffer, int * morebyte_toread, unsigned int *unicode){
		if(*morebyte_toread==0){
		  uint16_t value = (uint16_t) ( ((read_buffer[1]<<8)&0xFF00) | read_buffer[0]);
		  *unicode = (unsigned int) value;
		}
		else if(*morebyte_toread==2){
			uint16_t w1= ((read_buffer[1]<<8)&0xFF00)|read_buffer[0];
			uint16_t w2 = ((read_buffer[3]<<8)&0xFF00)|read_buffer[2];
			//uint16_t vl=(w2-0xDC00)&0x3FF; uint16_t vh=(w1-0xD800)&0X3FF; // make sure both of then are onlyt 10 bits
			//uint16_t vl = w2 & 0x3FF; uint16_t vh=w1&0x3FF
			//uint32_t vp=(uint32_t) ((vh<<10)|vl);
			//unsigned int value =(unsigned int) (vp+0x10000);
			uint16_t vh=w1&0x3FF; uint16_t vl=w2&0x3FF;
			uint32_t v= (((vh<<10)&0xFFC00)|vl)+0x10000;
			*unicode = v;
		}
}
void GetCodePointFromUTF16BE(uint8_t* read_buffer, int * morebyte_toread, unsigned int *unicode){
		if(*morebyte_toread==0){
		  uint16_t value = (uint16_t) ( (read_buffer[0]<<8) | read_buffer[1]);
		  *unicode = (unsigned int) value;
		}
		else if(*morebyte_toread==2){
			uint16_t w1= ((read_buffer[0]<<8)&0xFF00)|read_buffer[1];			
			uint16_t w2 = ((read_buffer[2]<<8)&0xFF00)|read_buffer[3];
			//uint16_t vl=(w2-0xDC00)&0x3FF; uint16_t vh=(w1-0xD800)&0X3FF; // make sure both of then are onlyt 10 bits
			//uint32_t vp=(uint32_t) ((vh<<10)|vl);
			//unsigned int value =(unsigned int) (vp+0x10000);
			uint16_t vh=w1&0x3FF; uint16_t vl=w2&0x3FF;
			uint32_t v=(((vh<<10)&0xFFC00)|vl)+0x10000;
			*unicode = v;
		}
}
void CodePointToUTF(unsigned int unicode, char * output_format,uint8_t* write_buffer,int* byte_towrite){
	if(strcmp(output_format,"UTF-8")==0){
		if(unicode>=0x0 && unicode<=0x7F){
			*byte_towrite=1;
			write_buffer[0]=0x7F& unicode;
		}
		else if(unicode>=0x80 && unicode<=0x7FF){
			*byte_towrite=2;
			write_buffer[0]=(unicode>>6) | 0xC0; //get the 5bits xxxxx | 1100 0000
			write_buffer[1]=(unicode & 0x3F)|0x80; // get the last 6 bits xxxxxx|1000 0000
		}
		else if(unicode>=0x800 && unicode<=0xFFFF){
			*byte_towrite=3;
			write_buffer[0]=(unicode>>12) | 0xE0;  //
			write_buffer[1]=((unicode>>6) & 0x3F)| 0x80;
			write_buffer[2]=(unicode & 0x3F)|0x80;
		}
		else if(unicode>=0x10000 && unicode<=0x1FFFFF){
			*byte_towrite=4;
			write_buffer[0]=(unicode>>18)|0xF0;
			write_buffer[1]=((unicode>>12)&0x3F)|0x80;
			write_buffer[2]=((unicode>>6)&0x3F)|0x80;
			write_buffer[3]=(unicode&0x3F)|0x80;
		}
		else{
			printf("it is not in any range\n");
		}
	}
	else if(strcmp(output_format,"UTF-16LE")==0){
		if (unicode>0x0 && unicode <0x10000){
			*byte_towrite=2;
			uint8_t byteLow =(uint8_t) (unicode & 0xFF);
			uint8_t byteHi = (uint8_t) ((unicode >>8)&0xFF);
			write_buffer[0]=byteLow; write_buffer[1]=byteHi;
		}
		else if(unicode>0x10000){
			*byte_towrite=4;
			unsigned int vp = unicode -0x10000;
			uint16_t vh=(vp>>10)&0x3FF;
			uint16_t vl = vp & 0x3FF;
			uint16_t w1=0xD800+vh; uint16_t w2=0xDC00+vl;
			write_buffer[1]=(w1>>8) & 0xFF; // WAS 0X3F should not matter here 
			write_buffer[0]=w1 & 0xFF;
			write_buffer[3]=(w2>>8) & 0xFF;
			write_buffer[2]=w2 &0xFF;
		}
	}
	else if(strcmp(output_format,"UTF-16BE")==0){
		if (unicode>=0x0 && unicode <0x10000){
			*byte_towrite=2;
			uint8_t byteLow =(uint8_t) (unicode & 0xFF);
			uint8_t byteHi = (uint8_t) ((unicode >>8)&0xFF);
			write_buffer[1]=byteLow; write_buffer[0]=byteHi;
		}
		else if(unicode>=0x10000){
			*byte_towrite=4;
			unsigned int vp = unicode -0x10000;
			uint16_t vh=(vp>>10)&0x3FF;
			uint16_t vl = vp & 0x3FF;
			uint16_t w1=0xD800+vh; uint16_t w2=0xDC00+vl;
			write_buffer[0]=(w1>>8) & 0xFF;
			write_buffer[1]=w1 & 0xFF;
			write_buffer[2]=(w2>>8) & 0xFF;
			write_buffer[3]=w2 &0xFF;
		}
		else{
			printf("unicode is not in any of this range\n");
		}
	
	}
	

}
	
	
void printfTheInformation(int*vflags,int codepoint,int total_byte, uint8_t * read_buffer, uint8_t*write_buffer,char*string_format){
if(*vflags==1){		
        if (codepoint>=32 && codepoint<=126){
            fprintf(stdout, "|       %c     |        %d        |    U+%04x      |\n", *read_buffer, total_byte,codepoint);
            fprintf(stdout, "+--------------+-----------------+----------------+\n");
        }
        else {           
            fprintf(stdout, "|     NONE     |    %d           |   U+%04x       |\n", total_byte, codepoint);
            fprintf(stdout, "+--------------+-----------------+----------------+\n");
        }    
    }
    else if(*vflags==2){   
        if (codepoint>=32 && codepoint<=126){
            fprintf(stdout,"|     %c       |        %d       |     U+%04x     |      0x%s      |\n", *read_buffer, total_byte,codepoint, string_format);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
        }
        else {
            fprintf(stdout,"|      NONE    |       %d        |       U+%04x    |      0x%s     |\n",  total_byte,codepoint, string_format);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+\n");
        }
    }
    else if(*vflags>=3){
		if (codepoint>=32 && codepoint<=126){
            fprintf(stdout,"|      %c      |        %d        |    U+%04x      |     0x%s       |     0x%s      |\n", *read_buffer,total_byte, codepoint, string_format, write_buffer);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
        }
        else {
            fprintf(stdout,"|     NONE     |      %d         |    U+%04x      |    0x%s       |     0x%s      |\n",total_byte,codepoint, string_format, write_buffer);
            fprintf(stdout,"+--------------+-----------------+-----------------+---------------+---------------+\n");
        }

    }
}
void Usage(){
	printf("%s\n","Usage: ./utfconverter [-h] [-v|-vv|-vvv] -e OUTPUT_ENCODING INPUT_FILE OUTPUT_FILE");
	printf("%s\n","Command line utility for converting files to and from UTF-8, UTF-16LE or UTF-16BE");
	printf("%s\n","");
	printf("%s\n","OPtion arguments:");
	printf("%s\n","-h    Display this usage Menu");
	printf("%s\n","-v    Enables verbose output. This argument can be used up to three times for a noticitble effect");
	printf("%s\n","-e    OUT_ENCODING  Format to encode the output file ");
	printf("%s\n","		Accepted values:");
	printf("%s\n","			UTF-8");
	printf("%s\n","			UTF-16LE");
	printf("%s\n","			UTF-16BE");
	printf("%s\n","If this flag is not provided or an invalid value is given the program should exit with the EXIT_FAILURE return code");
	printf("%s\n","");
	printf("%s\n","Positional Arguments:");
	printf("%s\n","INPUT_FILE	File to convert. Must contain a valid BOM. if it does not contain a valid BOM the program should");
	printf("%s\n","exit with the EXIT_FAILURE return code");
	printf("%s\n", "OUTPUT_FILE	Output file to create. If the file already exists and its not the same as the input file, it ");
	printf("%s\n","should be overwritten. If the OUTPUT_FILE is the same as the INPUT_FILE the program should exit with the EXIT_FAILURE return code");
}
