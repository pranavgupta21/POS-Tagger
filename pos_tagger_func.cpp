#include "pos_tagger_func.h"
using namespace std;

extern string hmm_folder;
extern map<vector<string>, int> wordlist;
map<vector<string>, int>::iterator wordlist_it;
extern map<string, int> tag_ids;
extern vector<string> tag_names;
extern int transitions[NO_TAGS][NO_TAGS];
extern int tag_count[NO_TAGS];
extern int no_corp_words;
extern int **emmissions;
extern vector<vector<string> > text_words;
extern vector<vector<string> >::iterator text_it;
float **pos_prob;
int *mle_tags;

/* initializes the tag set 'tag_names' to the PENN TREEBANK TAG-SET
creates a map from the tag_names to tag_ids which are subsequently used for most practical purposes */ 
void init(string tags_filename){
	fstream tags_file;
	string tag_name;
	int tag_no = 0;
	tags_file.open(tags_filename.c_str(), ios::in);
	while (tags_file >> tag_name){
		tag_ids[tag_name] = tag_no++;
		tag_names.push_back(tag_name);
	}
	tags_file.close();
}

/* reads the emmission, tag and transition counts from the HMM DATA FILES into appropriate lists and extract emmission count of all the words appearing in our given text, then clear the wordlist map to save on memory */
void gen_HMM(){
	int total_tags = 0, emmission_count = 0;
	string tag_name;
	string word;
	int tag_no = 0;
	int pretag = 0, posttag = 0;
	vector<string> wordtup;
	//wordtup.reserve(2);
	
	// Read Tag Count //
	fstream tag_count_file;
	tag_count_file.open((hmm_folder + TAG_COUNT_FILE).c_str(), ios::in);
	cout << "gen_HMM() : Tag count file opened \n";
	tag_count_file >> no_corp_words;
	tag_no = 0;
	while (tag_count_file >> tag_name){
		tag_count_file >> tag_count[tag_no++];
	}
	tag_count_file.close();
	cout << "gen_HMM() : Tag count read \n";
	
	// Read Transition Count //
	fstream trans_count_file;
	trans_count_file.open ((hmm_folder + TRANS_COUNT_FILE).c_str(), ios::in);
	cout << "gen_HMM() : Transition count file opened \n";
	for(pretag = 0; pretag < NO_TAGS; pretag++){
		for(posttag = 0; posttag < NO_TAGS; posttag++){
			trans_count_file >> tag_name;
			trans_count_file >> tag_name;
			trans_count_file >> transitions[pretag][posttag];
		}
	}
	trans_count_file.close();
	cout << "gen_HMM() : Transition count read \n";
	
	// Read Emmission Count //
	fstream filefreq;
	filefreq.open((hmm_folder + EMMISSION_COUNT_FILE).c_str(), ios::in);
	cout << "gen_HMM() : Emmision count file opened \n";
	while (filefreq >> word){
		wordtup.push_back(tolow(word));
		filefreq >> tag_name;
		wordtup.push_back(tag_name);
		filefreq >> emmission_count;
		wordlist[wordtup] = emmission_count;
		wordtup.clear();
	}
	filefreq.close();
	cout << "gen_HMM() : Emmission count read \n";
}

/* finds the most likely POS Tags for each word using the VITERBI ALGORITHM using the pos_prob_max() function */
int* tag_text(){
	cout << "tag_text() : Begin\n";
	pos_prob = new float*[text_it->size()];
	for (int loopVar = 0; loopVar < text_it->size(); loopVar++)
		pos_prob[loopVar] = new float[NO_TAGS];
	mle_tags = new int[text_it->size()];
	cout << "tag_text() : Started estimating most likely POS Tags\n";
	pos_prob_max(text_it->size()-1);
	cout << "tag_text() : End\n";
	return mle_tags;
}

// implements the recursive and dynamic VITERBI ALGORITHM and stores the most likely POS Tag id's in the array mle_tags of size (no of words in given text)
void pos_prob_max(int word_no){
	int tag_no = 0;
	float max_state_prob = 0, state_prob, mle_tag_prob = 0;
	int state_no = 0, state_of_max_prob;
	int mle_tag = 0;
	
	if(!word_no){
		cout << "pos_prob() : word number 0 : " << text_it->at(word_no) << "\n";
		for (; tag_no < NO_TAGS; tag_no++){
			pos_prob[word_no][tag_no] = ((float)emmissions[word_no][tag_no]/no_corp_words);
			cout << tag_no << " : " << pos_prob[word_no][tag_no] << "\n";
			if(pos_prob[word_no][tag_no] > mle_tag_prob){
				mle_tag_prob = pos_prob[word_no][tag_no];
				mle_tag = tag_no;
			}
		}
		mle_tags[word_no] = mle_tag;
		cout << mle_tag << " : " << mle_tag_prob << "\n";
		return;
	}
	cout << "pos_prob() : word number " << word_no << " : " << text_it->at(word_no) << "\n";
	pos_prob_max(word_no - 1);
	cout << "pos_prob() : evaluating word number " << word_no << " : " << text_it->at(word_no) << "\n";
	for (tag_no = 0; tag_no < NO_TAGS; tag_no++){
		max_state_prob = 0; // probability of the state with max probability : float
		state_of_max_prob = 0; // state no of state with max probability: int
		for (state_no = 0; state_no < NO_TAGS; state_no++){
			if ((state_prob = ((float)transitions[state_no][tag_no]/tag_count[state_no])*pos_prob[word_no-1][state_no]) > max_state_prob){
				max_state_prob = state_prob; //state prob - probability of this state : float
			}
		}
		pos_prob[word_no][tag_no] = (((float)emmissions[word_no][tag_no]/tag_count[tag_no]) * max_state_prob);
		/*
		if(!pos_prob[word_no][tag_no])
			pos_prob[word_no][tag_no] = 0.0000000000001;
		*/
		cout << tag_no << " : " << pos_prob[word_no][tag_no] << " | " << max_state_prob << " | " << emmissions[word_no][tag_no] << "\n";
		if(pos_prob[word_no][tag_no] > mle_tag_prob){
			mle_tag_prob = pos_prob[word_no][tag_no];     // mle_tag_prob - prob of state with max prob : float
			mle_tag = tag_no; // mle_tag - tag no/state no of state with max mle probability : int
		}
	}
	mle_tags[word_no] = mle_tag;
	cout << mle_tag << " : " << mle_tag_prob << "\n";
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

void extract_emmissions(){
	int tag_no = 0;
	vector<string> wordtup;
	cout << "extract_emmissions() : Starting emmission extraction \n";
	int word_no	= 0, no_words = text_it->size();
	for (; word_no < no_words; word_no++){
		wordtup.push_back(text_it->at(word_no));
		for (tag_no = 0; tag_no < NO_TAGS; tag_no++){
			wordtup.push_back(tag_names.at(tag_no));
			if((wordlist_it = wordlist.find(wordtup)) != wordlist.end()){
				emmissions[word_no][tag_no] = wordlist_it->second;
			}
			else{
				cout << "Emmission not found \n";
				emmissions[word_no][tag_no] = 0;
			}
			cout << text_it->at(word_no) << " " << tag_no << " : " << emmissions[word_no][tag_no] << "\n";
			wordtup.pop_back();
		}
		wordtup.clear();
	}
}
