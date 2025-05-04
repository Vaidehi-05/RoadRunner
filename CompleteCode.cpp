#include <bits/stdc++.h>
#include "abbreviation.h"
using namespace std;

struct TrieNode
{
    bool isEnd;
    int freq;  //increase frequency of search // not used///????
    unordered_map<char, TrieNode*> child;

    TrieNode(): isEnd(false){}
};

class Trie
{
    vector<string> correctedWords;
    TrieNode* root;
    public:

    Trie(){root= new TrieNode();}

    void remains(TrieNode* node, string& word, vector<pair<int ,string>>& res) //helper function to find all words that can be made using given prefix
    {
        if(node->isEnd) res.push_back({node->freq, word});

        for(auto [c, next]:node->child)
        {
            word.push_back(c);
            remains(next, word, res);
            word.pop_back(); 
        }
    }

    void insert(string& word)
    {
        TrieNode* node = root;
        for(char c : word)
            {
                if(node->child[c])
                    node = node->child[c];
                else
                    node = node->child[c] = new TrieNode();
            }
        node->isEnd = true;
        //node->freq++;
        correctedWords.push_back(word);
    }

    vector<string> getSuggestions(string& word)
    {
        TrieNode* node = root;
        for(char c: word)
            {
            if(node->child.find(c) == node->child.end())
                return{};
             node = node->child[c];
            }
        
        vector<pair<int, string>> res;
        string temp = word;
        remains(node, temp, res);
        sort(res.begin(), res.end());

        vector<string> result; //top 3 most searched!
        for(int i=0; i<min(3,(int)res.size()); i++)
            result.push_back(res[i].second);

        return result;
    }

    bool search(string& word)
    {
        TrieNode* node = root;
        for(char c : word)
            {
                if(node->child.find(c)==node->child.end()) return false;
                node = node->child[c];
            }
        return node->isEnd;
    }
};

string normalize(string  word)
{
    transform(word.begin(), word.end(), word.begin(), ::tolower); //to lower
    
    string specialChars = "-_/.,!&#";
    for(char& c: word)
      if(specialChars.find(c) != string::npos)
            c=' ';
    
    //Removing extra spaces
    int start = 0, end = word.size()-1;
    string temp="";
    while(start <= end && word[start]==' ') start++;
    while(start <= end && word[end]==' ')end--;
    
    for(int i=start; i<=end; i++) 
        temp+= word[i];
    
    word=temp;
    temp.clear();
    
    for(int i=0; i<word.size(); i++)
    {
        if(!(i+1<word.size() && word[i]==' '&& word[i+1]==' '))
                temp+=word[i];
    }
    
    //Abbreviation Decoder
    if(abbreviations.find(word) != abbreviations.end())
        word=abbreviations[word];
    
        return word;
}

int editDistance(string a, string b)  //Levishtein Distance(number of operations from Word1 to Word2)
{
    int m=a.size(), n=b.size();

    vector<vector<int>> dp(m+1, vector<int>(n+1,-1));
    for(int i=0; i<m; i++) dp[i][0]=i;
    for(int j=0; j<n; j++) dp[0][j]=j;

    for(int i=1; i<=m; i++)
        for(int j=1; j<=n; j++)
            if(tolower(a[i-1])==tolower(b[j-1])) 
                dp[i][j]= 0 + dp[i-1][j-1]; // char matches
            else
                dp[i][j]= 1 + min(dp[i-1][j], min(dp[i][j-1], dp[i-1][j-1]));   //(Insertion, Deletion, Replacement)
    return dp[m][n];
}

vector<pair<string, int>> topMatches(string userInput, vector<string> topics) // stores top matches of a word along w/ their score!
{
    int s;
    vector<pair<string, int>> score;
    for(string a: topics)
    {
        s=editDistance(userInput, a);
        score.push_back({a,s});
    }
    sort(score.begin(), score.end(), [](pair<string, int>& a, pair<string, int>& b){return a.second<b.second;});

    vector<pair<string, int>> top;
    for(int i=0; i< min(3, (int)score.size()); i++)
        top.push_back(score[i]);
    return top;
}

