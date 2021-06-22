//
//  main.cpp
//  SearchNewBooks
//
//  Created by Bohan Liu on 10/5/19.
//  Copyright © 2019 Liu Bohan. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

// 'chrono' Class //
class Time{
private:
    chrono::high_resolution_clock::time_point start, end;
public:
    void Reset(){
        start = chrono::high_resolution_clock::now();
    }
    void Current(){
        auto end = chrono::high_resolution_clock::now();
        double elapsed_us = chrono::duration<double, micro>(end - start).count();
        cout << "CPU time: " << elapsed_us << " microseconds" << endl;
    }
};

// Basic class 'Book' //
class Book{
  public:
    string ISBN;
    string language;
    string type;
    // Overload '<' operator //
    friend bool operator < (const Book &book1, const Book &book2);
};

// Defination of overload '<' operator //
bool operator < (const Book &book1, const Book &book2){
    if(book1.ISBN == book2.ISBN){
        if(book1.language[0] == book2.language[0]){
            return(book1.type[0] < book2.type[0]);
        }
        return(book1.language[0] < book2.language[0]);
    }
    return (book1.ISBN < book2.ISBN);
}

// Clipboard of all functions //
vector<Book> ReadFromStream_SaveToVector(ifstream &Stream);
char GetChoiceInput();
int linearSearch(vector<Book> newBooks_vector, vector<Book> reqBooks_vector);
int binarySearch(vector<Book> newBooks_vector, vector<Book> reqBooks_vector);

// MAIN() FUNCTION //
int main(int argc, const char * argv[]) {
    // Error: number of input files doesn't match. //
    if(argc != 3){
        cerr << "Usage: .SearchNewBooks <newBooks.dat> <request.dat> <result_file.dat>" << endl;
        return 1;
    }
    
    // Open the input file from command line. //
    ifstream newBooks_stream(argv[1]);
    ifstream reqBooks_stream(argv[2]);
    
    // Error: Invalid input file name. //
    if(!newBooks_stream){
        cerr << "Error: cannot open file " << argv[1] << endl;
        return 1;
    }
    if(!reqBooks_stream){
        cerr << "Error: cannot open file " << argv[2] << endl;
        return 1;
    }

    // Read files, and respectively store the content to each vector. //
    // Using getline(), divided by comma ','. //
    vector<Book> newBooks_vector = ReadFromStream_SaveToVector(newBooks_stream);
    vector<Book> reqBooks_vector = ReadFromStream_SaveToVector(reqBooks_stream);
    
    // Make a choice between [l]inear_search and [b]inary_search. //
    // If the input doesn't match l and b, throw an error. //
    char choice = GetChoiceInput();
    
    // algorithm in different choices. //
    // get ready the chronograph. //
    Time ct;
    ct.Reset(); // reset chronograph. //
    
    if(choice == 'l'){
        int count = linearSearch(newBooks_vector, reqBooks_vector);
        cout << "The Linear Search finds: " << count << " matched book(s)" << endl;
    }
    
    else if(choice == 'b'){
        sort(newBooks_vector.begin(), newBooks_vector.end());
        int count = binarySearch(newBooks_vector, reqBooks_vector);
        cout << "The BINARY Search finds: " << count << " matched book(s)" << endl;
    }
    
    ct.Current(); // Stop the chronograph, and there is an output in class. //
    return 0;
}

// Defination of functions. //
vector<Book> ReadFromStream_SaveToVector(ifstream &Stream){
    vector<Book> Vector;
    int index = 0;
    while(!Stream.eof()){
        Book empty;
        Vector.push_back(empty);
        getline (Stream, Vector[index].ISBN, ',');
        getline (Stream, Vector[index].language, ',');
        getline (Stream, Vector[index].type);
        index++;
    }
    Vector.pop_back();
    return Vector;
}
// Defination of functions. //
char GetChoiceInput(){
    char choice;
    cout << "Choice of search method ([l]inear, [b]inary)?" << endl;
    cin >> choice;
    while(choice != 'l' && choice != 'b'){
        cerr << "Incorrect choice" << endl;
        cin >> choice;
        continue;
    }
    return choice;
}

// Defination of functions. //
int linearSearch(vector<Book> newBooks_vector, vector<Book> reqBooks_vector){
    int count = 0;
    for(int i = 0; i < reqBooks_vector.size(); i++){
        for(int j = 0; j < newBooks_vector.size(); j++){
            if(reqBooks_vector[i].ISBN == newBooks_vector[j].ISBN){
                if(reqBooks_vector[i].language == newBooks_vector[j].language){
                    if(reqBooks_vector[i].type == newBooks_vector[j].type){
                        count += 1;
                    }
                }
            }
        }
    }
    return count;
}
// Defination of functions. //
int binarySearch(vector<Book> newBooks_vector, vector<Book> reqBooks_vector){
    int count = 0;
    for(int i = 0; i < reqBooks_vector.size(); i++){
        
        int low = 0;
        int high = (int)newBooks_vector.size() - 1;
        
        while(low <= high){
            int mid = (low + high) / 2;
            if(reqBooks_vector[i].ISBN == newBooks_vector[mid].ISBN){
                if(reqBooks_vector[i].language == newBooks_vector[mid].language){
                    if(reqBooks_vector[i].type == newBooks_vector[mid].type){
                        count += 1;
                        break;
                    }
                    else if(reqBooks_vector[i].type > newBooks_vector[mid].type){
                        low = mid + 1;
                    }
                    else{
                        high = mid - 1;
                    }
                }
                else if(reqBooks_vector[i].language > newBooks_vector[mid].language){
                    low = mid + 1;
                }
                else{
                    high = mid - 1;
                }
            }
            else if(reqBooks_vector[i].ISBN > newBooks_vector[mid].ISBN){
                low = mid + 1;
            }
            else{
                high = mid - 1;
            }
        }
    }
    return count;
}
