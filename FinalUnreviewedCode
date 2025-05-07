#include <bits/stdc++.h>
#include <conio.h>
#include <iomanip>
#include "abbreviation.h"
using namespace std;

// ANSI Color Codes
 string RESET = "\033[0m";
const string ORANGE = "\033[38;5;208m";  // For subtopics
const string CYAN = "\033[36m";          // For main topics
const string GREEN = "\033[32m";         // For time/difficulty
const string YELLOW = "\033[33m";        // For headers
const string BLUE = "\033[38;5;202m";
          // For menu options
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

    void remains(TrieNode* node, string& word, vector<string>& res) //helper function to find all words that can be made using given prefix
    {
        if(node->isEnd)
        {
            //cout << "[DEBUG] Suggestion found: " << word << endl;
            res.push_back(word);
        }

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
        //cout << "[DEBUG] Inserted into Trie:-" << word << endl;
        correctedWords.push_back(word);
    }

    vector<string> getSuggestions(string& word)
    {
        TrieNode* node = root;
        for(char c: word)
        {
            if(node->child.find(c) == node->child.end())
                return {};
            node = node->child[c];
        }

        vector<string> res;
        string temp = word;
        remains(node, temp, res);

        // Lexicographical sorting
        sort(res.begin(), res.end());

        vector<string> result;
        for(int i = 0; i < min(3, (int)res.size()); i++)
            result.push_back(res[i]);

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
    int start = 0, endN = word.size()-1;
    string temp="";
    while(start <= endN && word[start]==' ') start++;
    while(start <= endN && word[endN]==' ')endN--;

    for(int i=start; i<=endN; i++)
        temp+= word[i];

    word=temp;
    temp.clear();

    for(int i=0; i<word.size(); i++)
    {
        if(!(i+1<word.size() && word[i]==' '&& word[i+1]==' '))
                temp+=word[i];
    }

    //cout << "[DEBUG] Normalized: " << word << endl;

    //Abbreviation Decoder
    if(abbreviations.find(word) != abbreviations.end())
        word=abbreviations[word];

        return word;
}

int editDistance(string a, string b)  //Levishtein Distance(number of operations from Word1 to Word2)
{
    int m=a.size(), n=b.size();

    vector<vector<int>> dp(m+1, vector<int>(n+1,0));
    for(int i=0; i<=m; i++) dp[i][0]=i;
    for(int j=0; j<=n; j++) dp[0][j]=j;

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
        if(!a.empty())
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
    for(string w : topicsInput)
    {
        string corrected = normalize(w);
        if(trie.search(corrected))
        {
            results.push_back(corrected);
            continue;
        }

        // Compute top matches using edit distance to all topics
        vector<pair<string, int>> matches = topMatches(corrected, topics);

        cout << "For the word: " << w << "\nDid you mean:\n";
        for (int i = 0; i < matches.size(); ++i)
        {
            cout << i + 1 << ") " << matches[i].first
                 << " (score: " << matches[i].second << ")\n";
        }

        cout << "Enter the number of the correct topic (1-" << matches.size()
             << "), or 0 to enter manually: ";
        int choice;
        cin >> choice;

        if (choice >= 1 && choice <= matches.size())
        {
            results.push_back(matches[choice - 1].first);
        }
        else
        {
            string manual;
            bool correct = false;
            cin.ignore();
            while (!correct)
            {
                cout << "Enter the correct word manually: ";
                getline(cin, manual);
                manual = normalize(manual);
                if (trie.search(manual))
                {
                    correct = true;
                    results.push_back(manual);
                }
                else
                {
                    cout << "You have entered the wrong topic. Try again.\n";
                }
            }
        }
    }

    return results;
}


void buildTrie(Trie& trie, vector<string>& topics)
{
    for(auto a: topics)
    {
        string word=normalize(a);
        trie.insert(word);
    }
}

vector<string> getCorrectedTopics(Trie& trie, vector<string>& allTopics)
{
    vector<string> inputs;
    string topic;

    cout<< "Enter topics you already know(type 'done' to finish):\n";
    while(cin>> topic && topic!="done")
        inputs.push_back(topic);

    vector<string> corrected= smartSearch(inputs, allTopics, trie);
    //unordered_set<string> skillset(corrected.begin(), corrected.endN());
    return corrected;
}

struct KnapItem
{
    string name;
    int totTime;
    int totImp;
};


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

