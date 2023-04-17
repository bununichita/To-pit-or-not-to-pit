#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------- DO NOT MODIFY ------- //

static void tire_pressure_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->pressure >= 21 && t->pressure <= 26) {
		printf("Tire has normal pressure.\n");
	} else if (t->pressure > 26 && t->pressure <= 28) {
		printf("Tire has high pressure.\n");
	} else if (t->pressure >= 19 && t->pressure < 21) {
		printf("Tire has low pressure.\n");
	} else {
		printf("Tire has abnormal pressure.\n");
	}
}

static void tire_temperature_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->temperature >= 0 && t->temperature <= 120) {
		printf("Tire has normal temperature.\n");
	} else {
		printf("Tire has abnormal temperature.\n");
	}
}

static void tire_wear_level_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->wear_level >= 0 && t->wear_level <= 20) {
		printf("Tire is new.\n");
	} else if (t->wear_level >= 20 && t->wear_level <= 40) {
		printf("Tire is slightly used.\n");
	} else if (t->wear_level >= 40 && t->wear_level <= 60) {
		printf("Tire is used. Consider a pit stop!\n");
	} else {
		printf("Tire is extremely used. Box this lap!\n");
	}
}

static void tire_performance_score(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	int score = 0;

	// Check pressure
	if (t->pressure >= 21 && t->pressure <= 26) {
		score += 4;
	} else if (t->pressure >= 19 && t->pressure < 21) {
		score += 2;
	} else if (t->pressure > 26 && t->pressure <= 28) {
		score += 3;
	} else {
		score -= 1;
	}

	// Check temperature
	if (t->temperature >= 80 && t->temperature <= 100) {
		score += 4;
	} else if (t->temperature >= 100 && t->temperature < 120) {
		score += 3;
	} else if (t->temperature > 60 && t->temperature <= 80) {
		score += 2;
	} else {
		score -= 1;
	}

	// Check wear level
	if (t->wear_level >= 0 && t->wear_level < 20) {
		score += 4;
	} else if (t->wear_level >= 20 && t->wear_level < 40) {
		score += 2;
	} else if (t->wear_level > 40 && t->wear_level < 60) {
		score -= 1;
	} else {
		score -= 2;
	}

	// Cap the score at 10
	if (score > 10) {
		score = 10;
	} else if (score < 1) {
		score = 1;
	}

	t->performace_score = score;
	printf("The tire performance score is: %d\n", score);
}

static void pmu_compute_power(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	float power = pmu->voltage * pmu->current;

	printf("Power output: %.2f kW.\n", power);
}

static void pmu_regenerate_energy(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;

	pmu->energy_storage += pmu->energy_regen;
	pmu->energy_regen = 0;

	// Cap the energy storage to 100%
	if (pmu->energy_storage > 100)
		pmu->energy_storage = 100;

	printf("Energy left in storage: %d\n", pmu->energy_storage);
}

static void pmu_get_energy_usage(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	float energy = pmu->power_consumption * pmu->current;

	printf("Energy usage: %.2f kW\n", energy);
}

static void pmu_is_battery_healthy(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	int ok = 0;

	if (pmu->voltage < 10 || pmu->voltage > 20) {
		printf("Battery voltage out of range: %.2fV\n", pmu->voltage);
		ok = 1;
	}

	if (pmu->current < -100 || pmu->current > 100) {
		printf("Battery current out of range: %.2fA\n", pmu->current);
		ok = 1;
	}

	if (pmu->power_consumption < 0 || pmu->power_consumption > 1000) {
		printf("Power consumption out of range: %.2f kW\n",
			pmu->power_consumption);
		ok = 1;
	}

	if (pmu->energy_regen < 0 || pmu->energy_regen > 100) {
		printf("Energy regeneration out of range: %d%%\n", pmu->energy_regen);
		ok = 1;
	}

	if (pmu->energy_storage < 0 || pmu->energy_storage > 100) {
		printf("Energy storage out of range: %d%%\n", pmu->energy_storage);
		ok = 1;
	}

	if (ok == 1)
		return;

	printf("Battery working as expected!\n");
}

void get_operations(void **operations)
{
	operations[0] = tire_pressure_status;
	operations[1] = tire_temperature_status;
	operations[2] = tire_wear_level_status;
	operations[3] = tire_performance_score;
	operations[4] = pmu_compute_power;
	operations[5] = pmu_regenerate_energy;
	operations[6] = pmu_get_energy_usage;
	operations[7] = pmu_is_battery_healthy;
}

