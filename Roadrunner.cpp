#include <bits/stdc++.h>
#include <conio.h> //needed for getch() - Windoews Only
#include "abbreviation.h"
using namespace std;

// ANSI Color Codes
string RESET = "\033[0m";
const string ORANGE = "\033[38;5;208m";
const string CYAN = "\033[36m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
//----------//

//----------------------Trie------------------------//

struct TrieNode
{
    bool isEnd;
    unordered_map<char, TrieNode *> child;

    TrieNode() : isEnd(false) {}
};

class Trie
{
    vector<string> correctedWords;
    TrieNode *root;

public:
    Trie() { root = new TrieNode(); }

    // Helper function for {getSuggestions}-- stores all words from a given node
    void remains(TrieNode *node, string &prefix, vector<string> &res)
    {
        if (node->isEnd)
        {
            res.push_back(prefix);
        }

        for (auto [c, next] : node->child)
        {
            prefix.push_back(c);
            remains(next, prefix, res);
            prefix.pop_back();
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
                {node->child[c] = new TrieNode();
                node = node->child[c];}
        }
        node->isEnd = true;
        // why?
        correctedWords.push_back(word);
    }

    // Provides the whole word[exists] or suggestions[semi-exists]
    vector<string> getSuggestions(string &word)
    {
        TrieNode *node = root;
        // Find the word till it matches.
        for (char c : word)
        {
            if (node->child.find(c) == node->child.end())
                if (node == root)
                    return {};
                else
                    break;
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

    // Checks existence of word
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
//--------------------------------------------------//

// Normalize String
string normalize(string word)
{
    // lowerCase
    transform(word.begin(), word.end(), word.begin(), ::tolower);

    string specialChars = "-_/.,!&#@$%^*()+=:;\"\'|\\<>?~`";
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
        if (!(word[i] == ' ' && word[i + 1] == ' '))
            temp += word[i];
    }

    // Abbreviation Decoder
    if (abbreviations.find(word) != abbreviations.end())
        word = abbreviations[word];

    return word;
}

// Levishtein Distance(number of operations from Word1 to Word2)
int editDistance(string a, string b)
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

// Stores top matches of a word along w/ their score!
vector<pair<string, int>> topLevenMatches(string userInput, vector<string> topics)
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

/*  Stores best matches from Trie and Leven-Distance search (in final_match)*/
vector<string> top3Matches(string word, vector<string> &allTopics, Trie &trie)
{
    vector<pair<string, int>> leviMatches = topLevenMatches(word, allTopics);
    vector<string> trieMatches = trie.getSuggestions(word);
    set<string> matches;
    vector<string> final_match;

    for (auto &a : trieMatches)
        matches.insert(a);
    for (auto &a : leviMatches)
        matches.insert(a.first);

    // top 3
    int c = 0;
    for (auto &a : matches)
    {
        final_match.push_back(a);
        c++;
        if (c == 3)
            break;
    }

    return final_match;
}


vector<string> smartSearch(vector<string> topicsInput, vector<string> &allTopics, Trie &trie)
{
    vector<string> results;
    for (string w : topicsInput)
    {
        string corrected = normalize(w);
        string temp = corrected;
        //transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        // Store input if in Trie
        if (trie.search(temp))
        {
            results.push_back(corrected);
            continue;
        }

        vector<string> final_match = top3Matches(corrected, allTopics, trie);

        cout << "For the word: " << w << "\nDid you mean:\n";
        for (int i = 0; i < final_match.size(); i++)
            cout << i + 1 << ". " << final_match[i] << endl;

        cout << "Enter the number of the correct topic (1" << (final_match.size() == 1 ? "" : ("-" + to_string(final_match.size()))) << "), or 0 to enter manually: ";
        int choice;
        cin >> choice;

        if (choice >= 1 && choice <= final_match.size())
            results.push_back(final_match[choice - 1]);
        else
        {
            cout << "Enter the correct word manually: ";
            string manual;
            bool correct = false;
            cin.ignore();
            while (!correct)
            {
                getline(cin, manual);
                manual = normalize(manual);
                if (trie.search(manual))
                {
                    correct = true;
                    results.push_back(manual);
                }
                else if (manual == "cancel")
                    break;
                else
                    cout << "No such topic in database. Try again. Type 'cancel' to leave this topic\n";
            }
        }
    }
    return results;
}

//  Standardize the inputed topics
string smartSearchOneWord(string &word, vector<string> &allTopics, Trie &trie)
{
    vector<string> temp;
    temp.push_back(word);
    temp = smartSearch(temp, allTopics, trie);
    cout<< "Temp[o]=" << temp[0]<<endl;
    return temp[0];
}
/*  Promopts user to input their Present Skillset (topics-completed) */
map<string, int> getPresentSkillset(Trie &trie, vector<string> &allTopics)
{
    system("cls");
    map<string, int> present_skillset;
    cout << "\t     ===========Before we move on!===========\n\t You may enter topics you presently hold knowledge of.\n\t     The one's you are confident in.";
    cout << "\n\tDo you have any previous skills? (y/n): ";
    char hasSkills;
    cin >> hasSkills;

    if (hasSkills == 'y' || hasSkills == 'Y')
    {
        vector<string> inputs;
        string topic;

        cout << "Enter topics you already know (type 'done' to finish):\n";
        while (cin >> topic && topic != "done")
            inputs.push_back(topic);

        vector<string> correctedSkill = smartSearch(inputs, allTopics, trie);

        for (const string &word : correctedSkill)
            present_skillset[word] = 0;

        if (!present_skillset.empty())
        {
            cout << "\n"
                 << CYAN << "Your current skillset includes:\n"
                 << RESET;
            for (auto &skill : present_skillset)
                cout << "• " << skill.first << "\n";
        }
        else
        {

            cout << "\n\tAlright! We have a clean slate to fill,"<< CYAN<<" soldier"<< RESET <<". Buckle up!"<<endl;
        }
    }
    else
    {
        cout << "\n\tAlright! We have a clean slate to fill,"<< CYAN<<" soldier"<< RESET <<". Buckle up!"<<endl;
    }
            cout << "\n\nPress ENTER to go back to Main Menu!" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            system("cls");

    return present_skillset;
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

// struct CaseInsensitiveCompare {
//     bool operator()(const string &a, const string &b) const {
//         string aa = a, bb = b;
//         transform(aa.begin(), aa.end(), aa.begin(), ::tolower);
//         transform(bb.begin(), bb.end(), bb.begin(), ::tolower);
//         return aa < bb;
//     }
// };

class Graph
{
public:
    string name_of_graph;
    int no_of_nodes;
    vector<vector<pair<int, int>>> adjMat; // Creating a matrix so that each node is accessible directly
                                           // no need to traverse the entire list of connections if we know which node we want to connect with

    map<int, string> node_name;                       // stores each node index's name
    map<string, int > node_ind;                        // stores each node num mapped to its name
    map<string, pair<int, vector<string>> > outDegree; // strore outdegrees and all the reachable neighbours
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
    map<string, Graph > uni_map; // storing each Graph mapped to its name
    map<string, int > indegree;
    map<string, int > subtopic_adjustedImportance;
    map<string, set<string> > graphConnections;

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

    // Main Graph Connections
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
                            // Connection: "graph.first" -> "otherGraph.first" via node "neighbour"
                        }
                    }
    }

