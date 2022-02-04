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

    double alpha = stod(argv[3]);
    int k=stoi(argv[2]);
    double finalBits=99999999;
    string lang;
    string filelist[] = {"textos/dutch.txt","textos/eng.txt","textos/esp.txt","textos/fin.txt","textos/fr.txt","textos/ger.txt","textos/ita.txt","textos/pol.txt","textos/pt.txt","textos/swe.txt"};

    for (int i=0; i<10; i++){

        Fcm fcm(filelist[i], k, alpha);
        fcm.openfile();
        fcm.read();

        double n_bits=0;
        
        map<string, int>::iterator it;
        map<string, int> contexts = fcm.getCtx();
        map<string, int> alphabet = fcm.getA();
        vector<char> aux=fcm.getAlphabet();
       

        fstream target(argv[1]);
        long total=0;
        string ctx = string();
        char c;
        while(target.get(c)){
            if(c == '\n' or c == '\t') continue;
        
            if(ctx.length() == k + 1){
                n_bits += -log2((double) (alpha + alphabet[ctx]) / (contexts[ctx.substr(0,k)] + alpha * aux.size()));
                
		ctx = ctx.substr(1) + (char) tolower(c);
                total++;
            }
            else{
                ctx+=(char)c;
            }
        }

        if (n_bits/total < finalBits){
            lang = filelist[i];
            finalBits = n_bits/total;
        }
        cout << filelist[i] << endl;
        cout << "N_bits : " << n_bits/total << endl;
        
        fcm.close();

    }

    cout << "Linguagem : " << lang << ", com estimativa de : " << finalBits << " bits por simbolo" << endl;
    
    return 0;
}

