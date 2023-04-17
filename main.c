#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

int main(int argc, char const *argv[])
{
	// printf("test\n");
	sensor **s_vector;
	FILE *data = fopen(argv[1], "rb");
	int s_number;
	fread(&s_number, sizeof(int), 1, data);
	// printf("%d\n", s_number);
	s_vector = s_vector_alloc(s_number);
	// printf("test\n");
	for (int i = 0; i < s_number; i++) {
		fread(&(s_vector[i]->sensor_type), sizeof(int), 1, data);
		if (s_vector[i]->sensor_type == 0) {
			read_0(s_vector, i, data);
		} else {
			read_1(s_vector, i, data);
		}
	}
	// printf("test\n");
	// print(s_vector, s_number);
	char command[50];
	while (1) {
		fgets(command, 50, stdin);
		if (strncmp(command, "print", 5) == 0) {
			int index = parse(command);
			print(s_vector, index, s_number);
			// for (int i = 0; i < s_number; i++) {
			// 	print(s_vector, i, s_number);
			// }
		}
		if (strncmp(command, "analyze", 7) == 0) {
			int index = parse(command);
			analyze(s_vector, index, s_number);
		}
		if (strncmp(command, "clear", 5) == 0) {
			s_number = clear(s_vector, s_number);
		}
		if (strncmp(command, "exit", 4) == 0) {
			// printf("%d\n", s_number);
			exit_prog(s_vector, s_number);
			free(s_vector);
			fclose(data);
			return 0;
		}
	}
}
