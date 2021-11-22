#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]){
    if(argc!=3){
        cout<<"WRONG ARGUMENTS"<<endl;

    }

        ifstream ifs(argv[1]);
        ofstream ofs(argv[2]);
        string line;
        do{
            ifs>>line;
            ofs<<line<<endl;
        }while(getline(ifs,line));

    return 0;
}