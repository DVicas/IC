#include <fstream>
#include <iostream>
#include <sndfile.h>
#include <set>
#include <map>
#include <math.h>

using namespace std;

int main(int argc, char* argv[]){

    if(argc!=2){
        cout<<"WRONG NUMBER OF ARGUMENTS!"<<endl;
        return 1;
    }


    SNDFILE* file;
    SF_INFO sfinfo;
    sfinfo.format=0;

    file=sf_open(argv[1],SFM_READ,&sfinfo);

    printf("Opened file\n");
	printf("    Sample rate : %d\n", sfinfo.samplerate);
	printf("    Channels    : %d\n", sfinfo.channels);

    int num_items=(int)sfinfo.frames*sfinfo.channels;
    float* buffer=(float *)malloc(num_items*sizeof(int));

    float* channel1_buffer=(float *)malloc(sfinfo.frames*sizeof(float));
    float* channel2_buffer=(float *)malloc(sfinfo.frames*sizeof(float));



    float rd_data=sf_readf_float(file,buffer,num_items);

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

    map<float,int> channel1_histo;
    map<float,int> channel2_histo;
    map<float,int> mono_channel_histo;
    map<float,int>::iterator it;
    float average=0;

    for(int i=0;i<sfinfo.frames;i++){
        channel1_histo[channel1_buffer[i]]++;
        channel2_histo[channel2_buffer[i]]++;
        average=(channel1_buffer[i]+channel2_buffer[i])/2;
        mono_channel_histo[average]++;
    }
    double entropy=0;
    double p=0;
    cout<<"--------CHANNEL 1--------"<<endl;
    for(it=channel1_histo.begin(); it!=channel1_histo.end();it++){
        cout<<it->first<<" ";
        for(int i=0; i<it->second;i++){
            cout<<'*';
        }
        cout<<' '<<it->second<<endl;
        p=(double)channel1_histo[it->first]/(sfinfo.frames);
        entropy+=p*log(p);
        
    }
    cout<<"Entropy-> "<<entropy*-1<<endl;
    entropy=0;
    p=0;
    cout<<"--------CHANNEL 2--------"<<endl;
    for(it=channel2_histo.begin(); it!=channel2_histo.end();it++){
        cout<<it->first<<" ";
        for(int i=0; i<it->second;i++){
            cout<<'*';
        }
        cout<<' '<<it->second<<endl;
        p=(double)channel2_histo[it->first]/(sfinfo.frames);
        entropy+=p*log(p);
    }
    cout<<"Entropy-> "<<entropy*-1<<endl;

    entropy=0;
    p=0;
    cout<<"--------MONO CHANNEL --------"<<endl;
    for(it=mono_channel_histo.begin(); it!=mono_channel_histo.end();it++){
        cout<<it->first<<" ";
        for(int i=0; i<it->second;i++){
            cout<<'*';
        }
        cout<<' '<<it->second<<endl;
        p=(double)mono_channel_histo[it->first]/(sfinfo.frames);
        entropy+=p*log(p);
    }
    cout<<"Entropy-> "<<entropy*-1<<endl;

    sf_close(file);
    return 1;
}