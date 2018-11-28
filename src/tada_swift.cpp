#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include <fcm/fcm.h>
#include "features.h"
#include <Eigen/Dense>
#include <easy_logger/easy_logger.h>
//#include <algorithm>
#include <tabular_parser/parser.h>
#include "tada_swift.h"

using namespace std;

TADASwift::TADASwift(){
    m_parsed_data = nullptr;
    string log_fname="swift.log";
    fopen(log_fname.c_str(),"w");
    m_logger = new EasyLogger("swift.log");
    m_fcm = new FCM(2, 0.005);
    m_enabled_features[0]=false; // mean
    m_enabled_features[1]=true; // median
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
            m_logger->log("getting all clusters names");
            for(auto it = m_clusters_names.begin(); it != m_clusters_names.end(); it++) {
                m_logger->log(to_string(it->first) + "\t=>\t" + (it->second));
                //cout << (it->first) << "\t=>\t"<< (it->second)<<endl;
            }
            return this->set_model_from_features(features_list);
        }//if open
    else{
        cout<< __func__ << " ERROR: unable to open the model: "<< model_dir<<endl;
    }
}


std::list<string>* TADASwift::classify(int column_no){
    double value;
    std::list<double> *l;
    std::list<double> features_for_col;
    int idx=0;

    for(auto it=m_parsed_data->cbegin(); it!=m_parsed_data->cend();it++){
        if(idx==column_no){
            // get the list as numeric list
            l = this->get_numbers_from_list(*it);
            if(l!=nullptr){
                m_logger->log("size: "+to_string(l->size()));

                if(m_enabled_features[0]){
                    m_logger->log("will compute the mean");
                    features_for_col.push_back(Features::mean(l));
                }
                if(m_enabled_features[1]){
                    m_logger->log("will compute the median");
                    features_for_col.push_back(Features::median(l));
                }
                if(m_enabled_features[2]){
                    m_logger->log("will compute the std");
                    if(m_enabled_features[0]){// if mean is computed, don't compute it again
                        features_for_col.push_back(Features::stdev(l,*(features_for_col.cbegin())));
                    }
                    else{// if mean is not computed, then do compute it
                        features_for_col.push_back(Features::stdev(l,Features::mean(l)));
                    }
                }
                m_logger->log("creating the data matrix");
                MatrixXf *data = new MatrixXf;
                data->resize(1, features_for_col.size());
                m_logger->log("row: "+to_string(data->rows()));
                m_logger->log("row: "+to_string(data->cols()));
//                cout << "rows: "<<data->rows()<<endl;
//                cout << "cols: "<<data->cols()<<endl;
                int i=0;
                m_logger->log("filling the features to the matrix");
                for(auto it=features_for_col.cbegin();it!=features_for_col.cend();it++){
                    (*data)(0,i++) = *it;
                    //cout << "fill value: " <<*it<<endl;
                }
                m_logger->log("size of features: "+to_string(features_for_col.size()));
//                for(i=0;i<features_for_col.size();i++){
//                    cout << "stored data: " << (*data)(0,i) << endl;
//                }

                //Just to print the centers before fcm change the data
                stringstream s_centers;
                s_centers << (* (m_fcm->get_cluster_center()));
                m_logger->log("cluster centers from the training: ");
                m_logger->log("\n"+s_centers.str());



                m_logger->log("setting the data to the fcm model");
                m_fcm->set_data(data);


                stringstream sss;
                m_logger->log("getting the data");
                m_logger->log("updating the fcm membership");
                m_fcm->update_membership();
                m_logger->log("creating a stream");
                m_logger->log("feeding the stream with membership");
                sss << (* (m_fcm->get_membership()));
                m_logger->log("stream to string");
                m_logger->log("\n"+sss.str());
                m_logger->log("mem rows: "+to_string(m_fcm->get_membership()->rows()));
                m_logger->log("mem cols: "+to_string(m_fcm->get_membership()->cols()));


                //Just to print the centers after fcm change the data
                stringstream s_centers_after;
                s_centers_after << (* (m_fcm->get_cluster_center()));
                m_logger->log("cluster centers after the classification: ");
                m_logger->log("\n"+s_centers_after.str());
                delete l;
                return this->sorted_clus_membership();
                break;
            }
        }
        idx++;
    }
    return nullptr;
}

void TADASwift::parse(string model_dir){
    if(m_parsed_data!=nullptr){
        delete m_parsed_data;
    }
    m_logger->log("will parse the file: "+model_dir);
    Parser p(model_dir);
    m_parsed_data = p.parse();
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
    m_logger->log("\n"+s.str());
    m_fcm->set_membership(membership);
    m_fcm->compute_centers();
}

std::list<double> * TADASwift::get_numbers_from_list(std::list<string>* str_list){
    double v;
    std::list<double> *l = new std::list<double>;
    long max_of_nonnum;
    max_of_nonnum = str_list->size()/2;
    m_logger->log("the size of the list: "+to_string(str_list->size()));
    for(auto it=str_list->cbegin();it!=str_list->cend();it++){
        m_logger->log("values in a column: "+(*it));
        if(this->str_to_double(*it, v)){
            l->push_back(v);
        }
        else{
            if(max_of_nonnum==0){
                delete l;
                return nullptr;
            }
            max_of_nonnum--;
        }
    }
    return l;
}

void TADASwift::print_data(){
    if(m_parsed_data!=nullptr){
        for(auto it=m_parsed_data->cbegin();it!=m_parsed_data->cend();it++){
            for(auto jt=(*it)->cbegin();jt!=(*it)->cend();jt++){
                cout << (*jt) << " ";
            }
            cout << endl<<"----------"<<endl;
        }
    }
    else{
        cout << "No parsed data\n";
    }
}

std::list<string>* TADASwift::sorted_clus_membership(){
    /*
     * We assume the membership is for a single column
    */
    std::list<struct clus_memb_pair> clus_membs;
    std::list<string>* results = new std::list<string>;
    MatrixXf *membership = m_fcm->get_membership();
    string s;
    m_logger->log("pre sorted");
    for(int i=0;i<membership->cols();i++){
        clus_membs.push_back({i, (*membership)(0,i)});
        m_logger->log("adding: memb for cluster: "+to_string(i)+" with value: "+to_string(clus_membs.back().memb));
    }
    clus_membs.sort();
    for(auto it=clus_membs.cbegin();it!=clus_membs.cend();it++){
        s = m_clusters_names[(*it).clus];
        m_logger->log("sorted: cluster: "+to_string((*it).clus)+" with value: "+to_string((*it).memb)+" cluster name: "+s);
        results->push_back(s);
    }
    return results;
}

