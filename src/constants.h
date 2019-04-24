// Eleazar Gomez
// constants.h

#ifndef CONSTANTS_H
#define CONSTANTS_H

//#include <string>

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

// Start and end indicies for intial lower case letters
const int G1C1_START = 0;
const int G1C3_END = 1535;

// Start and end indicies for intial upper case letters
const int G2C1_START = 1536;
const int G4C3_END = 6143;

// Total size of the 4 groups of 3 chunks
const int TOTAL_SIZE = 6144;

// Group/Chunk info
const int NUM_GROUPS = 4;
const int NUM_CHUNKS = 3;
const int GROUP_SIZE = 1536;
const int CHUNK_SIZE = 512;

// For speed_check
const int SPEED = 5000;

#endif

