#include <iostream>
#include <fstream>
#include <map>
#include "Fcm.hh"
using namespace std;


int main(int argc, char* argv[]){

    Fcm fcm("textos/teste.txt", 3, .1);
    fcm.openfile();
    fcm.read();
    double entropy = fcm.calculate();
    cout << "Entropy of the text -> " << entropy << endl;
    fcm.close();
    map<string, int>::iterator it;
    map<string, double>::iterator dit;
    map<string, int> alph = fcm.getA();
    // for(it = alph.begin(); it != alph.end(); ++it){
        // if(it->second==0) alph.erase(it->first);
        // cout << it->first << '\t' << it->second << endl; 
    // }
    // for(auto ctx : fcm.getCtx()){
    //     cout << ctx.first << "\t" << ctx.second << endl ;
    // }
    
    
    
    // map<string, double> entropies = fcm.getH();
    // for(dit = entropies.begin(); dit != entropies.end(); ++dit){
    //     //cout << dit->first << '\t' << dit->second << endl;
    // }   
    return 0;
}