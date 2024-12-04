#include <bits/stdc++.h>
using namespace std;

template<class CT,class WT>
class HuffNode{
public:
    virtual WT getWeight() = 0;
    virtual bool isLeaf() = 0;
    virtual void setLeft(HuffNode<CT,WT>* child) = 0;
    virtual void setRight(HuffNode<CT,WT>* child) = 0;
    virtual HuffNode<CT,WT>* getLeft() = 0;
    virtual HuffNode<CT,WT>* getRight() = 0;
};  

template<class CT,class WT>
class LeafNode: public HuffNode<CT,WT>{
private:
    CT ch;
    WT wei;
public:
    LeafNode(const CT &ch,const WT &wei){
        this->ch = ch;
        this->wei = wei;
    }
    CT getChar(){return ch;}
    WT getWeight(){return wei;}
    bool isLeaf(){return true;}
    void setLeft(HuffNode<CT,WT>* child){}
    void setRighe(HuffNode<CT,WT>* child){}
    HuffNode<CT,WT>* getLeft(){return nullptr;}
    HuffNode<CT,WT>* getRight(){return nullptr;}
};

template<class CT,class WT>
class InNode: public HuffNode<CT,WT>{
private:
    WT wei;
    HuffNode<CT,WT>* Lchild;
    HuffNode<CT,WT>* Rchild;
public:
    InNode(WT wei,HuffNode<CT,WT>* lc,HuffNode<CT,WT>* rc){
        this->wei = wei;
        Lchild = lc;
        Rchild = rc;
    }
    WT getWeight(){return wei;}
    bool isLeaf(){return false;}
    void setLeft(HuffNode<CT,WT>* child){Lchild = child;}
    void setRight(HuffNode<CT,WT>* child){Rchild = child;}
    HuffNode<CT,WT>* getLeft(){return Lchild;}
    HuffNode<CT,WT>* getRight(){return Rchild;}
};

template<class CT,class WT>
class HuffTree{
protected: 
    HuffNode<CT,WT>* root;
    string charCodes[];
    HuffNode<CT,WT>* pCurNode;
    int num;
    unsigned int (* CharIndex)(const CT &);  //字符位置映射
    void CreatCode(HuffNode<CT,WT>* r,char code[],int len = 0);
    void Clear(HuffNode<CT,WT>* r);
public:
    HuffTree(CT ch[],WT w[],int n,unsigned int (* ChIndex)(const CT &));
    string Encode(CT ch);
    list<CT> Decode(string strCode);
};

template<class CT,class WT>
void HuffTree<CT,WT>::CreatCode(HuffNode<CT,WT>* r,char code[],int len){
    if(r->isLeaf()){
        CT ch = r->ch;
        code[len] = '\0';
        string strCode;
        strCode.assign(code);
        charCodes[(* CharIndex)(ch)] = strCode;
    }
    else{
        code[len] = '0';
        CreatCode(r->getLeft(),code,len + 1);

        code = '1';
        CreatCode(r->getRight(),code,len + 1);
    }
}  //创造节点的编码，形成映射关系

template<class CT,class WT>
struct HuffNodeHelp{
    HuffNode<CT,WT> *ptr;
};
template<class CT,class WT>
struct Compare {
    bool operator()(const HuffNodeHelp<CT, WT>& a, const HuffNodeHelp<CT, WT>& b) {
        // 对于小顶堆，如果 a 的权重小于 b 的权重，则返回 true，表示 a 应该在 b 的前面
        return a->ptr->getWeight() > b->ptr->getWeight();
    }
}; //为实现小顶堆写比较函数


template<class CT,class WT>
HuffTree<CT,WT>:: HuffTree(CT ch[],WT w[],int n,unsigned int (* ChIndex)(const CT &)){
    CharIndex = ChIndex;
    num = n;
    charCodes = new string[num];
    priority_queue<HuffNodeHelp<CT,WT>,vector<HuffNodeHelp<CT,WT>>, Compare<CT,WT>> minHeap;
    int pos = 0;
    for(pos = 0;pos<num;pos++){
        HuffNodeHelp<CT,WT> tem;
        tem.ptr = new LeafNode<CT,WT>(ch[pos],w[pos]);
        minHeap.push(tem);
    }//生成叶结点森林

    pos = 0;
    for(pos = 0;pos<num-1;pos++){
        HuffNodeHelp<CT,WT> r,r1,r2;
        minHeap.pop(r1);
        minHeap.pop(r2);
        r->ptr = new InNode<CT,WT>(r1.ptr.getWeight()+r2.ptr.getWeight(),r1.ptr,r2.ptr);
        minHeap.push(r);
    }
    HuffNodeHelp<CT,WT> rt;
    root = rt.ptr;
    pCurNode = root;
    char* code = new char[num];
    CreatCode(root,code);
    delete []code;
}

template<class CT,class WT>
list<CT> HuffTree<CT,WT>:: Decode(string strCode){
    list<CT> charList;
    for(int pos=0;pos<strCode.length();pos++){
        if(strCode[pos]=='0')pCurNode = pCurNode->getLeft;
        else pCurNode = pCurNode->getRight;

        if(pCurNode->isLeaf()){
            charList.push_back(pCurNode.getChar());
            pCurNode = root;
        }
    }
    return charList;
}


//字符缓存器
struct BufferType
{
    char ch;
    unsigned int bits;
};

class HuffCompress{
protected:
    HuffTree<char,unsigned long> *pHuffTree;
    ifstream*  infp;
    ofstream* outfp;
    BufferType buf;
    void Write(unsigned int bit);
    void WriteToOutfp();
public:
    HuffCompress();
    void Compress();
    void Decompress();
};

unsigned int CharIndex(const char &ch){
    return ch;    //不同ch的编码本身便不同,因此可以直接将ch视为无符号整数返回
}