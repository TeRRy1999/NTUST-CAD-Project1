#include <iostream>
#include <vector>
using namespace std;

class Node
{
private:
    string name;
    string type;
    vector <Node*> fin;
    vector <Node*> fout;

public:
    Node(/* args */){};
    
    //setter
    void setType(string in_tyep) {type = in_tyep;}
    void setName(string in_name) {name = in_name;}

    //getter
    string getType() {return type;}
    string getName() {return name;}

    // why not use the string to save, because string need to find from map with additional time comsuming
    void finAdd(Node* node){fin.push_back(node);}
    void foutAdd(Node* node){fout.push_back(node);}

    // list all node
    void finls(){for(auto &ele:fin) cout << ele->getName() << " "; cout << '\n';}
    void foutls(){for(auto &ele:fout) cout << ele->getName() << " "; cout << '\n';}
    
    ~Node(){};
};