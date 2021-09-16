#include <stdio.h>	
#include <stdlib.h>	
#include <string.h>	
#include <uuid/uuid.h>

char comma[4] = ", ";
char clear[60];
int lines = 25;

void getuuid(char* s);
char get_random_table();

int main(){	
	FILE* file;
	for (int f = 0; f < 1; f++){
		char file_name[20] = "votantes_";
		if(f == 0){
			strcat(file_name,"0.txt");
		}
		else if(f == 1){
			strcat(file_name,"1.txt");
		}
		else{
			strcat(file_name,"2.txt");
		}
		file = fopen(file_name,"w");
		int i = 1;
		while(i <= lines){ 
			char str_uuid[40]; 
			char table[2] = "\0"; 
			char assembly[16] = "110632, ";
			char input[60];
			
			getuuid(str_uuid);
			table[0] = get_random_table();
					
			strcat(input,str_uuid);
			strcat(input,assembly);
			strcat(input,table);
			strcat(input,"\n");
			
			fprintf(file,"%s",input);
			strcpy(input,clear);
			i++;
		}
	}
	fclose(file);	
	return 0;
}

void getuuid(char*s){     
	uuid_t uuid;
	uuid_clear(uuid);
	uuid_generate_random(uuid);
	uuid_unparse(uuid,s);
	strcat(s,comma);  
}

char get_random_table(){
    int min = 65, max = 74, num = 0;
    do{
		num = rand()%100; 
	}while(num > max || num < min);
    char c = num;
    return c;
}