#ifndef TADASWIFT_H
#define TADASWIFT_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <fcm/fcm.h>
#include <tabular_parser/parser.h>
#include <easy_logger/easy_logger.h>


using namespace std;


class TADASwift{
    public:
        struct clus_memb_pair
        {
            long clus;
            double memb;
            friend inline bool operator < (const struct clus_memb_pair& first, const struct clus_memb_pair& second){
                //return first.memb < second.memb;
                return first.memb > second.memb; // because we want it in a decending order
            }

        };
        TADASwift();
        void train(string);
        //std::list<string>* classify(string);
        std::list<string>* classify(int);
        void parse(string);
    private:
        //string m_kg_dir; // the directory of the TSV the has the features computed for a knowledge graph
        FCM * m_fcm;
        std::list<std::list<string>*>* m_parsed_data; // tabular data that is parsed
//        std::list<string>
        std::map<int,string> m_clusters_names;
        std::list<string>* parse_line(string);
        std::list<double>* get_numbers_from_list(std::list<string>*);
        bool m_enabled_features[3];
        EasyLogger *m_logger;
        bool str_to_double(string, double&);
        void set_model_from_features(std::list<std::list<double>*>*);
        std::list<string>* sorted_clus_membership();
        void print_data();
};

#endif
