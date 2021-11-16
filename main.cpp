#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

struct Node{
    char ch;
    int freq;
    Node *left, *right;
};

Node* getNode(char ch, int freq, Node* left, Node* right){
    Node* node = new Node();
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

struct comp{
    bool operator()(Node* l, Node* r){
        return l->freq > r->freq;
    }
};

void encode(Node* root, string str, unordered_map<char, string> &huffmanCode){
    if (root == nullptr){ return;}
    // found a leaf node
    if (!root->left && !root->right) { huffmanCode[root->ch] = str; }
    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void decode(Node* root, int &index, string str){
    if (root == nullptr) { return;}
    if (!root->left && !root->right){ cout << root->ch; return;}
    index++;
    if (str[index] =='0'){ decode(root->left, index, str);}
    else{ decode(root->right, index, str);}
}

void HuffmanAlg()
{   unordered_map<char, int> freq;
    long double BitCount=0, BitCount2=0;
    cout<<"Enter File name to compress."<<endl;
    string FileName, text;
    cin>>FileName;
    ifstream INPUT;
    INPUT.open(FileName);
    if(INPUT.is_open()) {
        while (getline(INPUT, text)) {
            text = text + "\n";
            for (char ch: text) { freq[ch]++;BitCount++;}
        }
        BitCount=BitCount*8;
        INPUT.close();
    }
    priority_queue<Node*, vector<Node*>, comp> pq;
    for (auto pair: freq) {pq.push(getNode(pair.first, pair.second, nullptr, nullptr)); }
    while (pq.size() != 1){
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top();	pq.pop();
        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }
    Node* root = pq.top();

    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    ofstream OUTPUT;
    OUTPUT.open("NewHuffCode.txt");
    INPUT.open(FileName);
    if(INPUT.is_open()) {
        BitCount2=0;
        while (getline(INPUT, text)) {
            int length = text.length();
            for (int i = 0; i < length; i++){
                BitCount2=BitCount2+huffmanCode[text[i]].length();
                OUTPUT<<huffmanCode[text[i]];
            }
        }
        OUTPUT.close();
        INPUT.close();
    }
    int option=0;
    while(option!=-1){
        cout<<endl<<endl;
        cout<<"1) Print Original Message Decoded:"<<endl;
        cout<<"2) Print Encoded Message:"<<endl;
        cout<<"3) Print Stats:"<<endl;
        cout<<"4) Print Huffman codes:"<<endl;
        cout<<"5) Quit"<<endl;
        cin>>option;
        if(option==1){
            INPUT.open("NewHuffCode.txt");
            if(INPUT.is_open()) {
                cout << "Decoded Message is:"<<endl;
                while (getline(INPUT, text)) {
                    int index=-1;
                    while (index < (int)text.size()) { decode(root, index, text); }
                }
                INPUT.close();
            }
        }else
        if(option==2){ ofstream OUTPUT;
            OUTPUT.open("NewHuffCode.txt");
            INPUT.open(FileName);
            if(INPUT.is_open()) {
                cout<<"Encoded Message: "<<endl;
                BitCount2=0;
                while (getline(INPUT, text)) {
                    int length = text.length();
                    for (int i = 0; i < length; i++) {
                        BitCount2 = BitCount2 + huffmanCode[text[i]].length();
                        cout << huffmanCode[text[i]];}
                }   }
        }else
        if(option==3){
            cout.precision(17);
            cout<<"Original Message is Composed of "<<BitCount<<" bits."<< endl;
            cout<<"Encoded message contains "<<BitCount2<<" bits."<<endl;
            cout<<"Original Message Compressed by "<<1-BitCount2/BitCount<<"%"<<endl;
            cout<<"Number of unique characters is: "<<huffmanCode.size()<<endl;
        }else
        if(option==4){
            cout << "Huffman Codes are :\n";
            for (auto pair: huffmanCode) {
                if(pair.first==10){cout << "/n" << " " << pair.second << '\n';}else
                {cout << pair.first << " " << pair.second << '\n';}
            }
        }else
        if(option==5){option=-1;}
    }
}

int main()
{
    HuffmanAlg();
    return 0;
}
