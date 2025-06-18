#include <bits/stdc++.h>
using namespace std;

// CODE FOR COMPLETING A TOPIC FROM SCRATCH TO ADVANCED

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
