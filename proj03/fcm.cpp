#include <iostream>
#include <fstream>
#include <map>
#include "Fcm.hh"
using namespace std;


int main(int argc, char* argv[]){

    Fcm fcm("teste.txt", 2, 10);
    fcm.openfile();
    fcm.read();
    double entropy = fcm.calculate();
    cout << "Entropy of the text -> " << entropy << endl;
    fcm.close();
    map<string, int>::iterator it;
    map<string, double>::iterator dit;
    map<string, int> alph = fcm.getCtx();
    // for(it = alph.begin(); it != alph.end(); ++it){
    //     if(it->first.length() != 1){
    //         cout << it->first << '\t' << it->second << endl;
    //     }   
    // }
    // for(auto ctx : fcm.getCtx()){
    //     //cout << ctx << endl ;
    // }
    
    
    
    map<string, double> entropies = fcm.getH();
    for(dit = entropies.begin(); dit != entropies.end(); ++dit){
        //cout << dit->first << '\t' << dit->second << endl;
    }   
    return 0;
}