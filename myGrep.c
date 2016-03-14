#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>

#define BUFF 1024
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

/*Function Declaration*/
int grep_stream(FILE*, char*, char*);
char *get_next_line(FILE*);

int main(int argc, char *argv[])
{
	char *search_str = NULL;
	char *filename = NULL;
	FILE *fp = NULL;
	/* checks executable and something else were called in command line*/	
	if(argc > 1){
		int i = 0;
		for(i = 0; i < argc; i++){
			switch (i) {
				case 0:
					break;
				case 1:
					search_str = argv[i];
					break;
				default:
					filename = argv[i];
					if((fp = fopen(argv[i], "r")) == NULL) {
						perror("ERROR: FILE could not be read");
						break;
					}
					grep_stream(fp, search_str, filename);
					if (ferror(fp)) {
						perror("Error closing file");
						break;
					}else{
						fclose(fp);
					}
					break;
			}
		}
		/*A pattern is given without specified file(s)*/
		if(2 == i){
			fp = stdin;
			filename = "stdin";
			grep_stream(fp, search_str, filename);
			fclose(fp);
		}
	/*execution without pattern or file*/
	}else{
		fprintf(stderr, "Usage: myGrep PATTERN [FILE]\n");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;	
}
int grep_stream(FILE *fp, char *search_str, char *filename)
{
	int found = 0;
	char *line = NULL;
	while ((line = get_next_line(fp))) {
		if(ferror(fp)){
			perror("Error Reading line");
			exit(EXIT_FAILURE);
		}
		int is_present_in_line = strstr(line, search_str) ? 1 : 0;  
		if (is_present_in_line) {
			fprintf(stdout, "%s\n", line);
			found = 0;
		}
	free (line);
	}
	if(ferror(fp)){
		perror("Error Reading line");
		exit(EXIT_FAILURE);
	}
	return found;
}
/*

char *get_next_line(FILE *fp)
{
	char buff[BUFF];
	int ch;
	char *tmp = buff;
	char *ret_line = NULL;

	while((ch = fgetc(fp)) != EOF){
		if('\n' == ch){
			*tmp = '\0';
			ret_line = (char *) malloc((tmp - buff + 1) * sizeof(char));
			memcpy(ret_line, buff, tmp-buff+1);
			return ret_line;
		}else{
			*tmp++ = ch;
		}
	}
	if(tmp != buff) {
		*tmp = '\0';
		ret_line = (char *) malloc((tmp - buff + 1) * sizeof(char));
		memcpy(ret_line, buff, tmp-buff+1);	
		return ret_line;
		}
	return ret_line;
}
*/
char *get_next_line(FILE *fp)
{
	/*When statically declaring a variable, the size of the memory must be explicitly stated, which is not how malloc works*/
	char *ret_line = (char *) malloc(BUFF * sizeof(char));
	char *tmp = ret_line;
	int ch;
	
	while((ch = fgetc(fp)) != EOF) {
		if('\n' == ch){
			*tmp = '\0';
			return ret_line;
		}	
		*tmp++ = ch;
	}
	/*check for hitting EOF before newline character*/
	if(tmp != ret_line) {
		*tmp = '\0';
		return ret_line;
	}
	return NULL;
}
