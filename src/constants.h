// Eleazar Gomez
// constants.h

#ifndef CONSTANTS_H
#define CONSTANTS_H

//#include <string>

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

// Group 1
const int G1C1_START = 0;
const int G1C1_END = 511;

const int G1C2_START = 512;
const int G1C2_END = 1023;

const int G1C3_START = 1024;
const int G1C3_END = 1535;

// Group 2
const int G2C1_START = 1536;
const int G2C1_END = 2047;

const int G2C2_START = 2048;
const int G2C2_END = 2559;

const int G2C3_START = 2560;
const int G2C3_END = 3071;

// Group 3
const int G3C1_START = 3072;
const int G3C1_END = 3583;

const int G3C2_START = 3584;
const int G3C2_END = 4095;

const int G3C3_START = 4096;
const int G3C3_END = 4607;

// Group 4
const int G4C1_START = 4608;
const int G4C1_END = 5119;

const int G4C2_START = 5120;
const int G4C2_END = 5631;

const int G4C3_START = 5632;
const int G4C3_END = 6143;

// Enough space for the 4 groups of 3 chunks
const int SEGMENT_SIZE = 6144;

#endif

