#include <iostream>
#include "tada_swift.h"

using namespace std;

int main(int argc, char** argv){
    cout << "Hello World!" <<endl;
    TADASwift tada;
    for(int i=0;i<argc;i++){
        cout<< "arg " << i << "> " << argv[i] <<endl;
    }
    tada.train("golfplayers.tsv");
}
