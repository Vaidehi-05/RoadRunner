struct KnapItem
{
    string name;
    int totTime;
    int totImp;
};

vector<string> KnapsackRecommendation(int maxTime, unordered_map<string, int> userCompletedTopics, map<string, int>& indegree, map<string, Graph>& uni_map, map<string, int>& subtopic_adjustedImportance) //need to exclude the already done topics
{
    unordered_set<string> neighb;
    for(auto& [a,nonono]:userCompletedTopics)
        {
            if(uni_map.find(a)!=uni_map.end())
            {   Graph& b=uni_map[a];
                for(auto& neighbour: b.outDegree)
                    for(string& reachable: neighbour.second.second)
                        if(userCompletedTopics.find(reachable)==userCompletedTopics.end())
                            neighb.insert(reachable);
            }
        }
    
    vector<KnapItem> topics;

    for(auto graph:uni_map)
    {
        if (userCompletedTopics.find(graph.first)!=userCompletedTopics.end())
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
