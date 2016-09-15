#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
int Usage();
int Icommand();
int Rcommand(int withU);
int Ocommand(int withU);


int main(int argc, char**argv){
	char hstring[3]="-h";
	char istring[3]="-i";
	char ustring[3]="-u";
	char rstring[3]="-r";
	char ostring[3]="-o";
	if(argc==2){
		if(strcmp(argv[1],hstring)==0){
			Usage();
		}
		else if(strcmp(argv[1],istring)==0){
			Icommand();
		}
		else if(strcmp(argv[1],rstring)==0){
			Rcommand(0);
		}
		else if(strcmp(argv[1],ostring)==0){
			Ocommand(0);
		}
	}
	else if(argc==3){
		if(strcmp(argv[1],istring)==0 && strcmp(argv[2],ustring)==0){
			fprintf(stdout, "%-8s%-8s%-8s\n","TYPE","COUNT","PERCENT");
			Icommand();
		}
		else if(strcmp(argv[1],rstring)==0 && strcmp(argv[2],ustring)==0){
			fprintf(stdout,"%-10s%-10s%-10s%-10s%-12s%-10s\n","REG","USE","R-TYPE","I-TYPE","J-TYPE","PERCENT");
			Rcommand(1);
		}
		else if(strcmp(argv[1],ostring)==0 && strcmp(argv[2],ustring)==0){
			Ocommand(1);
		}
	}
	else{
		printf("this is not a valid input\n");
	}
}

int Usage(){
	fprintf(stdout,"%s\n","Usage: ./mstat [OPTION]");
	fprintf(stdout,"%42s\n","./mstat -h Displays this help menu.");
	fprintf(stdout,"%66s\n","./mstat -i [-u] Display statistics about instruction types.");
	fprintf(stdout,"%62s\n","./mstat -r [-u] Display informtion about the registers.");
	fprintf(stdout,"%68s\n","./mstat -o [-u] Display number and percentage of opcode used.");
	fprintf(stdout,"\n");
	fprintf(stdout, "%s\n","Optional flags");
	fprintf(stdout,"%s  %30s\n", "-u","Display human readable headers for the different outputs");
	return 0;
}

int Icommand(){
	char str[100];
	int s;
	for(s=0;s<100;s++){
		str[s]=-1;
	}
	FILE *fp;
	char *mode = "r";
	fp = fopen("mips4.txt",mode);
	if(fp==NULL){
	fprintf(stderr,"Can't open input file in.list!\n");
	exit(1);
	}
	unsigned long long int readvalue;
	char* endptr;
	
	errno=0;
	int R_type=0; int J_type=0; int I_type=0; int Total_type =0;
	int OPCODE_EXTRACT=63;
	//while(scanf("%s", str) != EOF ){ 
	while (fgets(str,99,fp)!=NULL){
		//see where is the 
		if(str[10]!='\n'&& str[10]!=-1){
			printf("the hexidecimal value %s is out of range\n",str);   
            return EXIT_FAILURE;
		
		}// remove the newline
		int b;
		for(b=0;b<99;b++){
			if(str[b]=='\n')
				str[b]='\0';
		}		
		readvalue = strtoull(str, &endptr,16);
		if(*endptr!=0){
            printf("can not parse %s it's not a valid 32-bit hexdecimal number\n",str);
            return EXIT_FAILURE;
        }
		else{
			//
			int shift_value = readvalue >>26;
			if( (shift_value & OPCODE_EXTRACT) ==0){
				R_type++;
			}
			else if(  (shift_value==2) || (shift_value ==3)){
				J_type++;
			}
			else{
				I_type++;
			}
			
		}
	
	}
	if(fclose(fp)!=0){
		fprintf(stderr,"%s\n","failed to close the file");
	}
	Total_type=R_type+J_type+I_type;
	fprintf(stdout,"%-10s%-8d%3.1f%s\n","I_Type",I_type,((double)I_type/Total_type)*100,"%");
	fprintf(stdout,"%-10s%-8d%3.1f%s\n","J_Type",J_type,((double)J_type/Total_type)*100,"%");
	fprintf(stdout,"%-10s%-8d%3.1f%s\n","R_Type",R_type,((double)R_type/Total_type)*100,"%");
	return EXIT_SUCCESS;
}

