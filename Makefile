







output: SearchNewBooks.o
	g++ SearchNewBooks.o -o SearchNewBooks

SearchNewBooks.o: SearchNewBooks.cpp
	g++ -c SearchNewBooks.cpp

clean:
	rm *.o SearchNewBooks
