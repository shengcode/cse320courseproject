#include <stdio.h>
void Usage();


int main(int argc, char**argv){
	Usage();
   


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

