#!/bin/python

import os
import sys

filename = ""
wordlist = {};

def freq_gen():
	filecorp = open (filename, "r" );
	filetext = filecorp.readlines();
	for line in filetext:
		linelist = line.split(" ");
		for word in linelist:
			#print "WORDWORDWORDWORDWORDWORDWORDWORDWORDWORD", word
			wordtup = ()
			wordtup = tuple(word.split("/"))
			if not wordlist.has_key(wordtup):
				wordlist[wordtup] = 1
			else:
				wordlist[wordtup] = wordlist[wordtup] + 1
	filecorp.close();	
		
def freq_write():
	filefreq = open (filename + "freq", "w")
	for wordtup in wordlist:
		print wordtup, " : ", wordlist[wordtup]
	wordlist.clear()


if __name__ == "__main__":
	filename = raw_input("Please enter the filename : ");
	freq_gen();
	freq_write();
	exit(0);
