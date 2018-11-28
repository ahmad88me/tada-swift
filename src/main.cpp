#include <iostream>
#include <list>
#include <iterator>
#include "features.h"
#include "tada_swift.h"

using namespace std;

int main(int argc, char** argv){
    cout << "Hello TADA Swift!" <<endl;
    TADASwift tada;
    std::list<string>* results = new std::list<string>;
    string help_string =  "Expected parameters: <model file> <csv file to annotate> column_no1 column_no2 ...";
    for(int i=0;i<argc;i++){
        cout<< "arg " << i << "> " << argv[i] <<endl;
    }
    if(argc>1){
        tada.train(argv[1]);
    }
    else{
        cout << help_string <<endl;
        return 1;
    }

    if(argc==3){
        cout << "detection of numerical columns in the input file is not supported at the moment\n";
        cout << help_string <<endl;
    }

    else if(argc>3){
        int i;
        int col_idx;
        string s(argv[2]);
        tada.parse(s);
        for(i=0;i<(argc-3);i++){
            col_idx = atoi(argv[3+i]);
            results = tada.classify(col_idx);
            cout << "results for classification of the file " << s << " column: " << col_idx << endl;
            std::list<string>::iterator it = results->begin();
            for(int k=0;k<5;k++){
                cout << "\tk: " << (*it++) << endl;
            }
            cout << "\n\n";
            delete results;
        }
    }
    else{
        cout << "No input file is provided for the annotation process\n";
    }
}
