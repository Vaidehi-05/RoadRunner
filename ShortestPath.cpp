#include <bits/stdc++.h>
using namespace std;


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
