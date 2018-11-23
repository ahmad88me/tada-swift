#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <fcm/fcm.h>
#include <Eigen/Dense>
#include <easy_logger/easy_logger.h>
//#include <algorithm>
#include "tada_swift.h"


TADASwift::TADASwift(){
    m_logger = new EasyLogger("swift.log");
    m_fcm = new FCM(2, 0.005);
    m_enabled_features[0]=false; // mean
    m_enabled_features[1]=true; // media
    m_enabled_features[2]=true; // std
}


void TADASwift::train(string model_dir){
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
                line_elements = this->parse_line(line);
                m_clusters_names[clus_id++] = (*(line_elements->cbegin())) + "\t" + (*(++(line_elements->cbegin())));
                features_for_point = new std::list<double>;
                feature_idx=0;
                for(auto it=++++++(line_elements->cbegin()); it != line_elements->cend(); it++, feature_idx++){
                    if(m_enabled_features[feature_idx]){
                        if(this->str_to_double(*it, value)){
                            features_for_point->push_back(value);
                        }
                        else{
                            cout << __func__ << "ERROR: invalid value: " << (*it)<<endl;
                        }
                    }
                }
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
    //cout << "num of row: " << features_list->size()<< endl;
    //cout << "nu of cols: " << (*(features_list->cbegin()))->size() <<endl;
    data->resize(features_list->size(), (*(features_list->cbegin()))->size());
    i=0;
    //cout << "resized: "<<endl;
    for(auto it=features_list->cbegin(); it!= features_list->cend(); it++, i++){
        //cout << "outer " <<endl;
        j=0;
        for(auto jt=(*it)->cbegin(); jt!=(*it)->cend() ; jt++, j++){
            //cout << "inner ";
            //cout << *jt << " " ;
            //cout << i << ", " << j<<endl;
            (*data)(i,j) = *jt;
        }
        //cout << endl;
    }
    cout << "data: "<<endl;
    cout << (*data);
    cout<<endl;
    m_fcm->set_data(data);
    MatrixXf* membership;
    membership = new MatrixXf;
    membership->resize(data->rows(), data->rows());
    membership->setZero();
    i=0;
    for(auto it=features_list->cbegin(); it!= features_list->cend(); it++, i++){
        (*membership)(i,i) = 1;
    }
    cout << "membership: "<<endl;
    cout << (*membership);
    cout<<endl;
    m_fcm->set_membership(membership);
}


