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
            pi = (double) (smoothing_param + alphabet[it->first + x]) / (it->second + smoothing_param * aux.size());
            probability += pi * log2(pi);
        }
        entropies[it->first] = 1*probability;
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


    double alpha = 0.1;
    int k=3;
    double most_sim_ent=999999;
    string lang;
    string filelist[] = {"textos/czech.txt","textos/dutch.txt","textos/eng.txt","textos/esp.txt","textos/fin.txt","textos/fr.txt","textos/ger.txt","textos/ita.txt","textos/pol.txt","textos/pt.txt","textos/slovak.txt","textos/swe.txt"};

    for (int i=0; i<12; i++){

        Fcm fcm(filelist[i], k, 0.1);
        fcm.openfile();
        fcm.read();
        
        map<string, int>::iterator it;
        map<string, int> contexts = fcm.getCtx();
        map<string, int> alphabet = fcm.getA();

        for(it = contexts.begin(); it != contexts.end(); ++it){
            
                contexts.find(it->first)->second=0;
                // cout << it->first << "\t" << it->second << endl;
            
        }

        for(it = contexts.begin(); it != contexts.end(); ++it){
            
                // cout << it->first << "\t" << it->second << endl;
                alphabet.find(it->first)->second=0;
            
        }

        fstream target("textos/teste.txt");

        string ctx = string();
        char c;
        while(target.get(c)){
            if(c == '\n' or c == '\n') continue;
            ctx += tolower(c);
            string s = lower(string(1, c));
            alphabet[s]++;

            if(ctx.length() == k + 1){
                alphabet[ctx]++;
                contexts[ctx.substr(0, k)]++;
                ctx = ctx.substr(1);
            }
        }

        double aux = calculate(alphabet, alpha, contexts);

        
        if (abs(fcm.calculate()-aux) < most_sim_ent){
            most_sim_ent = abs(fcm.calculate()-aux);
            lang = filelist[i];
        }
        cout << filelist[i] << endl;
        cout << "ent " << fcm.calculate() << endl;
        cout << "ent " << aux << endl;


        fcm.close();

    }

    cout << "sda" << lang << endl;
    cout << most_sim_ent << endl;
    
    return 0;
}

