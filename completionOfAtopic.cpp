#include <bits/stdc++.h>
using namespace std;

// CODE FOR COMPLETING A TOPIC FROM SCRATCH TO ADVANCED

static bool comp_for_temp_pq(pair <string, int> a, pair <string, int> b)
{
    return a.second>b.second;
}

vector<string> completionOfATopic(string topicName)
{
    Graph& topic=uni_map[topicName];
    map <int, bool> vis;
    stack <string> st;
    vector <string> completePath;
    priority_queue <pair<string, int>, vector<pair<string, int>>, comp_for_temp_pq> pq;
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
