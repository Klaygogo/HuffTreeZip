#include <bits/stdc++.h>
using namespace std;

template<class CT, class WT>
class HuffNode {
public:
    virtual WT getWeight() = 0;
    virtual bool isLeaf() = 0;
    virtual void setLeft(HuffNode<CT, WT>* child) = 0;
    virtual void setRight(HuffNode<CT, WT>* child) = 0;
    virtual HuffNode<CT, WT>* getLeft() = 0;
    virtual HuffNode<CT, WT>* getRight() = 0;
};  //创建父类节点，方便继承

template<class CT, class WT>
class LeafNode : public HuffNode<CT, WT> {
private:
    CT ch;
    WT wei;
public:
    LeafNode(const CT &ch, const WT &wei) {
        this->ch = ch;
        this->wei = wei;
    }
    CT getChar() { return ch; }
    WT getWeight() { return wei; }
    bool isLeaf() { return true; }
    void setLeft(HuffNode<CT, WT>* child) {}
    void setRight(HuffNode<CT, WT>* child) {}
    HuffNode<CT, WT>* getLeft() { return nullptr; }
    HuffNode<CT, WT>* getRight() { return nullptr; }
}; //叶子节点，存储权重及字符

template<class CT, class WT>
class InNode : public HuffNode<CT, WT> {
private:
    WT wei;
    HuffNode<CT, WT>* Lchild;
    HuffNode<CT, WT>* Rchild;
public:
    InNode(WT wei, HuffNode<CT, WT>* lc, HuffNode<CT, WT>* rc) {
        this->wei = wei;
        Lchild = lc;
        Rchild = rc;
    }
    ~InNode() {
        delete Lchild;
        delete Rchild;
    }
    WT getWeight() { return wei; }
    bool isLeaf() { return false; }
    void setLeft(HuffNode<CT, WT>* child) { Lchild = child; }
    void setRight(HuffNode<CT, WT>* child) { Rchild = child; }
    HuffNode<CT, WT>* getLeft() { return Lchild; }
    HuffNode<CT, WT>* getRight() { return Rchild; }
}; //内部的节点，只存权重

template<class CT, class WT>
class HuffTree {
protected: 
    HuffNode<CT, WT>* root;    //存储编码树的根节点
    HuffNode<CT, WT>* pCurNode;    //在译码时记录当前节点位置
    string charCodes[256];            //一个索引，记录每个字符对应的字符串编码
    unsigned int (*CharIndex)(const CT &);         //字符位置映射函数，输入字符返还其索引
    void CreatCode(HuffNode<CT, WT>* r, char code[], int len = 0);  //创造编码，记录每个叶子节点的编码
    void Clear(HuffNode<CT, WT>* r);
public:
    HuffTree(CT ch[], WT w[], int n, unsigned int (*ChIndex)(const CT &));
    ~HuffTree();
    string Encode(CT ch);
    list<CT> Decode(string strCode);
};

template<class CT, class WT>
void HuffTree<CT, WT>::CreatCode(HuffNode<CT, WT>* r, char code[], int len) {
    if (r->isLeaf()) {
        CT ch = ((LeafNode<CT, WT>*)r)->getChar();
        code[len] = '\0';                  
        string strCode(code);
        unsigned int index = (*CharIndex)(ch);
        if (index >= 0 && index < 256) {
            charCodes[index] = strCode;    //因为是字符，防止索引溢出
        } else {
            cerr << "Error: Invalid index " << index << " for character " << ch << endl;   //cerr是标准错误流对象，用于输出错误信息。
        }
    } else {
        code[len] = '0';
        CreatCode(r->getLeft(), code, len + 1);   //0向左搜索，1向右搜索，这里是递归查找

        code[len] = '1';
        CreatCode(r->getRight(), code, len + 1);
    }
}

template<class CT, class WT>
struct HuffNodeHelp {
    HuffNode<CT, WT> *ptr;
};
template<class CT, class WT>
struct Compare {
    bool operator()(const HuffNodeHelp<CT, WT>& a, const HuffNodeHelp<CT, WT>& b) {
        return a.ptr->getWeight() > b.ptr->getWeight();
    }
}; //创造比较函数，方便建立小顶堆

