#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <iostream>
#include <vector>

class log {
public:

    std::ofstream f_log;

    void f_del_data(){f_log.open("log_file.txt", std::ios_base::trunc); f_log.close();}

    void f_log_vec(const std::string& str, const std::vector<std::string>& vec){
        f_log.open("log_file.txt", std::ios_base::app);
        f_log << str;
        for(int i = 0; i < vec.size(); i++){
            f_log <<"{" << vec[i] << "}, ";
        }
        f_log << "\n";
        f_log.close();
    }

    void f_log_vec(const std::string& str, const std::vector<double>& vec){
        f_log.open("log_file.txt", std::ios_base::app);
        f_log << str;
        f_log <<"{";
        for(int i = 0; i < vec.size(); i++){
            f_log << vec[i] << "; ";
        }
        f_log << "}\n";
        f_log.close();
    }

    void f_log_text(const std::string& str){
        f_log.open("log_file.txt", std::ios_base::app);
        f_log << str;
        f_log.close();
    }

    void f_log_int(int i){
        f_log.open("log_file.txt", std::ios_base::app);
        f_log << i;
        f_log.close();
    }

};
#endif // LOG_H
