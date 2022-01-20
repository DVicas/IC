#include <iostream>
#include <fstream>
#include <set>
#include "Fcm.hh"
using namespace std;


int main(int argc, char* argv[]){

    Fcm fcm("teste.txt", 3);
    for(auto c : fcm.getA()){
        cout << c << endl;
    }

    
    return 0;
}