// ------- DO NOT MODIFY ------- //


sensor **s_vector_alloc(int size)
{
	sensor **s_v;
	s_v = malloc(size * sizeof(sensor *));
	for (int i = 0; i < size; i++) {
		s_v[i] = malloc(sizeof(sensor));
	}
	return s_v;
}

int parse(char s[50])
{
	char *p;
	p = strtok(s, " ");
	p = strtok(NULL, " ");
	return atoi(p);
}

void read_0(sensor **s_vector, int i, FILE *data)
{
	s_vector[i]->sensor_data = malloc(sizeof(tire_sensor));
	fread(&((tire_sensor *)s_vector[i]->sensor_data)->pressure, sizeof(float), 1, data);
	fread(&((tire_sensor *)s_vector[i]->sensor_data)->temperature, sizeof(float), 1, data);
	fread(&((tire_sensor *)s_vector[i]->sensor_data)->wear_level, sizeof(int), 1, data);
	fread(&((tire_sensor *)s_vector[i]->sensor_data)->performace_score, sizeof(int), 1, data);
	((tire_sensor *)s_vector[i]->sensor_data)->performace_score = -1;
	fread(&s_vector[i]->nr_operations, sizeof(int), 1, data);
	s_vector[i]->operations_idxs = malloc(s_vector[i]->nr_operations * sizeof(int));
	// for (int j = 0; j < s_vector[i]->nr_operations; j++) {
	// 	fread(&s_vector[i]->operations_idxs[j], sizeof(int), 1, data);
	// }

	fread(s_vector[i]->operations_idxs, sizeof(int), s_vector[i]->nr_operations, data);
}

void read_1(sensor **s_vector, int i, FILE *data)
{
	s_vector[i]->sensor_data = malloc(sizeof(power_management_unit));
	fread(&((power_management_unit *)s_vector[i]->sensor_data)->voltage, sizeof(float), 1, data);
	fread(&((power_management_unit *)s_vector[i]->sensor_data)->current, sizeof(float), 1, data);
	fread(&((power_management_unit *)s_vector[i]->sensor_data)->power_consumption, sizeof(float), 1, data);
	fread(&((power_management_unit *)s_vector[i]->sensor_data)->energy_regen, sizeof(int), 1, data);
	fread(&((power_management_unit *)s_vector[i]->sensor_data)->energy_storage, sizeof(int), 1, data);
	fread(&s_vector[i]->nr_operations, sizeof(int), 1, data);
	s_vector[i]->operations_idxs = malloc(s_vector[i]->nr_operations * sizeof(int));
	// for (int j = 0; j < s_vector[i]->nr_operations; j++) {
	// 	fread(&s_vector[i]->operations_idxs[j], sizeof(int), 1, data);
	// }

	fread(s_vector[i]->operations_idxs, sizeof(int), s_vector[i]->nr_operations, data);
}

int get_s_index(sensor **s_vector, int index, int  s_number)
{
	if (index < 0 || index >= s_number) {
		return -1;
	}
	for (int i = 0; i < s_number; i++) {
		if (s_vector[i]->sensor_type == 1) {
			if (!index) {
				return i;
			} else {
				index--;
			}
		}
	}
	for (int i = 0; i < s_number; i++) {
		if (s_vector[i]->sensor_type == 0) {
			if (!index) {
				return i;
			} else {
				index--;
			}
		}
	}
	return -1;
}

void print(sensor **s_vector, int index, int s_number)
{


	index = get_s_index(s_vector, index, s_number);
	// printf("\nindex = %d\n", index);
	if (index < 0) {
		printf("Index not in range!\n");
		return;
	}
	if (s_vector[index]->sensor_type == 0) {
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", ((tire_sensor *)s_vector[index]->sensor_data)->pressure);
		printf("Temperature: %.2f\n", ((tire_sensor *)s_vector[index]->sensor_data)->temperature);
		printf("Wear Level: %d%%\n", ((tire_sensor *)s_vector[index]->sensor_data)->wear_level);
		if (((tire_sensor *)s_vector[index]->sensor_data)->performace_score >= 0) {
			printf("Performance Score: %d\n", ((tire_sensor *)s_vector[index]->sensor_data)->performace_score);
		} else {
			printf("Performance Score: Not Calculated\n");
		}

	} else if (s_vector[index]->sensor_type == 1) {
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", ((power_management_unit *)s_vector[index]->sensor_data)->voltage);
		printf("Current: %.2f\n", ((power_management_unit *)s_vector[index]->sensor_data)->current);
		printf("Power Consumption: %.2f\n", ((power_management_unit *)s_vector[index]->sensor_data)->power_consumption);
		printf("Energy Regen: %d%%\n", ((power_management_unit *)s_vector[index]->sensor_data)->energy_regen);
		printf("Energy Storage: %d%%\n", ((power_management_unit *)s_vector[index]->sensor_data)->energy_storage);
	}

}

