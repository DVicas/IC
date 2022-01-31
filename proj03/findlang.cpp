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
    double most_sim_ent=999999;
    double finalEntropy;
    string lang;
    string filelist[] = {"textos/dutch.txt","textos/eng.txt","textos/esp.txt","textos/fin.txt","textos/fr.txt","textos/ger.txt","textos/ita.txt","textos/pol.txt","textos/pt.txt","textos/swe.txt"};

    for (int i=0; i<10; i++){

        Fcm fcm(filelist[i], k, alpha);
        fcm.openfile();
        fcm.read();
        double ent = fcm.calculate();
        
        map<string, int>::iterator it;
        map<string, int> contexts = fcm.getCtx();
        map<string, int> alphabet = fcm.getA();


        for(it = contexts.begin(); it != contexts.end(); ++it){
            contexts[it->first]=0;
        }

        for(it = alphabet.begin(); it != alphabet.end(); ++it){
            alphabet[it->first]=0;
        }
       

        fstream target(argv[1]);
        long total=0;
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
                total++;
            }  
        }

        double a = calculate(alphabet, alpha, contexts, fcm.getAlphabet(), total);

        if (abs(ent-a) < most_sim_ent){
            most_sim_ent = abs(ent-a);
            lang = filelist[i];
            finalEntropy = ent;
        }
        cout << filelist[i] << endl;
        cout << "entropia referencia: " << ent << endl;
        cout << "entropia target: " << a << endl;
        
        fcm.close();

    }

    cout << "Linguagem : " << lang.substr(7, lang.length()-3) << ", com entropia de : " << finalEntropy << endl;
    cout << most_sim_ent << endl;
    
    return 0;
}

