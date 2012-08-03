freq_gen_install: freq_gen.o freq_gen_func.o
	g++ freq_gen.o freq_gen_func.o -o freq_gen
freq_gen_func.o:
	g++ -c freq_gen_func.cpp -o freq_gen_func.o
freq_gen.o: freq_gen_func.o
	g++ -c freq_gen.cpp -o freq_gen.o
freq_gen_clean:
	rm -f freq_gen*.o
	
pos_tagger_install: pos_tagger.o pos_tagger_func.o
	g++ pos_tagger.o pos_tagger_func.o -o pos_tagger
pos_tagger_func.o:
	g++ -c pos_tagger_func.cpp -o pos_tagger_func.o
pos_tagger.o: pos_tagger_func.o
	g++ -c pos_tagger.cpp -o pos_tagger.o
pos_tagger_clean:
	rm -f pos_tagger*.o
	
txsm_install: textrank.o vertex.o
	g++ textrank.o vertex.o -o textrank
vertex.o:
	g++ -c vertex.cpp -o vertex.o
textrank.o: vertex.o
	g++ -c textrank.cpp -o textrank.o
txsm_clean:
	rm -f vertex.o textrank.o	
