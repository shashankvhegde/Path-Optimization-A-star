#include<bits/stdc++.h>
#define INF 9999
using namespace std;

typedef struct state{
    state* prev;
    int node;
    unordered_set<int> keys;
    int g;
    int f;
}state;

state* createState(state* prev, int node, unordered_set<int> keys, int g, int h){
    state* p = (state*)malloc(sizeof(state));
    p->prev = prev;
    p->node = node;
    for(auto it = keys.begin();it != keys.end();it++){
        p->keys.insert(*it);
    }
    p->g = g;
    p->f = g+h;
    return p;
}

unordered_set<int> Union(unordered_set<int> A, unordered_set<int> B){
    cout << "Entered Union"<<endl;
    unordered_set<int> out = new unordered_set<int>();
    out.insert(A.begin(), A.end());
    out.insert(B.begin(), B.end());
    cout << "Union Returning"<<endl;
    return out;
}

bool comp(state* a, state* b){
    bool retval = true;
    int fxa = a->f;
    int fxb = b->f;
    if(fxb < fxa){
        retval = false;
    }
    return retval;
}

void insertPQ(vector<state*>& pq, state* p){
    pq.push_back(p);
    sort(pq.begin(), pq.end(), comp);
}

state* popFrontPQ(vector<state*>& pq){
    int n = pq.size();
    state* retval = nullptr;
    if(n > 0){
        reverse(pq.begin(), pq.end());
        retval = pq.back();
        pq.pop_back();
        reverse(pq.begin(), pq.end());
    }
    return retval;
}

void computeHeuristics(vector<vector<int>>& a, int src, int cost[]){
    int n = a.size();
    bool found[n];
    for(int i = 0;i<n;i++){
        cost[i] = a[src][i];
        found[i] = false;
    }
    cost[src] = 0;
    found[src] = true;
    int minpos;
    for(int i = 0;i<n-1;i++){
        int Min = INF;
        for(int j = 0;j<n;j++){
            if(!found[j]){
                Min = min(Min, cost[j]);
                if(Min == cost[j]){
                    minpos = j;
                }
            }
        }
        found[minpos] = true;
        for(int j = 0;j<n;j++){
            if(!found[j]){
                if(cost[j] > cost[minpos] + a[minpos][j]){
                    cost[j] = cost[minpos] + a[minpos][j];
                }
            }
        }
    }
    cout << "Printing Heuristics "<<endl;
    for(int i = 0;i<n;i++){
        cout << cost[i] << " ";
    }
    cout << endl;
}

bool canBeUnblocked(vector<vector<int>>& a, int src, int dest, bool blocked[], unordered_set<int> keys){

    bool retval = true;
    if(blocked[dest] == true){
        unordered_set<int>::const_iterator it = keys.find(dest);
        if(a[src][dest] == INF || it == keys.end()){
            retval = false;
        }
    }
    return retval;
}

int Astar(vector<vector<int>>& a, int source, int dest, vector<vector<int>>& keys, bool blocked[]){
    int cost = 0;
    int n = a.size();
    int heuristics[n];
    computeHeuristics(a, dest, heuristics);
    vector<state*> pq;
    unordered_set<int> k;
    cout << "Start state Keys"<<endl;
    for(int i = 0;i<keys[source].size();i++){
        k.insert(keys[source][i]);
    }
    state* p = createState(nullptr, source, k, 0, heuristics[source]);
    insertPQ(pq, p);
    bool found = false;
    state* target;
    cout << "Printing States Explored"<<endl;
    while(pq.size()>0 && !found){
        state* x = popFrontPQ(pq);
        cout <<"Node No.: " << x->node << endl;
        cout <<"F value: " << x->f<<endl;

        if(x->node == dest){
            target = x;
            found = true;
        }
        else{
            cout<<"Hmm"<<endl;
            int nodeno = x->node;
            cout<<nodeno<<endl;
            unordered_set<int> parentkeys;
            cout<<"Printing X keys"<<endl;
            parentkeys = Union(parentkeys, x->keys);
            cout<<"Oh"<<endl;
            for(auto it = parentkeys.begin();it!=parentkeys.end();it++){
                cout << *it << " ";
            }
            cout << endl;
            int gx = x->g;
            for(int i = 0;i<n;i++){

                if(a[nodeno][i] != INF && (canBeUnblocked(a, nodeno, i, blocked, parentkeys) == true)){
                    cout<<"Hello "<< i <<endl;
                    unordered_set<int> newkeys;
                    newkeys.insert(keys[i].begin(), keys[i].end());
                    newkeys = Union(parentkeys, newkeys);
                    state* neighbour = createState(x, i, newkeys, gx + a[nodeno][i], heuristics[i]);
                    insertPQ(pq, neighbour);
                    newkeys.clear();
                }
            }
        }

    }
    if(found){
        state* i = target;
        vector<int> path;
        while((i->prev) != nullptr){
            path.push_back(i->node);
            i = i->prev;
        }
        reverse(path.begin(), path.end());
        cost = target->g;
        cout << "Path is : "<<endl;
        for(int i = 0;i<(int)path.size();i++){
            cout << path[i] << " ";
        }
        cout << endl;
    }
    else{
        cost = INF;
        cout << "Target Unreachable"<<endl;
    }
    return cost;
}

int main() {

	int n;
	cout << "Enter Number of Nodes"<<endl;
	cin >> n;
	vector<vector<int>> a(n);
	ifstream file;
	file.open("input.txt");
	for(int i = 0;i<n;i++){
		for(int j = 0;j<n;j++){
			//int x;
			//cin >> x;
			//a[i].push_back(x);
			int x;
			file >> x;
			a[i].push_back(x);

		}
	}
	file.close();

	int source, dest;
	cout << "Enter Source Node no." <<endl;
	cin >> source;
	cout << "Enter Destination Node no." <<endl;
	cin >> dest;

    bool blocked[n];
    for(int i = 0;i<n;i++){
        blocked[i] = false;
    }
    int noOfBlocked = 0;
    cout << "Enter the number of nodes that are blocked"<<endl;
    cin >> noOfBlocked;
    cout << "Enter the node number of blocked nodes"<<endl;
    for(int i = 0;i<noOfBlocked;i++){
        int x;
        cin >> x;
        blocked[x] = true;
    }
    vector<vector<int>> keys(n);
    for(int i = 0;i<n;i++){
        int numKeys;
        cout << "Enter the no of keys in node "<<i<<endl;
        cin >> numKeys;
        if(numKeys > 0){
            cout<<"Enter the key numbers in node "<<i<<endl;
            for(int j = 0;j<numKeys;j++){
                int x;
                cin >> x;
                keys[i].push_back(x);
            }
        }
    }

	int cost = Astar(a, source, dest, keys, blocked);
    cout << "Minimum Cost is : " << cost <<endl;

	return 0;
}
