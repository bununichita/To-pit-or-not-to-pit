#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum sensor_type {
	TIRE,
	PMU
};

typedef struct {
	enum sensor_type sensor_type;
	void *sensor_data;
	int nr_operations;
	int *operations_idxs;
} sensor;

typedef struct __attribute__((__packed__))
{
	float voltage;
	float current;
	float power_consumption;
	int energy_regen;
	int energy_storage;
} power_management_unit;

typedef struct __attribute__((__packed__))
{
	float pressure;
	float temperature;
	int wear_level;
	int performace_score;
} tire_sensor;

sensor **s_vector_alloc(int size);
int parse(char s[50]);
void read_0(sensor **s_vector, int i, FILE *data);
void read_1(sensor **s_vector, int i, FILE *data);
void print(sensor **s_vector, int index, int s_number);
void analyze(sensor **s_vector, int n, int s_number);
int clear(sensor **s_vector, int s_number);
void exit_prog(sensor **s_vector, int s_number);

#endif // _STRUCTS_H_