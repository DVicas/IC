#include <iostream>
#include <fstream>
#include <map>
#include "Fcm.hh"
using namespace std;

double calculate(map<string, int> alphabet, double smoothing_param, map<string, int> contexts){
    map<string, int>::iterator it;
    map<string, double> entropies;
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

string lower(string s){
    
    string x;

    for(char elem : s){
        x += tolower(elem);
    }
    return x;
}

int main(int argc, char* argv[]){

    int n_bits=0;

    Fcm fcm("message.txt", 3, 0.1);
    fcm.openfile();
    fcm.read();
    
    map<string, int>::iterator it;
    map<string, int> alph = fcm.getCtx();
    
    fcm.close();

    for(it = alph.begin(); it != alph.end(); ++it){
        if(it->first.length() != 1){
            // cout << it->first << "\t" << it->second << endl;
            alph.find(it->first)->second=0;
        }   
    }

    fstream target("message.txt");

    string ctx = string();
    char c;
    while(target.get(c)){
        if(c == '\n' or c == '\n') continue;
        ctx += tolower(c);
        string s = lower(string(1, c));
        
        if(ctx.length() == 3 + 1){
            alph[ctx.substr(0, 3)]++;
            ctx = ctx.substr(1);
        }
    }

    for(it = alph.begin(); it != alph.end(); ++it){
        if(it->first.length() != 1){
            cout << it->first << "\t" << it->second << endl;
        }   
    }

    cout << fcm.calculate() << endl;

    double a = calculate(fcm.getA(), 0.1, alph);

    cout << a << endl;
    
    return 0;
}

