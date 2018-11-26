#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include <fcm/fcm.h>
#include <Eigen/Dense>
#include <easy_logger/easy_logger.h>
//#include <algorithm>
#include "tada_swift.h"


TADASwift::TADASwift(){
    string log_fname="swift.log";
    fopen(log_fname.c_str(),"w");
    m_logger = new EasyLogger("swift.log");
    m_fcm = new FCM(2, 0.005);
    m_enabled_features[0]=false; // mean
    m_enabled_features[1]=true; // media
    m_enabled_features[2]=true; // std
}


void TADASwift::train(string model_dir){
    /* This is to train the model from a given tsv file of features */
    string line;
    std::list<std::list<double>*> *features_list = new std::list<std::list<double>*>;
    std::list<string> *line_elements;
    std::list<double> *features_for_point;
    ifstream in_file(model_dir);
    double value;
    int clus_id=0;
    int feature_idx=0;
    if(in_file.is_open()){
            while(getline(in_file, line)){
                //cout << line <<endl;
                m_logger->log("The line: ");
                m_logger->log(line);
                line_elements = this->parse_line(line);  // get the class, property, num of objects and set of features
                // To store the names of the clusters
                m_clusters_names[clus_id++] = (*(line_elements->cbegin())) + "\t" + (*(++(line_elements->cbegin())));
                features_for_point = new std::list<double>; // features for a class/property pair
                feature_idx=0;
                // Loop for each feature
                for(auto it=++++++(line_elements->cbegin()); it != line_elements->cend(); it++, feature_idx++){
                    // If the feature is enabled
                    if(m_enabled_features[feature_idx]){
                        // check if it is a number
                        if(this->str_to_double(*it, value)){
                            // and store it
                            features_for_point->push_back(value);
                        }
                        else{
                            cout << __func__ << "ERROR: invalid value: " << (*it)<<endl;
                        }
                    }
                }
                // append the list of features
                features_list->push_back(features_for_point);
            }//while
            this->set_model_from_features(features_list);
        }//if open
    else{
        cout<< __func__ << " ERROR: unable to open the model: "<< model_dir<<endl;
    }
}

std::list<string>* TADASwift::classify(string model_dir){
    return nullptr;
}

std::list<string>* TADASwift::classify(string model_dir, int column_no){
    return nullptr;
}



std::list<string>* TADASwift::parse_line(string line){
    std::list<string> *elements = new std::list<string>;
    int start_idx = 0;
    string t;
    for(int i=0;i<line.size();i++){
        if(line[i]=='\t' || line[i]=='\n' || i==line.length()-1){
            t = line.substr(start_idx,i-start_idx);
            elements->push_back(t);
            start_idx = i+1;
        }//if
    }
    return elements;
}

bool TADASwift::str_to_double(string s, double & val){
        int i;
        string num_str="";
        bool got_dec;
        got_dec=0;
        for(i=0;i<s.length();i++){
            if(s[i]>='0' && s[i]<='9'){
                num_str += s[i];
            }
            else if(s[i]=='.' && !got_dec){
                num_str += s[i];
            }
        }
        if(num_str=="" || num_str=="."){
            return 0;
        }
        else{
            val = strtod(num_str.c_str(),NULL);
            return 1;
        }
}

void TADASwift::set_model_from_features(std::list<std::list<double>*>* features_list){
    MatrixXf* data;
    long i,j;
    data = new MatrixXf;
    m_logger->log(" num of rows: "+to_string(features_list->size()));
    m_logger->log(" num of cols: "+to_string((*(features_list->cbegin()))->size()));
    data->resize(features_list->size(), (*(features_list->cbegin()))->size());
    i=0;
    for(auto it=features_list->cbegin(); it!= features_list->cend(); it++, i++){
        j=0;
        for(auto jt=(*it)->cbegin(); jt!=(*it)->cend() ; jt++, j++){
            (*data)(i,j) = *jt;
        }
    }
    m_fcm->set_data(data);
    MatrixXf* membership;
    membership = new MatrixXf;
    membership->resize(data->rows(), data->rows());
    membership->setZero();
    i=0;
    for(auto it=features_list->cbegin(); it!= features_list->cend(); it++, i++){
        (*membership)(i,i) = 1;
    }
    stringstream s;
    s << (*membership);
    m_logger->log(s.str());
    m_fcm->set_membership(membership);
}


