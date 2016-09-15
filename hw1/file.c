#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){
	FILE *ifp, *ofp;
	char *mode = "r";
	char outputFilename[]="out.list";
	char str[999];
	int c;


	ifp = fopen("in.list",mode);
	if(ifp==NULL){
	fprintf(stderr,"Can't open input file in.list!\n");
	exit(1);
	}
	//while(fscanf(ifp,"%s",str)!=EOF)
		//printf("%s",str);
	while(( c=getc(ifp))!=EOF)
		putchar(c);
	fclose(ifp);

	ofp=fopen(outputFilename,"w");
	if(ofp==NULL){
	fprintf(stderr,"Can't open output file %s!\n",outputFilename);
	exit(1);

   }


}
