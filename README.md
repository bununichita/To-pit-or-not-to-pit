
# Sensor Monitoring and Analysis System in C

### Author: Nichita-Adrian Bunu, 323CA Facultatea de Automatica si Calculatoare UNSTPB 
**Contact:** [nichita_adrian.bunu@stud.acs.upb.ro](mailto:nichita_adrian.bunu@stud.acs.upb.ro)

---

## Overview

This project is a C-based system designed for monitoring and analyzing data from two types of sensors: tire sensors and power management units (PMU). It reads data from binary files, processes sensor information based on commands, and applies specific operations to assess sensor statuses. The system also includes memory management and error checking functionalities.

---

## Features and Functionality

### Sensor Initialization

Allocates memory for a vector of sensors using `s_vector_alloc`, which creates and initializes memory for either `tire_sensor` or `power_management_unit`.

### Commands

1. **Print**: Displays sensor data based on its type.
   - Tire sensors: Shows pressure, temperature, wear level, and performance score.
   - PMUs: Displays voltage, current, power consumption, energy regeneration, and energy storage.

2. **Clear**: Iterates through each sensor, using `check_0` and `check_1` functions to validate sensor parameters. Deletes sensors that do not meet defined criteria and reallocates memory.

3. **Analyze**: Executes operations from `operations.c` based on a list of operation indices, applying relevant functions to analyze each sensor’s parameters.

4. **Exit**: Releases all allocated memory and exits the program.

---

## Code Structure

### `main.c`

Implements the core functionalities:
- **Sensor Allocation**: Initializes a vector of `sensor` structures.
- **Sensor Data Processing**: Reads data from files, processes commands (`print`, `clear`, `analyze`, and `exit`), and manages the main program loop.
- **Command Parsing**: Interprets user commands to decide actions on the sensor data.
- **Memory Management**: Deallocates memory on exit to prevent memory leaks.

### `operations.c`

Defines specific operations to assess sensor parameters:
- **Tire Sensor Operations**:
  - `tire_pressure_status`: Assesses pressure range.
  - `tire_temperature_status`: Checks temperature range.
  - `tire_wear_level_status`: Evaluates wear level.
  - `tire_performance_score`: Calculates an overall performance score.

- **PMU Operations**:
  - `pmu_compute_power`: Computes power output.
  - `pmu_regenerate_energy`: Manages energy storage.
  - `pmu_get_energy_usage`: Calculates energy consumption.
  - `pmu_is_battery_healthy`: Verifies PMU health based on voltage, current, and energy levels.

### `structs.h`

Contains essential data structures for the sensor types:
- **`sensor` struct**: Represents a generic sensor with type, data, operation indices, and operation count.
- **`power_management_unit` struct**: Stores PMU data, including voltage, current, power consumption, and energy metrics.
- **`tire_sensor` struct**: Stores tire-specific data, including pressure, temperature, wear level, and performance score.

---

## How to Compile and Run

1. **Compile the project**:
   ```bash
   make build
   ```

2. **Run the executable**:
   ```bash
   make run
   ```

3. **Clean up compiled files**:
   ```bash
   make clean
   ```

---
