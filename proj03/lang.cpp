#include <iostream>
#include <fstream>
#include <map>
#include "Fcm.hh"
using namespace std;

double calculate(map<string, int> alphabet, double smoothing_param, map<string, int> contexts, vector<char> aux, long total){
    map<string, int>::iterator it;
    map<string, double> entropies;
    double pi = 0.0;
    double total_entropy = 0.0;

    for(it = contexts.begin(); it != contexts.end(); it++){
        for(char x : aux){
            pi = (double) (smoothing_param + alphabet[it->first + x]) / (it->second + smoothing_param * aux.size());
            entropies[it->first] += -pi * log2(pi);
        }
        total_entropy += ((double) it->second * entropies[it->first]) / total;
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

    int k = stoi(argv[2]);
    double alpha=stod(argv[3]);
    double n_bits=0;

    Fcm fcm(argv[1], k, alpha);
    fcm.openfile();
    fcm.read();
    cout << fcm.calculate() << endl;
    
    map<string, int>::iterator it;
    map<string, int> contexts = fcm.getCtx();
    map<string, int> alphabet = fcm.getA();
    vector<char> aux=fcm.getAlphabet();
    
    fcm.close();

    fstream target(argv[4]);

    string ctx = string();
    long total=0;
    char c;
    while(target.get(c)){
        if(c == '\n' or c == '\t') continue;
        
        if(ctx.length() == k + 1){
            
            n_bits += -log2((double) (alpha + alphabet[ctx]) / (contexts[ctx.substr(0,k)] + alpha * aux.size()));
            
            ctx = ctx.substr(1) + (char) tolower(c);
            total++;
        }
        else{
            ctx+=(char)tolower(c);
        }
    }

    //double a = calculate(alphabet, alpha, contexts, fcm.getAlphabet(), total);

    cout << "Estimated bits : " << n_bits/total << endl;
    
    return 0;
}

