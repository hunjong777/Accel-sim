#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

int getkernelInfo();

class ChannelState{
    int ChannelID;
    int freespace;
    int* freelistptr;
    public:
    int getfreespace(){return freespace;};
    friend class ChannelStateTable;

};

class ChannelStateTable{
    vector<ChannelState> channel;                   //Channel ID 0번부터 순서대로 insert 해야함.

    public:
    int getfreespace(int k);
};

int ChannelStateTable::getfreespace(int candchns){
    int freespace = 0;
    int offchns;
    for(int k = 0; k<5; k++){                       //5는 channel 갯수
    if( (candchns>>k) & 1){
        if(freespace < channel.at(k).getfreespace()){
        freespace = channel.at(k).getfreespace();
        offchns = k;
        }
    }
    }
    offchns = 1 << offchns;
    return offchns;
}

class KernelState{
    int kernelID;
    int channelcandidacy;
    int channelstatebitvector;
    int in_out_tensorID;
    int slowdown;
    bool activate;

    public:
    KernelState(int kernelID, int channelstatebitvector , int in_out_tensorID, int slowdown, int channelcandidacy = 0B11111, bool activate=true){
    this-> kernelID = kernelID;
    this-> channelcandidacy = channelcandidacy;
    this-> in_out_tensorID = in_out_tensorID;    
    this-> channelstatebitvector = channelstatebitvector;
    this-> slowdown = slowdown;
    this-> activate = activate;
    };
    int getkernelslowdown(){return slowdown;};
    void activate(){this->activate = true;};
    void removecandidate(int prev){channelcandidacy &= ~prev;}
    friend class KernelStateTable;
};

class KernelStateTable{
    vector<KernelState *> kernel;
    
    public:
    void appendkernel(int kernelID, KernelState* current){kernel.insert(kernel.begin() + kernelID, current);};
    int getslowdown(int kernel_num);
    void reactivate(int k, int prevoffchn);
    void removeCandidate(int k, int prevoffchn);
    int tensors_begin(int k){};
    int tensors_end(int k){};
    int tensorsend(int k);
    int getcandidatechannels(int k);
};

int KernelStateTable::getslowdown(int kernel_num){
    return kernel.at(kernel_num)->getkernelslowdown();
}

void KernelStateTable::reactivate(int k, int prevoffchn){
    cout<<"reactivate kernel : "<<k<<" done"<<endl;
    kernel.at(k)->activate();
}

void KernelStateTable::removeCandidate(int k, int prevoffchn){         
    cout<<"removeCandidate kerenl : "<<k<<" done"<<endl;
    kernel.at(k)->removecandidate(prevoffchn);
}

int KernelStateTable::getcandidatechannels(int k){
                                            //candidacy bit vector로부터 channel id를 알 수 있는 방법?
    return kernel[k]->channelcandidacy;
}

class TensorState{
    int tensorID;
    int Residentchannelbitvector;
    map<string, int> base_size;
    int lastdeact_iter;
    public:
    friend class TensorStateTable;
};

class TensorStateTable{
    vector<TensorState *> Tensor;

    public:
    void appendtensor(TensorState * current, int tensorID){Tensor.insert(Tensor.begin() + tensorID, current);};
    int getlastdeact_iter(int k);
};

int TensorStateTable::getlastdeact_iter(int k){
    return Tensor.at(k)->lastdeact_iter;
}

int getkernelInfo(KernelStateTable &KST, int k){
    int kernelID = k;
    int channelcandidacy;
    int channelstatebitvector;
    int in_out_tensorID;            //input이 들어온다고 가정
    int slowdown;

    KernelState *current = new KernelState(kernelID, channelstatebitvector, in_out_tensorID, slowdown);
    KST.appendkernel(kernelID, current);    //KST에 추가
    return kernelID;
}

int main()                                                     //kernel(k), iteration 변수 추가
{
    ChannelStateTable CST;
    KernelStateTable KST;
    TensorStateTable TST;
    int slowdnlimit;                                           //user-specified performance slowdown limit
    int k;                                                     //current kerenl ID(가정)
    int prevoffchn = -1;                                       //the channel deactivated for kernel k in previous iteration
    int offchn;                                                //the channel to deactivate for kernel k in this iteration
    int currentiteration;

    getkernelInfo(KST, k);                                    //getkernelInfo()함수에 input이 들어간다고 가정

    if(prevoffchn >= 0 && KST.getslowdown(k) > slowdnlimit)
    {
        KST.reactivate(k, prevoffchn);                          
        KST.removeCandidate(k, prevoffchn);                     
        
        for(TensorState t = KST.tensors_begin(k); t<KST.tensors_end(k); t++)    //kernel당 tensor가 한개씩 할당?(KST)
        //undo the movement of some of the pages in t in previous iteration
    }

    for(int t = KST.tensors(k); t < KST.tensorsend(k); t++){
    if(TST.getlastdeact_iter(t)==currentiteration) break;
    }

    //////////////Line 23//////////////
    int candchns = KST.getcandidatechannels(k);
    offchn = CST.getfreespace(candchns);

    //////////////Line 26//////////////
    movingpages(offchn);
    
    

    return 0;
}

//kernel당 channel 따로 설정하는지?
