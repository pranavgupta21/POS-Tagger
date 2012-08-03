#include "freq_gen_func.h"
using namespace std;

map <vector<string>, int> wordlist;
vector<string> filenames;
string target_folder;
string hmm_folder = "hmm_data_oanc/";
int transitions[NO_TAGS][NO_TAGS] = {0,0};
int tag_count[NO_TAGS] = {0};
map<string, int> tag_ids;
vector<string> tag_names;
int no_corp_words = 0;

int main(int argc, char **argv){
	string tags_filename;
	if (argc > 3)
		init((tags_filename = argv[3]));
	else
		init();
	
	string dirname = argv[1];
	if(dirname.at(dirname.length()-1) != '/'){
		dirname += '/'; 
	}
	/*
	target_folder = argv[2];
	if(target_folder.at(target_folder.length()-1) != '/'){
		target_folder += '/'; 
	}
	*/
	//cout << dirname << " | " << target_folder << "\n";
	cout << "Number of files processed\t:\t" << processFolder(dirname) << endl;
	return 0;
}

