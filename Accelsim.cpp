#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

int getkernelInfo();

class ChannelState{
    public:
    int ChannelID;
    int freespace;
    int freelistptr;
};

class KernelState{
    int kernelID;
    int channelcandidacy;
    int channelstatebitvector;
    int in_out_tensorID;
    int slowdown;
    bool activate;

    public:
    KernelState(int kernelID, int candidacybitvector, int channelstatebitvector, int in_out_tensorID, int slowdown,bool activate=true){
    this-> kernelID = kernelID;
    this-> channelcandidacy = channelstatebitvector;
    this-> in_out_tensorID = in_out_tensorID;    
    this-> channelstatebitvector = channelstatebitvector;
    this-> slowdown = slowdown;
    };
    int getkernelslowdown(){return slowdown;};
    friend KernelStateTable;
};

class ChannelStateTable{

    public:
    int getfreespace(int k);
};

int ChannelStateTable::getfreespace(int k){

}

class KernelStateTable{
    vector<KernelState *> kernel;

    public:
    void appendkernel(int kernelID, KernelState& current){kernel.insert(kernel.begin() + kernelID, current);};
    int getslowdown(int kernel_num);
    void reactivate(int k, int prevoffchn);
    void removeCandidate(int k, int prevoffchn);
    int tensors(int k);
    int tensorsend(int k);
    int getcandidatechannels(int k);
};

int KernelStateTable::getslowdown(int kernel_num){
    return kernel.at(kernel_num)->getkernelslowdown();
}

void KernelStateTable::reactivate(int k, int prevoffchn){
    cout<<"reactivate kernel : "<<k<<" done"<<endl;
    kernel[k]->activate = false;
}

void KernelStateTable::removeCandidate(int k, int prevoffchn){          //?
    cout<<"removeCandidate kerenl : "<<k<<" done"<<endl;
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
    friend TensorStateTable;
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

int getkernelInfo(KernelStateTable &KST){
    int kernelID;
    int channelcandidacy;
    int channelstatebitvector;
    int in_out_tensorID;            //input이 들어온다고 가정
    int slowdown;

    KernelState *current = new KernelState(kernelID, channelcandidacy, channelstatebitvector, in_out_tensorID, slowdown);
    KST.appendkernel(kernelID, current);    //KST에 추가
    return kernelID;
}

int main()
{
    ChannelStateTable CST;
    KernelStateTable KST;
    TensorStateTable TST;
    int k;                                                     //current kerenl ID
    int prevoffchn = -1;                                       //the channel deactivated for kernel k in previous iteration
    int offchn;                                                //the channel to deactivate for kernel k in this iteration
    int slowdnlimit;                                           //user-specified performance slowdown limit
    int currentiteration;

    k = getkernelInfo(KST);                                    //getkernelInfo()함수에 input이 들어간다고 가정

    if(prevoffchn >= 0 && KST.getslowdown(k) > slowdnlimit)
    {
        KST.reactivate(k, prevoffchn);                          //prevoffchn이 들어가는 이유?
        KST.removeCandidate(k, prevoffchn);                     //prevoffchn이 들어가는 이유?
        
        for(int t = KST.tensors(k))
        //undo the movement of some of the pages in t in previous iteration
    }

    for(int t = KST.tensors(k); t < KST.tensorsend(k); t++){
    if(TST.getlastdeact_iter(t)==currentiteration) break;
    }

    //////////////Line 23//////////////
    int candchns = KST.getcandidatechannels();
    offchn = CST.getfreespace(c);

    //////////////Line 26//////////////
    movingpages();
    
    

    return 0;
}
