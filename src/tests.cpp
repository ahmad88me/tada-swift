#include <string>
#include <list>
#include "tada_swift.h"
#include <gtest/gtest.h>

namespace {
    TEST(SwiftTests, TestConfig){
        EXPECT_EQ(1,1);
    }

    TEST(SwiftTests, Golf){
        TADASwift tada;
        std::list<string>* results = new std::list<string>;
        tada.train("golfplayers.tsv");
        tada.parse("aaagolfplayers.csv");
        results = tada.classify(10);
        //cout << "\nfront: "<< results->front() <<endl;
        EXPECT_EQ(results->front(),"http://dbpedia.org/ontology/GolfPlayer\thttp://dbpedia.org/ontology/Person/height");
        delete results;
        results = tada.classify(11);
        //cout << "\nfront: "<< results->front()<< endl;
        EXPECT_EQ(results->front(),"http://dbpedia.org/ontology/GolfPlayer\thttp://dbpedia.org/ontology/Person/weight");
        delete results;
     }

}// namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