template<class CT, class WT>
HuffTree<CT, WT>::HuffTree(CT ch[], WT w[], int n, unsigned int (*ChIndex)(const CT &)) {
    CharIndex = ChIndex;
    priority_queue<HuffNodeHelp<CT, WT>, vector<HuffNodeHelp<CT, WT>>, Compare<CT, WT>> minHeap;  //创建一个优先队列，根据我们的比较函数，这是一个小顶堆
    for (int pos = 0; pos < n; pos++) {
        HuffNodeHelp<CT, WT> tem;
        tem.ptr = new LeafNode<CT, WT>(ch[pos], w[pos]);
        minHeap.push(tem);
    }//为每一个字符创建一个叶子节点

    for (int pos = 0; pos < n - 1; pos++) {
        HuffNodeHelp<CT, WT> r, r1, r2;
        r1 = minHeap.top(); 
        minHeap.pop();
        r2 = minHeap.top(); 
        minHeap.pop();
        r.ptr = new InNode<CT, WT>(r1.ptr->getWeight() + r2.ptr->getWeight(), r1.ptr, r2.ptr);
        minHeap.push(r);
    }//构建哈夫曼树
    HuffNodeHelp<CT, WT> rt = minHeap.top();
    minHeap.pop();
    root = rt.ptr;
    char code[256];
    CreatCode(root, code);
}

template<class CT, class WT>
HuffTree<CT, WT>::~HuffTree() {
    Clear(root);
}

template<class CT, class WT>
void HuffTree<CT, WT>::Clear(HuffNode<CT, WT>* r) {
    if (r == nullptr) return;
    if (!r->isLeaf()) {
        Clear(r->getLeft());
        Clear(r->getRight());
    }
    delete r;    //递归删除节点，释放内存
}

template<class CT, class WT>
string HuffTree<CT, WT>::Encode(CT ch) {
    unsigned int index = (*CharIndex)(ch);
    if (index >= 0 && index < 256) {
        return charCodes[index];
    } else {
        cerr << "Error: Invalid index " << index << " for character " << ch << endl;
        return "";
    }
}

template<class CT, class WT>
list<CT> HuffTree<CT, WT>::Decode(string strCode) {
    list<CT> charList;
    pCurNode = root; // 初始化 pCurNode 为根节点
    for (int pos = 0; pos < strCode.length(); pos++) {
        if (strCode[pos] == '0') {
            if (pCurNode->getLeft() != nullptr) {
                pCurNode = pCurNode->getLeft();
            } else {
                cerr << "Error: Left child is null at position " << pos << endl;
                break;
            }
        } else {
            if (pCurNode->getRight() != nullptr) {
                pCurNode = pCurNode->getRight();
            } else {
                cerr << "Error: Right child is null at position " << pos << endl;
                break;
            }
        }

        if (pCurNode->isLeaf()) {
            charList.push_back(((LeafNode<CT, WT>*)pCurNode)->getChar());
            pCurNode = root; // 重置 pCurNode 为根节点
        }
    }
    return charList;
}

struct BufferType {
    char ch;
    unsigned int bits;
};

class HuffCompress {
protected:
    HuffTree<char, unsigned long> *pHuffTree;
    FILE* infp;
    FILE* outfp;
    BufferType buf;
    
public:
    void Write(unsigned int bit);
    void WriteToOutfp();
    void Compress();
    void Decompress();
};

unsigned int CharIndex(const char &ch) {
    return static_cast<unsigned int>(static_cast<unsigned char>(ch)); // 确保值在0-255之间
}

void HuffCompress::Write(unsigned int bit) {
    buf.bits++;
    buf.ch = (buf.ch << 1) | bit;     //将ch左移一位，在将其与bit 进行或操作（bit 取值为0/1）
    if (buf.bits == 8) {
        fputc(buf.ch, outfp);
        buf.bits = 0;
        buf.ch = 0;   
    }
}

void HuffCompress::WriteToOutfp() {
    unsigned int len = buf.bits;
    if (len > 0) {
        for (unsigned int i = 0; i < 8 - len; i++) Write(0); 
    }
}