    // Main Graph's Importance (along w/ children)
    void computeAdjustedImportance()
    {
        for (auto graph : uni_map)
        {
            int imp = graph.second.totalImportance;
            for (auto subGraph : graphConnections[graph.first])
                imp += uni_map[subGraph].totalImportance;

            subtopic_adjustedImportance[graph.first] = imp;
        }
    }

    // Utility function to remove hidden whitespaces
    string trim(string &s)
    {
        int start = s.find_first_not_of(" \t\r\n");
        int end = s.find_last_not_of(" \t\r\n");
        if (start == -1)
            return ""; // string::npos =-1
        string res = "";
        for (int i = start; i <= end; i++)
            res += s[i];
        return res;
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
        map<string, int > mp;             // storing node value of each node
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
                transform(str.begin(), str.end(), str.begin(), ::tolower);
                nm = trim(str);
                if (mp.find(nm) == mp.end())
                {
                    mp[nm] = 0;
                    rev_mp[0] = nm;
                }
            }
            else if (line.rfind("ConnectedNodes: ", 0) == 0)
            {
                // "ConnectingNodes:"
                string str = line.substr(16);
                transform(str.begin(), str.end(), str.begin(), ::tolower);
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
                transform(str.begin(), str.end(), str.begin(), ::tolower);
                string start = trim(str);

                vector<int> a;
                a.push_back(mp[start]);
                edgeDetails.push_back(a);
            }
            else if (line.rfind("EdgeEnd: ", 0) == 0)
            {
                string str = line.substr(9);
                transform(str.begin(), str.end(), str.begin(), ::tolower);
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
                obj.node_name = rev_mp;
                obj.node_ind = mp;
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

    // A Beautiful display of branching Topics
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

    // Display function for Knapsack Recommendation
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

    //  Branching Topics from a starting node within time-period
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

        stack<StackNode> st;
        //{GraphName, NodeIndex, PathTraversed, totImp, totTime, NodesVisited}

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

                        //  Creates path that can be traversed, stores it w/(TotalImportance, TotalTime, Visited nodes)
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

                        //  Creates path that can be traversed, stores it w/(TotalImportance, TotalTime, Visited nodes)
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
    }

