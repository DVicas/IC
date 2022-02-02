/*! \file fcm.cpp 
    \brief Developed program to test and execute the Fcm header.
            Firstly, verifies if the number of program arguments are correct; if so, creates and instance of the fcm class, opens the file, and reads it.
            It then calculates the entropy of the given text and displays it on the command line. It also outputs a context table file that is available on context_table.out
*/
#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include "Fcm.hh"
using namespace std;


int main(int argc, char* argv[]){
    if(argc < 4){
        cout << "Wrong number of arguments!" << endl;
        exit(EXIT_FAILURE);
    }else{
        Fcm fcm(argv[1], stoi(argv[2]), atof(argv[3]));
        fcm.openfile();
        fcm.read();
        double entropy = fcm.calculate();
        cout << "Entropy of the text -> " << entropy << endl;
        fcm.close();
        map<string, int>::iterator it;
        map<string, double>::iterator dit;
        map<string, int> alph = fcm.getA();
        int c = 0;
        // for(it = alph.begin(); it != alph.end(); ++it){
        //     if(it->first.length() == 1) {
        //         cout << it->first << " ";
        //         c++;
        //         if(c % 3 == 0) cout << endl;
        //     }
            
        // }
        // for(auto ctx : fcm.getCtx()){
        //     cout << ctx.first << "\t" << ctx.second << endl ;
        // }
        
        
        
        // map<string, double> entropies = fcm.getH();
        // for(dit = entropies.begin(); dit != entropies.end(); ++dit){
        //     //cout << dit->first << '\t' << dit->second << endl;
        // }   
        }
    
    
    return 0;
}
