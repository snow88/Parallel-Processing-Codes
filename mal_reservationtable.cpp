#include<iostream>
#include<fstream>
#include<string.h>
#include<math.h>
#include<unordered_map>
#include<list>
#include<vector>
#include<set>
using namespace std;

set<int> forbdlat;
int colvec = 0;
unordered_map<int,list<pair<int,int> > > mp;
unordered_map<int,bool> visited;
float mal = 0;

string binaryrep(int i)
{
    string s;
    if (i & 1)
        s = "1";
    else
        s = "0";
    if (i>1)
        s = binaryrep(i >> 1) + s;
    return s;
}

void buildgraph(int node)
{
    int i=0;
    int n = node;
    while (n != 0)
    {
        int last = n & 1;
        n = n >> 1;
        i++;
        if (last == 0)
        {
            int newnode = n | colvec;
            mp[node].push_back(make_pair(newnode,i));
            if (mp.find(newnode) == mp.end())
                buildgraph(newnode);
        }
    }
    mp[node].push_back(make_pair(colvec,*(forbdlat.rbegin()) + 1));
}

void printgraph(int node)
{
    visited[node] = true;
    cout<<binaryrep(node)<<"->";
    for (auto p : mp[node])
    {
        cout<<" ("<<binaryrep(p.first)<<","<<p.second<<"),";
    }
    cout<<endl;
    for (auto p : mp[node])
    {
        if (visited.find(p.first) == visited.end())
            printgraph(p.first);
    }
}

void simplecycleshelper(int start, int node, vector<pair<int,int> > &cycle)
{
    visited[node] = true;
    for (auto p : mp[node])
    {
        if (p.first == start)
        {
            cycle.push_back(p);
            vector<pair<int,int> >::iterator it = cycle.begin();
            cout<<binaryrep((*it).first)<<" -> ";
            it++;
            for ( ; it!=cycle.end(); it++)
            {
                cout<<binaryrep((*it).first)<<","<<(*it).second<<" -> ";
            }
            cout<<endl;
            cycle.pop_back();     
        }
        else if (visited.find(p.first) != visited.end())
        {
            continue;
        }
        else
        {
            cycle.push_back(p);
            simplecycleshelper(start, p.first, cycle);
            cycle.pop_back();     
        }
    }
    visited.erase(node);
}

void printsimplecycles()
{
    for (auto kv : mp)
    {
        visited.clear();     
        vector<pair<int,int> > cycle;
        cycle.push_back(make_pair(kv.first, -1));
        simplecycleshelper(kv.first, kv.first, cycle);
    }
}

void greedycycleshelper(int start, int node, vector<pair<int,int> > &cycle)
{
    visited[node] = true;
    int minedge = 9999;
    int minnode;
    for (auto p : mp[node])
    {
        if (p.second < minedge)
        {
            minedge = p.second;
            minnode = p.first;
        }
    }
    if (minnode == start)
    {
        float tempmal = 0;
        int edges = 0;
        cycle.push_back(make_pair(minnode, minedge));
        vector<pair<int,int> >::iterator it = cycle.begin();
        cout<<binaryrep((*it).first)<<" -> ";
        it++;
        for ( ; it!=cycle.end(); it++)
        {
            cout<<binaryrep((*it).first)<<","<<(*it).second<<" -> ";
            tempmal += (*it).second;
            edges++;
        }
        cout<<endl;
        cycle.pop_back();     
        tempmal /= edges;
        mal = max(mal, tempmal);
    }
    else if (visited.find(minnode) != visited.end())
    {
        return;
    }
    else
    {
        cycle.push_back(make_pair(minnode,minedge));
        greedycycleshelper(start, minnode, cycle);
        cycle.pop_back();    
    }
}

void printgreedycycles()
{
    for (auto kv : mp)
    {
        visited.clear();    
        vector<pair<int,int> > cycle;
        cycle.push_back(make_pair(kv.first, -1));
        greedycycleshelper(kv.first, kv.first, cycle);
    }
}

int main(int argc, char* argv[])
{
    char* filename = argv[1];             

    ifstream fin;
    fin.open(filename);

    while (!fin.eof())
    {
        string str;
        getline(fin, str);
        int xpos[50];
        int idx=0, j=0;
        for (int i=0; i<str.length(); i++)
        {
            if (str[i] == 'x')
            {
                xpos[idx] = j;
                idx++;
            }
            if (str[i] != ',')
                j++;
        }
        for (int p=0; p<idx; p++)
        {
            for (int q=p+1; q<idx; q++)
                forbdlat.insert(xpos[q]-xpos[p]);
        }
    }

    cout<<"\nForbidden Latencies: ";
    for (auto val : forbdlat)
        cout<<val<<" ";
    cout<<endl;

    cout<<"\nPermissible Latencies: ";
    for (int val=1 ; val< *(forbdlat.rbegin()); val++)
    {
        if (forbdlat.find(val) == forbdlat.end())
        {
            cout<<val<<" ";
        }
    }
    cout<<endl;

    for (int i = 1; i<=*(forbdlat.rbegin()); i++)
    {
        if (forbdlat.find(i) != forbdlat.end())
            colvec += pow(2,i-1);
    }
    cout<<"\nCollision Vector: "<<binaryrep(colvec)<<endl;

    buildgraph(colvec);
    cout<<"\nState Diagram:"<<endl;
    visited.clear();
    printgraph(colvec);

    cout<<"\nSimple Cycles:"<<endl;
    printsimplecycles();

    cout<<"\nGreedy Cycles:"<<endl;
    printgreedycycles();

    cout<<"\nMinimal Average Latency (MAL) : "<<mal<<endl;
    cout<<endl;

    return 0;
}
