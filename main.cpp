#include <iostream>
#include "parser.h"

using namespace std;

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