/*
*	CSE 23004 
*	Richmond Frimpong
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YES 1
#define NO 0

void trim(char *str){
	int len = strlen(str)+1;
	char *dest = (char*)malloc(len * sizeof(char));
	char *src ;
	src = str; // char pointer to original string	
	int j = 0;
	j = 0;
	while(*src !='\n'){
		if(isspace((unsigned char)*src)){
			src++; // move one byte
			continue;	
		}
		//woow isdigit i was miissing took me hours
		while(isalpha((unsigned char)*src) || isdigit((unsigned char)*src)){ 
			dest[j++] = *src++;
		}
		dest[j++]= ' ';
	}
	strncpy(str, dest , strlen(dest) + 1);
}

void *readline(){
char args[128]; // buffer
char *temp = NULL;
int count = 1;
while(fgets(args , 128 , stdin)){
	if(count == 1){ 
		temp = (char *)malloc(sizeof(char) * 128);
		if(temp != NULL){
			strcpy(temp , args);
		}
		else{
			fprintf(stderr , "Not enought memory!! \n");//not enought memory
			free(temp);
		}
	}
	else{
		char *more_mem = (char *)realloc(temp , sizeof(char) * 128 * count);
		temp = more_mem;
		strcat(temp , args);
	}
	if(strchr (temp , '\n') != NULL)
		return temp;
	count++;
}
	return temp;

}

char  *file_memalloc(FILE *in){
	char *str = (char *) malloc(128 * sizeof(char));
	char *temp = NULL;
	int counter = 1;
	while(fgets(str , 128 , in)){
		if(counter == 1){
			temp = (char *) malloc(128 * sizeof(char)); //temporary memory space
			strcpy(temp , str);
			counter++;
		} 
		else{
			char  * temp_str = (char *) realloc(temp , sizeof(char) * 128 * counter); // give me more memory starting right for where we left off
			if(temp_str == NULL)
				fprintf(stderr , "Virtual memory exhausted\n");	
			
			temp = temp_str;
			strcat(temp , str); //i'm thinkng the first run of ten chars the strcat does the same thing strcpy
			counter++;
		}
		if(strchr(temp , '\n') != NULL)
			return temp;
		counter++;
	}
	return temp;
}

//global variable for printdb only
//int printdb = 0;

main(){
	char *args;
	char *first_args;
	char *second_args;
	char *extra;
	int boolean = 0; 
	//---------------------------------------------------------------------------------------------------------------------------------------------------------/
	char *exact_lines;
	char *first;
	char *second;
	char *third;	
	char **outfile;
	outfile = malloc( 10 * sizeof(char *)); // intial intialization for index like but [ptr] so needs intialization
	int increment_for_realloc = 0;
	do{
		printf("Grock- ");
			args = readline();
			printf("The command -> %s" , args); 
			if(strcmp(args, "quit\n") == 0){
				break;
			} 	
		// notes sscanf read strings with no whitespaces , special edit conversion code
		//specifications makes sscanf read variety of chars including blanks by [^\n] this replace the newline 
		// in the buffer with null value and makes after it into one string.
		//Intilalize the first second and third args with strlen of args because its long enough
		//int len = strlen(args);
		//printf("%d \n" , len);
		first_args = (char*) malloc(strlen(args) * sizeof(char)); 
		second_args =(char*) malloc(strlen(args) * sizeof(char));
		extra = (char*) malloc(strlen(args) * sizeof(char));  
		int tokens = sscanf(args ,"%s %s %[^\n]s" , first_args , second_args , extra);		
		if(tokens < 0 || strcmp(args ," \n") == 0)
			continue;
	
		if(tokens == 1){
			int cmp = strcmp(first_args , "setdb");
			if(cmp == 0 && tokens == 1){
				printf("Missing argument.\n");
			}
			else{
		 		int cmp3 = strcmp(first_args , "printdb");
				if(cmp3 == 0) 
					if(boolean){ // if you are able to read file
						//clean outfile otherwise you will have repeating correct format files
						int i = 0;
						for(i = 0 ; i < increment_for_realloc ; i++){
							//printf("Should be based on increment => %d\n" , i);
								printf("%s" , outfile[i]);
						}
						//clean up garbage collection
						//increment_for_realloc = 0;
						boolean = YES;	// set the switch if you came here already					 
					}
					else{
						printf("Database is empty.\n");
					}
				else{				
					printf("Unrecognized command.\n");
				}
			}
			if(strncmp(first_args , "putrel", strlen(first_args)) == 0){
				if(boolean){
					int i = 0;
					printf("Implemented functionanlity .....\n");
				}
			}
		}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		if (tokens >= 2){
			int cmp2 = strcmp(first_args , "setdb");
			if(cmp2 == 0){
				FILE *database;
				int c;
				//second args is my filename
				database = fopen(second_args,"r");
				if( database == NULL){ //case where file might not exist.Seg fault if you try to close the file 
					printf("Unable to read from %s\n" , second_args);
					boolean = NO;
					continue;
				}
				else{
					int word_count = 0;
					int line_counter = 0;
					int lines = 0;
					//increment_for_realloc++;
					while(exact_lines = file_memalloc(database)){
						increment_for_realloc++;
		
						int len = strlen(exact_lines) + 1;
						//allocate memory according to exact_lines
						first = (char *) malloc(len * sizeof(char));
						second =  (char *) malloc(len * sizeof(char));
						third =  (char *) malloc(len * sizeof(char));
						extra =  (char *) malloc(len * sizeof(char));
						if(increment_for_realloc >= 5){
							outfile = realloc(outfile , increment_for_realloc * 10 * sizeof(char *));
						}
						printf("more space counter -> %d\n" , increment_for_realloc);
						outfile[word_count] = malloc(len * sizeof(char));

						lines = sscanf(exact_lines ,"%s %s %s %[^\n]s" , first ,second ,third ,extra );
						//printf("lines -> %d\n" , lines);
						if(lines < 0) {
							word_count++;
							line_counter++;
							continue; // a fix because empty lines were not counted
						}
						if (lines != 3){
							line_counter++; // number of malformed lines as a check
							boolean = YES;
						}
						//added for putrel 
						if(lines == 4){
							trim(exact_lines);
							//printf("rel-> %s ent-> %s ent-> %s\n" , first ,second , third); 
							strncpy(outfile[word_count] , exact_lines , strlen(exact_lines) + 1);
							printf("%s\n" , outfile[word_count]);
							strcat(outfile[word_count] , "\n");
							//printdb = word_count; // refrence to print over outfile
							boolean = YES;
						}
						else{
							trim(exact_lines);
							printf("rel-> %s ent-> %s ent-> %s\n" , first ,second , third); 
							strncpy(outfile[word_count] , exact_lines , strlen(exact_lines) + 1);
							printf("%s\n" , outfile[word_count]);
							strcat(outfile[word_count] , "\n");
							//printdb = word_count; // refrence to print over outfile
							boolean = YES;
						}
						word_count++;
						free(exact_lines);
					}	
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------															
					//just for formatting with sscanf return value
					printf("Read %d facts from %s\n" , (word_count - line_counter) , second_args);
					if(line_counter > 0){
						printf("%d malformed lines were ignored.\n" , line_counter);
					}
					if(tokens == 3){  
						printf("Extra arguments ignored.\n");
					}
					fclose(database); //closes the buffer stream				
				}
			}
			else{
				printf("Unrecognized command.\n");
			}	
		}
	}while(1); 
}


