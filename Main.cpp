#include <bits/stdc++.h>
using namespace std;
class Graph{
    public:
    string name_of_graph;
    int no_of_nodes;
    vector <vector<pair<int,int>>> adjMat; //Creating a matrix so that each node is accessible directly
                                 //no need to traverse the entire list of connections if we know which node we want to connect with
    map <int,string> node_name;     //stores each node index's name
    map <string,int> node_ind;    //stores each node num mapped to its name
    map <string, pair<int, vector<string>>> outDegree; //strore outdegrees and all the reachable neighbours

    Graph(int n)
    {
        no_of_nodes=n;
        name_of_graph="";
        adjMat.resize(n,vector<pair<int,int>>(n,{0,0}));  //storing pair as: edge weight, edge time taken
    }
    void addEdge(int s, int e, int wt, int time)
    {
        if(s<no_of_nodes&&e<no_of_nodes)
        {
        adjMat[s][e].first=wt;
        adjMat[s][e].second=time;
        }
    }

    void outdegree()// gives n_o_n reachable, and their names;
    {
        int m=0;
        vector<string>reachables;
        for(int i=0; i<n; i++)
           { 
            for(int j=0; j<n; j++)
                if(adjMat[i][j]!=0)
                {   
                    reachables.push_back(node_index[i]);
                    m++;
                }
                outDegree[node_index[i]]={m,reachables};
                m=0;
                reachables.clear();
           }
    }
};
class RoadRunner{
    vector <Graph> topic;
    map <string, Graph> uni_map;    //storing each Graph mapped to its name
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
        string node_name,line,parent, nm;
        vector<vector<int>> edgeDetails;  //stores all edges in format: starting node, ending node, edge-weight, time needed
        map <string,int> mp;   //storing node value of each node
        map <int,string> rev_mp;  //storing name of each node
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
                nm=line.substr(6);
                parent=line.substr(6);
                rev_mp[0]=nm;
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
                    {
                        mp[wrd]=++n;
                        rev_mp[n]=wrd;
                    }
                    i+=1;
                }
                mp[wrd]=++n;
                rev_mp[n]=wrd;
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
                obj.node_name=rev_mp; 
                obj.node_ind=mp;
                obj.name_of_graph=nm;
                for(int i=0;i<edgeCount+1;i++)
                {
                    obj.addEdge(edgeDetails[i][0],edgeDetails[i][1],edgeDetails[i][2],edgeDetails[i][3]);
                }
                topic.push_back(obj);
                edgeDetails.clear();
                mp.clear();
            }
        }
        file.close();
        return;
    } 
};
