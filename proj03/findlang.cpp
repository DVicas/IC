#include <iostream>
#include <fstream>
#include <map>
#include "Fcm.hh"
using namespace std;

// double calculate(map<string, int> alphabet, double smoothing_param, map<string, int> contexts){
//     map<string, int>::iterator it;
//     map<string, double> entropies;
//     vector<string> aux;
//     map<string, int> all_elems;
//     for(it = alphabet.begin(); it != alphabet.end(); ++it){
//         if(it->first.length() == 1){
//             aux.push_back(it->first);
//         }
//     }
    
//     double probability = 0.0;
//     double pi = 0.0;
//     int total = 0;

//     for(it = contexts.begin(); it != contexts.end(); it++){
//         total += it->second;
//         for(string x : aux){
//             pi = (double) alphabet[it->first + x] / it->second;
//             if(pi == 0) pi = (double)smoothing_param / (it->second + smoothing_param * aux.size()); //-TODO : CHANGE THIS 
//             probability += pi * (log(pi)/log(2));
//         }
//         entropies[it->first] = -1 * probability;
//         probability = 0.0;
//     }

//     double total_entropy = 0.0;
//     for(it = contexts.begin(); it != contexts.end(); it++){
//         total_entropy += ((double) it->second / total) * entropies[it->first];
//     }
//     return total_entropy;        
// }

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
    int j=0;

    for(it = contexts.begin(); it != contexts.end(); it++){
        total += it->second;
        for(string x : aux){
            pi = (double) alphabet[it->first + x] / it->second;
            // if (it->second > 0) cout << it->second << "\t " << aux.size() << endl;
            if(pi == 0 or it->second == 0) {
                pi = (double)smoothing_param / (it->second + smoothing_param * aux.size()); //-TODO : CHANGE THIS 
            }
            probability += pi * (log(pi)/log(2));
        }
        entropies[it->first] = -1 * probability;
        // if (isinf(entropies[it->first])) cout << it->first << "\t " << probability << endl;
        // cout << entropies[it->first] << endl;
        probability = 0.0;
    }

    double total_entropy = 0.0;
    for(it = contexts.begin(); it != contexts.end(); it++){
        // cout << entropies[it->first] << endl;
        total_entropy += ((double) it->second / total) * entropies[it->first];
        // if (isnan(((double) it->second / total) * entropies[it->first])) cout << "it: " << it->second << "\t ent -> "<< entropies[it->first] <<endl;
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

    int k=3;
    double most_sim_ent=999999;
    string lang;
    string filelist[] = {"textos/czech.txt","textos/dutch.txt","textos/eng.txt","textos/esp.txt","textos/fin.txt","textos/fr.txt","textos/ger.txt","textos/ita.txt","textos/pol.txt","textos/pt.txt","textos/slovak.txt","textos/swe.txt"};

    for (int i=0; i<12; i++){

        Fcm fcm(filelist[i], k, 0.1);
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

        fstream target("textos/teste.txt");

        string ctx = string();
        char c;
        while(target.get(c)){
            if(c == '\n' or c == '\n') continue;
            ctx += tolower(c);
            string s = lower(string(1, c));
            
            if(ctx.length() == k + 1){
                alph[ctx.substr(0, k)]++;
                ctx = ctx.substr(1);
            }
        }

        double aux = calculate(fcm.getA(), 0.1, alph);
        cout << aux << endl;
        if (abs(fcm.calculate()-aux) < most_sim_ent){
            most_sim_ent = abs(fcm.calculate()-aux);
            lang = filelist[i];
        }

        fcm.close();

    }

    cout << lang << endl;
    cout << most_sim_ent << endl;
    
    return 0;
}