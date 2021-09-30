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
    void finls(){for(auto &ele:fin) cout << ele->getName() << ' '; cout << '\n';}
    void foutls(){for(auto &ele:fout) cout << ele->getName() << ' '; cout << '\n';}
    
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


int main(int argc, char* argv[])
{
    ifstream infile(argv[argc - 1]);
    if(!infile) {cerr << "The file is not exist! " << endl; exit(1);}
    unordered_map <string, Node> graph;
    string funciton, script;
    string term;

    while(infile >> term) if(term == ".model"){ infile >> term; break;}
    
    // start to read the file
    vector <string> correspondingTerm;
    while(infile >> term){
        if(term == ".inputs" || term == ".outputs" || term == ".names" || term == ".end"){
            if(term == ".end") funciton+=script;

            string line, parameter;
            getline(infile, line); // parameter
            while (breakLineDetection(line)){
                string nextLine;
                getline(infile, nextLine);
                line += nextLine;
            }
            
            if(term == ".names"){
                if(correspondingTerm.size() > 0){
                    correspondingTerm.clear();
                }
            }

            stringstream ss(line);

            while(ss >> parameter){
                if(graph.count(parameter) == 0){
                    Node node = Node();
                    node.setType(term);
                    node.setName(parameter);
                    graph[parameter] = node;
                }
                if(term == ".names") correspondingTerm.push_back(parameter);
            }

            if(term == ".names") {
                // construct the graph
                Node* outNode = &graph[correspondingTerm[correspondingTerm.size()-1]];
                for(unsigned int i = 0; i < correspondingTerm.size() - 1; i++){
                    Node* inNode = &graph[correspondingTerm[i]];
                    outNode->finAdd(inNode);
                    inNode->foutAdd(outNode);
                }

                if(script != ""){
                    funciton+=script + '\n';
                    script = "";
                }
            }
        }

        else{ // truth_val
            string truth_val = term;
            infile >> term;
            truth_val += term;
            if(truth_val[truth_val.length()-2] == '\\') truth_val.erase(truth_val.end()-2); // eliminate the '\\'(next line) in truth value 
            if(correspondingTerm.size() != truth_val.length()){cout << "No match" << endl;} // ligalization checker
            int last = truth_val.length() - 1;
            
            string word, posi;

            for(int i = 0; i < truth_val.length(); i++){
                if(truth_val[i] != '-'){
                    posi = correspondingTerm[i];
                    if(truth_val[i] == '0'){posi += '\'';}
                    if(i == last && script == "")
                        word = posi + " = " + word;
                    else{word += posi;}
                }
            }      
            
            if(script == "")
                script += word;
            else
                script += " + " + word;
        }
    }
    cout << funciton << endl;
}