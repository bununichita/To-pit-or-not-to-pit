// # Copyright 2023 Nichita - Adrian Bunu <bununichita@gmail.com>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"

sensor *s_vector_alloc(int size)
{
	sensor *s_v;
	s_v = malloc(size * sizeof(sensor));
	return s_v;
}

int parse(char s[50])
{
	char *p;
	p = strtok(s, " ");
	p = strtok(NULL, " ");
	return atoi(p);
}

void read_0(sensor *s_vector, int i, FILE *data)
{
	s_vector[i].sensor_data = malloc(sizeof(tire_sensor));
	fread(&((tire_sensor *)s_vector[i].sensor_data)->pressure,
	sizeof(float), 1, data);
	fread(&((tire_sensor *)s_vector[i].sensor_data)->temperature,
	sizeof(float), 1, data);
	fread(&((tire_sensor *)s_vector[i].sensor_data)->wear_level,
	sizeof(int), 1, data);
	fread(&((tire_sensor *)s_vector[i].sensor_data)->performace_score,
	sizeof(int), 1, data);
	((tire_sensor *)s_vector[i].sensor_data)->performace_score = -1;
	fread(&s_vector[i].nr_operations, sizeof(int), 1, data);
	size_t s_int = sizeof(int);
	s_vector[i].operations_idxs = malloc(s_vector[i].nr_operations * s_int);
	fread(s_vector[i].operations_idxs, sizeof(int),
	s_vector[i].nr_operations, data);
}

void read_1(sensor *s_vector, int i, FILE *data)
{
	s_vector[i].sensor_data = malloc(sizeof(power_management_unit));
	fread(&((power_management_unit *)s_vector[i].sensor_data)->voltage,
	sizeof(float), 1, data);
	fread(&((power_management_unit *)s_vector[i].sensor_data)->current,
	sizeof(float), 1, data);
	fread(&((power_management_unit *)s_vector[i].sensor_data)
	->power_consumption, sizeof(float), 1, data);
	fread(&((power_management_unit *)s_vector[i].sensor_data)
	->energy_regen, sizeof(int), 1, data);
	fread(&((power_management_unit *)s_vector[i].sensor_data)
	->energy_storage, sizeof(int), 1, data);
	fread(&s_vector[i].nr_operations, sizeof(int), 1, data);
	size_t s_int = sizeof(int);
	s_vector[i].operations_idxs = malloc(s_vector[i].nr_operations * s_int);
	fread(s_vector[i].operations_idxs, sizeof(int),
	s_vector[i].nr_operations, data);
}

int get_s_index(sensor *s_vector, int index, int  s_number)
{
	if (index < 0 || index >= s_number) {
		return -1;
	}
	for (int i = 0; i < s_number; i++) {
		if (s_vector[i].sensor_type == 1) {
			if (!index) {
				return i;
			} else {
				index--;
			}
		}
	}
	for (int i = 0; i < s_number; i++) {
		if (s_vector[i].sensor_type == 0) {
			if (!index) {
				return i;
			} else {
				index--;
			}
		}
	}
	return -1;
}

void print(sensor *s_vector, int index, int s_number)
{


	index = get_s_index(s_vector, index, s_number);
	if (index < 0) {
		printf("Index not in range!\n");
		return;
	}
	if (s_vector[index].sensor_type == 0) {
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", ((tire_sensor *)s_vector[index].
		sensor_data)->pressure);
		printf("Temperature: %.2f\n", ((tire_sensor *)s_vector[index].
		sensor_data)->temperature);
		printf("Wear Level: %d%%\n", ((tire_sensor *)s_vector[index].
		sensor_data)->wear_level);
		if (((tire_sensor *)s_vector[index].sensor_data)
		->performace_score >= 0) {
			printf("Performance Score: %d\n", ((tire_sensor *)s_vector
			[index].sensor_data)->performace_score);
		} else {
			printf("Performance Score: Not Calculated\n");
		}

	} else if (s_vector[index].sensor_type == 1) {
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", ((power_management_unit *)s_vector[index].
		sensor_data)->voltage);
		printf("Current: %.2f\n", ((power_management_unit *)s_vector[index].
		sensor_data)->current);
		printf("Power Consumption: %.2f\n", ((power_management_unit *)
		s_vector[index].sensor_data)->power_consumption);
		printf("Energy Regen: %d%%\n", ((power_management_unit *)s_vector
		[index].sensor_data)->energy_regen);
		printf("Energy Storage: %d%%\n", ((power_management_unit *)s_vector
		[index].sensor_data)->energy_storage);
	}

}

