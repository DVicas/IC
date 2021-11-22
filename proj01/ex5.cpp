#include <iostream>
#include <fstream>
#include <map>
#include <math.h>

using namespace std;


int main(int argc, char* argv[]){

    if(argc!=2){
        cout<<"WRONG NUMBER OF ARGUMENTS!"<<endl;
        return 1;
    }
    ifstream ifs(argv[1]);
    char ch;
    map<char,int> letters;
    double total_letters=0;

    char alphabet[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','z'};
    //init map
    for(int i=0;i<=23;i++){
        letters[alphabet[i]]=0;
    }
    while(ifs>>noskipws>>ch){
        letters[ch]++;
        total_letters++;
    }

    for(int i=0;i<=23;i++){
        cout<<alphabet[i]<<" -> ";

        for(int j=0; j<letters[alphabet[i]];j++){
            cout<<'*';
        }
        cout<<" ("<<letters[alphabet[i]]<<")"<<endl;
    }

    //entropy->24 symbols (alphabet) (we have the probability of each of them)
    double entropy=0;
    double p=0;
    for(int i=0;i<=23;i++){
        p=(double)letters[alphabet[i]]/total_letters;   //probability of the element
        entropy+=p*log(p);  //entropy=-(sum(p*log(p)))
    }

    cout <<"Histogram Entropy-> "<< entropy*-1<<endl;
        
    return 1;
}