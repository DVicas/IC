#include <fstream>
#include <iostream>
#include <sndfile.h>
#include <set>
#include <map>

using namespace std;

int main(int argc, char* argv[]){

    SNDFILE* file;
    SF_INFO sfinfo;
    sfinfo.format=0;

    file=sf_open("sample02.wav",SFM_READ,&sfinfo);

    printf("Opened file\n");
	printf("    Sample rate : %d\n", sfinfo.samplerate);
	printf("    Channels    : %d\n", sfinfo.channels);

    int num_items=(int)sfinfo.frames*sfinfo.channels;
    int* buffer=(int *)malloc(num_items*sizeof(int));

    int* channel1_buffer=(int *)malloc(sfinfo.frames*sizeof(int));
    int* channel2_buffer=(int *)malloc(sfinfo.frames*sizeof(int));



    int rd_data=sf_read_int(file,buffer,num_items);

    cout<<"Read Data-> "<<rd_data<<endl;

    for(int i=0;i<rd_data;i+=sfinfo.channels){
        for(int j=0;j<sfinfo.channels;j++){
            if(j==0){
                channel1_buffer[i/2]=buffer[i+j];
                //cout<<"Channel 1-> "<<channel1_buffer[i/2]<<endl;
            }else{
                channel2_buffer[i/2]=buffer[i+j];
                //cout<<"Channel 2-> "<<channel2_buffer[i/2]<<endl;
            }
        }
    }

    map<int,int> channel1_histo;
    map<int,int> channel2_histo;
    map<int,int>::iterator it;

    for(int i=0;i<sfinfo.frames;i++){
        channel1_histo[channel1_buffer[i]]+=1;
        channel2_histo[channel2_buffer[i]]+=1;
    }

    for(it=channel1_histo.begin(); it!=channel1_histo.end();it++){
        cout<<it->first<<" "<<it->second<<endl;
    }

    for(it=channel2_histo.begin(); it!=channel2_histo.end();it++){
        cout<<it->first<<" "<<it->second<<endl;
    }
    sf_close(file);
    return 1;
}