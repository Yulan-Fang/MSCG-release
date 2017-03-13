//
//  misc.cpp
//  
//
//  Copyright (c) 2016 The Voth Group at The University of Chicago. All rights reserved.
//

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <sstream>

#include "misc.h"

// Global variable assignments

const double VERYSMALL = 1.0e-14;
const float VERYSMALL_F = 1.0e-6; // Small number for single precision
const double MAX_INPUT_FORCE_VALUE = 1000.0; // Filter some noisy data
const int MAX_CG_TYPE_NAME_LENGTH = 24; // Max length for CG type names
const double DEGREES_PER_RADIAN = 180.0 / M_PI;

// An error-catching wrapper for fopen.

FILE* open_file(const char* file_name, const char* mode)
{
    FILE* filepointer = fopen(file_name, mode);
    if (filepointer == NULL) {
        fprintf(stderr, "Failed to open file %s.\n", file_name);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    return filepointer;
}

// Integrate function to calculate a potential from a force and distance vectors.

void integrate_force(const std::vector<double> &axis_vals, const std::vector<double> &force_vals, std::vector<double> &potential_vals) 
{
    potential_vals = std::vector<double>(axis_vals.size());
    potential_vals[axis_vals.size() - 1] = 0.0;
    for(int k = axis_vals.size() - 2; k >= 0; k--) {     
        potential_vals[k] = potential_vals[k+1] + 0.5 * (axis_vals[k + 1] - axis_vals[k]) * (force_vals[k] + force_vals[k + 1]);
    }
}

// Function to pad 2 vectors so that the first runs between low and high values with fpad

void pad_values_front(const double low, std::vector<double>& axis_vals, std::vector<double>& force_vals, const double fpad)
{
	std::vector<double>::iterator axis_it;
  	std::vector<double>::iterator force_it;
 
	double spacing = axis_vals[1] - axis_vals[0];
	while (axis_vals[0] - spacing > low) {
		axis_it = axis_vals.begin();
 		force_it = force_vals.begin();
 
		axis_vals.insert(axis_it, axis_vals[0] - spacing);
		force_vals.insert(force_it, fpad);	
	}
	
	if (axis_vals[0] - 0.01 > low) {
		axis_it = axis_vals.begin();
 		force_it = force_vals.begin();
 
		axis_vals.insert(axis_it, low);
		force_vals.insert(force_it, fpad);
	}
}

void pad_values_back(const double high, std::vector<double>& axis_vals, std::vector<double>& force_vals, const double fpad)
{
	double spacing = axis_vals[2] - axis_vals[1];
	int size = axis_vals.size();	
	while (axis_vals[size] + spacing < high) {
		axis_vals.push_back(axis_vals[size] + spacing);
		force_vals.push_back(fpad);
		size++;
	}
	
	if (axis_vals[size] + 0.01 < high) {
		axis_vals.push_back(axis_vals[size] + spacing);
		force_vals.push_back(fpad);
	}
	
}

// Find the index of the minimum value in a vector.

unsigned get_min_index(const std::vector<double> &potential_vals) 
{
    double min_val = potential_vals[0];
    unsigned min_index = 0;
    for(unsigned k = 0; k < potential_vals.size(); k++) {
        if (potential_vals[k] < min_val) {
            min_val = potential_vals[k];
            min_index = k;
        }
    }
    return min_index;
}

// Subtract off minimum value from a vector.

void standardize_potential(std::vector<double> &potential_vals) 
{
    unsigned min_index = get_min_index(potential_vals);
    // Standardize by the minimum value.
    double min_val = potential_vals[min_index];
    for(unsigned k = 0; k < potential_vals.size(); k++) {
        potential_vals[k] -= min_val;
    }
    // run twice to refine any floating-point problems.
    min_val = potential_vals[min_index];
    for(unsigned k = 0; k < potential_vals.size(); k++) {
        potential_vals[k] -= min_val;
    }
}

// C++ tokenizing function for strings.

int StringSplit(std::string source, const char *const delimiter, std::string* results)
{
	int count = 0;
    size_t prev = 0;
    size_t next = 0;

    while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
    {
        if (next - prev != 0)
        {
            results[count] = source.substr(prev, next - prev);
	        count++;
        }
        prev = next + 1;
    }

    if (prev < source.size())
    {
        results[count] = source.substr(prev);
        count++;
    }
    return count;
}

// A function to match a string against the list of type names.
int match_type(std::string &source, char** name, const int n_types)
{
	char unknown[10];
	sprintf(unknown, "%s", source.c_str());
	
	// Check if this type is a name.
	for (int i = 0; i < n_types; i++) {
		if( strcmp(unknown, name[i]) == 0 ) {
			return i + 1;
		}
	}
	
	// Check if this type is a number.
	if(isdigit(unknown[0]) != 0) {
		return atoi(unknown); 
	}
	return -1;	
}

void check_and_open_in_stream(std::ifstream &in_stream, const char* filename) 
{
	in_stream.open(filename, std::ifstream::in);
    if (in_stream.fail()) {
		fprintf(stderr, "Problem opening file %s\n", filename);
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
}

void check_and_read_next_line(std::ifstream &in_stream, std::string &line)
{
	if(!std::getline(in_stream, line)) {
			fprintf(stderr, "\nIt appears that the file is no longer open.\n");
			fprintf(stderr, "Please check that you are not attempting to read past the end of the file and try again.\n");
			fflush(stderr);
			exit(EXIT_FAILURE);
	}	
}

void check_and_read_next_line(std::ifstream &in_stream, std::string &line, int &line_num)
{
	check_and_read_next_line(in_stream, line);
	line_num++;	
}


void swap_pair(int& a, int& b) 
{
	int tn = b;
	b = a;
	a = tn;
}
