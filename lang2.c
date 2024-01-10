#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Error { SAFE = 0, CMD } Error;

typedef enum Command { ADD = 0, SUB, MUL, XOR, PRINT } Command;

Error Execute(Command command, int* registers, int dest, int src) {
	
	switch( command ) {
		case ADD:
			registers[dest] = registers[dest] + registers[src];
			break;
		case SUB:
			registers[dest] = registers[dest] - registers[src];
			break;
		case MUL:
			registers[dest] = registers[dest] * registers[src];
			break;
		case XOR:
			registers[dest] = registers[dest] ^ registers[src];
			break;
		case PRINT:
			printf("%c", registers[src]);
			break;
		default:
			break;
	}
	
	return SAFE;
}

Error decode_line(char* line, ssize_t length, int* registers) {

	if (strcmp(line, "\n") == 0) return SAFE;	
	
	if (length < 5) {
		printf("Error: reading command");
		return CMD;
	}

	char cmd[6];
	
	int i = 0;
	char c = line[i];
	
	while (c != ':' && i < 5) {
		cmd[i] = c;
		i++;
		c = line[i];
	}
	cmd[5] = '\0';
	
	Command to_execute;
	
	if (strcmp(cmd, "ADD") == 0) to_execute = ADD;
	else if (strcmp(cmd, "SUB") == 0) to_execute = SUB;
	else if (strcmp(cmd, "MUL") == 0) to_execute = MUL;
	else if (strcmp(cmd, "XOR") == 0) to_execute = XOR;
	else if (strcmp(cmd, "PRINT") == 0) to_execute = PRINT;
	else {
		printf("Error: reading command");
		return CMD;
	}
	
	while (c != '%') {
		i++;
		
		if (i >= length) {
			printf("Error: reading dest register");
			return CMD;
		}
		
		c = line[i];
	}
	
	if (i + 4 >= length) {
		printf("Error: reading dest register");
		return CMD;
	}
	
	if (line[i + 1] != 'r') {
		printf("Error: reading dest register");
		return CMD;
	}
	if (!(line[i + 2] >= 48 && line[i + 2] <= 55)) {
		printf("Error: reading dest register");
		return CMD;
	}
	if (line[i + 3] != ',') {
		printf("Error: missing comma");
		return CMD;
	}
	
	int register_one = line[i + 2] - '0';
	
	i += 4;
	c = line[i];
	

	
	while (c != '$' && c != '%') {
		i++;
		
		if (i >= length) {
			printf("Error: reading src register, out of space");
			return CMD;
		}
		
		c = line[i];
	}
	
	int register_two;
	
	if (c == '%') {
	
		if (i + 2 >= length) return CMD;
		
		if (line[i + 1] != 'r') {
			printf("Error: reading src register");
			return CMD;
		}
		if (!(line[i + 2] >= 48 && line[i + 2] <= 55)) {
			printf("Error: reading src register");
			return CMD;
		}
		
		register_two = line[i + 2] - '0';
		
	} else {
		i++;
		c = line[i];
		if (i > length) {
			printf("Error: reading immediate value");
			return CMD;
		}
		
		char num[6];
		
		int k = 0;
		
		while(line[i] >= 48 && line[i] <= 57) {
			num[k] = c;
		
			i++;
			k++;
			if (i >= length || k == 5) {
				printf("Error: reading immediate value");
				return CMD;
			}
			c = line[i];
		}
		num[k] = '\0';
		
		register_two = 8;
		registers[8] = atoi(num);
	}
	
	Error ret = Execute(to_execute, registers, register_one, register_two);
	
	return ret;	
}

int main(int argc, char* argv[]) {

	if (argc != 2) {
        printf("Please input a file to compile, yo~.\n");
        return -1;
    }
    
    FILE* data = fopen(argv[1], "r");
    
    if (data == NULL) {
    	printf("Mistah White, I Can't open this file, b#$%^~.\n");
        return -1;
    }
    
    //---------------------------------------------------
    
    int registers[9] = { 0 };
    
    char* line = NULL;
    size_t len = 0;
    ssize_t length;
    
    while ((length = getline(&line, &len, data)) != -1) {
    	if (decode_line(line, length, registers) != SAFE) {
    		printf("Mistah White, like, theres an error, yo~\n");
    		break;
    	}
    }
    
    fclose(data);
    if (line) free(line);

	return 0;
}