void analyze(sensor **s_vector, int n, int s_number)
{
	n = get_s_index(s_vector, n, s_number);
	void (**operations)(void *) = malloc(8 * sizeof(void (*)(void *)));
	get_operations((void **)operations);
	for (int i = 0; i < s_vector[n]->nr_operations; i++) {
		(*operations[s_vector[n]->operations_idxs[i]])(s_vector[n]->sensor_data);
	}
	free(operations);
}

int check_0(sensor **s_vector, int index)
{
	float pressure;
	pressure = ((tire_sensor *)s_vector[index]->sensor_data)->pressure;
	if (!(pressure >= 19 && pressure <= 28)) {
		return 1;
	}
	float temp;
	temp = ((tire_sensor *)s_vector[index]->sensor_data)->temperature;
	if (!(temp >= 0 && temp <= 120)) {
		return 1;
	}
	int wear;
	wear = ((tire_sensor *)s_vector[index]->sensor_data)->wear_level;
	if (!(wear >= 0 && wear <= 100)) {
		return 1;
	}
	return 0;
}

int check_1(sensor **s_vector, int index)
{
	float voltage;
	voltage = ((power_management_unit *)s_vector[index]->sensor_data)->voltage;
	if (!(voltage >= 10 && voltage <= 20)) {
		return 1;
	}
	float current;
	current = ((power_management_unit *)s_vector[index]->sensor_data)->current;
	if (!(current >= -100 && current <= 100)) {
		return 1;
	}
	float p_c;
	p_c = ((power_management_unit *)s_vector[index]->sensor_data)->power_consumption;
	if (!(p_c >= 0 && p_c <= 1000)) {
		return 1;
	}
	int energy_regen;
	energy_regen = ((power_management_unit *)s_vector[index]->sensor_data)->energy_regen;
	if (!(energy_regen >= 0 && energy_regen <= 100)) {
		return 1;
	}
	int energy_storage;
	energy_storage = ((power_management_unit *)s_vector[index]->sensor_data)->energy_storage;
	if (!(energy_storage >= 0 && energy_storage <= 100)) {
		return 1;
	}
	return 0;
}

void remove_elem(sensor **s_vector, int index, int s_number)
{
	free(s_vector[index]->operations_idxs);
	free(s_vector[index]->sensor_data);
	free(s_vector[index]);
	s_vector[index] = NULL;
	while (index < s_number - 1) {
		s_vector[index] = s_vector[index + 1];
		index++;
	}
	s_vector[index] = NULL;
}

int clear(sensor **s_vector, int s_number)
{
	for (int i = 0; i < s_number; i++) {
		if (s_vector[i]->sensor_type == 1) {
			if (check_1(s_vector, i)) {
				// printf("test 1\n");
				remove_elem(s_vector, i, s_number);
				s_number--;
				i--;
			}
		}
	}
	for (int i = 0; i < s_number; i++) {
		if (s_vector[i]->sensor_type == 0) {
			if (check_0(s_vector, i)) {
				// printf("test 1\n");
				remove_elem(s_vector, i, s_number);
				s_number--;
				i--;
			}
		}
	}
	return s_number;
}

// void clear(sensor **s_vector, int s_number)
// {
// 	for (int i = 0; i < s_number; i++) {
// 		if (s_vector[i]->sensor_type == 0) {
// 			if (check_0(s_vector, i)) {
// 				remove_elem(s_vector, i, s_number);
// 				s_number--;
// 			}
// 		} else if (s_vector[i]->sensor_type == 1) {
// 			if (check_1(s_vector, i)) {
// 				remove_elem(s_vector, i, s_number);
// 				s_number--;
// 			}
// 		}
// 	}
// }

void exit_prog(sensor **s_vector, int s_number)
{
	for (int i = 0; i < s_number; i++) {
		free(s_vector[i]->operations_idxs);
		free(s_vector[i]->sensor_data);
		free(s_vector[i]);
	}
}