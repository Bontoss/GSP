#include <bits/stdc++.h>
using namespace std;

#define IO                        \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);                   \
    cout.tie(0);
#define sz(s) ((int)(s.size()))
#define ull unsigned long long
#define ld long double
#define ll long long
#define el '\n'

vector<string> DataSet;
map<string, ll> Support;
set<string> Levels[100];
vector<vector<string>> FrequentSets(100);

void SupportCount(int Level)
{
    for (auto x : Levels[Level])
    {
        for (int i = 0; i < sz(DataSet); i++)
        {
            int j = 0, c = 0, leftdata = -1, rightdata = -1, leftc = -1, rightc = -1;
            while (j < sz(DataSet[i]))
            {
                if (DataSet[i][j] == '(')
                {
                    leftdata = j;
                    int n = leftdata;
                    while (DataSet[i][n++] != ')')
                        rightdata = n;
                }
                if (x[c] == '(')
                {
                    leftc = c;
                    int n = leftc;
                    while (x[n++] != ')')
                        rightc = n;
                }
                if (x[c] == DataSet[i][j] && x[c] == '(')
                {
                    c++, j++;
                    while (DataSet[i][j] != ')')
                    {
                        if (x[c] == DataSet[i][j])
                            c++;
                        j++;
                    }
                    c++;
                    if (c != rightc + 1)
                    {
                        c = leftc;
                        j = rightdata;
                    }
                }
                else if (x[c] == DataSet[i][j])
                {
                    c++;
                    if (j > leftdata && j < rightdata)
                        j = rightdata;
                }
                if (c == sz(x))
                {
                    Support[x]++;
                    break;
                }
                j++;
            }
        }
    }
}

int main()
{
    IO;
    ld SupportPercentage, NumOfRecords;
    cin >> SupportPercentage >> NumOfRecords;
    ld MinSupport = (ceil)((SupportPercentage / 100) * NumOfRecords);

    //////////////////////////////////////////////Input Database/////////////////////////////////////////////
    for (int i = 0; i < NumOfRecords; i++)
    {
        string s;
        cin >> s;
        int l = -1, r = -1;
        for (int j = 0; j < sz(s); j++)
        {
            if (s[j] == '(')
            {
                l = j;
                while (s[j++] != ')')
                    r = j;
            }
            if (l != -1 && r != -1)
                sort(s.begin() + l, s.begin() + r);
        }
        for (int j = 0; j < sz(s); j++)
        {
            string tmp = "";
            tmp += s[j];
            if ((tmp >= "A" && tmp <= "Z") || (tmp >= "a" && tmp <= "z"))
                Levels[0].insert(tmp);
        }
        DataSet.push_back(s);
    }

    ////////////////////////////////////////////Counting Support Of Level 1/////////////////////////////////////////
    for (int i = 0; i < NumOfRecords; i++)
    {
        string tmp = "";
        map<string, ll> cnt;
        for (int j = 0; j < sz(DataSet[i]); j++)
        {
            tmp += DataSet[i][j];
            cnt[tmp]++;
            tmp.clear();
        }
        for (auto x : Levels[0])
            if (cnt[x] != 0)
                Support[x]++;
    }

    for (auto x : Levels[0])
        if (Support[x] >= MinSupport)
            FrequentSets[0].push_back(x);
    for (auto x : FrequentSets[0])
        cout << x << ' ' << Support[x] << el;

    ///////////////////////////////////////////////// ItemSets of size 2 /////////////////////////////////////////////
    // All possible Temporal joins
    for (auto x : FrequentSets[0])
        for (auto y : FrequentSets[0])
            Levels[1].insert(x + y);
    // All possible non-Temporal joins
    for (auto x : FrequentSets[0])
        for (auto y : FrequentSets[0])
            if (x != y)
            {
                string s = x + y;
                sort(s.begin(), s.end());
                Levels[1].insert('(' + s + ')');
            }

    SupportCount(1);

    for (auto x : Levels[1])
        if (Support[x] >= MinSupport)
            FrequentSets[1].push_back(x);

    for (auto x : FrequentSets[1])
        cout << x << ' ' << Support[x] << el;

    ///////////////////////////////////////////// ItemSets of size 3 OR More//////////////////////////////////////////
    int i = 2;
    while (true)
    {
        for (auto x : FrequentSets[i - 1])
        {
            int l = -1, r = -1;
            string x2 = "";
            for (int i = 0; i < sz(x); i++)
            {
                if (x[i] == '(')
                {
                    l = i;
                    while (x[i++] != ')')
                        r = i;
                }
                if (l != -1 && r != -1)
                    sort(x.begin() + l, x.begin() + r);
            }
            for (auto a : x)
                if (a != '(' && a != ')')
                    x2 += a;
            for (auto y : FrequentSets[i - 1])
            {
                int l = -1, r = -1;
                string y2 = "";
                for (int i = 0; i < sz(y); i++)
                {
                    if (y[i] == '(')
                    {
                        l = i;
                        while (y[i++] != ')')
                            r = i;
                    }
                    if (l != -1 && r != -1)
                        sort(y.begin() + l, y.begin() + r);
                }
                for (auto a : y)
                    if (a != '(' && a != ')')
                        y2 += a;
                if (x != y)
                {
                    if (sz(x.substr(1, sz(x) - 1)) == 1 && y[0] == '(' && y[3] == ')')
                    {
                        for (auto a : y)
                            if (a == x[1])
                            {
                                Levels[i].insert(x[0] + y);
                                break;
                            }
                    }
                    else if (x[0] == '(' && x[3] == ')' && sz(y.substr(0, sz(y) - 1)) == 1 && x[2] == y[0])
                    {
                        for (auto a : x)
                            if (a == y[0])
                            {
                                Levels[i].insert(x + y[1]);
                                break;
                            }
                    }
                    else
                    {
                        if (x2.substr(1, sz(x2) - 1) == y2.substr(0, sz(y2) - 1))
                        {
                            if (x[0] == '(')
                                Levels[i].insert(x + y[sz(y) - 1]);
                            else
                                Levels[i].insert(x[0] + y);
                        }
                    }
                }
            }
        }

        SupportCount(i);

        for (auto x : Levels[i])
            if (Support[x] >= MinSupport)
                FrequentSets[i].push_back(x);

        if (sz(FrequentSets[i]) == 0)
            break;

        for (auto x : FrequentSets[i])
            cout << x << ' ' << Support[x] << el;
        i++;
    }
    return 0;
}