//    Graph();
//    Graph(int n);
//    void addEdge(int s, int e, int wt, int time);
//    void outdegree();
//    void getTotalTime();
//    void getTotalImportance();

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
        if(s<no_of_nodes&&e<no_of_nodes && s!=e)
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
                totalTime+= adjMat[i][j].second;
    }

    void getTotalImportance()
    {
        for(auto i: outDegree)
            totalImportance+= i.second.first;
    }

    };

    class RoadRunner{
        public:
        vector <Graph> topic;   //???
        map<string, Graph> uni_map;    //storing each Graph mapped to its name
        map<string, int> indegree;     //storing the no. of indegree of nodes;
        map<string, vector<tuple<string, int, int>>> indegreeNodes;      //storing the name of indegree nodes
        map<string, int> subtopic_adjustedImportance;
        map<string, set<string>> graphConnections; //

//        void computeAdjustedImportance();
//        void computeIndegree();
//        void createGraphConnections();

void computeIndegree() {
    indegree.clear();
    indegreeNodes.clear();

    for (auto &[gname, graph] : uni_map) {
        int n = graph.no_of_nodes;
        for (int u = 0; u < n; ++u) {
            string from = graph.node_name[u];
            for (int v = 0; v < n; ++v) {
                auto [difficulty, time] = graph.adjMat[u][v];
                if (difficulty > 0) {  // or time > 0, depending on your encoding
                    string to = graph.node_name[v];
                    indegree[to]++;
                    indegreeNodes[to].emplace_back(from, difficulty, time);
                }
            }
        }
    }

    // Debug print
    cout << "[DEBUG] indegreeNodes map with weights:\n";
    for (auto &p : indegreeNodes) {
        cout << "  " << p.first << " <- ";
        for (auto &[src, diff, time] : p.second)
            cout << "(" << src << ", diff=" << diff << ", time=" << time << ") ";
        cout << "\n";
    }
}


        void createGraphConnections()
        {
        for(auto& graph:uni_map) //iterate through all graphs
            for(auto& outD:graph.second.outDegree)// for each node and its outgoing edges
                for (string& neighbour : outD.second.second)
                    // Check if this neighbour node exists in any other graph
                    for(auto& otherGraph: uni_map)
                    {
                        if (graph.first == otherGraph.first) continue;
                        if(otherGraph.second.node_ind.count(neighbour)>0)   // Found a connection in another graph
                            {
                            graphConnections[graph.first].insert(otherGraph.first);
                            cout << "  Found connection: " << graph.first
                                 << " -> " << otherGraph.first
                                 << " via node " << neighbour << endl;
                        }

                    }
        }

        void computeAdjustedImportance()
            {
                for(auto& graph: uni_map)
                {
                    int imp = graph.second.totalImportance;
                    for(auto subGraph: graphConnections[graph.first])
                        imp += uni_map[subGraph].totalImportance;

                    subtopic_adjustedImportance[graph.first] = imp;
                    cout<<subtopic_adjustedImportance[graph.first]<<endl;
                }
               // printAllAdjacencyMatrices();/////////////////////////////////////////////////////////////////////////////
            }

        string trim( string& s)
        {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t endN = s.find_last_not_of(" \t\r\n");
        return (start == string::npos) ? "" : s.substr(start, endN - start + 1);
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

            int no_of_nodes,edgeCount, n=0;
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
                    n=0;
                }
                else if(line.rfind("Name: ",0)==0)
                {
                    string str=line.substr(6);
                    nm= trim(str);
                    if(mp.find(nm)==mp.end())
                    {
    //                    int id = mp.size();
                        mp[nm]= 0;
                        rev_mp[0]= nm;
                    }

                }
                else if(line.rfind("ConnectedNodes: ",0)==0)
                    {
                        cout<<"ConnectingNodes:"<<endl;
                        string str=line.substr(16);
                        string nodes= trim(str), wrd="";
                         int i=0,len=nodes.length();
                            while(i<len)
                            {

                                if(nodes[i]!=' ')
                                wrd+=nodes[i];
                                else
                                {
                                    wrd=trim(wrd);
                                    mp[wrd]=++n;
                                    rev_mp[n]=wrd;
                                    wrd="";
                                }
                                i+=1;
                            }
                            wrd=trim(wrd);
                            mp[wrd]=++n;
                            rev_mp[n]=wrd;
                    }

                else if(line.rfind("EdgeStart: ",0)==0)
                {
                    edgeCount+=1;
                    string str=line.substr(11);
                    string start = trim(str);

                    vector<int> a;
                    a.push_back(mp[start]);
                    edgeDetails.push_back(a);
                }
                else if(line.rfind("EdgeEnd: ",0)==0)
                {
                    string str=line.substr(9);
                    string endN = trim(str);
                    edgeDetails[edgeCount].push_back(mp[endN]);
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
                    no_of_nodes=mp.size();
                    Graph obj(no_of_nodes);
                    //cout<<no_of_nodes<<endl;
                    obj.node_name=rev_mp;
                    obj.node_ind=mp;
                    //print(mp, rev_mp);
                    obj.name_of_graph=nm;
                    for(int i=0;i<edgeCount+1;i++)
                    {
                        obj.addEdge(edgeDetails[i][0],edgeDetails[i][1],edgeDetails[i][2],edgeDetails[i][3]);
                    }
                    printNodeMappings(mp, rev_mp);
                    uni_map[nm]=obj;
                    topic.push_back(obj);
                    edgeDetails.clear();
                    mp.clear();
                    rev_mp.clear();
                }
            }
            file.close();
            return;
        }

        // void printAllAdjacencyMatrices()
        // {
        //     for (auto &[graphName, graph] : uni_map) {
        //         cout << "\n[DEBUG] Adjacency Matrix for Graph: " << graphName << "\n";
        //         if (graph.node_name.size() != graph.no_of_nodes || graph.adjMat.size() != graph.no_of_nodes) {
        //             cout << "[ERROR] Mismatch in node_name or adjMat size for graph: " << graphName << "\n";
        //             continue;
        //         }
        //         // Print header row (node names)
        //         cout << "      ";
        //         for (const auto &name : graph.node_name)
        //             cout << setw(15) << name.first<<" "<<name.second<<" ";
        //         cout << "\n";

        //         for (int i = 0; i < graph.no_of_nodes; i++) {
        //             cout << setw(5) << graph.node_name[i] << " ";
        //             for (int j = 0; j < graph.no_of_nodes; j++) {
        //                 auto &edge = graph.adjMat[i][j];
        //                 if (edge.first != 0)
        //                     cout << setw(15) << "(" + to_string(edge.first) + "," + to_string(edge.second) + ")";
        //                 else
        //                     cout << setw(15) << "-";
        //             }
        //             cout << "\n";
        //         }
        //     }
        // }

        void printNodeMappings(map<string, int>& ni, map<int, string>& nn) {
            // Print NodeName Mapping (index -> node name)
            cout << "NodeName Mapping:\n";
            for (auto& pair : nn) {
                cout << pair.first << " -> " << pair.second << endl;
            }

            // Print NodeIndex Mapping (node name -> index)
            cout << "\nNodeIndex Mapping:\n";
            for (auto& pair : ni) {
                cout << pair.first << " -> " << pair.second << endl;
            }
        }

    //    void printSubgraph(string node,
    //                   map<string, vector<pair<string, pair<int, int>>>>& branches,
    //                   int timeConstraint,
    //                   string indent = "", bool isLast = true,
    //                   pair<int, int> edgeInfo = {-1, -1},
    //                   unordered_set<string>& visited = *(new unordered_set<string>()),
    //                   int timeSpent = 0)
    //{
    //    // If the current time spent exceeds the constraint, stop
    //    if (timeSpent > timeConstraint) {
    //        return;
    //    }
    //
    //    // Check if we can print the current node with the edge time
    //    int newTimeSpent = timeSpent + edgeInfo.first;
    //    if (newTimeSpent > timeConstraint) {
    //        return;  // Don't print and stop recursion if we exceed the time constraint
    //    }
    //
    //    if (visited.count(node)) return;
    //    visited.insert(node);
    //
    //    // Print current node
    //    cout << indent;
    //    if (!indent.empty()) {
    //        cout << (isLast ? "└── " : "├── ");
    //    }
    //
    //    cout << node;
    //    if (edgeInfo.first != -1) {
    //        cout << " (Time: " << edgeInfo.first << ", Diff: " << edgeInfo.second << ")";
    //    }
    //    cout << endl;
    //
    //    // Prepare indentation for next level
    //    indent += (isLast ? "    " : "│   ");
    //
    //    // Recurse for all children, updating timeSpent
    //    const auto& children = branches[node];
    //    for (size_t i = 0; i < children.size(); ++i) {
    //        const auto& [child, weight] = children[i];
    //        bool childIsLast = (i == children.size() - 1);
    //
    //        // Add the edge time to timeSpent for next level
    //        printSubgraph(child, branches, timeConstraint, indent, childIsLast, weight, visited, newTimeSpent);
    //    }
    //}

        void printSubgraph(string node,map<string, vector<pair<string, pair<int, int>>>> &branches,int timeConstraint, unordered_set<string>& present_skillset,string indent = "", bool isLast = true,pair<int, int> edgeInfo = {-1, -1}, unordered_set<string> &visited = *(new unordered_set<string>()),int timeSpent = 0)
        {
            // If the current time spent exceeds the constraint, stop
            if (timeSpent > timeConstraint) return;


            // Check if we can print the current node with the edge time
            int newTimeSpent = timeSpent + edgeInfo.first;
            if (newTimeSpent > timeConstraint)
            {
                return; // Don't print and stop recursion if we exceed the time constraint
            }

            if (visited.count(node))
                return;
            visited.insert(node);

            // Print current node
            cout << indent;
            if (!indent.empty())
            {
                cout << (isLast ? "└── " : "├── ");
            }

            cout << node;

            if (edgeInfo.first != -1)
            {
                cout << " (Time: " << edgeInfo.first << ", Diff: " << edgeInfo.second << ")";
            }
            if(present_skillset.count(node)>0)cout<<"   --✔";
            cout << endl;

            // Prepare indentation for next level
            indent += (isLast ? "    " : "│   ");

            // Recurse for all children, updating timeSpent
            auto &children = branches[node];
            for (size_t i = 0; i < children.size(); ++i)
            {
                auto &[child, weight] = children[i];
                bool childIsLast = (i == children.size() - 1);

                // Add the edge time to timeSpent for next level
                printSubgraph(child, branches, timeConstraint, present_skillset, indent, childIsLast, weight, visited, newTimeSpent);
            }
        }

        void topicsPossibleInTime(string startNode, int timeConstraint, unordered_set<string>& present_skillset)
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
        int totalTimeUsed = 0;
    set<pair<string, string>> usedEdges;

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

            reachableNodes.insert(curNode);
            for ( string& node : curStack.path)
        reachableNodes.insert(node);

            if(curStack.totTime > timeConstraint) continue; //skip if exceedign totalTime

    //        if(curStack.vis[curNode]) continue;
    //        curStack.vis[curNode]=true;

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

                        bool isCompleted= present_skillset.count(a_new.node_name[i]); //update curStackNode.Time&Imp if userNotCompletedTopic
                        if(!isCompleted)
                            {
                            edgeTime+= a_new.adjMat[b][i].first;
                            importance+= a_new.outDegree[a_new.node_name[i]].first;
                            }
                        pair<string, string> edgeKey = {curNode, a_new.node_name[i]};
    int edgeTimeToAdd = a_new.adjMat[b][i].first;

    if (usedEdges.find(edgeKey) == usedEdges.end() && curStack.totTime + edgeTimeToAdd <= timeConstraint) {
        curStack.totTime += edgeTimeToAdd;
        usedEdges.insert(edgeKey);
                            vector<string> newPath= curStack.path;
                            map<string, bool> newVis= curStack.vis;
                            newPath.push_back(a_new.node_name[i]);
                            newVis[a_new.node_name[i]] = true;

                            st.push({a_new.name_of_graph, i, newPath, importance, edgeTime, newVis});

                            reachableNodes.insert(curNode);
                            reachableNodes.insert(a_new.node_name[i]);
                            reachableEdges[{curNode, a_new.node_name[i]}]=a_new.adjMat[b][i];
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

                        bool isCompleted= present_skillset.count(a.node_name[i]); //update curStackNode.Time&Imp if userNotCompletedTopic
                        if(!isCompleted)
                            {
                            edgeTime+= a.adjMat[b][i].first;
                            importance+= a.outDegree[a.node_name[i]].first;
                            }
                        pair<string, string> edgeKey = {curNode, a.node_name[i]};
    int edgeTimeToAdd = a.adjMat[b][i].first;

    if (usedEdges.find(edgeKey) == usedEdges.end() && curStack.totTime + edgeTimeToAdd <= timeConstraint) {
                            curStack.totTime += edgeTimeToAdd;
                            usedEdges.insert(edgeKey);
                            vector<string> newPath= curStack.path;
                            map<string, bool> newVis= curStack.vis;
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
        printSubgraph(startNode, branches, timeConstraint, present_skillset);

        // vector<string> topics; // use reachableNodes instead;
        // for(auto a: vis)
        //     topics.push_back(a.first);
    }

         vector<string> KnapsackRecommendation(int maxTime, map<string, int>& present_skillset) //need to exclude the already done topics
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
            cout << "Adding topic: " << graph.first << " with time " << graph.second.totalTime << " and imp " << imp << endl;
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

    static bool compForPQDjikastra(pair <pair<Graph,string>, int> a, pair <pair<Graph,string>, int> b)
            {
                return a.second>b.second;
        }
    vector <string> findMostOptimalPath(string start, string endN, int alpha_val, int beta_val, map<string, int>& present_skillset)
    {
        map <string, int> vis,shortest; //map storing shortet distance of all uptil now visited
        priority_queue <pair<pair<Graph,string>, int>, vector<pair<pair<Graph,string>,int>>, bool(*)(pair<pair<Graph,string>, int>, pair<pair<Graph,string>, int>)> pq(compForPQDjikastra);  //pq storing all nodes with their minimum distances
        vector <string> shortest_path;  //storing final path from start to endN node
        int alpha=alpha_val, beta=beta_val;  //constants that decide the weightage given to weight(level) and time respectively
        Graph start_node, end_node;

        vis[start]=1;
        start_node=uni_map[start];
        end_node=uni_map[endN];
        shortest[start]=0;
        shortest_path.push_back(start);
        pq.push({{start_node,start}, 0});

        while(!pq.empty())
        {
                Graph s=pq.top().first.first;
                int minDist=pq.top().second, curr=s.node_ind[pq.top().first.second];
                string st=pq.top().first.second;
                pq.pop();

              //  st=s.name_of_graph;
              if(!vis[st])
              {
               shortest_path.push_back(st);
                shortest_path.push_back(st);
                vis[st]=1;
               }


                if(st==endN)
                return shortest_path;

               // vector<vector<pair<int, int>>> adjMat=s.adjMat;

                for(int i=0; i<s.no_of_nodes; i++)  //traversing all the connections of the current node
                {
                    if(s.adjMat[curr][i].first!=0&&vis.find(s.node_name[i])==vis.end())
                    {
                        int x=(alpha*s.adjMat[curr][i].first)+(beta*s.adjMat[curr][i].second);  //custom comparator
                        if(((shortest.find(s.node_name[i])==shortest.end())||(minDist+x<shortest[s.node_name[i]]))) //create a present_skillset map to store current skillset of user
                        {
                            if(uni_map.find(s.node_name[i])==uni_map.end())
                            pq.push({{s, s.node_name[i]}, x});
                            else
                            pq.push({{uni_map[s.node_name[i]],s.node_name[i]},x});
                            shortest[s.node_name[i]]=x+minDist;
                          //  shortest_path.push_back()
                            //parent[s.node_name[i]]=st;
                        }
                    }
                }
        }
        return shortest_path;
    }

    static bool comp_for_pq_forTopicCompletion(pair <string, int> a, pair <string, int> b)
            {
                return a.second<b.second;
            }

    vector<string> completionOfATopic(string topicName)     // CODE FOR COMPLETING A TOPIC FROM SCRATCH TO ADVANCED
    {
        Graph& topic=uni_map[topicName];
        map <string, bool> vis;
        stack <string> st;
        vector <string> completePath;
        priority_queue <pair<string, int>, vector<pair<string, int>>, bool(*)(pair<string, int>, pair<string, int>)> pq(comp_for_pq_forTopicCompletion);
        st.push(topicName);
        int num_of_nodes_added=0;

        cout << "Starting topic completion from node: " << topicName << "\n";

        while(!st.empty())
        {
            string currNode = st.top();
            st.pop();
            cout << "\nPopped from stack: " << currNode << endl;

             if (topic.node_ind.find(currNode) == topic.node_ind.end())
                continue; // Skip if node is not present in the graph
              //  cout << "Current Node: " << currNode << endl;

           // int curr = topic.node_ind[currNode];

            int curr=topic.node_ind[currNode];
            if(vis.find(currNode)==vis.end())
            {
            completePath.push_back(currNode);
            num_of_nodes_added+=1;
            vis[currNode]=true;
            cout << "Visited and added to path: " << currNode << "\n";
            }
            else
            continue;
           // st.pop();

           cout << "Looking at neighbors of: " << currNode << "\n";
            for(int i=0;i<topic.no_of_nodes;i++)
            {
                if(topic.adjMat[curr][i].first!=0&&(vis.find(topic.node_name[i])==vis.end()))
                {
                pq.push({topic.node_name[i], topic.adjMat[curr][i].first});
                cout << "    Neighbor added to PQ: " << topic.node_name[i]
                         << " with weight: " << topic.adjMat[curr][i].first << "\n";
               // vis[topic.node_name[i]]=true;
              // cout << "Pushed to stack from: " << topic.node_name[i] << " with weight " << topic.adjMat[curr][i].first << endl;

                }
            }

            while(!pq.empty())  //STORED TEMPORARILY IN THE PRIORITY QUEUE SO THAT NODES ARE ENTERED IN THE STACK ACC. TO THE ORDER
            {
                    st.push(pq.top().first);
                pq.pop();
            }

            cout << "Current completePath: ";
            for (auto &node : completePath)
                cout << node << " ";
            cout << "\n---------------------------\n";

           /* if(num_of_nodes_added>=15)
            break;*/
    }return completePath;
    }


    void BasicDisplayOfRoadmap(vector<string>& vec, map<string, int>& present_skillset)
    {
        cout<<"THE ROADMAP GENERATED ACCORDING TO THE GIVEN CONSTRAINTS IS AS FOLLOWS: "<<endl;
        for(int i=0; i<vec.size(); i++)
        {
            if(present_skillset.find(vec[i])==present_skillset.end())
            cout<<vec[i]<<endl;
    }
    }

    // ==== Display Path with Main and Sub Topics ====
    void displayPathWithMainAndSubTopics(vector<string>& path) {
        if (path.empty()) {
            cout << YELLOW << "\nNo path found.\n" << RESET;
            return;
        }

        cout << YELLOW << "\n===== Learning Path =====\n" << RESET;

        for (size_t i = 0; i < path.size(); ++i) {
            string& topic = path[i];

            // Display main topic with proper indentation
            cout << string(i * 2, ' ');  // Indentation based on level

            // Check if this is a main topic (exists in uni_map)
            if (uni_map.find(topic) != uni_map.end()) {
                cout << CYAN << "→ " << topic << " (Main Topic)" << RESET << "\n";

                // Display subtopics
                Graph& g = uni_map[topic];
                for (const auto& outD : g.outDegree) {
                    for (const string& subtopic : outD.second.second) {
                        cout << string((i + 1) * 2, ' ');  // Increased indentation for subtopics
                        cout << ORANGE << "└─ " << subtopic << RESET;

                        // Find and display edge information
                        int topicIndex = g.node_ind[topic];
                        for (int j = 0; j < g.no_of_nodes; ++j) {
                            if (g.node_name[j] == subtopic) {
                                cout << " (Time: " << GREEN << g.adjMat[topicIndex][j].second << RESET;
                                cout << ", Difficulty: " << GREEN << g.adjMat[topicIndex][j].first << RESET << ")\n";
                                break;
                            }
                        }
                    }
                }
            } else {
                // This is a subtopic
                cout << ORANGE << "→ " << topic << " (Sub Topic)" << RESET << "\n";
            }
        }
        cout << "\n";
    }

    // ==== Helper to Validate and Correct Topic ====

// Graph& findGraphForNode(const string &node, map<string, Graph> &uni_map) {
//         for (auto &[gname, graph] : uni_map)
//             if (graph.node_ind.count(node))
//                 return graph;
//         throw runtime_error("Node not found in any graph: " + node);
//     }


//     map<string, int>calculateDistancesToGoal(
//         const string& goal,
//         map<string, vector<tuple<string, int, int>>>& indegreeNodes,
//         int alpha, int beta
//     ) {
//         map<string, int> distances;
//         priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

//         for (const auto& [graph_name, g] : uni_map) {
//             for (auto& name : g.node_name) {
//                 distances[name.second] = 1e9;
//             }
//         }

//         distances[goal] = 0;
//         pq.push({0, goal});

//         while (!pq.empty()) {
//             auto [currDist, current] = pq.top(); pq.pop();

//             if (currDist > distances[current]) continue;

//             cout << "[HEURISTIC] Exploring: " << current << " (dist=" << currDist << ")\n";

//             for (auto& [from, diff, time] : indegreeNodes[current]) {
//                 int steps = 1;
//                 int weight = alpha * steps + beta * diff;

//                 int newDist = currDist + weight;
//                 if (newDist < distances[from]) {
//                     distances[from] = newDist;
//                     pq.push({newDist, from});
//                     cout << "[HEURISTIC] Updated: " << from << " --> " << newDist << "\n";
//                 }
//             }
//         }

//         return distances;
//     }


    // You must write this helper if not already present
// string getGraphNameOfNode(const string& node) {
//     for (auto& [graph_name, g] : uni_map) {
//         if (g.node_ind.count(node)) return graph_name;
//     }
//     return "";  // Not found
// }



// // Heuristic function to estimate the distance from the current node to the goal
// int heuristic(string curNode, string endNode, Graph& g) {
//     // A simple heuristic: for now, we return the remaining time or distance.
//     // In practice, you might use a more advanced heuristic depending on your data.
//     return 0; // You can replace this with a meaningful heuristic.
// }

// vector<string> findPathWithAStar(string startNode, string endNode, int timeConstraint, unordered_set<string>& present_skillset, double alpha, double beta) {
//     string gname;
//     int nodeVal;

//     // Find the graph and node index for startNode
//     for (auto& [graphName, graph] : uni_map) {
//         if (graph.node_ind.count(startNode)) {
//             gname = graphName;
//             nodeVal = graph.node_ind[startNode];
//             break;
//         }
//     }

//     priority_queue<PQNode> pq;
//     Graph g = uni_map[gname];
//     int startImp = g.outDegree[startNode].first;

//     pq.push({gname, nodeVal, {startNode}, startImp, 0, {}, alpha * startImp - beta * 0});

//     set<string> visitedGlobally;

//     while (!pq.empty()) {
//         PQNode cur = pq.top(); pq.pop();
//         Graph g = uni_map[cur.gname];
//         string curNode = g.node_name[cur.nodeVal];

//         if (cur.totTime > timeConstraint) continue;
//         if (visitedGlobally.count(curNode)) continue;

//         visitedGlobally.insert(curNode);

//         // If we reached the endNode, return the path
//         if (curNode == endNode) {
//             return cur.path;
//         }

//         for (int i = 0; i < g.adjMat.size(); ++i) {
//             if (g.adjMat[cur.nodeVal][i].second == 0) continue;
//             string neighbor = g.node_name[i];

//             if (cur.vis.count(neighbor)) continue;

//             int edgeTime = g.adjMat[cur.nodeVal][i].first;
//             int newImp = cur.totImportance;
//             int newTime = cur.totTime;

//             if (!present_skillset.count(neighbor)) {
//                 newTime += edgeTime;
//                 newImp += g.outDegree[neighbor].first;
//             }

//             if (newTime > timeConstraint) continue;

//             vector<string> newPath = cur.path;
//             map<string, bool> newVis = cur.vis;
//             newPath.push_back(neighbor);
//             newVis[neighbor] = true;

//             double newPriority = alpha * newImp - beta * newTime + heuristic(neighbor, endNode, g);
//             pq.push({g.name_of_graph, i, newPath, newImp, newTime, newVis, newPriority});
//         }
//     }

//     return {}; // No path found within constraints
// }





        // ==== Helper to Get Present Skillset ====

        // ==== Display Options Menu ==== %%%%%%
        int displayOptions()
        {
            cout << "\n"
                << YELLOW << "===== Learning Path Options =====\n"
                << RESET;
            cout << BLUE << "1. Learning a topic from Scratch from Advanced!" << endl;
            cout << BLUE << "2. Get to know the maximum topics you can cover from your present skillset given the time you can devote!" << endl;
            cout << "3. View Topic Completion Status" << endl;
            cout << "4. View Connected Topics" << endl;
            cout << "5. View shortest path from current to aimed skillset" << endl;
            cout << " 6. Get to know the maximum topics you can cover from a given specific skill given the time you can invest!" << endl;
            cout << "7. Exit\n\n"
                << RESET;

            cout << "Enter your choice (1-5): ";
            int choice;
            cin >> choice;
            return choice;
        }

        // Helper Functions
        void setColor(string color)
        {
            cout << color;
        }

        void resetText()
        {
            cout << RESET;
        }

        void clearScreen()
        {
            cout << "\033[2J";
        }

        void setTextStyle(bool bold, bool underline, bool italic)
        {
            if (bold)
                cout << "\033[1m";
            if (underline)
                cout << "\033[4m";
        }

        void moveTo(int x, int y)
        {
            cout << "\033[" << y << ";" << x << "H";
        }

        void centerText(string &text, int width = 100, int height = 24)
        {
            int x = (width - text.length()) / 2;
            int y = height / 2;
            moveTo(x, y);
            cout << text;
        }

        // ==== Display Results ==== %%%%%%
        void displayResults(vector<string> &path, string &title)
        {
            cout << "\n"
                << YELLOW << "===== " << title << " =====\n"
                << RESET;
            displayPathWithMainAndSubTopics(path);
            cout << "\nPress any key to continue...";
            _getch();
        }

        // ==== Main Display Integration ====
        void displayMain()
        {
        }

    // ==== Find Directly Connected Topics ====
    vector<string> findConnectedTopics(const string& topic) {
        vector<string> connectedTopics;
        set<string> visited;
        queue<string> q;

        if (uni_map.find(topic) == uni_map.end()) {
            cout << YELLOW << "\nTopic not found in the learning graph.\n" << RESET;
            return connectedTopics;
        }

        q.push(topic);
        visited.insert(topic);

        while (!q.empty()) {
            string current = q.front();
            q.pop();

            // Intra-graph traversal
            if (auto it = uni_map.find(current); it != uni_map.end()) {
                Graph& g = it->second;
                for (const auto& [_, data] : g.outDegree) {
                    const auto& neighbors = data.second;
                    for (const string& neighbor : neighbors) {
                        if (visited.insert(neighbor).second) {
                            connectedTopics.push_back(neighbor);
                            q.push(neighbor);
                        }
                    }
                }
            }

            // Inter-graph traversal
            for (auto [graphName, otherGraph]:uni_map) {
                if (graphName == current) continue;

                auto it = otherGraph.node_ind.find(current);
                if (it != otherGraph.node_ind.end()) {
                    int currentIndex = it->second;
                    for (int i = 0; i < otherGraph.no_of_nodes; ++i) {
                        if (otherGraph.adjMat[currentIndex][i].first > 0 ||
                            otherGraph.adjMat[currentIndex][i].second > 0) {
                            string neighbor = otherGraph.node_name[i];
                            if (visited.insert(neighbor).second) {
                                connectedTopics.push_back(neighbor);
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }

        return connectedTopics;
    }



    void displayConnectedTopics(const vector<string>& topics) {
        cout << "\n" << YELLOW << "===== Connected Topics =====\n" << RESET;

        if (topics.empty()) {
            cout << "No connected topics found.\n";
            return;
        }

        cout << CYAN << "Connected topics:\n" << RESET;
        for (size_t i = 0; i < topics.size(); ++i) {
            cout << "  " << (i + 1) << ". " << ORANGE << topics[i] << RESET;

            // Get the graph for this connected topic
            if (uni_map.find(topics[i]) != uni_map.end()) {
                cout << " (Main Topic)";
            } else {
                cout << " (Sub Topic)";
            }

            // Display edge information if available
            for (const auto& pair : uni_map) {
                const Graph& g = pair.second;
                if (g.node_ind.find(topics[i]) != g.node_ind.end()) {
                    int topicIndex = g.node_ind.at(topics[i]);
                    for (int j = 0; j < g.no_of_nodes; ++j) {
                        if (g.adjMat[topicIndex][j].first > 0 || g.adjMat[topicIndex][j].second > 0) {
                            cout << " (Time: " << GREEN << g.adjMat[topicIndex][j].second << RESET;
                            cout << ", Difficulty: " << GREEN << g.adjMat[topicIndex][j].first << RESET << ")";
                            break;
                        }
                    }
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }
    };

    void displayKnapsackRecommendation( vector<string>& recommendedTopics) {
        cout << "\nRecommended Topics to Study (Based on Knapsack Optimization):\n";
        if (recommendedTopics.empty()) {
            cout << "No topics could be recommended within the time limit.\n";
            return;
        }

        int idx = 0;
        for ( string& topic : recommendedTopics) {
            cout << " " << idx++ << ". " << topic << endl;
        }
    }

    map<string, int> getPresentSkillset(Trie& trie, vector<string>& allTopics) {
        map<string, int> present_skillset;

        cout << "\nDo you have any previous skills? (y/n): ";
        char hasSkills;
        cin >> hasSkills;

        if(hasSkills == 'y' || hasSkills == 'Y') {
            // cout << "\nEnter your skills (one per line, empty line to finish):\n";
            // string skill;
            // cin.ignore();  // Clear newline

            // while(getline(cin, skill) && !skill.empty()) {
                vector<string> correctedSkill = getCorrectedTopics(trie,allTopics);
                for (const string& word : correctedSkill) {
                    present_skillset[word]=0;
                }

            if(!present_skillset.empty()) {
                cout << "\n" << CYAN << "Your current skillset includes:\n" << RESET;
                for( auto& skill : present_skillset) {
                    cout << "• " << skill.first << "\n";
                }
            }
        }

        return present_skillset;
    }
    string validateAndCorrectTopic(string& input, Trie& trie, vector<string>& allTopics) {
        // Use smartSearch to validate and correct the topic
        vector<string> corrected = getCorrectedTopics(trie, allTopics);
        if(corrected.empty()) {
            cout << "\nNo matching topics found.\n";
            cout << "Press any key to continue...";
            _getch();
            return "";  // Return empty string to indicate no valid topic
        }

        return corrected[0];  // Return the corrected topic
    }

    vector<string> getAllUniqueTopics(RoadRunner& obj)
{
    unordered_set<string> uniqueTopics;
    for(auto& [a,b]:obj.uni_map)
            {uniqueTopics.insert(a);
            cout<<a<<endl;
            }
    return vector<string>(uniqueTopics.begin(), uniqueTopics.end());
}
    int main()
{

    system("chcp 65001 > nul");
    Trie trie;
    RoadRunner roadrunner;
    roadrunner.createGraphsUsingFile("Editedtxt.txt");
    cout<<"graphs created"<<endl;

    vector<string> allTopics = getAllUniqueTopics(roadrunner);
    map<string, int> present_skillmap;
    unordered_set<string> present_skillset;
    string startTopic, endTopic;
    int timeConstraint= 0;
    int timeConstraintInUnits= 0;

    for(auto& graph:roadrunner.uni_map)
    {
        graph.second.outdegree();
        graph.second.getTotalImportance();
        graph.second.getTotalTime();
    }

    roadrunner.computeIndegree();
    roadrunner.createGraphConnections();
    roadrunner.computeAdjustedImportance();

    int n;
    while (true) {
        int choice = roadrunner.displayOptions();
        map<string, int> present_skillset=getPresentSkillset(trie, allTopics);
        string topic, str, start, endN;
        int maxTime, alpha, beta, ch;
        vector<string> completePath;

        switch (choice) {
            case 1: {
                cout << "\nEnter the topic you want to study: "<<endl;
                cin >> topic;
                vector<string> completePath = roadrunner.completionOfATopic(topic);
                cout<<"Complete Learning Path for " + topic<<endl;
                roadrunner.BasicDisplayOfRoadmap(completePath, present_skillset);
                break;
            }
            case 2: {
              //  int maxTime;
                cout << "\nEnter maximum time available (in hours): "<<endl;
                cin >> maxTime;
                completePath = roadrunner.KnapsackRecommendation(maxTime, present_skillset);
                str="Maximum topics possible to be covered in "+maxTime;
                roadrunner.displayResults(completePath, str);
                break;
            }
            case 3: {
                // cout << "\nEnter the Goal\n";
                // cin>>endN;
                // cout << "\nEnter the Start\n";
                // cin>>start;
                // map<string, int> present_skillset=getPresentSkillset(trie, allTopics);
                // completePath=roadrunner.findPathWithAStar(start,endN,timeConstraint,alpha = 2,beta = 1);
                break;
            }
            case 4: {
               // string topic;
                cout << "\nEnter the topic to find connected topics: ";
                cin >> topic;
               vector<string> connected = roadrunner.findConnectedTopics(topic);
               str="Maximum topics possible to be covered in "+maxTime;
              roadrunner.displayResults(connected, str);
                break;
            }
            case 5:
               // string start, endN;
              //  int alpha, beta, ch;
                cout<<"Enter the topic you'd prefer your learning journey to start with"<<endl;
                cin>>start;
                cout<<"Enter the skill you're aiming for"<<endl;
                cin>>endN;
                cout<<"Wait a minute!!! We have an extra customization to offer! Choose what you'd like to prefer: "<<endl;
                cout<<"Enter 1 if you want to prioritize easier learning and 2 if you want to prioritize lesser time and any number if a balanced is what you'd prefer!"<<endl;
                cin>>ch;
                if(ch==1)
                {
                    alpha=2;
                    beta=4;
                }
                else if(ch==2)
                {
                    alpha=4;
                    beta=2;
                }
                else
                {
                    alpha=1;
                    beta=1;
                }
                completePath = roadrunner.findMostOptimalPath(start, endN, alpha, beta, present_skillset);
                cout<<"Most optimal path according to given constraints is: "<<endl;
                roadrunner.BasicDisplayOfRoadmap(completePath, present_skillset);
                break;
                case 6:
                   // string st;
                    //int maxTime;
                    cout<<"Enter the skill you'd like to begin with: "<<endl;
                    cin>>topic;
                    cout<<"Enter the maximum time you can invest:"<<endl;
                    cin>>maxTime;
                 //   vector<string> priorityPath = topicsPossibleInTime(st, maxTime, present_skillset);
                    str="Maximum topics possible to be covered in "+maxTime;
                    roadrunner.displayResults(completePath, str);
                break;
            case 7:
                cout << "\nThank you for using ROADRUNNER!\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
                _getch();
        }
    }
};
