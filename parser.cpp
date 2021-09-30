#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

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

bool breakLineDetection(string &line)
{
    if(line[line.length() - 1] == '\\'){
        line = line.substr(0, line.length() - 1);
        return true;
    }
    else return false;
}

class Parse
{
private:
    /* data */
    string content;
public:
    Parse(/* args */);
    ~Parse();

    void parsing(ifstream &infile, unordered_map <string, Node> &graph);
    void printContent(){cout << content << '\n'; cout << "END";}
};

Parse::Parse(/* args */)
{
}

Parse::~Parse()
{
}

void Parse::parsing(ifstream &infile, unordered_map <string, Node> &graph)
{
    string equation;
    string word;
    while(infile >> word) if(word == ".model"){ infile >> word; break;}
    
    // start to read the file
    vector <string> correspondingTerm;
    while(infile >> word){
        if(word == ".inputs" || word == ".outputs" || word == ".names" || word == ".end"){
            if(word == ".end") content+=equation;

            string line, parameter;
            getline(infile, line); // parameter
            while (breakLineDetection(line)){
                string nextLine;
                getline(infile, nextLine);
                line += nextLine;
            }
            
            if(word == ".names"){
                if(correspondingTerm.size() > 0){
                    correspondingTerm.clear();
                }
            }

            stringstream ss(line);

            while(ss >> parameter){
                if(graph.count(parameter) == 0){
                    Node node = Node();
                    node.setType(word);
                    node.setName(parameter);
                    graph[parameter] = node;
                }
                if(word == ".names") correspondingTerm.push_back(parameter);
            }

            if(word == ".names") {
                // construct the graph
                Node* outNode = &graph[correspondingTerm[correspondingTerm.size()-1]];
                for(unsigned int i = 0; i < correspondingTerm.size() - 1; i++){
                    Node* inNode = &graph[correspondingTerm[i]];
                    outNode->finAdd(inNode);
                    inNode->foutAdd(outNode);
                }

                if(equation != ""){
                    content+=equation + '\n';
                    equation = "";
                }
            }
        }

        else{ // truth_val
            string truth_val = word;
            infile >> word;
            truth_val += word;
            if(truth_val[truth_val.length()-2] == '\\') truth_val.erase(truth_val.end()-2); // eliminate the '\\'(next line) in truth value 
            if(correspondingTerm.size() != truth_val.length()){cout << "No match" << endl;} // ligalization checker
            int last = truth_val.length() - 1;
            
            string term, posi;

            for(int i = 0; i < truth_val.length(); i++){
                if(truth_val[i] != '-'){
                    posi = correspondingTerm[i];
                    if(truth_val[i] == '0'){posi += '\'';}
                    if(i == last && equation == "")
                        term = posi + " = " + term;
                    else{term += posi;}
                }
            }      
            
            if(equation == "")
                equation += term;
            else
                equation += " + " + term;
        }
    }
}

int main(int argc, char* argv[])
{
    ifstream infile(argv[argc - 1]);
    if(!infile) {cerr << "The file is not exist! " << endl; exit(1);}
    unordered_map <string, Node> graph;

    Parse parser;
    parser.parsing(infile, graph);
    parser.printContent();

    string node; 
    cout << "Please input a node: ";
    while (cin >> node){
        if(node == "0") break;
        if(graph.count(node) == 0) cout << "node" << node <<  "does not exist" << endl;
        else {
            cout << "predecessor: "; graph[node].finls(); 
            cout << "successor: "; graph[node].foutls();
        }
        cout << "Please input a node: ";
    }
    
}