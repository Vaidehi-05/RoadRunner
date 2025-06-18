#include <bits/stdc++.h>
#include <conio.h>
#include "abbreviation.h"
using namespace std;

// ANSI Color Codes
string RESET = "\033[0m";
const string ORANGE = "\033[38;5;208m"; // For subtopics
const string CYAN = "\033[36m";         // For main topics
const string GREEN = "\033[32m";        // For time/difficulty
const string YELLOW = "\033[33m";       // For headers
const string BLUE = "\033[34m";         // For menu options
struct TrieNode
{
    bool isEnd;
    int freq; // increase frequency of search // not used///????
    unordered_map<char, TrieNode *> child;

    TrieNode() : isEnd(false) {}
};

class Trie
{
    vector<string> correctedWords;
    TrieNode *root;

public:
    Trie() { root = new TrieNode(); }

    void remains(TrieNode *node, string &word, vector<string> &res) // helper function to find all words that can be made using given prefix
    {
        if (node->isEnd)
        {
            // cout << "[DEBUG] Suggestion found: " << word << endl;
            res.push_back(word);
        }

        for (auto [c, next] : node->child)
        {
            word.push_back(c);
            remains(next, word, res);
            word.pop_back();
        }
    }

    void insert(string &word)
    {
        TrieNode *node = root;
        for (char c : word)
        {
            if (node->child[c])
                node = node->child[c];
            else
                node = node->child[c] = new TrieNode();
        }
        node->isEnd = true;
        // node->freq++;
        // cout << "[DEBUG] Inserted into Trie:-" << word << endl;
        correctedWords.push_back(word);
    }

    vector<string> getSuggestions(string &word)
    {
        TrieNode *node = root;
        for (char c : word)
        {
            if (node->child.find(c) == node->child.end())
                return {};
            node = node->child[c];
        }

        vector<string> res;
        string temp = word;
        remains(node, temp, res);

        // Lexicographical sorting
        sort(res.begin(), res.end());

        vector<string> result;
        for (int i = 0; i < min(3, (int)res.size()); i++)
            result.push_back(res[i]);

        return result;
    }

    bool search(string &word)
    {
        TrieNode *node = root;
        for (char c : word)
        {
            if (node->child.find(c) == node->child.end())
                return false;
            node = node->child[c];
        }
        return node->isEnd;
    }
};

string normalize(string word)   //normalizing string
{
    transform(word.begin(), word.end(), word.begin(), ::tolower); // to lower

    string specialChars = "-_/.,!&#";
    for (char &c : word)
        if (specialChars.find(c) != string::npos)
            c = ' ';

    // Removing extra spaces
    int start = 0, end = word.size() - 1;
    string temp = "";
    while (start <= end && word[start] == ' ')
        start++;
    while (start <= end && word[end] == ' ')
        end--;

    for (int i = start; i <= end; i++)
        temp += word[i];

    word = temp;
    temp.clear();

    for (int i = 0; i < word.size(); i++)
    {
        if (!(i + 1 < word.size() && word[i] == ' ' && word[i + 1] == ' '))
            temp += word[i];
    }

    // cout << "[DEBUG] Normalized: " << word << endl;

    // Abbreviation Decoder
    if (abbreviations.find(word) != abbreviations.end())
        word = abbreviations[word];

    return word;
}

int editDistance(string a, string b) // Levishtein Distance(number of operations from Word1 to Word2)
{
    int m = a.size(), n = b.size();

    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j;

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (tolower(a[i - 1]) == tolower(b[j - 1]))
                dp[i][j] = 0 + dp[i - 1][j - 1]; // char matches
            else
                dp[i][j] = 1 + min(dp[i - 1][j], min(dp[i][j - 1], dp[i - 1][j - 1])); //(Insertion, Deletion, Replacement)
    return dp[m][n];
}

vector<pair<string, int>> topMatches(string userInput, vector<string> topics) // stores top matches of a word along w/ their score!
{
    int s;
    vector<pair<string, int>> score;
    for (string a : topics)
    {
        s = editDistance(userInput, a);
        if (!a.empty())
            score.push_back({a, s});
    }
    sort(score.begin(), score.end(), [](pair<string, int> &a, pair<string, int> &b)
         { return a.second < b.second; });

    vector<pair<string, int>> top;
    for (int i = 0; i < min(3, (int)score.size()); i++)
        top.push_back(score[i]);
    return top;
}

