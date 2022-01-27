/*! \file Fcm.hh
    \brief Class that allows 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

class Fcm{
    public: 
        
        Fcm(string file, int k, int alpha);
        void openfile();
        void read();
        double calculate();
        map<string, int> getA();
        map<string, int> getCtx();
        map<string, double> getH();
        string lower(string s);
        void close();

    private:
        int k_arg;
        int smoothing_param;
        string i_file;
        fstream f;
        map<string, int> contexts;
        map<string, int> alphabet;
        map<string, double> entropies;
};

Fcm::Fcm(string file, int k, int alpha){
    k_arg = k;
    i_file = file;
    smoothing_param = alpha;
}
void Fcm::openfile(){
    if(not i_file.empty()){
        f.open(i_file);
    }else{
        cout << "Could not open input file!" << endl;
    }
}
void Fcm::read(){
    char c;
    string ctx = "";
    while(f.get(c)){
        if(c == '\n' or c == '\n') continue;
        ctx += tolower(c);
        string s = lower(string(1, c));
        alphabet[s]++;
        
        if(ctx.length() == k_arg + 1){
            alphabet[ctx]++;
            contexts[ctx.substr(0, k_arg)]++;
            ctx = ctx.substr(1);
        }     
    }
}

double Fcm::calculate(){
    map<string, int>::iterator it;
    vector<string> aux;
    map<string, int> all_elems;
    for(it = alphabet.begin(); it != alphabet.end(); ++it){
        if(it->first.length() == 1){
            aux.push_back(it->first);
        }
    }
    double probability = 0.0;
    double pi = 0.0;
    int total = 0;

    for(it = contexts.begin(); it != contexts.end(); it++){
        total += it->second;
        for(string x : aux){
            pi = (double) alphabet[it->first + x] / it->second;
            if(pi == 0) pi = (double)smoothing_param / (it->second + smoothing_param * aux.size()); //-TODO : CHANGE THIS 
            probability += pi * (log(pi)/log(2));
        }
        entropies[it->first] = -1 * probability;
        probability = 0.0;
    }

    double total_entropy = 0.0;
    for(it = contexts.begin(); it != contexts.end(); it++){
        total_entropy += ((double) it->second / total) * entropies[it->first];
    }
    return total_entropy;        
}

map<string, int> Fcm::getA(){
    
    return alphabet;
}
map<string, int> Fcm::getCtx(){
    
    return contexts;
}
map<string, double> Fcm::getH(){
    
    return entropies;
}
string Fcm::lower(string s){
    
    string x;

    for(char elem : s){
        x += tolower(elem);
    }
    return x;
}
void Fcm::close(){
    /**
    * Useful method to reallocate memory
    */

    if(f.is_open()){
        f.close();
    }
}