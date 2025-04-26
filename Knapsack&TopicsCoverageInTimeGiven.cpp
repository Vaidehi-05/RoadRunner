#include <bits/stdc++.h>

using namespace std;

struct StackNode+
{
    string gname;
    int nodeVal;
    vector<string> path;
    int totImportance;
    int totTime;
    map<string, bool> vis;
    //int diff; //variable constraints! 
};

void printSubgraph(string node, map<string, /*map<string, int>& indegree,*/vector<pair<string, pair<int, int>>>>& branches, string indent= "", bool isLast = false)
{
    cout<< indent;
    if(!indent.empty())
        if(isLast) cout<<"└── ";
        else cout<< "├── ";

    cout<<node;
    
    // if(indegree.find(node) != indegree.end() && indegree[node]> 0)
    //     cout<<" (⚠)";
    cout<<endl;

    if(!branches[node].empty())cout<<endl;

    for(int i=0; i<branches.size(); i++)
    {
        auto[branch, weight]= branches[node][i];
        string p= indent +(isLast?"    ": "│   ");
        cout<< p<< (i+1==branches[node].size()? "└── " : "├── ");
        cout<< branch<<" (Time: " << weight.first<< ", Diff: "<<weight.second<< ")";

        // if(indegree.find(node) != indegree.end() && indegree[node]> 0)
        //     cout<<" (⚠)";
        cout<<endl;
        printSubgraph(branch, branches, p, i+1==branches[node].size());
    }
    /* DISPLAY FORMAT:
    Arrays
    ├── Sorting (Time: 5, Diff: 2)
    │   ├── Merge Sort (Time: 7, Diff: 3)
    │   └── Quick Sort (Time: 6, Diff: 4)
    └── Searching (Time: 4, Diff: 2)
        └── Binary Search (Time: 6, Diff: 3) */
}

void topicsPossibleInTime(string startNode, int timeConstraint)
{
    int nodeVal;
    string gname;
    for(auto[graphName, graph]: uni_map)    //finding the startNode's(GraphName & NodeIndex) by traverse the whole Graph
    {
            if(graph.node_ind.find(startNode)!=graph.node_ind.end())
            { 
                gname= graphName;
                nodeVal=graph.node_ind[startNode];
                break;
            }
    }

    stack<StackNode> st; //{GraphName, NodeIndex, PathTraversed, totImp, totTime, NodesVisited}

    set<string> reachableNodes;              //stores all nodes reachable from startNode(under Constraint)
    map<pair<string, string>, pair<int, int>> reachableEdges;    //<<node, node's_neighbour><Time, Difficulty>>
    //int maxImportance=0;

    Graph a = uni_map[gname];
    st.push({gname, nodeVal, {startNode}, a.outDegree[a.node_name[nodeVal]].first}, 0);
    // map<string, bool> vis;
    vector<vector<string>> allPaths;
    while(!st.empty())
    {
        int c=0;
        StackNode curStack= st.top();
        Graph a=uni_map[curStack.gname];
        int b=curStack.nodeVal;
        string curNode=a.node_name[b];
        int edgeTime, edgeDiff;

        st.pop();

        if(curStack.totTime > timeConstraint) continue; //skip if exceedign totalTime

        if(curStack.vis[curNode]) continue;
        curStack.vis[curNode]=true;

        allPaths.push_back(curStack.path);
        
        for(int i=0; i<a.adjMat.size(); i++) //Check if it is an endNode(leading to another mainTopic)
            if(a.adjMat[b][i]!=0)
                  c++;

        if(c==0 && uni_map.find(curNode)!=uni_map.end()) //if yes, find in uni_map, insert its children in stack(update the newTotTime, newTotImp beforehand, taking consideration userCompletedTopics), reachableNodes, reachableEdges;
        {
            Graph a_new=uni_map[curNode];
            b=a_new.node_ind[curNode];
            for(int i=0; i<a_new.adjMat.size(); i++)
                if(a_new.adjMat[b][i]!=0 &&!curStack.vis[a_new.node_name[i]]) //if(edge exists & notVisited)
                {
                    int edgeTime= curStack.totTime;
                    int importance= curStack.totImportance;
                    
                    bool isCompleted = userCompletionList.count(a.node_name[i]); //update curStackNode.Time&Imp if userNotCompletedTopic
                    if(!isCompleted)
                        {
                        edgeTime+= a.adjMat[b][i].first;
                        importance+= a.outDegree[a.node_name[i]].first;
                        }
                    if(edgeTime <= timeConstraint)  //if within timeConstraint (push data in StackNode, ReachableNode&Edges)
                    {
                        vector<string> newPath = curStack.path;
                        map<string, bool> newVis = curStack.vis;
                        newPath.push_back(a.node_name[i]);
                        newVis[a.node_name[i]] = true;

                        st.push({a.name_of_graph, i, newPath, importance, edgeTime, newVis});

                        reachableNodes.insert(curNode);
                        reachableNodes.insert(a.node_name[i]);
                        reachableEdges[{curNode, a.node_name[i]}]=a.adjMat[b][i];
                    }
                }
            continue;
        }


        for(int i=0; i<a.adjMat.size(); i++)
        {
            if(a.adjMat[b][i]!=0 && !curStack.vis[a.node_name[i]])  //if(edge exists & notVisited)
            {
                {
                    int edgeTime= curStack.totTime;
                    int importance= curStack.totImportance;
                    
                    bool isCompleted = userCompletionList.count(a.node_name[i]); //update curStackNode.Time&Imp if userNotCompletedTopic
                    if(!isCompleted)
                        {
                        edgeTime+= a.adjMat[b][i].first;
                        importance+= a.outDegree[a.node_name[i]].first;
                        }
                    if(edgeTime <= timeConstraint)  //if within timeConstraint (push data in StackNode, ReachableNode&Edges)
                    {
                        vector<string> newPath = curStack.path;
                        map<string, bool> newVis = curStack.vis;
                        newPath.push_back(a.node_name[i]);
                        newVis[a.node_name[i]] = true;
                        st.push({a.name_of_graph, i, newPath, importance, edgeTime, newVis});

                        reachableNodes.insert(curNode);
                        reachableNodes.insert(a.node_name[i]);
                        reachableEdges[{curNode, a.node_name[i]}]=a.adjMat[b][i];
                    }
                }
            }
        }
    }

    map<string, vector<pair<string, pair<int, int>>>> branches; //displayFormat
    for(auto e:reachableEdges)
        {
            string a=e.first.first;
            string b=e.first.second;
            branches[a].push_back({b,e.second});
        }
    
    cout<<"Visual Tree:"<<endl;
    printSubgraph(startNode, branches);

    // vector<string> topics; // use reachableNodes instead;
    // for(auto a: vis)
    //     topics.push_back(a.first);
}
