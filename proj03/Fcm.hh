/*! \file Fcm.hh
    \brief Class that allows 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;


class Fcm{
    public: 
        
        Fcm(string file, int k);
        void openfile();
        void read();
        map<string, int> getA();
        string lower(string s);

    private:
        int k_arg;
        string i_file;
        fstream f;
        map<string, int> alphabet;
};

Fcm::Fcm(string file, int k){
    k_arg = k;
    i_file = file;
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
        ctx += tolower(c);
        string s = lower(string(1, c));
        alphabet[s]++;
 
        if(ctx.length() == k_arg){
            alphabet[ctx]++;
            ctx = ctx.substr(1);
        }
            
    }
}
map<string, int> Fcm::getA(){
    
    return alphabet;
}

string Fcm::lower(string s){
    
    transform(s.begin(), s.end(), s.begin(),[](char c){ 
        return tolower(c);});
    return s;
}


