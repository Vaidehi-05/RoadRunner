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
