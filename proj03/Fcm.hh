/*! \file Fcm.hh 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class Fcm{
    public: 
        
        Fcm(string file, int k, double alpha);
        void openfile();
        void read();
        double calculate();
        vector<char> getAlphabet();
        map<string, int> getA();
        map<string, int> getCtx();
        map<string, double> getH();
        int getTotal();
        string lower(string s);
        void close();

    private:
        int k_arg;
        double smoothing_param;
        string i_file;
        ofstream o;
        fstream f;
        map<string, int> contexts;
        map<string, int> alphabet;
        map<string, double> entropies;
        vector<char> aux;
        int total;
};

Fcm::Fcm(string file, int k, double alpha){
    /**
    * Class constructor, accepts a file name, order of the model k, and smoothing operator alpha
    */
    k_arg = k;
    i_file = file;
    smoothing_param = alpha;
    total=0;
}
void Fcm::openfile(){
    /**
    * Allows the opening of the input/output streams
    */
    o.open("context_table.out");
    if(not i_file.empty()){
        f.open(i_file);
    }else{
        cout << "Could not open input file!" << endl;
    }
}
void Fcm::read(){
    /**
    * Reading function, stores the alphabet and contexts with length k and k+1; also, increments its values everytime they appear
    */
    char c;
    string ctx = "";
    while(f.get(c)){
        if(c == '\n' or c == '\n') continue;
        ctx += tolower(c);
        if (!count(aux.begin(), aux.end(), (char) tolower(c))) aux.push_back((char)tolower(c));
        string s = lower(string(1, c));
        alphabet[s]++;
        if(ctx.length() == k_arg + 1){
            alphabet[ctx]++;
            contexts[ctx.substr(0, k_arg)]++;
            ctx = ctx.substr(1);
            total++;
        }     
    }
    
}

double Fcm::calculate(){
    /**
    * Calculates the total entropy. Iterates through all contexts, adding each character of the alphabet for each one, and using each value for entropy calculations purposes 
    */
    map<string, int>::iterator it;
    double probability = 0.0;
    double pi = 0.0;
    double total_entropy = 0.0;

    for(it = contexts.begin(); it != contexts.end(); it++){
        o << "CTX: "<< it->first << "\t" << "TOTAL: "<< it->second << "\t";
        for(char x : aux){
            pi = (double) (smoothing_param + alphabet[it->first + x]) / (it->second + smoothing_param * aux.size());
            entropies[it->first] += -pi * log2(pi);
             o << "'" << x << "': " << alphabet[it->first + x] << "\t";   
        }
        o << "\n";
        total_entropy += ((double) it->second * entropies[it->first]) / total;
    }
    o.close();
    return total_entropy;        
}

vector <char> Fcm::getAlphabet(){
    /**
    * getter
    */
    return aux;
}

int Fcm::getTotal(){
    /**
    * getter
    */
    return total;
}

map<string, int> Fcm::getA(){
    /**
    * k+1 contexts and alphabet getter
    */
    
    return alphabet;
}
map<string, int> Fcm::getCtx(){
    /**
    * k contexts getter
    */
    
    return contexts;
}
map<string, double> Fcm::getH(){
    /**
    * entropy getter
    */
    return entropies;
}
string Fcm::lower(string s){
    /**
    * method that allows a given string to be returned with lower case letters only
    */
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