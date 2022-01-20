#include <iostream>
#include <fstream>
#include <map>
#include "Fcm.hh"
using namespace std;


int main(int argc, char* argv[]){

    Fcm fcm("teste.txt", 3);
    fcm.openfile();
    fcm.read();
    map<string, int>::iterator it;
    map<string, int> alph = fcm.getA();
    for(it = alph.begin(); it != alph.end(); ++it){
        cout << it->first << '\t' << it->second << endl;
    }

    
    return 0;
}