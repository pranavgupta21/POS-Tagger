#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <ctype.h>
using namespace std;

#define NO_TAGS 35									// PENN TREEBANK TAG-SET
#define FREQ_DATA_FILE "corpFreqData.freq"			// REDUNDANT AS OF NOW
#define TRANS_COUNT_FILE "trans_count"				// TRASITION COUNT HMM_DATA FILE
#define TAG_COUNT_FILE "tags_count"					// TAG COUNT HMM_DATA FILE
#define TAGS_FILE "penn_tags"						// FILE CONTAINING THE TAG-SET TO BE USED
#define EMMISSION_COUNT_FILE "corpFreqData.freq"	// EMMISSION COUNT HMM_DATA FILE
#define TAG_SEPARATOR "_"							// CHARACTER SEPARATING THE WORD FROM THE TAG IN CORPUS

void init(string tags_filename = TAGS_FILE);
void gen_HMM();
int* tag_text();
void pos_prob_max(int);
string tolow(string);
void extract_emmissions();