int Rcommand(int withU){
	char str[100];
	FILE *fp;
	char *mode = "r";
	fp = fopen("mips4.txt",mode);
	if(fp==NULL){
	fprintf(stderr,"Can't open input file in.list!\n");
	exit(1);
	}
	unsigned long long int readvalue;
	int total =0;
	char *endptr;
	char* Register[]={"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
        "$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
	errno=0;
	int OPCODE_EXTRACT =63;
	int i;
	int registers_R[32];
	int registers_I[32];
	for(i=0;i<32;i++){
		registers_R[i]=0;
		registers_I[i]=0;
	}
	while (fgets(str,99,fp)!=NULL){
		if(str[10]!='\n'&& str[10]!=-1){
			printf("the hexidecimal value %s is out of range\n",str);   
            return EXIT_FAILURE;
		
		}// remove the newline
		int b;
		for(b=0;b<99;b++){
			if(str[b]=='\n')
				str[b]='\0';
		}		
		readvalue = strtoull(str, &endptr,16);
		if(*endptr!=0){
            printf("can not parse %s it's not a valid 32-bit hexdecimal number\n",str);
            return EXIT_FAILURE;
        }		
		else{
			//
			int shift_value = readvalue >>26;
			total++;
			if( (shift_value & OPCODE_EXTRACT) ==0){
				// it is R type
				int Rshift_11=readvalue>>11;
				int Rs=(Rshift_11 & 0x7c00)>>10;
				int Rt=(Rshift_11 & 0x3e0)>>5;
				int Rd=Rshift_11 & 0x1f;
				registers_R[Rs]++;
				registers_R[Rt]++;
				registers_R[Rd]++;
			}
			else if( ( (shift_value & OPCODE_EXTRACT) ==2) || ((shift_value & OPCODE_EXTRACT) ==3)){
				// it is J type, do nothing
			}
			else{
				// it is I type
				int Rshift_16=readvalue>>16;
				int Rs=(Rshift_16 & 0x3e0)>>5;
				int Rt=Rshift_16 & 0x1f;
				registers_I[Rs]++;
				registers_I[Rt]++;
			}
		}
	}
	if(fclose(fp)!=0){
		fprintf(stderr,"%s\n","failed to close the file");
	}
	int k;
	int sum=0;
	for(k=0;k<32;k++){
		sum=sum+registers_R[k]+registers_I[k];
	}
	
	int j;
	for(j=0;j<32;j++){
		if(withU==0){
			if(registers_R[j]+registers_I[j]!=0)
	fprintf(stdout,"%1s%-4d %-5d %-5d %-5d %-3d %3.1f%%\n","$",
	j,registers_R[j]+registers_I[j],registers_R[j],registers_I[j],0,(double)(registers_R[j]+registers_I[j])*100/total);
			else if(registers_R[j]+registers_I[j]==0)
	fprintf(stdout,"%1s%-4d %-5d %-5d %-5d %-3d %d%%\n","$",
	j,registers_R[j]+registers_I[j],registers_R[j],registers_I[j],0,0);
		}
		else if(withU==1){
			if(registers_R[j]+registers_I[j]!=0)
				fprintf(stdout,"%-10s%-10d %-10d %-10d %-10d %3.1f%%\n",
				Register[j],registers_R[j]+registers_I[j],registers_R[j],registers_I[j],0,(double)(registers_R[j]+registers_I[j])*100/total);
			else if(registers_R[j]+registers_R[j]==0)
				fprintf(stdout,"%-10s%-10d %-10d %-10d %-10d %d%%\n",
				Register[j],registers_R[j]+registers_I[j],registers_R[j],registers_I[j],0,0);
			
		}
	}
	return EXIT_SUCCESS;
}

int Ocommand(int withU){
char str[100];
	FILE *fp;
	char *mode = "r";
	fp = fopen("mips4.txt",mode);
	if(fp==NULL){
	fprintf(stderr,"Can't open input file in.list!\n");
	exit(1);
	}
	unsigned long long int readvalue;
	char *endptr;
	int Opcode_array[63];
	int Func_array[63];
	int i;
	for(i=0;i<63;i++){
		Opcode_array[i]=0;
		Func_array[i]=0;
	}
	while (fgets(str,99,fp)!=NULL){
		if(str[10]!='\n'&& str[10]!=-1){
			printf("the hexidecimal value %s is out of range\n",str);   
            return EXIT_FAILURE;
		
		}// remove the newline
		int b;
		for(b=0;b<99;b++){
			if(str[b]=='\n')
				str[b]='\0';
		}		
		readvalue = strtoull(str, &endptr,16);
		if(*endptr!=0){
            printf("can not parse %s it's not a valid 32-bit hexdecimal number\n",str);
            return EXIT_FAILURE;
        }		
		else{
			int shift_value = readvalue >>26;
			int opcode_value=(shift_value | 0x0);
			Opcode_array[opcode_value]++;
			if(opcode_value==0){
				int func=(readvalue & 0x3f);
					Func_array[func]++;		
			}
		}
	}
	if(fclose(fp)!=0){
		fprintf(stderr,"%s\n","failed to close the file");
	}
	int j;
	int sum_Opcode=0;
	int sum_Func=0;
	for(j=0;j<63;j++){
		sum_Opcode=sum_Opcode+Opcode_array[j];
		sum_Func=sum_Func+Func_array[j];
	}
	if(withU==0){
		int k;
		for(k=0;k<63;k++){
			if(Opcode_array[k]!=0)
			fprintf(stdout,"0x%-10X%-10d%3.1f%%\n",k,Opcode_array[k],(double)Opcode_array[k]*100/sum_Opcode);	
			else if(Opcode_array[k]==0)
			fprintf(stdout,"0x%-10X%-10d%d%%\n",k,Opcode_array[k],0);
		}
	}
	else if(withU==1){
		fprintf(stdout,"%-12s%-10s%-10s\n","OPCODE","COUNT","PERCENTAGE");
		int k;
		for(k=0;k<63;k++){
			if(Opcode_array[k]!=0)
			fprintf(stdout,"0x%-10X%-10d%3.1f%%\n",k,Opcode_array[k],(double)Opcode_array[k]*100/sum_Opcode);
			else if(Opcode_array[k]==0)
			fprintf(stdout,"0x%-10X%-10d%d%%\n",k,Opcode_array[k],0);
		}
		fprintf(stdout,"%s","\n\n");
		fprintf(stdout,"%-12s%-10s%-10s\n","FUNC","COUNT","PERCENTAGE");
		int a;
		for(a=0;a<63;a++){
			if(Func_array[a]!=0)
			fprintf(stdout,"0x%-10X%-10d%3.1f%%\n",a,Func_array[a],(double)Func_array[a]*100/sum_Func);
			else if(Func_array[a]==0)
			fprintf(stdout,"0x%-10X%-10d%d%%\n",a,Func_array[a],0);
		}
	}
	return EXIT_SUCCESS;
}