void HuffCompress::Compress() {
    char infName[256], outfName[256];
    cout << "请输入源文件名：";
    cin >> infName;
    if ((infp = fopen(infName, "rb")) == NULL) {
        cout << "打开源文件失败,程序自动退出！" << endl;
        exit(1);
    }  
    fgetc(infp);
    if (feof(infp)) {
        cout << "空源文件！" << endl;
        exit(2);
    }   

    cout << "请输入目标文件：";
    cin >> outfName;
    if ((outfp = fopen(outfName, "wb")) == NULL) {
        cout << "打开目标文件失败！";
        exit(3);
    }
   
    cout << "这在处理中，请等待………" << endl;
    const unsigned long n = 256;   
    char ch[n];
    unsigned long w[n];
    unsigned long size = 0;
    for (int pos = 0; pos < n; pos++) {
        ch[pos] = static_cast<char>(pos);        
        w[pos] = 0;
    }
    
    rewind(infp);   
    char cha;
    while ((cha = fgetc(infp)) != EOF) {
        w[static_cast<unsigned int>(static_cast<unsigned char>(cha))]++;
        size++;
    }

    pHuffTree = new HuffTree<char, unsigned long>(ch, w, n, CharIndex);
    rewind(outfp);
    fwrite(&size, sizeof(unsigned long), 1, outfp);   //在压缩文件开始写入源文件大小（bit）
    for (int pos = 0; pos < n; pos++) {
        fwrite(&w[pos], sizeof(unsigned long), 1, outfp);
    }  //写入每个字符的权重
    buf.bits = 0;
    buf.ch = 0;
    rewind(infp);
    while ((cha = fgetc(infp)) != EOF) {
        string strTem = pHuffTree->Encode(cha);
        for (int pos = 0; pos < strTem.length(); pos++) {
            if (strTem[pos] == '0') Write(0);
            else   Write(1);
        }
    }
    WriteToOutfp();
    fclose(infp);
    fclose(outfp);
    cout << "处理结束" << endl;
}

void HuffCompress::Decompress() {
    char infName[256], outfName[256];
    cout << "请输入压缩文件名：";
    cin >> infName;
    if ((infp = fopen(infName, "rb")) == NULL) {
        cout << "打开压缩文件失败,程序自动退出！" << endl;
        exit(4);
    }  
    fgetc(infp);
    if (feof(infp)) {
        cout << "压缩文件为空！" << endl;
        exit(5);
    }   

    cout << "请输入目标文件：";
    cin >> outfName;
    if ((outfp = fopen(outfName, "wb")) == NULL) {
        cout << "打开目标文件失败！";
        exit(6);
    }
   
    cout << "这在解压中，请等待………" << endl;
    const unsigned long n = 256;
    char ch[n];
    unsigned long w[n];
    unsigned long size = 0;
    for (int pos = 0; pos < n; pos++) {
        ch[pos] = static_cast<char>(pos);
    } 
    rewind(infp);
    fread(&size, sizeof(unsigned long), 1, infp);
    for (int pos = 0; pos < n; pos++) {
        fread(&w[pos], sizeof(unsigned long), 1, infp);
    } 
    pHuffTree = new HuffTree<char, unsigned long>(ch, w, n, CharIndex);
    unsigned long len = 0;
    char cha;
    string strTem = "";
    while ((cha = fgetc(infp)) != EOF && len < size) {
        unsigned char c = static_cast<unsigned char>(cha);
        for (int pos = 0; pos < 8; pos++) {
            if (c & 128) strTem += '1';
            else         strTem += '0';
            c = c << 1;
        }
    }
    list<char> lkText = pHuffTree->Decode(strTem);
    string strTemp(lkText.begin(), lkText.end());
    for (int pos = 0; pos < strTemp.length(); pos++) {
        len++;
        fputc(strTemp[pos], outfp);
         if (len == size) break;
    }
    fclose(infp);
    fclose(outfp);
    cout << "处理结束" << endl;
}

int main() {
    HuffCompress compressor;
    int choice;
    cout << "请选择操作: 1. 压缩 2. 解压缩" << endl;
    cin >> choice;
    if (choice == 1) {
        compressor.Compress();
    } else if (choice == 2) {
        compressor.Decompress();
    } else {
        cout << "无效的选择!" << endl;
    }
    return 0;
}



