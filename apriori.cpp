#include <bits/stdc++.h>
using namespace std;

vector<vector<string>> csvToVector(string filename)
{
    vector<vector<string>> result;
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Failed to open file " << filename << endl;
        return result;
    }

    string line;
    while (getline(file, line))
    {
        vector<string> row;
        stringstream ss(line);
        string field;

        while (getline(ss, field, ','))
        {
            row.push_back(field);
        }

        result.push_back(row);
    }

    file.close();
    return result;
}

float minsup = 0.5;
float minconf = 0.7;
void generateSubsets(vector<string> &nums, vector<string> &subset, int k, int start, vector<vector<string>> &result)
{
    if (subset.size() == k)
    {
        result.push_back(subset);
        return;
    }

    for (int i = start; i < nums.size(); i++)
    {
        subset.push_back(nums[i]);
        generateSubsets(nums, subset, k, i + 1, result);
        subset.pop_back();
    }
}

vector<vector<string>> subsets(vector<string> nums, int k)
{
    vector<vector<string>> result;
    vector<string> subset;
    generateSubsets(nums, subset, k, 0, result);
    return result;
}

vector<vector<string>> combo(int isize, vector<vector<string>> items)
{
    set<string> ditems;
    for (int i = 0; i < items.size(); i++)
    {
        for (int j = 0; j < items[i].size(); j++)
        {
            ditems.insert(items[i][j]);
        }
    }
    vector<string> setTovec;
    for (auto it : ditems)
    {
        setTovec.push_back(it);
    }
    return subsets(setTovec, isize);
}

float support(vector<vector<string>> items, vector<string> it)
{
    float freq;
    for (int i = 0; i < items.size(); i++)
    {
        vector<string> df = items[i];
        int f1 = 1;
        for (int j = 0; j < it.size(); j++)
        {
            int f = 0;
            for (int k = 0; k < df.size(); k++)
            {
                if (it[j] == items[i][k])
                {
                    f = 1;
                }
            }
            if (!f)
            {
                f1 = 0;
                break;
            }
        }
        if (f1)
        {
            freq++;
        }
    }
    return (float)freq / (float)items.size();
}

vector<vector<string>> pruned(vector<vector<string>> subs, vector<vector<string>> items)
{
    vector<vector<string>> ans;
    for (int i = 0; i < subs.size(); i++)
    {
        for (int j = 0; j < subs[i].size(); j++)
        {
            cout << subs[i][j] << " ";
        }
        float sp = support(items, subs[i]);
        cout << sp << endl;
        if (sp >= minsup)
        {
            ans.push_back(subs[i]);
        }
    }
    return ans;
}

void assoc(vector<string> last, vector<vector<string>> items)
{
    float num = support(items, last);
    vector<vector<string>> rules;
    vector<vector<string>> rules2;
    vector<vector<string>> ans;
    vector<vector<string>> ans2;
    for (int i = 0; i < last.size(); i++)
    {
        vector<string> temp;
        vector<string> temp2;
        temp2.push_back(last[i]);
        for (int j = 0; j < last.size(); j++)
        {
            if (j != i)
            {
                temp.push_back(last[j]);
                temp2.push_back(last[j]);
            }
        }
        temp.push_back(last[i]);
        rules.push_back(temp);
        rules2.push_back(temp2);
    }
    for (int i = 0; i < rules.size(); i++)
    {
        vector<string> temp;
        for (int j = 0; j < rules[i].size() - 1; j++)
        {
            temp.push_back(rules[i][j]);
        }
        float confidence = num / support(items, temp);
        if (confidence >= minconf)
        {
            ans.push_back(rules[i]);
        }
    }
    for (int i = 0; i < rules2.size(); i++)
    {
        vector<string> temp;
        temp.push_back(rules2[i][0]);
        float confidence = num / support(items, temp);
        if (confidence >= minconf)
        {
            ans2.push_back(rules[i]);
        }
    }
    for (int i = 0; i < ans.size(); i++)
    {
        for (int j = 0; j < ans[i].size() - 1; j++)
        {
            cout << ans[i][j] << ",";
        }
        cout << "-->" << ans[i][ans[i].size() - 1] << endl;
    }
    for (int i = 0; i < ans2.size(); i++)
    {
        cout << ans[i][ans[i].size() - 1] << "-->" << endl;
        for (int j = 1; j < ans2[i].size() - 1; j++)
        {
            cout << ans2[i][j] << ",";
        }
    }
}

int main()
{
    vector<vector<string>> orig;
    vector<vector<string>> items = {{"apple", "dog", "cat"}, {"cow", "dog", "egg"}, {"apple", "cow", "dog", "egg"}, {"cow", "egg"}};
    orig = items;

    int isize = 1;

    vector<vector<string>> ans;
    vector<vector<string>> prev = items;
    while (items.size() > 0)
    {
        cout << "step : " << isize << endl;
        vector<vector<string>> allSubs = combo(isize, items);
        cout << "All subsets: " << endl;
        prev = items;
        items = pruned(allSubs, orig);
        cout << endl;
        cout << "pruned : " << endl;

        for (int i = 0; i < items.size(); i++)
        {
            for (int j = 0; j < items[i].size(); j++)
            {
                cout << items[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        isize++;
    }
    cout << "freuent itemset : " << endl;

    for (int i = 0; i < prev.size(); i++)
    {
        for (int j = 0; j < prev[i].size(); j++)
        {
            cout << prev[i][j] << " ";
        }
        cout << endl;
    }
    assoc(prev[0], orig);
    // for (int i = 0; i < items.size(); i++)
    // {
    //     for (int j = 0; j < items[i].size(); j++)
    //     {
    //         cout << items[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    return 0;
}