void analyze(sensor *s_vector, int n, int s_number)
{
	n = get_s_index(s_vector, n, s_number);
	void (**operations)(void *) = malloc(8 * sizeof(void (*)(void *)));
	get_operations((void **)operations);
	for (int i = 0; i < s_vector[n].nr_operations; i++) {
		(*operations[s_vector[n].operations_idxs[i]])(s_vector[n].sensor_data);
	}
	free(operations);
}

int check_0(sensor *s_vector, int index)
{
	float pressure;
	pressure = ((tire_sensor *)s_vector[index].sensor_data)->pressure;
	if (!(pressure >= 19 && pressure <= 28)) {
		return 1;
	}
	float temp;
	temp = ((tire_sensor *)s_vector[index].sensor_data)->temperature;
	if (!(temp >= 0 && temp <= 120)) {
		return 1;
	}
	int wear;
	wear = ((tire_sensor *)s_vector[index].sensor_data)->wear_level;
	if (!(wear >= 0 && wear <= 100)) {
		return 1;
	}
	return 0;
}

int check_1(sensor *s_vector, int index)
{
	float voltage;
	voltage = ((power_management_unit *)s_vector[index].sensor_data)
	->voltage;
	if (!(voltage >= 10 && voltage <= 20)) {
		return 1;
	}
	float current;
	current = ((power_management_unit *)s_vector[index].sensor_data)
	->current;
	if (!(current >= -100 && current <= 100)) {
		return 1;
	}
	float p_c;
	p_c = ((power_management_unit *)s_vector[index].sensor_data)
	->power_consumption;
	if (!(p_c >= 0 && p_c <= 1000)) {
		return 1;
	}
	int energy_regen;
	energy_regen = ((power_management_unit *)s_vector[index].sensor_data)
	->energy_regen;
	if (!(energy_regen >= 0 && energy_regen <= 100)) {
		return 1;
	}
	int energy_storage;
	energy_storage = ((power_management_unit *)s_vector[index].sensor_data)
	->energy_storage;
	if (!(energy_storage >= 0 && energy_storage <= 100)) {
		return 1;
	}
	return 0;
}

void remove_elem(sensor **s_vect, int index, int s_number)
{
	sensor *s_vector = *s_vect;
	free(s_vector[index].operations_idxs);
	free(s_vector[index].sensor_data);
	while (index < s_number - 1) {
		s_vector[index] = s_vector[index + 1];
		index++;
	}
	s_vect = realloc(s_vect, s_number - 1);
}

int clear(sensor **s_vect, int s_number)
{
	sensor *s_vector = *s_vect;
	for (int i = 0; i < s_number; i++) {
		if (s_vector[i].sensor_type == 1) {
			if (check_1(s_vector, i)) {
				remove_elem(s_vect, i, s_number);
				s_number--;
				i--;
			}
		}
	}
	for (int i = 0; i < s_number; i++) {
		if (s_vector[i].sensor_type == 0) {
			if (check_0(s_vector, i)) {
				remove_elem(s_vect, i, s_number);
				s_number--;
				i--;
			}
		}
	}
	return s_number;
}

void exit_prog(sensor *s_vector, int s_number)
{
	for (int i = 0; i < s_number; i++) {
		free(s_vector[i].operations_idxs);
		free(s_vector[i].sensor_data);
	}
}

int main(int argc, char const *argv[])
{
	sensor *s_vector;
	FILE *data = fopen(argv[1], "rb");
	int s_number;
	fread(&s_number, sizeof(int), 1, data);
	s_vector = s_vector_alloc(s_number);
	for (int i = 0; i < s_number; i++) {
		fread(&(s_vector[i].sensor_type), sizeof(int), 1, data);
		if (s_vector[i].sensor_type == 0) {
			read_0(s_vector, i, data);
		} else {
			read_1(s_vector, i, data);
		}
	}
	char command[50];
	while (1) {
		fgets(command, 50, stdin);
		if (strncmp(command, "print", 5) == 0) {
			int index = parse(command);
			print(s_vector, index, s_number);
		}
		if (strncmp(command, "analyze", 7) == 0) {
			int index = parse(command);
			analyze(s_vector, index, s_number);
		}
		if (strncmp(command, "clear", 5) == 0) {
			s_number = clear(&s_vector, s_number);
		}
		if (strncmp(command, "exit", 4) == 0) {
			exit_prog(s_vector, s_number);
			free(s_vector);
			fclose(data);
			return 0;
		}
	}
}
