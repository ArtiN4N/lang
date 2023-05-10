#include <stdio.h>
#include <stdlib.h>

typedef enum CompileError { FACTOR_LENGTH = 0, COMMAND_LENGTH, MISSING_SEMICOLON } CompileError;

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        printf("Please input a file to compile, yo~.\n");
        return -1;
    }

    FILE* data;

    if ((data = fopen(argv[1], "r")) == NULL) {
        printf("Cannot open file, bitch~.\n");
        return -1;
    }
    
    char factor_text[4];
    unsigned char factor = 1;
    char command[6];
    
    int bytes[256] = { 0 };
    unsigned char byte_pointer = 0;

    int c = getc(data);

    while (c != EOF) {
    	
    	// Read factor
    	int factor_index = 0;
    	
    	while (c >= 49 && c <= 57) {
    	
    		if (factor_index > 2) {
    			compile_error(FACTOR_LENGTH);
    			return;
    		}
    		
    		factor_text[factor_index] = c;
    		factor_index++;
    	}
    	
    	factor_text[3] = '\0';
    	
    	sscanf(factor_text, "%d", &factor);
    	
    	// Read command
    	int command_index = 0;
    	
    	while (c != ';') {
    	
    		if (factor_index > 4) {
    			compile_error(COMMAND_LENGTH);
    			return;
    		}
    		
    		command[command_index] = c;
    		command_index++;
    	}
    	
    	factor_text[5] = '\0';
    	
    	execute_command(factor, command, bytes, byte_pointer);
    	
    	factor = 1;
        c = getc(data); 
    }

    printf("\nMy name is Skyler White, YO~\n");

    fclose(data);

    return 0;
}
