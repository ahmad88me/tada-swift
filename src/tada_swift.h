#ifndef TADASWIFT_H
#define TADASWIFT_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <fcm/fcm.h>
#include <easy_logger/easy_logger.h>


using namespace std;


class TADASwift{
    public:
        TADASwift();
        void train(string);
        std::list<string>* classify(string);
        std::list<string>* classify(string, int);
    private:
        //string m_kg_dir; // the directory of the TSV the has the features computed for a knowledge graph
        FCM * m_fcm;
//        std::list<string>
        std::map<int,string> m_clusters_names;
        std::list<string>* parse_line(string);
        bool m_enabled_features[3];
        EasyLogger *m_logger;
        bool str_to_double(string, double&);
        void set_model_from_features(std::list<std::list<double>*>*);
};

#endif
