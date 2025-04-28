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

int main()
{
    vector<string> allDSATopics;

    Trie trie;
    for(string t: allDSATopics)
        trie.insert(normalize(t));

    vector<string> userInputedTopics;

    vector<string> correctedTopics = smartSearch(userInputedTopics, allDSATopics, trie);
}