    // Main topic recommendations (excluding Children time)
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

    ///////////////////////////////////////////////////////////////////////////////////////////////

    // Comparator for Dijikastra
    static bool compForPQDjikastra(pair<pair<Graph, string>, int> a, pair<pair<Graph, string>, int> b)
    {
        return a.second > b.second;
    }

    // Shorest Route to cover necessary material before starting w/ a topic - Dijikastra Path Generation
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

    //  Comparator for TopicCompletion
    static bool comp_for_pq_forTopicCompletion(pair<string, int> a, pair<string, int> b)
    {
        return a.second < b.second;
    }

    //  Completion of a topic from Scratch to Advance
    vector<string> completionOfATopic(string topicName) // CODE FOR COMPLETING A TOPIC FROM SCRATCH TO ADVANCED
    {
        Graph &topic = uni_map[topicName];
        map<string, bool> vis;
        stack<string> st;
        vector<string> completePath;
        priority_queue<pair<string, int>, vector<pair<string, int>>, bool (*)(pair<string, int>, pair<string, int>)> pq(comp_for_pq_forTopicCompletion);
        st.push(topicName);
        int num_of_nodes_added = 0;

        while (!st.empty())
        {
            string currNode = st.top();
            st.pop();

            if (topic.node_ind.find(currNode) == topic.node_ind.end())
                continue; // Skip if node is not present in the graph

            int curr = topic.node_ind[currNode];
            if (vis.find(currNode) == vis.end())
            {
                completePath.push_back(currNode);
                num_of_nodes_added += 1;
                vis[currNode] = true;
            }
            else
                continue;

            for (int i = 0; i < topic.no_of_nodes; i++)
            {
                if (topic.adjMat[curr][i].first != 0 && (vis.find(topic.node_name[i]) == vis.end()))
                {
                    pq.push({topic.node_name[i], topic.adjMat[curr][i].first});
                    // Neighbor added to PQ: "topic.node_name[i]" with weight: "topic.adjMat[curr][i].first"
                }
            }

            while (!pq.empty()) // STORED TEMPORARILY IN THE PRIORITY QUEUE SO THAT NODES ARE ENTERED IN THE STACK ACC. TO THE ORDER
            {
                st.push(pq.top().first);
                pq.pop();
            }
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

    int displayOptions()
    {
        cout << "\n"
             << ORANGE << "================| ROADRUNNER Menu |=================\n"
             << RESET;
        cout << YELLOW << "1. Learning a topic from Scratch from Advanced!" << endl;
        cout << YELLOW << "2. Get to know the maximum topics you can cover from your present skillset given the time you can devote!" << endl;
        cout << "3. View shortest path from current to aimed skillset" << endl;
        cout << "4. Get to know the maximum topics you can cover from a given specific skill given the time you can invest!" << endl;
        cout << "5. Exit\n\n"
             << RESET;

        cout << "Enter your choice (1-5): ";
        int choice;
        cin >> choice;
        return choice;
    }

};

    void buildTrie(Trie &trie, vector<string> &topics)
    {
        for (auto a : topics)
        {
            string word = normalize(a);
            //transform(word.begin(), word.end(), word.begin(), ::tolower);
            //cout << word <<endl;
            trie.insert(word);
        }
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

        // Initializing data
        system("chcp 65001 > nul");
        Trie trie;
        RoadRunner roadrunner;
        roadrunner.createGraphsUsingFile("GraphDetails.txt");

        vector<string> allTopics = getAllUniqueTopics(roadrunner);
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

        buildTrie(trie, allTopics);
        int n;
        cout << GREEN << "\t\t==================================\n\t\tWelcome to Roadrunner!\n\t\t==================================\nGet ready to embark on a thrilling journey of self assessed growth,\nwhere we will provide you with the assistance but the journey will be all yours!\nJust fill up our questionaire and wait until the magic unfolds and we present you with a specially customized roadmap,\ntailored specifically to your needs and preferences!"<< RESET << endl;
            cout<<"\n\nPress enter to continue...";
            cin.ignore();
        map<string, int> present_skillset = getPresentSkillset(trie, allTopics);
        while (true)
        {
            int choice = roadrunner.displayOptions();
            string topic, str, start, end;
            int maxTime, alpha, beta, ch;
            vector<string> completePath;
            switch (choice)
            {
            case 1:
            {
                cout << "\nEnter the topic you want to study: " << endl;
                cin >> topic;
                vector<string> completePath = roadrunner.completionOfATopic(smartSearchOneWord(topic, allTopics, trie));
                cout << "Complete Learning Path for " + topic << endl;
                roadrunner.BasicDisplayOfRoadmap(completePath, present_skillset);
                break;
            }
            case 2:
            {
                cout << "\nEnter maximum time available (in hours): " << endl;
                cin >> maxTime;
                completePath = roadrunner.KnapsackRecommendation(maxTime, present_skillset);
                str = "Maximum topics possible to be covered in " + maxTime;
                roadrunner.displayKnapsackRecommendation(completePath);
                break;
            }
            case 3:
               {
                cout << "Enter the topic you'd prefer your learning journey to start with" << endl;
                cin >> start;
                start = smartSearchOneWord(start, allTopics, trie);
                cout << "Enter the skill you're aiming for" << endl;
                cin >> end;
                end = smartSearchOneWord(end, allTopics, trie);
                cout << "\t\t\t\tWait a minute!!!  \n\t\t\t\t We have an extra customization to offer! \n\t\t\t\t Choose what you'd like to prefer:\n " << endl;
                cout << "Enter [1] to prioritize easier learning, [2] to prioritize lesser time, \n\t[any number] if a balanced roadmap is what you'd prefer!" << endl;
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
                break;}
            case 4:
                {cout << "Enter the skill you'd like to begin with: " << endl;
                cin >> topic;
                topic = smartSearchOneWord(topic, allTopics, trie);
                cout << "Enter the maximum time you can invest:" << endl;
                cin >> maxTime;
                roadrunner.topicsPossibleInTime(topic, maxTime, present_skillset);
                break;}
            case 5:
                {cout << "\nThank you for using ROADRUNNER!\n";
                return 0;
                break;
                }
            default:
                {cout << "\nInvalid choice! Please try again.\n";
                break;
                }
            }
            cout << "Press ENTER to go back to Main Menu!" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            system("cls");
        }
    }
