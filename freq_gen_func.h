#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;

#define NO_TAGS 35									// PENN TREEBANK TAG-SET
#define TRANS_COUNT_FILE "trans_count"				// TRASITION COUNT HMM_DATA FILE
#define TAG_COUNT_FILE "tags_count"					// TAG COUNT HMM_DATA FILE
#define TAGS_FILE "penn_tags"						// FILE CONTAINING THE TAG-SET TO BE USED
#define EMMISSION_COUNT_FILE "corpFreqData.freq"	// EMMISSION COUNT HMM_DATA FILE
#define TAG_SEPARATOR "_"							// CHARACTER SEPARATING THE WORD FROM THE TAG IN CORPUS

void init(string tags_filename = TAGS_FILE);
void freqGen(string);
void freqWrite(string);
int processFolder(string);
void listFiles(string, bool);
bool isDir(string);
bool exist_tag(string);
string tolow(string);
