#include <bits/stdc++.h>
using namespace std;


static bool comp(pair <Graph, int> a, pair <Graph, int> b)
{
    return a.second<b.second;
}


vector <string> findMostOptimalPath(string start, int start_lvl, string end, int end_lvl, int alpha_val, int beta_val)
{
    map <string, int> vis,shortest; //map storing shortet distance of all uptil now visited
    priority_queue <pair<Graph, int>, vector<pair<Graph,int>>, comp> pq;  //pq storing all nodes with their minimum distances
    vector <string> shortest_path;  //storing final path from start to end node
    int alpha=alpha_val, beta=beta_val;  //constants that decide the weightage given to weight(level) and time respectively
    Graph start_node, end_node;

    vis[start]=1;
    start_node=unimap[start];
    end_node=unimap[end];
    shortest[start]=0;
    shortest_path.push_back(start);
    pq.push({start_node, 0});

    while(!pq.empty())
    {
            Graph s=pq.top().first;
            int minDist=pq.top().second, curr=0;
            string st;
            pq.pop();
        
            st=s.name_of_graph;
            curr=s.node_ind[st];
            vis[st]=1

            vector<vector<pair<int, int>>> adjMat=s.adjMat;

            for(int i=0; i<s.no_of_nodes; i++)  //traversing all the connections of the current node
            {
                if(adjMat[curr][i].first!=0&&vis.find(s.node_name[i])==mp.end()))
                {
                    int x=(alpha*adjMat[curr][i].first)+(beta*adjMat[curr][i].second);  //custom comparator 
                    if(present_skillset[uni_map[s.node_name[i]]==present_skillset.end())  //create a present_skillset map to store current skillset of user
                    pq.push_back({uni_map[s.node_name[i]], x});  
                    shortest[s.node_name[i]]=minDist+x;
                    shortest_path.push_back(s.node_name[i];

                    if(s.node_name[i]==end)  //returning the path as soon as the ending node is added 
                    return shortest_path;
                }
            }
    }
}
