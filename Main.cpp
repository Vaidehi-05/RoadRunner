#include <bits/stdc++.h>
using namespace std;
class Graph{
    int nodes;
    vector <vector<pair<int,int>>> adjMat; //Creating a matrix so that each node is accessible directly
                                 //no need to traverse the entire list of connections if we know which node we want to connect with
    public:
    map <string,int> node_name;
    Graph(int n)
    {
        nodes=n;
        adjMat.resize(n,vector<pair<int,int>>(n,{0,0}));
    }
    void addEdge(int s, int e, int wt, int time)
    {
        if(s<nodes&&e<nodes)
        {
        adjMat[s][e].first=wt;
        adjMat[e][s].first=wt;
        adjMat[s][e].second=time;
        adjMat[e][s].second=time;
        }
    }
};
class RoadRunner{
    vector <Graph> topic;
    public:
    void createGraphsUsingFile(string filename)
    {
        ifstream file(filename);
        /*
            FORMAT FOLLOWED: 
            
            Num_of_nodes: 
            Name: 
            ConnectedNodes: 
            EdgeStart:
            EdgeEnd:   
            EdgeWeight: 
            EdgeTimeNeeded: 
            
            */
        if(!file.is_open())
        {
            cout<<"Error! Could not open file. "<<endl;
            return;
        }

        int no_of_nodes,edgeCount;
        string node_name,line,parent;
        vector<vector<int>> edgeDetails;  //stores all edges in format: starting node, ending node, edge-weight, time needed
        map <string,int> mp;   //storing node value of each node
        while(getline(file,line))
        {
            if(line.rfind("Num_of_Nodes: ",0)==0)
            {
                no_of_nodes=stoi(line.substr(14));
                edgeCount=-1;
            }
            else if(line.rfind("Name: ",0)==0)
            {
                mp[line.substr(6)]=0;
            }
            else if(line.rfind("ConnectedNodes: ",0)==0)
            {
                string nodes=line.substr(16),wrd="";
                int i=0,len=nodes.length(),n=0;
                while(i<len)
                {
                    if(nodes[i]!=' ')
                    wrd+=nodes[i];
                    else
                    mp[wrd]=++n;
                }
                mp[wrd]=++n;
            }
            else if(line.rfind("EdgeStart: ",0)==0)
            {
                edgeCount+=1;
                string start=line.substr(11);
                vector<int> a;
                a.push_back(mp[parent]);
                edgeDetails.push_back(a);
            }
            else if(line.rfind("EdgeEnd: ",0)==0)
            {
                string end=line.substr(9);
                edgeDetails[edgeCount].push_back(mp[end]);
            }
            else if(line.rfind("EdgeWeight: ",0)==0)
            {
                int weight=stoi(line.substr(12));
                edgeDetails[edgeCount].push_back(weight);
            }
            else if(line.rfind("EdgeTimeNeeded: ",0)==0)
            {
                int time=stoi(line.substr(16));
                edgeDetails[edgeCount].push_back(time);
            }
            else if(line.rfind("*****",0)==0)
            {
                Graph obj(no_of_nodes);
                obj.node_name=mp; 
                for(int i=0;i<edgeCount+1;i++)
                {
                    obj.addEdge(edgeDetails[i][0],edgeDetails[i][1],edgeDetails[i][2],edgeDetails[i][3]);
                }
                edgeDetails.clear();
                mp.clear();
            }
        }
        file.close();
        return;
    } 
};
