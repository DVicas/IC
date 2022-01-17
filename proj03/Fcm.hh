#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;


class Fcm{
    public: 
        Fcm(string file, int k);
        void read();
        set<char> getA();
        vector<char> getV();
        void print_permutations(char prefix[], int n) ;
    private:
        fstream f;
        set<char> alphabet;
        vector<char> v;
        map<char, int> m;
        int count = 0;
        int kkk;
};

Fcm::Fcm(string file, int k){
    kkk = k;
    f.open(file);
}

void Fcm::read(){
    
    char c;
    while(f.get(c)){
        alphabet.insert(tolower(c));
    }
    
    for(auto c : alphabet){
        v.push_back(c);
    }
}
set<char> Fcm::getA(){
    
    return alphabet;
    
}
vector<char> Fcm::getV(){
    
    return v;
    
}

void Fcm::print_permutations(char prefix[], int n, int k)  
{ 
        int i,j,l=strlen(prefix); 
        char newprefix[l+2];  
         
        if(kkk==0) 
        { 
           printf("%d %s\n",++count,prefix); 
           return; 
        } 
        for(i=0;i<n;i++)  
        { 
            //Concatenation of currentPrefix + arr[i] = newPrefix 
            for(j=0;j<l;j++)    
            newprefix[j] = prefix[j]; 
            newprefix[l] = v.at(i); 
            newprefix[l+1] = '\0';  
             
            print_permutations(newprefix, n, kkk-1); 
        } 
}