vector<string> smartSearch(vector<string> topicsInput, vector<string> topics, Trie &trie)
{
    vector<string> results;
    for (string w : topicsInput)
    {
        string corrected = normalize(w);
        if (trie.search(corrected))
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

void buildTrie(Trie &trie, vector<string> &topics)
{
    for (auto a : topics)
    {
        string word = normalize(a);
        trie.insert(word);
    }
}

vector<string> getCorrectedTopics(Trie &trie, vector<string> &allTopics)
{
    vector<string> inputs;
    string topic;

    cout << "Enter topics you already know(type 'done' to finish):\n";
    while (cin >> topic && topic != "done")
        inputs.push_back(topic);

    vector<string> corrected = smartSearch(inputs, allTopics, trie);
    // unordered_set<string> skillset(corrected.begin(), corrected.end());
    return corrected;
}

struct KnapItem
{
    string name;
    int totTime;
    int totImp;
};

struct StackNode // USED FOR DFS
{
    string gname;
    int nodeVal;
    vector<string> path;
    int totImportance;
    int totTime;
    map<string, bool> vis;
    // int diff; //variable constraints!
};

class Graph
{
public:
    string name_of_graph;
    int no_of_nodes;
    vector<vector<pair<int, int>>> adjMat; // Creating a matrix so that each node is accessible directly
                                           // no need to traverse the entire list of connections if we know which node we want to connect with

    map<int, string> node_name;                       // stores each node index's name
    map<string, int> node_ind;                        // stores each node num mapped to its name
    map<string, pair<int, vector<string>>> outDegree; // strore outdegrees and all the reachable neighbours
    int totalImportance;                              // stores sum of importance of all subnodes --deep
    int totalTime;                                    // stores total time of only direct connections --only main

    Graph()
    {
        totalTime = 0;
        totalImportance = 0;
        no_of_nodes = 0;
        name_of_graph = "";
        // adjMat.resize(n,vector<pair<int,int>>(n,{0,0}));  //storing pair as: edge weight, edge time taken
    }

    Graph(int n)
    {
        totalTime = 0;
        totalImportance = 0;
        no_of_nodes = n;
        name_of_graph = "";
        adjMat.resize(n, vector<pair<int, int>>(n, {0, 0})); // storing pair as: edge weight, edge time taken
    }

    void addEdge(int s, int e, int wt, int time)
    {
        if (s < no_of_nodes && e < no_of_nodes && s != e)
        {
            adjMat[s][e].first = wt;
            adjMat[s][e].second = time;
        }
    }

    void outdegree() // gives n_o_n reachable, and their names;
    {
        int m = 0;
        int n = no_of_nodes;
        vector<string> reachables;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                if (adjMat[i][j].first != 0 || adjMat[i][j].second != 0)
                {
                    reachables.push_back(node_name[j]);
                    m++;
                }
            outDegree[node_name[i]] = {m, reachables};
            m = 0;
            reachables.clear();
        }
    }

    void getTotalTime()
    {
        for (int i = 0; i < no_of_nodes; i++)
            for (int j = 0; j < no_of_nodes; j++)
                totalTime += adjMat[i][j].second;
    }

    void getTotalImportance()
    {
        for (auto i : outDegree)
            totalImportance += i.second.first;
    }
};

class RoadRunner
{
public:
    vector<Graph> topic;
    map<string, Graph> uni_map; // storing each Graph mapped to its name
    map<string, int> indegree;
    map<string, int> subtopic_adjustedImportance;
    map<string, set<string>> graphConnections;

    void computeIndegree()
    {
        for (auto &graph : uni_map) // Iterate over each graph in the uni_map
            for (auto &outD : graph.second.outDegree)
            {
                if (indegree.count(outD.first) == 0)
                    indegree[outD.first] = 0;

                // Iterate over outgoing edges
                // For each outgoing connection, increase the indegree of the target nodes
                for (auto &neighbour : outD.second.second)
                {
                    if (indegree.count(neighbour) == 0)
                        indegree[neighbour] = 0;
                    indegree[neighbour]++; // Increase the indegree for the neighbour node
                                           // cout<<" "<<neighbour<<":"<<indegree[neighbour]<<endl;
                }
            }
    }

    void createGraphConnections()
    {
        for (auto &graph : uni_map)                   // iterate through all graphs
            for (auto &outD : graph.second.outDegree) // for each node and its outgoing edges
                for (string &neighbour : outD.second.second)
                    // Check if this neighbour node exists in any other graph
                    for (auto &otherGraph : uni_map)
                    {
                        if (graph.first == otherGraph.first)
                            continue;
                        if (otherGraph.second.node_ind.count(neighbour) > 0) // Found a connection in another graph
                        {
                            graphConnections[graph.first].insert(otherGraph.first);
                            // cout << "  Found connection: " << graph.first
                            //     << " -> " << otherGraph.first
                            //    << " via node " << neighbour << endl;
                        }
                    }
    }

    void computeAdjustedImportance()
    {
        for (auto graph : uni_map)
        {
            int imp = graph.second.totalImportance;
            for (auto subGraph : graphConnections[graph.first])
                imp += uni_map[subGraph].totalImportance;

            subtopic_adjustedImportance[graph.first] = imp;
            //  cout<<subtopic_adjustedImportance[graph.first]<<endl;
        }
    }

    string trim(string &s)
    {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        return (start == string::npos) ? "" : s.substr(start, end - start + 1);
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
        if (!file.is_open())
        {
            cout << "Error! Could not open file. " << endl;
            return;
        }

        int no_of_nodes, edgeCount, n = 0;
        string node_name, line, nm;
        vector<vector<int>> edgeDetails; // stores all edges in format: starting node, ending node, edge-weight, time needed
        map<string, int> mp;             // storing node value of each node
        map<int, string> rev_mp;         // storing name of each node
        while (getline(file, line))
        {
            if (line.rfind("Num_of_Nodes: ", 0) == 0)
            {
                no_of_nodes = stoi(line.substr(14));
                edgeCount = -1;
                n = 0;
            }
            else if (line.rfind("Name: ", 0) == 0)
            {
                string str = line.substr(6);
                nm = trim(str);
                if (mp.find(nm) == mp.end())
                {
                    //                    int id = mp.size();
                    mp[nm] = 0;
                    rev_mp[0] = nm;
                }
            }
            else if (line.rfind("ConnectedNodes: ", 0) == 0)
            {
                //   cout<<"ConnectingNodes:"<<endl;
                string str = line.substr(16);
                string nodes = trim(str), wrd = "";
                int i = 0, len = nodes.length();
                while (i < len)
                {

                    if (nodes[i] != ' ')
                        wrd += nodes[i];
                    else
                    {
                        wrd = trim(wrd);
                        mp[wrd] = ++n;
                        rev_mp[n] = wrd;
                        wrd = "";
                    }
                    i += 1;
                }
                wrd = trim(wrd);
                mp[wrd] = ++n;
                rev_mp[n] = wrd;
            }

            else if (line.rfind("EdgeStart: ", 0) == 0)
            {
                edgeCount += 1;
                string str = line.substr(11);
                string start = trim(str);

                vector<int> a;
                a.push_back(mp[start]);
                edgeDetails.push_back(a);
            }
            else if (line.rfind("EdgeEnd: ", 0) == 0)
            {
                string str = line.substr(9);
                string end = trim(str);
                edgeDetails[edgeCount].push_back(mp[end]);
            }
            else if (line.rfind("EdgeWeight: ", 0) == 0)
            {
                int weight = stoi(line.substr(12));
                edgeDetails[edgeCount].push_back(weight);
            }
            else if (line.rfind("EdgeTimeNeeded: ", 0) == 0)
            {
                int time = stoi(line.substr(16));
                edgeDetails[edgeCount].push_back(time);
            }
            else if (line.rfind("*", 0) == 0)
            {
                no_of_nodes = mp.size();
                Graph obj(no_of_nodes);
                // cout<<no_of_nodes<<endl;
                obj.node_name = rev_mp;
                obj.node_ind = mp;
                // print(mp, rev_mp);
                obj.name_of_graph = nm;
                for (int i = 0; i < edgeCount + 1; i++)
                {
                    obj.addEdge(edgeDetails[i][0], edgeDetails[i][1], edgeDetails[i][2], edgeDetails[i][3]);
                }
                uni_map[nm] = obj;
                topic.push_back(obj);
                edgeDetails.clear();
                mp.clear();
                rev_mp.clear();
            }
        }
        file.close();
        return;
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

    void printSubgraph(string node,
                       map<string, vector<pair<string, pair<int, int>>>> &branches,
                       int timeConstraint,
                       string indent = "", bool isLast = true,
                       pair<int, int> edgeInfo = {-1, -1},
                       unordered_set<string> &visited = *(new unordered_set<string>()),
                       int timeSpent = 0)
    {
        // If the current time spent exceeds the constraint, stop
        if (timeSpent > timeConstraint)
        {
            return;
        }

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
            printSubgraph(child, branches, timeConstraint, indent, childIsLast, weight, visited, newTimeSpent);
        }
    }

    void topicsPossibleInTime(string startNode, int timeConstraint, map<string, int> &present_skillset)
    {

        int nodeVal;
        string gname;
        // can create a function
        for (auto [graphName, graph] : uni_map) // finding the startNode's(GraphName & NodeIndex) by traverse the whole Graph
        {
            if (graph.node_ind.find(startNode) != graph.node_ind.end())
            {
                gname = graphName;
                nodeVal = graph.node_ind[startNode];
                break;
            }
        }

        stack<StackNode> st; //{GraphName, NodeIndex, PathTraversed, totImp, totTime, NodesVisited}

        set<string> reachableNodes;                               // stores all nodes reachable from startNode(under Constraint)
        map<pair<string, string>, pair<int, int>> reachableEdges; //<<node, node's_neighbour><Time, Difficulty>>
        // int maxImportance=0;
        int totalTimeUsed = 0;
        set<pair<string, string>> usedEdges;

        Graph a = uni_map[gname];
        st.push({gname, nodeVal, {startNode}, a.outDegree[a.node_name[nodeVal]].first, 0});
        // map<string, bool> vis;
        vector<vector<string>> allPaths;
        while (!st.empty())
        {
            int c = 0;
            StackNode curStack = st.top();
            Graph a = uni_map[curStack.gname];
            int b = curStack.nodeVal;
            string curNode = a.node_name[b];
            int edgeTime, edgeDiff;

            st.pop();

            reachableNodes.insert(curNode);
            for (string &node : curStack.path)
                reachableNodes.insert(node);

            if (curStack.totTime > timeConstraint)
                continue; // skip if exceedign totalTime

            //        if(curStack.vis[curNode]) continue;
            //        curStack.vis[curNode]=true;

            allPaths.push_back(curStack.path);

            for (int i = 0; i < a.adjMat.size(); i++) // Check if it is an endNode(leading to another mainTopic)
                if (a.adjMat[b][i].second != 0)
                    c++;

            if (c == 0 && uni_map.find(curNode) != uni_map.end()) // if yes, find in uni_map, insert its children in stack(update the newTotTime, newTotImp beforehand, taking consideration present_skillset), reachableNodes, reachableEdges;
            {
                Graph a_new = uni_map[curNode];
                b = a_new.node_ind[curNode];
                for (int i = 0; i < a_new.adjMat.size(); i++)
                    if (a_new.adjMat[b][i].second != 0 && !curStack.vis[a_new.node_name[i]]) // if(edge exists & notVisited)
                    {
                        int edgeTime = curStack.totTime;
                        int importance = curStack.totImportance;

                        bool isCompleted = present_skillset.count(a_new.node_name[i]); // update curStackNode.Time&Imp if userNotCompletedTopic
                        if (!isCompleted)
                        {
                            edgeTime += a_new.adjMat[b][i].first;
                            importance += a_new.outDegree[a_new.node_name[i]].first;
                        }
                        pair<string, string> edgeKey = {curNode, a_new.node_name[i]};
                        int edgeTimeToAdd = a_new.adjMat[b][i].first;

                        if (usedEdges.find(edgeKey) == usedEdges.end() && curStack.totTime + edgeTimeToAdd <= timeConstraint)
                        {
                            curStack.totTime += edgeTimeToAdd;
                            usedEdges.insert(edgeKey);
                            vector<string> newPath = curStack.path;
                            map<string, bool> newVis = curStack.vis;
                            newPath.push_back(a_new.node_name[i]);
                            newVis[a_new.node_name[i]] = true;

                            st.push({a_new.name_of_graph, i, newPath, importance, edgeTime, newVis});

                            reachableNodes.insert(curNode);
                            reachableNodes.insert(a_new.node_name[i]);
                            reachableEdges[{curNode, a_new.node_name[i]}] = a_new.adjMat[b][i];
                        }
                    }
                continue;
            }

            for (int i = 0; i < a.adjMat.size(); i++)
            {
                if (a.adjMat[b][i].second != 0 && !curStack.vis[a.node_name[i]]) // if(edge exists & notVisited)
                {
                    {
                        int edgeTime = curStack.totTime;
                        int importance = curStack.totImportance;

                        bool isCompleted = present_skillset.count(a.node_name[i]); // update curStackNode.Time&Imp if userNotCompletedTopic
                        if (!isCompleted)
                        {
                            edgeTime += a.adjMat[b][i].first;
                            importance += a.outDegree[a.node_name[i]].first;
                        }
                        pair<string, string> edgeKey = {curNode, a.node_name[i]};
                        int edgeTimeToAdd = a.adjMat[b][i].first;

                        if (usedEdges.find(edgeKey) == usedEdges.end() && curStack.totTime + edgeTimeToAdd <= timeConstraint)
                        {
                            curStack.totTime += edgeTimeToAdd;
                            usedEdges.insert(edgeKey);
                            vector<string> newPath = curStack.path;
                            map<string, bool> newVis = curStack.vis;
                            newPath.push_back(a.node_name[i]);
                            newVis[a.node_name[i]] = true;
                            st.push({a.name_of_graph, i, newPath, importance, edgeTime, newVis});

                            reachableNodes.insert(curNode);
                            reachableNodes.insert(a.node_name[i]);
                            reachableEdges[{curNode, a.node_name[i]}] = a.adjMat[b][i];
                        }
                    }
                }
            }
        }

        map<string, vector<pair<string, pair<int, int>>>> branches; // displayFormat
        for (auto e : reachableEdges)
        {
            string a = e.first.first;
            string b = e.first.second;
            branches[a].push_back({b, e.second});
        }

        cout << "Visual Tree:" << endl;
        printSubgraph(startNode, branches, timeConstraint);

        // vector<string> topics; // use reachableNodes instead;
        // for(auto a: vis)
        //     topics.push_back(a.first);
    }

    vector<string> KnapsackRecommendation(int maxTime, map<string, int> &present_skillset) // need to exclude the already done topics
    {
        unordered_set<string> neighb;
        for (auto &[a, nonono] : present_skillset)
        {
            if (uni_map.find(a) != uni_map.end())
            {
                Graph &b = uni_map[a];
                for (auto &neighbour : b.outDegree)
                    for (string &reachable : neighbour.second.second)
                        if (present_skillset.find(reachable) == present_skillset.end())
                            neighb.insert(reachable);
            }
        }

        vector<KnapItem> topics;

        for (auto graph : uni_map)
        {
            if (present_skillset.find(graph.first) != present_skillset.end())
                continue;
            int imp = subtopic_adjustedImportance[graph.first];
            if (neighb.count(graph.first)) // prioritizing the neighbours;
                imp += 10;

            if (indegree.find(graph.first) != indegree.end() && indegree[graph.first] == 0)
                imp += 5;
            topics.push_back({graph.first, graph.second.totalTime, imp});
            // cout << "Adding topic: " << graph.first << " with time " << graph.second.totalTime << " and imp " << imp << endl;
        }

        int n = topics.size();
        vector<vector<int>> dp(n + 1, vector<int>(maxTime + 1, 0));
        vector<vector<bool>> vis(n + 1, vector<bool>(maxTime + 1, false));

        for (int i = 1; i <= n; i++)
            for (int t = 0; t <= maxTime; t++)
            {
                if (t >= topics[i - 1].totTime)
                {
                    dp[i][t] = max(dp[i - 1][t], dp[i - 1][t - topics[i - 1].totTime] + topics[i - 1].totImp);
                    vis[i][t] = true;
                }
                else
                    dp[i][t] = dp[i - 1][t];
            }

        vector<string> listOfTopics;
        int w = maxTime;
        for (int i = n; i > 0; i--)
        {
            if (vis[i][w])
            {
                listOfTopics.push_back(topics[i - 1].name);
                w -= topics[i - 1].totTime;
            }
        }

        return listOfTopics;
    }

    static bool compForPQDjikastra(pair<pair<Graph, string>, int> a, pair<pair<Graph, string>, int> b)
    {
        return a.second > b.second;
    }
    vector<string> findMostOptimalPath(string start, string end, int alpha_val, int beta_val, map<string, int> &present_skillset)
    {
        map<string, int> vis, shortest;                                                                                                                                                          // map storing shortet distance of all uptil now visited
        priority_queue<pair<pair<Graph, string>, int>, vector<pair<pair<Graph, string>, int>>, bool (*)(pair<pair<Graph, string>, int>, pair<pair<Graph, string>, int>)> pq(compForPQDjikastra); // pq storing all nodes with their minimum distances
        vector<string> shortest_path;                                                                                                                                                            // storing final path from start to end node
        int alpha = alpha_val, beta = beta_val;                                                                                                                                                  // constants that decide the weightage given to weight(level) and time respectively
        Graph start_node, end_node;

        vis[start] = 1;
        start_node = uni_map[start];
        end_node = uni_map[end];
        shortest[start] = 0;
        shortest_path.push_back(start);
        pq.push({{start_node, start}, 0});

        while (!pq.empty())
        {
            Graph s = pq.top().first.first;
            int minDist = pq.top().second, curr = s.node_ind[pq.top().first.second];
            string st = pq.top().first.second;
            pq.pop();

            //  st=s.name_of_graph;
            if (!vis[st])
            {
                shortest_path.push_back(st);
                vis[st] = 1;
            }

            if (st == end)
            {
                if (vis.find(end) == vis.end())
                    shortest_path.push_back(end);
                return shortest_path;
            }

            // vector<vector<pair<int, int>>> adjMat=s.adjMat;

            for (int i = 0; i < s.no_of_nodes; i++) // traversing all the connections of the current node
            {
                if (s.adjMat[curr][i].first != 0 && vis.find(s.node_name[i]) == vis.end())
                {
                    int x = (alpha * s.adjMat[curr][i].first) + (beta * s.adjMat[curr][i].second);                       // custom comparator
                    if (((shortest.find(s.node_name[i]) == shortest.end()) || (minDist + x < shortest[s.node_name[i]]))) // create a present_skillset map to store current skillset of user
                    {
                        if (uni_map.find(s.node_name[i]) == uni_map.end())
                            pq.push({{s, s.node_name[i]}, x});
                        else
                            pq.push({{uni_map[s.node_name[i]], s.node_name[i]}, x});
                        shortest[s.node_name[i]] = x + minDist;
                    }
                }
            }
        }
        if (vis.find(end) == vis.end())
            shortest_path.push_back(end);
        return shortest_path;
    }

    static bool comp_for_pq_forTopicCompletion(pair<string, int> a, pair<string, int> b)
    {
        return a.second < b.second;
    }

    vector<string> completionOfATopic(string topicName) // CODE FOR COMPLETING A TOPIC FROM SCRATCH TO ADVANCED
    {
        Graph &topic = uni_map[topicName];
        map<string, bool> vis;
        stack<string> st;
        vector<string> completePath;
        priority_queue<pair<string, int>, vector<pair<string, int>>, bool (*)(pair<string, int>, pair<string, int>)> pq(comp_for_pq_forTopicCompletion);
        st.push(topicName);
        int num_of_nodes_added = 0;

        //  cout << "Starting topic completion from node: " << topicName << "\n";

        while (!st.empty())
        {
            string currNode = st.top();
            st.pop();
            //            cout << "\nPopped from stack: " << currNode << endl;

            if (topic.node_ind.find(currNode) == topic.node_ind.end())
                continue; // Skip if node is not present in the graph
                          //  cout << "Current Node: " << currNode << endl;

            // int curr = topic.node_ind[currNode];

            int curr = topic.node_ind[currNode];
            if (vis.find(currNode) == vis.end())
            {
                completePath.push_back(currNode);
                num_of_nodes_added += 1;
                vis[currNode] = true;
                //  cout << "Visited and added to path: " << currNode << "\n";
            }
            else
                continue;
            // st.pop();

            //  cout << "Looking at neighbors of: " << currNode << "\n";
            for (int i = 0; i < topic.no_of_nodes; i++)
            {
                if (topic.adjMat[curr][i].first != 0 && (vis.find(topic.node_name[i]) == vis.end()))
                {
                    pq.push({topic.node_name[i], topic.adjMat[curr][i].first});
                    //  cout << "    Neighbor added to PQ: " << topic.node_name[i]
                    //         << " with weight: " << topic.adjMat[curr][i].first << "\n";
                    // vis[topic.node_name[i]]=true;
                    // cout << "Pushed to stack from: " << topic.node_name[i] << " with weight " << topic.adjMat[curr][i].first << endl;
                }
            }

            while (!pq.empty()) // STORED TEMPORARILY IN THE PRIORITY QUEUE SO THAT NODES ARE ENTERED IN THE STACK ACC. TO THE ORDER
            {
                st.push(pq.top().first);
                pq.pop();
            }

            /*  cout << "Current completePath: ";
              for (auto &node : completePath)
                  cout << node << " ";
              cout << "\n---------------------------\n";  */

            /* if(num_of_nodes_added>=15)
             break;*/
        }
        return completePath;
    }

    void BasicDisplayOfRoadmap(vector<string> &vec, map<string, int> &present_skillset)
    {
        cout << "THE ROADMAP GENERATED ACCORDING TO THE GIVEN CONSTRAINTS IS AS FOLLOWS: " << endl;
        for (int i = 0; i < vec.size(); i++)
        {
            if (present_skillset.find(vec[i]) == present_skillset.end())
                cout << vec[i] << endl;
            else
                cout << vec[i] << "     --✔" << endl;
        }
    }

    // ==== Display Path with Main and Sub Topics ====
    void displayPathWithMainAndSubTopics(vector<string> &path)
    {
        if (path.empty())
        {
            cout << "No path found.\n";
            return;
        }

        for (size_t i = 0; i < path.size(); ++i)
        {
            string &topic = path[i];

            // Display main topic in cyan
            cout << CYAN << topic << RESET;

            // Check if this topic has subtopics in any graph
            bool hasSubtopic = false;
            for (auto &graph : uni_map)
            {
                if (graph.second.node_ind.count(topic))
                {
                    int topicIndex = graph.second.node_ind.at(topic);
                    for (int j = 0; j < graph.second.no_of_nodes; ++j)
                    {
                        if (graph.second.adjMat[topicIndex][j].first > 0 ||
                            graph.second.adjMat[topicIndex][j].second > 0)
                        {
                            hasSubtopic = true;
                            break;
                        }
                    }
                }
            }

            if (hasSubtopic)
            {
                cout << " (Main Topic)\n";
                // Display subtopics in orange
                for (auto &graph : uni_map)
                {
                    if (graph.second.node_ind.count(topic))
                    {
                        int topicIndex = graph.second.node_ind.at(topic);
                        for (int j = 0; j < graph.second.no_of_nodes; ++j)
                        {
                            if (graph.second.adjMat[topicIndex][j].first > 0 ||
                                graph.second.adjMat[topicIndex][j].second > 0)
                            {
                                cout << "└── " << ORANGE << graph.second.node_name.at(j) << RESET;
                                cout << " (Time: " << GREEN << graph.second.adjMat[topicIndex][j].first << RESET;
                                cout << ", Diff: " << GREEN << graph.second.adjMat[topicIndex][j].second << RESET << ")\n";
                            }
                        }
                    }
                }
            }
            else
            {
                cout << "\n";
            }
        }
    }

    // ==== Helper to Validate and Correct Topic ====

    // Calculate weighted distances to goal using Dijkstra
    map<string, int> calculateDistancesToGoal(string &goal, map<string, Graph> &uni_map,
                                              int alpha = 2, int beta = 1)
    {
        map<string, int> distances;
        priority_queue<pair<int, string>, vector<pair<int, string>>,
                       greater<pair<int, string>>>
            pq;

        // Initialize distances
        for (auto &[topic, _] : uni_map)
        {
            distances[topic] = INT_MAX;
        }
        distances[goal] = 0;
        pq.push({0, goal});

        while (!pq.empty())
        {
            string current = pq.top().second;
            int currDist = pq.top().first;
            pq.pop();

            if (currDist > distances[current])
                continue;

            Graph &g = uni_map.at(current);
            for (int i = 0; i < g.no_of_nodes; i++)
            {
                if (g.adjMat[g.node_ind.at(current)][i].first == 0)
                    continue;

                string neighbor = g.node_name[i];
                if (uni_map.find(neighbor) == uni_map.end())
                    continue;

                // Calculate weighted distance with priority to steps
                int steps = 1; // One step to neighbor
                int difficulty = g.adjMat[g.node_ind.at(current)][i].first;
                // Higher weight for steps (alpha) than difficulty (beta)
                int weight = (alpha * steps) + (beta * difficulty);

                int newDist = currDist + weight;

                if (newDist < distances[neighbor])
                {
                    distances[neighbor] = newDist;
                    pq.push({newDist, neighbor});
                }
            }
        }
        return distances;
    }

    // A* search using weighted heuristics
    vector<string> findPathWithHeuristics(string &start, string &end,
                                          map<string, Graph> &uni_map,
                                          map<string, int> &present_skillset,
                                          int alpha = 2, int beta = 1)
    { // Default values prioritize steps
        // Calculate weighted heuristic values
        map<string, int> heuristic = calculateDistancesToGoal(end, uni_map, alpha, beta);

        priority_queue<pair<int, string>, vector<pair<int, string>>,
                       greater<pair<int, string>>>
            pq;
        map<string, int> g_score;      // Cost from start to current
        map<string, int> f_score;      // Total estimated cost
        map<string, string> came_from; // Path reruction
        set<string> closed_set;        // Set of evaluated nodes

        // Initialize start node
        g_score[start] = 0;
        f_score[start] = heuristic[start];
        pq.push({f_score[start], start});

        while (!pq.empty())
        {
            string current = pq.top().second;
            pq.pop();

            if (current == end)
            {
                // Reconstruct path
                vector<string> path;
                while (current != start)
                {
                    path.push_back(current);
                    current = came_from[current];
                }
                path.push_back(start);
                reverse(path.begin(), path.end());
                return path;
            }

            if (closed_set.count(current))
                continue;
            closed_set.insert(current);

            Graph &g = uni_map.at(current);
            for (int i = 0; i < g.no_of_nodes; i++)
            {
                if (g.adjMat[g.node_ind.at(current)][i].first == 0)
                    continue;

                string neighbor = g.node_name[i];
                if (uni_map.find(neighbor) == uni_map.end())
                    continue;

                // Skip if neighbor is in closed set
                if (closed_set.count(neighbor))
                    continue;

                // Calculate tentative g_score
                int steps = 1; // One step to neighbor
                int difficulty = g.adjMat[g.node_ind.at(current)][i].first;
                int weight = (alpha * steps) + (beta * difficulty);

                // If the topic is in present_skillset, reduce the weight
                if (present_skillset.count(neighbor))
                {
                    weight = weight / 2; // Reduce weight for known topics
                }

                int tentative_g_score = g_score[current] + weight;

                if (g_score.find(neighbor) == g_score.end() ||
                    tentative_g_score < g_score[neighbor])
                {
                    came_from[neighbor] = current;
                    g_score[neighbor] = tentative_g_score;
                    f_score[neighbor] = g_score[neighbor] + heuristic[neighbor];
                    pq.push({f_score[neighbor], neighbor});
                }
            }
        }

        return vector<string>(); // No path found
    }
    // ==== Helper to Get Present Skillset ====

    // ==== Display Options Menu ==== %%%%%%
    int displayOptions()
    {
        cout << "\n"
             << ORANGE << "===== ROADRUNNER Menu =====\n"
             << RESET;
        cout << YELLOW << "1. Learning a topic from Scratch from Advanced!" << endl;
        cout << YELLOW << "2. Get to know the maximum topics you can cover from your present skillset given the time you can devote!" << endl;
        // cout << "3. View Connected Topics"<<endl;
        cout << "3. View shortest path from current to aimed skillset" << endl;
        cout << "4. Get to know the maximum topics you can cover from a given specific skill given the time you can invest!" << endl;
        cout << "5. Exit\n\n"
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
    vector<string> findConnectedTopics(string &topic, map<string, Graph> &uni_map)
    {
        vector<string> connectedTopics;
        set<string> visited;

        // Check if the topic exists in uni_map
        if (!uni_map.count(topic))
        {
            cout << "Topic not found in the learning graph.\n";
            return connectedTopics;
        }

        // Get the graph for the given topic
        Graph &g = uni_map.at(topic);

        // Find the index of the topic in the graph
        if (!g.node_ind.count(topic))
        {
            cout << "Topic not found in the graph structure.\n";
            return connectedTopics;
        }

        int topicIndex = g.node_ind.at(topic);

        // Find all directly connected topics
        for (int i = 0; i < g.no_of_nodes; ++i)
        {
            // Check if there's a direct connection (edge exists)
            if (g.adjMat[topicIndex][i].first > 0 || g.adjMat[topicIndex][i].second > 0)
            {
                string connectedTopic = g.node_name.at(i);

                // Add the connected topic if not already visited
                if (!visited.count(connectedTopic))
                {
                    connectedTopics.push_back(connectedTopic);
                    visited.insert(connectedTopic);
                }
            }
        }

        return connectedTopics;
    }
};

void displayKnapsackRecommendation(vector<string> &recommendedTopics)
{
    cout << "\nRecommended Topics to Study (Based on Knapsack Optimization):\n";
    if (recommendedTopics.empty())
    {
        cout << "No topics could be recommended within the time limit.\n";
        return;
    }

    int idx = 0;
    for (string &topic : recommendedTopics)
    {
        cout << " " << idx++ << ". " << topic << endl;
    }
}

map<string, int> getPresentSkillset(Trie &trie, vector<string> &allTopics)
{
    map<string, int> present_skillset;

    cout << "\nDo you have any previous skills? (y/n): ";
    char hasSkills;
    cin >> hasSkills;

    if (hasSkills == 'y' || hasSkills == 'Y')
    {
        // cout << "\nEnter your skills (one per line, empty line to finish):\n";
        // string skill;
        // cin.ignore();  // Clear newline

        // while(getline(cin, skill) && !skill.empty()) {
        vector<string> correctedSkill = getCorrectedTopics(trie, allTopics);
        for (const string &word : correctedSkill)
        {
            present_skillset[word] = 0;
        }

        if (!present_skillset.empty())
        {
            cout << "\n"
                 << CYAN << "Your current skillset includes:\n"
                 << RESET;
            for (auto &skill : present_skillset)
            {
                cout << "• " << skill.first << "\n";
            }
        }
    }

    return present_skillset;
}
string validateAndCorrectTopic(string &input, Trie &trie, vector<string> &allTopics)
{
    // Use smartSearch to validate and correct the topic
    vector<string> corrected = getCorrectedTopics(trie, allTopics);
    if (corrected.empty())
    {
        cout << "\nNo matching topics found.\n";
        cout << "Press any key to continue...";
        _getch();
        return ""; // Return empty string to indicate no valid topic
    }

    return corrected[0]; // Return the corrected topic
}

vector<string> getAllUniqueTopics(RoadRunner &obj)
{
    unordered_set<string> uniqueTopics;
    for (auto &[a, b] : obj.uni_map)
    {
        uniqueTopics.insert(a);
    }
    return vector<string>(uniqueTopics.begin(), uniqueTopics.end());
}
int main()
{

    system("chcp 65001 > nul");
    Trie trie;
    RoadRunner roadrunner;
    roadrunner.createGraphsUsingFile("GraphDetails.txt");
    // cout<<"graphs created"<<endl;

    vector<string> allTopics = getAllUniqueTopics(roadrunner);
    map<string, int> present_skillmap;
    unordered_set<string> present_skillset;
    string startTopic, endTopic;
    int timeConstraint = 0;
    int timeConstraintInUnits = 0;

    for (auto &graph : roadrunner.uni_map)
    {
        graph.second.outdegree();
        graph.second.getTotalImportance();
        graph.second.getTotalTime();
    }

    roadrunner.computeIndegree();
    roadrunner.computeAdjustedImportance();

    int n;
    cout << GREEN << "Welcome to Roadrunner!\nGet ready to embark on a thrilling journey of self assessed growth, where we will provide you with the assistance but the journey will be all yours!\nJust fill up our questionairre and wait until the magic unfolds and we present you with a specially customized roadmap, tailored specifically to your needs and preferences!" << endl;
    while (true)
    {
        int choice = roadrunner.displayOptions();
        if(choice==5)
        {
            cout << "\nThank you for using ROADRUNNER!\n";
            return 0;
        }
        map<string, int> present_skillset = getPresentSkillset(trie, allTopics);
        string topic, str, start, end;
        int maxTime, alpha, beta, ch;
        vector<string> completePath;
        switch (choice)
        {
        case 1:
        {
            cout << "\nEnter the topic you want to study: " << endl;
            cin >> topic;
            vector<string> completePath = roadrunner.completionOfATopic(topic);
            cout << "Complete Learning Path for " + topic << endl;
            roadrunner.BasicDisplayOfRoadmap(completePath, present_skillset);
            break;
        }
        case 2:
        {
            //  int maxTime;
            cout << "\nEnter maximum time available (in hours): " << endl;
            cin >> maxTime;
            completePath = roadrunner.KnapsackRecommendation(maxTime, present_skillset);
            str = "Maximum topics possible to be covered in " + maxTime;
            displayKnapsackRecommendation(completePath);
            break;
        }
            /*  case 3: {
                 // string topic;
                  cout << "\nEnter the topic to find connected topics: ";
                  cin >> topic;
                //  vector<string> connected = findConnectedTopics(topic, uni_map);
                //  displayResults(connected, "Topics Connected to " + topic, uni_map);
                  break;
              }*/
        case 3:
            // string start, end;
            //  int alpha, beta, ch;
            cout << "Enter the topic you'd prefer your learning journey to start with" << endl;
            cin >> start;
            cout << "Enter the skill you're aiming for" << endl;
            cin >> end;
            cout << "Wait a minute!!! We have an extra customization to offer! Choose what you'd like to prefer: " << endl;
            cout << "Enter 1 if you want to prioritize easier learning and 2 if you want to prioritize lesser time and any number if a balanced roadmap is what you'd prefer!" << endl;
            cin >> ch;
            if (ch == 1)
            {
                alpha = 2;
                beta = 4;
            }
            else if (ch == 2)
            {
                alpha = 4;
                beta = 2;
            }
            else
            {
                alpha = 1;
                beta = 1;
            }
            completePath = roadrunner.findMostOptimalPath(start, end, alpha, beta, present_skillset);
            cout << "Most optimal path according to given constraints is: " << endl;
            roadrunner.BasicDisplayOfRoadmap(completePath, present_skillset);
            break;
        case 4:
            // string st;
            // int maxTime;
            cout << "Enter the skill you'd like to begin with: " << endl;
            cin >> topic;
            cout << "Enter the maximum time you can invest:" << endl;
            cin >> maxTime;
            roadrunner.topicsPossibleInTime(topic, maxTime, present_skillset);
            // str="Maximum topics possible to be covered in "+maxTime;
            // roadrunner.printSubgraph(completePath);
            break;
        case 5:
            cout << "\nThank you for using ROADRUNNER!\n";
            return 0;
            break;
        default:
            cout << "\nInvalid choice! Please try again.\n";
            _getch();
        }
    }
}