vector<string> smartSearch(vector<string> topicsInput, vector<string> topics, Trie& trie)
{
    vector<string> results;
    for(string w: topicsInput)
    {
        string corrected = normalize(w);
        if(trie.search(corrected))
        {
            results.push_back(corrected);
            continue;
        }

        vector<string> suggestions = trie.getSuggestions(corrected);
        bool starting = true;
        if(suggestions.empty())
        {
            auto m= topMatches(corrected, topics);
            for(auto a:m)
                suggestions.push_back({a.first});
            starting = false;
        }

        cout<<"For the word: "<< w <<"\nDid you mean: ";
            for(int i=0; i<suggestions.size(); i++)
                {cout<< i+1<<") "<<suggestions[i];
                if(!starting)
                cout <<" (score: "<<editDistance(corrected, suggestions[i])<<")\n";
                }
            cout<<"Enter the number of the correct topic( 1-"<< suggestions.size()<< "), or 0 to enter manually: ";
            int choice;
            cin>> choice;
            // if(cin.fail() || choice<0 ||choice>suggestions.size()) //not needed
            // {
            //     cin.clear();
            //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
            //     choice= -1;
            // }

            if(choice>=1 && choice<=suggestions.size()) 
                results.push_back(suggestions[choice-1]);
            else
            {
                string manual;
                bool correct=false;
                while(!correct)
                {
                    cout<< "Enter the correct word manually:";
                    cin.ignore();
                    getline(cin, manual);
                    manual= normalize(manual);
                    if(trie.search(manual))
                    {
                        correct=true;
                        results.push_back(manual);
                    }
                    else
                    {
                        cout<<"You have entered the wrong choice!";
                    }
                }
            }
    }
    return results;

}

struct StackNode    //USED FOR DFS
{
    string gname;
    int nodeVal;
    vector<string> path;
    int totImportance;
    int totTime;
    map<string, bool> vis;
    //int diff; //variable constraints! 
};

struct KnapItem
{
    string name;
    int totTime;
    int totImp;
};

class Graph{
    public:
    string name_of_graph;
    int no_of_nodes;
    vector <vector<pair<int,int>>> adjMat; //Creating a matrix so that each node is accessible directly
                                 //no need to traverse the entire list of connections if we know which node we want to connect with
   
    map <int,string> node_name;     //stores each node index's name
    map <string,int> node_ind;    //stores each node num mapped to its name
    map <string, pair<int, vector<string>>> outDegree; //strore outdegrees and all the reachable neighbours
    int totalImportance;    //stores sum of importance of all subnodes --deep
    int totalTime;      //stores total time of only direct connections --only main

    Graph()
    {
        totalTime=0;
        totalImportance=0;
        no_of_nodes=0;
        name_of_graph="";
       // adjMat.resize(n,vector<pair<int,int>>(n,{0,0}));  //storing pair as: edge weight, edge time taken
    }

    Graph(int n)
    {
        totalTime=0;
        totalImportance=0;
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
        int n=no_of_nodes;
        vector<string>reachables;
        for(int i=0; i<n; i++)
           { 
            for(int j=0; j<n; j++)
                if(adjMat[i][j].first!=0 || adjMat[i][j].second!=0)
                {   
                    reachables.push_back(node_name[j]);
                    m++;
                }
                outDegree[node_name[i]]={m,reachables};
                m=0;
                reachables.clear();
           }
    }

    void getTotalTime()    
    { 
        for(int i=0; i<no_of_nodes; i++)
            for(int j=0; j<no_of_nodes; j++)
                totalTime += adjMat[i][j].second;
    }

    void getTotalImportance()
    {
        for(auto i: outDegree)
            totalImportance += i.second.first;
    }
};

class RoadRunner{
    public:
    vector <Graph> topic;   //???
    map <string, Graph> uni_map;    //storing each Graph mapped to its name
    map <string, int> indegree;     
    map <string, int> subtopic_adjustedImportance;
    map <string, set<string>> graphConnections;

     static bool compForPQDjikastra(pair <Graph, int> a, pair <Graph, int> b)
        {
            return a.second>b.second;
        }

    static bool comp_for_pq_forTopicCompletion(pair <string, int> a, pair <string, int> b)    
        {
            return a.second>b.second;
        }

    void computeIndegree()
    {
    for(auto graph: uni_map)
        for(auto outDegree: graph.second.outDegree)
        {
            if(uni_map.find(outDegree.first) != uni_map.end())
                graphConnections[graph.first].insert(outDegree.first);
            for(auto neighbour: outDegree.second.second)
                indegree[neighbour]++;
        }
    }

