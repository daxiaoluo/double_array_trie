main:main.o double_array_trie.o
	g++ -o main main.o double_array_trie.o
main.o:main.cc double_array_trie.h
	g++ -c main.cc -o main.o
double_array_trie.o:double_array_trie.h
	g++ -c double_array_trie.cc -o double_array_trie.o
clean:
	rm -f *.o main
