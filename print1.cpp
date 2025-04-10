#include <bits/stdc++.h>
using namespace std;
int editDistance(string a, string b) //Levishtein Distance
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
                dp[i][j]= 1 + min(dp[i-1][j], min(dp[i][j-1], dp[i-1][j-1]));   //opertaion performed
    return dp[m][n];
}

vector<pair<string, int>> topMatches(string userInput, vector<string> topics) // stores top matches of a word along w/ their score!
{
    int s;
    vector<pair<string, int>> score;
    for(string a: topics)
    {
        s=editDistance(userInput, a);
        cout<<s<<" "<<a<<endl;
        score.push_back({a,s});
    }
    sort(score.begin(), score.end(), [](pair<string, int>& a, pair<string, int>& b){return a.second<b.second;});

    vector<pair<string, int>> top(score.begin(), score.begin() + min(3, (int)score.size()));
    return top;
}

vector<string> DSATopics={"Dynamic Programming", "Graphs", "Trees", "Stack", "Queue", "Array", "Greedy"};

vector<string> wTake(vector<string> wrd, vector<string> DSATopics)
{
    int score;
    vector<string> wtake;
    //vector<vector<pair<string, int>>> mismatched;
    vector<pair<string, int>> tmp;
    for(string w: wrd)
    {
        tmp=topMatches(w, DSATopics);
        if(!tmp.empty() && tmp[0].second==0)
        {
            wtake.push_back(w);
            continue;
        }
        else
        {
            cout<<"For the word: "<< w <<"\nDid you mean these?: ";
            for(pair<string, int> a: tmp)
                cout<< a.first<< " || ";
            cout<<"Enter the number of the correct topic( 1-"<< (int)tmp.size() << "), or 0 to skip/enter manually: ";
            int choice;
            cin>> choice;
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice= -1;
            }

            if(choice>=1 && choice<=(int)tmp.size()) wtake.push_back(tmp[choice-1].first);
            else
            {
                bool correct=true;
                while(correct)
                cout<< "Enter the correct word manually:";
                string word;
                cin.ignore();
                getline(cin, word);

                for(auto& a: DSATopics)
                if(a==word)
                {correct=false;
                break;
                }
                if(!correct)
                wtake.push_back(word); 
                else
                {
                    cout<<"You have entered the wrong choice!";
                }
            }

        }
    }
    return wtake;
}
int main()
{
    vector<string> a;
    vector<string> b={"Sta"};
    a=wTake(b, DSATopics);
    for(auto i:a)
        cout<<i<<endl;
}