    void computeAdjustedImportance()
        {
            for(auto graph: uni_map)
            {
                int imp = graph.second.totalImportance;
                for(auto subGraph: graphConnections[graph.first])
                    imp += uni_map[subGraph].totalImportance;
                
                subtopic_adjustedImportance[graph.first] = imp;
            }
        }

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
        string node_name,line,/*parent,*/ nm;
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
               // parent=line.substr(6);
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
                        wrd="";
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
                a.push_back(mp[start]);
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
                uni_map[nm]=obj;
                topic.push_back(obj);
                edgeDetails.clear();
                mp.clear();
            }
        }
        file.close();
        return;
    } 

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
        //can create a function
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
    st.push({gname, nodeVal, {startNode}, a.outDegree[a.node_name[nodeVal]].first, 0});
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
            if(a.adjMat[b][i].second!=0)
                  c++;

        if(c==0 && uni_map.find(curNode)!=uni_map.end()) //if yes, find in uni_map, insert its children in stack(update the newTotTime, newTotImp beforehand, taking consideration present_skillset), reachableNodes, reachableEdges;
        {
            Graph a_new=uni_map[curNode];
            b=a_new.node_ind[curNode];
            for(int i=0; i<a_new.adjMat.size(); i++)
                if(a_new.adjMat[b][i].second!=0 &&!curStack.vis[a_new.node_name[i]]) //if(edge exists & notVisited)
                {
                    int edgeTime= curStack.totTime;
                    int importance= curStack.totImportance;
                    
                    bool isCompleted = present_skillset.count(a.node_name[i]); //update curStackNode.Time&Imp if userNotCompletedTopic
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
            if(a.adjMat[b][i].second!=0 && !curStack.vis[a.node_name[i]])  //if(edge exists & notVisited)
            {
                {
                    int edgeTime= curStack.totTime;
                    int importance= curStack.totImportance;
                    
                    bool isCompleted = present_skillset.count(a.node_name[i]); //update curStackNode.Time&Imp if userNotCompletedTopic
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

vector<string> KnapsackRecommendation(int maxTime, map<string, int> present_skillset) //need to exclude the already done topics
{
    unordered_set<string> neighb;
    for(auto& [a,nonono]:present_skillset)
        {
            if(uni_map.find(a)!=uni_map.end())
            {   Graph& b=uni_map[a];
                for(auto& neighbour: b.outDegree)
                    for(string& reachable: neighbour.second.second)
                        if(present_skillset.find(reachable)==present_skillset.end())
                            neighb.insert(reachable);
            }
        }
    
    vector<KnapItem> topics;

    for(auto graph:uni_map)
    {
        if (present_skillset.find(graph.first)!=present_skillset.end())
            continue;
        int imp = subtopic_adjustedImportance[graph.first];
        if(neighb.count(graph.first)) //prioritizing the neighbours;
            imp += 10;

        if(indegree.find(graph.first)!= indegree.end() && indegree[graph.first]==0)
            imp += 5;
        topics.push_back({graph.first, graph.second.totalTime, imp});
    }



    int n = topics.size();
    vector<vector<int>> dp(n+1, vector<int>(maxTime+1, 0));
    vector<vector<bool>> vis(n+1, vector<bool>(maxTime+1, false));

    for(int i=1; i<=n; i++)
        for(int t=0; t<=maxTime; t++)
        {
            if(t >= topics[i-1].totTime)
                {
                    dp[i][t]= max(dp[i-1][t], dp[i-1][t-topics[i-1].totTime]+ topics[i-1].totImp);
                    vis[i][t]= true;
                }
            else
                dp[i][t]= dp[i-1][t];
        }
    
    vector<string> listOfTopics;
    int w=maxTime;
    for(int i = n; i>0; i--)
    {
        if(vis[i][w])
        {
            listOfTopics.push_back(topics[i-1].name);
            w -= topics[i-1].totTime;
        }
    }

    return listOfTopics;
}


vector <string> findMostOptimalPath(string start, string end, int alpha_val, int beta_val, map<string, int> presnt_skillset)
{
    map <string, int> vis,shortest; //map storing shortet distance of all uptil now visited
    priority_queue <pair<Graph, int>, vector<pair<Graph,int>>, decltype (compForPQDjikastra)> pq(compForPQDjikastra);  //pq storing all nodes with their minimum distances
    vector <string> shortest_path;  //storing final path from start to end node
    int alpha=alpha_val, beta=beta_val;  //constants that decide the weightage given to weight(level) and time respectively
    Graph start_node, end_node;

    vis[start]=1;
    start_node=uni_map[start];
    end_node=uni_map[end];
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
            if(!vis[st])
            shortest_path.push_back(st);
            curr=s.node_ind[st];
            vis[st]=1;

            vector<vector<pair<int, int>>> adjMat=s.adjMat;

            for(int i=0; i<s.no_of_nodes; i++)  //traversing all the connections of the current node
            {
                if(adjMat[curr][i].first!=0&&vis.find(s.node_name[i])==vis.end())
                {
                    int x=(alpha*adjMat[curr][i].first)+(beta*adjMat[curr][i].second);  //custom comparator 
                    if(present_skillset.find(s.node_name[i])==present_skillset.end())  //create a present_skillset map to store current skillset of user
                    {
                        pq.push({uni_map[s.node_name[i]], x});  
                        shortest[s.node_name[i]]=minDist+x;
                    }

                    if(s.node_name[i]==end)  //returning the path as soon as the ending node is added 
                    return shortest_path;
                }
            }
        return shortest_path;
    }
}


vector<string> completionOfATopic(string topicName)     // CODE FOR COMPLETING A TOPIC FROM SCRATCH TO ADVANCED
{
    Graph& topic=uni_map[topicName];
    map <int, bool> vis;
    stack <string> st;
    vector <string> completePath;
    priority_queue <pair<string, int>, vector<pair<string, int>>, decltype(comp_for_pq_forTopicCompletion)> pq(comp_for_pq_forTopicCompletion);
    st.push(topicName);

    while(!st.empty())
    {
        int curr=topic.node_ind[st.top()];
        if(!vis[topic.node_ind[st.top()]])
        completePath.push_back(st.top());  
        vis[topic.node_ind[st.top()]]=true;
        st.pop();
        
        for(int i=0;i<topic.no_of_nodes;i++)
        {
            if(topic.adjMat[curr][i].first!=0&&vis[i]==false)  
            pq.push({topic.node_name[i], topic.adjMat[curr][i].first});
        }

        while(!pq.empty())  //STORED TEMPORARILY IN THE PRIORITY QUEUE SO THAT NODES ARE ENTERED IN THE STACK ACC. TO THE ORDER
        {
            st.push(pq.top().first);
            pq.pop();
        }
    }
    return completePath;
}


// Graph: node -> list of (neighbor, (difficulty, time))
// using Graph = unordered_map<string, vector<pair<string, pair<float, float>>>>;

// Dijkstra's algorithm
/*unordered_map<string, float> dijkstra(string start, float a = 0.5f, float b = 1.0f) {
    unordered_map<string, float> distance;
    Graph st=uni_map[start];

    for ( auto node : st) {
        distance[node.first] = numeric_limits<float>::infinity();
    }
    distance[start] = 0.0f;

    priority_queue<pair<float, string>, vector<pair<float, string>>, greater<pair<float, string>>> pq;
    pq.push({0.0f, start});

    while (!pq.empty()) {
        auto [currentDistance, u] = pq.top();
        pq.pop();

        if (currentDistance > distance[u]) continue;

        for (const auto& neighbor : st.at(u)) {
            string v = neighbor.first;
            float difficulty = neighbor.second.first;
            float time = neighbor.second.second;
            float weight = a * time + b * difficulty;

            if (distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                pq.push({distance[v], v});
            }
        }
    }

    return distance;
}

// Function to compute heuristic values properly
unordered_map<string, float> computeHeuristic(string start, string goal, float a = 0.5f, float b = 1.0f) {
    unordered_map<string, float> heuristic;
    Graph st=uni_map[start];

    // Reverse graph needed for correct heuristic (because goal becomes start)
    Graph reversed;
    for (const auto& [u, neighbors] : st) {
        for (const auto& [v, wt] : neighbors) {
            reversed[v].push_back({u, wt});
            if (reversed.find(u) == reversed.end()) {
                reversed[u] = {}; // Make sure all nodes appear
            }
        }
    }

    auto distance = dijkstra(reversed, goal, a, b);

    for (const auto& [node, _] : graph) {
        if (distance.find(node) != distance.end() && distance[node] != numeric_limits<float>::infinity()) {
            heuristic[node] = distance[node];
        } else {
            heuristic[node] = 0.0f; // unreachable nodes are assigned 0
        }
    }

    return heuristic;
}   */

};

int main()
{
    RoadRunner obj;
    obj.createGraphsUsingFile("filename.txt");

    for(Graph &g: obj.topic)
        {
            g.outdegree();
            g.getTotalTime();
            g.getTotalImportance();
            obj.uni_map[g.name_of_graph]=g;
        }
    return 0;
}



