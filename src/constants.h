// Eleazar Gomez, Anton Shirokov
// main.cpp
//
// This file contains the constants used throughout our program so that
// we can eliminate magic numbers. It includes arrays of characters for,
// initializeing chunk data, group and chunk attributes, and the value
// to check speed_check against to slow down the processes.

#ifndef CONSTANTS_H
#define CONSTANTS_H

const int NUM_PROCESSES = 5;

const char LOWER_CASE_LETTERS[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
						 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
						 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
						 'y', 'z'};

const char UPPER_CASE_LETTERS[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
						 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
						 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
						 'Y', 'Z'};

// Size letter arrays
const int L_SIZE = 26;

// Start and end indicies for intial setup
const int CHUNK1_START = 0;
const int CHUNK3_END = 1535;

// Group/Chunk info
const int NUM_GROUPS = 4;
const int NUM_CHUNKS = 3;
const int GROUP_SIZE = 1536;
const int CHUNK_SIZE = 512;

// For speed_check
const int SPEED = 5000;

#endif

