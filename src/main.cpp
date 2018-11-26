#include <iostream>
#include "tada_swift.h"

using namespace std;

int main(int argc, char** argv){
    cout << "Hello TADA Swift!" <<endl;
    TADASwift tada;
    for(int i=0;i<argc;i++){
        cout<< "arg " << i << "> " << argv[i] <<endl;
    }
    if(argc>1){
        tada.train(argv[1]);
    }
    else{
        //tada.train("golfplayers.tsv");
    }
}
