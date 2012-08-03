#include "freq_gen_func.h"
using namespace std;

// map to store the word-frequency list //
extern map<vector<string>, int> wordlist;
map<vector<string>, int>::iterator wordlist_it;
extern vector<string> filenames;
extern string target_folder;
extern string hmm_folder;
extern int transitions[NO_TAGS][NO_TAGS];
extern int tag_count[NO_TAGS];
extern map<string, int> tag_ids;
extern vector<string> tag_names;
extern int no_corp_words;

/* initializes the tag set 'tag_names' to the PENN TREEBANK TAG-SET
creates a map from the tag_names to tag_ids which are subsequently used for most practical purposes */ 
void init(string tags_filename){
	fstream tags_file;
	string tag_name;
	int tag_no = 0;
	tags_file.open(tags_filename.c_str(), ios::in);
	while (tags_file >> tag_name){
		//cout << tag_name << "\n";
		tag_ids[tag_name] = tag_no++;
		tag_names.push_back(tag_name);
	}
	tags_file.close();
}

/* reads word by word from a corpus file and stores the words' emmission count (in wordlist), tag count(in tag_count) and transition count (in trans_count) */
void freqGen(string file_corp_name){
	// Read file and generate word-frequency list
	fstream filecorp;
	filecorp.open(file_corp_name.c_str(), ios::in);
	string word;
	vector<string> wordtup;									// key for the map
	int pos, no_words = 0, ch = 'a';
	string tag_name = "";
	while (filecorp >> word){
		//cout << "Word Number " << no_words << "\n";
		//cout << word;
		pos = word.find(TAG_SEPARATOR);
		
		//word
		wordtup.push_back(word.substr(0, pos));				// may have to check the 
		//POS Tag
		wordtup.push_back(word.substr(pos+1));				// index given to substr
		
		if(!exist_tag(wordtup[1])){
			wordtup.clear();
			tag_name = "";
			continue;
		}
		
		no_corp_words++;
		no_words++;
		
		//cout << "freqGen() : breaker 1 \n";
		// inserting into tag_count list 
		tag_count[tag_ids[wordtup[1]]]++;
		
		//cout << "freqGen() : breaker 2 \n";
		// inserting into transitions list
		if(tag_name != "")
			transitions[tag_ids[tag_name]][tag_ids[wordtup[1]]]++;
		
		//cout << "freqGen() : breaker 3 \n";
		// if the word DOESN'T exist in the map - VERY IMPORTANT PART
		if((wordlist_it = wordlist.find(wordtup)) == wordlist.end()){
			wordlist[wordtup] = 1;
		}
		// if the word exists in the map
		else{
			wordlist[wordtup] = wordlist_it->second + 1;	
		}

		//cout << "freqGen() : breaker 4 \n";
		
		// refresh the running tag_name when entering a new line //
		ch = filecorp.get();
		if(ch == '\n')
			tag_name = "";
		else
			tag_name = wordtup[1];
		
		//cout << "freqGen() : breaker 5 \n";
		wordtup.clear();									// refresh the key for next word
	}
	//cout << "Words : " << no_words << "\t" << "Distinct Words : " << wordlist.size() << "\n";
	filecorp.close();
}

/* prints the word-frequency (emmission count) list stored in the map wordlist to HMM Data file corpFreqData.freq */
void freqWrite(string file_freq_name){
	fstream filefreq;
	filefreq.open(file_freq_name.c_str(), ios::out);
	
	// write the word-frequency list into a file
	for(wordlist_it = wordlist.begin(); wordlist_it != wordlist.end(); wordlist_it++){
		filefreq << wordlist_it->first[0] << " " << wordlist_it->first[1] << " " << wordlist_it->second << "\n";
	}
	filefreq.close();
}

/* takes the absolute path of the corpus's root folder, extracts the list of the absolute paths of all the text files
in that folder by recursively traversing it, calls the function freqGen() for all the files and stores the result in the HMM Data files tags_count, trans_count and calls freqWrite() for writing emmission count into corpFreqData.freq  */
int processFolder(string dirname){
	int no_files = 0, last_dir_sep;
	listFiles(dirname, true);
	vector<string>::iterator file_list_it;
	for(file_list_it = filenames.begin(); file_list_it != filenames.end(); file_list_it++){
		cout << *file_list_it << " ... \n";
		freqGen(*file_list_it);
		//freqWrite(target_folder + (*file_list_it).substr((*file_list_it).rfind("/") + 1) + ".freq");
		//wordlist.clear();
	}
	freqWrite(hmm_folder + EMMISSION_COUNT_FILE);
	cout << "processFolder() : Frequency Data stored \n";
	
	fstream trans_count_file, tag_count_file;
	int pretag = 0, posttag = 0, tag_no = 0;

	tag_count_file.open((hmm_folder + TAG_COUNT_FILE).c_str(), ios::out);
	cout << "processFolder() : Tag count file opened \n";
	tag_count_file << no_corp_words << "\n";
	for(; tag_no < NO_TAGS; tag_no++){
		tag_count_file << tag_names.at(tag_no) << " " << tag_count[tag_no] << "\n";
	}
	tag_count_file.close();
	cout << "processFolder() : Tag count stored \n";
	
	trans_count_file.open ((hmm_folder + TRANS_COUNT_FILE).c_str(), ios::out);
	cout << "processFolder() : Transition count file opened \n";
	for(; pretag < NO_TAGS; pretag++){
		posttag = 0;
		for(; posttag < NO_TAGS; posttag++){
			trans_count_file << tag_names[pretag] << " " << tag_names[posttag] << " " << transitions[pretag][posttag] << "\n";
		}
	}
	trans_count_file.close();
	cout << "processFolder() : Transition count stored \n";

	return filenames.size();
}

/* recursively traverses a folder and generates a list of absolute paths of all the files in it */
void listFiles(string baseDir, bool recursive){
	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(baseDir.c_str())) == NULL){
		cout << "Couldn't open " << baseDir << "." << endl;
		return;
	}
	else{
		while((dirp = readdir(dp)) != NULL){
			if (dirp->d_name != string(".") && dirp->d_name != string("..")) {
				if (isDir(baseDir + dirp->d_name) == true && recursive == true) {
					//cout << "[DIR]\t" << baseDir << dirp->d_name << "/" << endl;
					listFiles(baseDir + dirp->d_name + "/", true);
				}
				else{
					filenames.push_back(baseDir + dirp->d_name);
					//cout << "[FILE]\t" << baseDir << dirp->d_name << endl;
				}
			}
		}
		closedir(dp);
	}
}

/* checks if a file is directory file */
bool isDir(string dir){
    struct stat fileInfo;
    stat(dir.c_str(), &fileInfo);
    if (S_ISDIR(fileInfo.st_mode)) {
        return true;
    } else {
        return false;
    }
}

bool exist_tag(string tag_name){
	for(vector<string>::iterator tag_it = tag_names.begin(); tag_it != tag_names.end(); tag_it++){
		if (tag_name == *tag_it)
			return true;
	}
	return false;
}

/* convert all letters of a string to lower case */
string tolow(string word){
	for (int loopVar = 0; loopVar < word.length(); loopVar++){
		if(isupper(word.at(loopVar))){
			word[loopVar] = tolower(word.at(loopVar));
		}
	}
	return word;
}

