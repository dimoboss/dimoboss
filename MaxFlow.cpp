#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <stack>
#include <cstring>  
using namespace std;

 
class MaxFlowAlgorithms {
public:
    int n;
    vector<vector<int>> capacity;
    vector<vector<int>> adj;  

    MaxFlowAlgorithms(int n) : n(n) {
        capacity.resize(n, vector<int>(n, 0));
        adj.resize(n);
    }
 
    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, int>> q;
        q.push({s, INT_MAX});

        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next]) {
                    parent[next] = cur;
                    int new_flow = min(flow, capacity[cur][next]);
                    if (next == t)
                        return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
        return 0;
    }

 
    int maxFlow(int s, int t) {
        int flow = 0;
        vector<int> parent(n);
        int new_flow;

        while (new_flow = bfs(s, t, parent)) {
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                capacity[prev][cur] -= new_flow;
                capacity[cur][prev] += new_flow;
                cur = prev;
            }
        }
        return flow;
    }

    
    void dfs(int s, bool visited[]) {
        visited[s] = true;
        for (int i = 0; i < n; i++) {
            if (capacity[s][i] > 0 && !visited[i]) {
                dfs(i, visited);
            }
        }
    }

  
    void minCut(int s, int t) {
         
        vector<vector<int>> rGraph = capacity;

        
        vector<int> parent(n);
        maxFlow(s, t);

      
        bool visited[n];
        memset(visited, false, sizeof(visited));
        dfs(s, visited);

     
        cout << "Min Cut edges:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (visited[i] && !visited[j] && capacity[i][j]) {
                    cout << i << " - " << j << endl;
                }
            }
        }
    }

    
    bool dfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        stack<pair<int, int>> q;
        q.push({s, INT_MAX});

        while (!q.empty()) {
            int cur = q.top().first;
            int flow = q.top().second;
            q.pop();

            for (int next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next]) {
                    parent[next] = cur;
                    int new_flow = min(flow, capacity[cur][next]);
                    if (next == t)
                        return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
        return 0;
    }

    
    int maxFlowDFS(int s, int t) {
        int flow = 0;
        vector<int> parent(n);
        int new_flow;

        while (new_flow = dfs(s, t, parent)) {
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                capacity[prev][cur] -= new_flow;
                capacity[cur][prev] += new_flow;
                cur = prev;
            }
        }
        return flow;
    }

     
    struct FlowEdge {
        int v, u;
        long long cap, flow = 0;
        FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
    };

    struct Dinic {
        const long long flow_inf = 1e18;
        vector<FlowEdge> edges;
        vector<vector<int>> adj;
        int n, m = 0;
        int s, t;
        vector<int> level, ptr;
        queue<int> q;

        Dinic(int n, int s, int t) : n(n), s(s), t(t) {
            adj.resize(n);
            level.resize(n);
            ptr.resize(n);
        }

        void add_edge(int v, int u, long long cap) {
            edges.emplace_back(v, u, cap);
            edges.emplace_back(u, v, 0);
            adj[v].push_back(m);
            adj[u].push_back(m + 1);
            m += 2;
        }

        bool bfs() {
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                for (int id : adj[v]) {
                    if (edges[id].cap - edges[id].flow < 1)
                        continue;
                    if (level[edges[id].u] != -1)
                        continue;
                    level[edges[id].u] = level[v] + 1;
                    q.push(edges[id].u);
                }
            }
            return level[t] != -1;
        }

        long long dfs(int v, long long pushed) {
            if (pushed == 0)
                return 0;
            if (v == t)
                return pushed;
            for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
                int id = adj[v][cid];
                int u = edges[id].u;
                if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
                    continue;
                long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
                if (tr == 0)
                    continue;
                edges[id].flow += tr;
                edges[id ^ 1].flow -= tr;
                return tr;
            }
            return 0;
        }

        long long flow() {
            long long f = 0;
            while (true) {
                fill(level.begin(), level.end(), -1);
                level[s] = 0;
                q.push(s);
                if (!bfs())
                    break;
                fill(ptr.begin(), ptr.end(), 0);
                while (long long pushed = dfs(s, flow_inf)) {
                    f += pushed;
                }
            }
            return f;
        }
    };
};

int main() {
    int n = 6;  
    MaxFlowAlgorithms maxFlowAlgo(n);

   
    maxFlowAlgo.capacity[0][1] = 16;
    maxFlowAlgo.capacity[0][2] = 13;
    maxFlowAlgo.capacity[1][2] = 10;
    maxFlowAlgo.capacity[1][3] = 12;
    maxFlowAlgo.capacity[2][1] = 4;
    maxFlowAlgo.capacity[2][4] = 14;
    maxFlowAlgo.capacity[3][2] = 9;
    maxFlowAlgo.capacity[3][5] = 20;
    maxFlowAlgo.capacity[4][3] = 7;
    maxFlowAlgo.capacity[4][5] = 4;

    maxFlowAlgo.adj[0] = {1, 2};
    maxFlowAlgo.adj[1] = {2, 3};
    maxFlowAlgo.adj[2] = {1, 4};
    maxFlowAlgo.adj[3] = {2, 5};
    maxFlowAlgo.adj[4] = {3, 5};

    cout << "Max Flow (Edmonds-Karp BFS): " << maxFlowAlgo.maxFlow(0, 5) << endl;
     maxFlowAlgo.capacity[0][1] = 16;
    maxFlowAlgo.capacity[0][2] = 13;
    maxFlowAlgo.capacity[1][2] = 10;
    maxFlowAlgo.capacity[1][3] = 12;
    maxFlowAlgo.capacity[2][1] = 4;
    maxFlowAlgo.capacity[2][4] = 14;
    maxFlowAlgo.capacity[3][2] = 9;
    maxFlowAlgo.capacity[3][5] = 20;
    maxFlowAlgo.capacity[4][3] = 7;
    maxFlowAlgo.capacity[4][5] = 4;

    maxFlowAlgo.adj[0] = {1, 2};
    maxFlowAlgo.adj[1] = {2, 3};
    maxFlowAlgo.adj[2] = {1, 4};
    maxFlowAlgo.adj[3] = {2, 5};
    maxFlowAlgo.adj[4] = {3, 5};
    
    maxFlowAlgo.minCut(0, 5);
 
    MaxFlowAlgorithms maxFlowAlgoDFS(n);
    maxFlowAlgoDFS.capacity[0][1] = 16;
    maxFlowAlgoDFS.capacity[0][2] = 13;
    maxFlowAlgoDFS.capacity[1][2] = 10;
    maxFlowAlgoDFS.capacity[1][3] = 12;
    maxFlowAlgoDFS.capacity[2][1] = 4;
    maxFlowAlgoDFS.capacity[2][4] = 14;
    maxFlowAlgoDFS.capacity[3][2] = 9;
    maxFlowAlgoDFS.capacity[3][5] = 20;
    maxFlowAlgoDFS.capacity[4][3] = 7;
    maxFlowAlgoDFS.capacity[4][5] = 4;

    maxFlowAlgoDFS.adj[0] = {1, 2};
    maxFlowAlgoDFS.adj[1] = {2, 3};
    maxFlowAlgoDFS.adj[2] = {1, 4};
    maxFlowAlgoDFS.adj[3] = {2, 5};
    maxFlowAlgoDFS.adj[4] = {3, 5};

    cout << "Max Flow (Edmonds-Karp DFS): " << maxFlowAlgoDFS.maxFlowDFS(0, 5) << endl;

   
      MaxFlowAlgorithms::Dinic dinicFlow(n, 0, 5);
    dinicFlow.add_edge(0, 1, 16);
    dinicFlow.add_edge(0, 2, 13);
    dinicFlow.add_edge(1, 2, 10);
    dinicFlow.add_edge(1, 3, 12);
    dinicFlow.add_edge(2, 4, 14);
    dinicFlow.add_edge(3, 2, 9);
    dinicFlow.add_edge(3, 5, 20);
    dinicFlow.add_edge(4, 3, 7);
    dinicFlow.add_edge(4, 5, 4);

    cout << "Max Flow (Dinic's Algorithm): " << dinicFlow.flow() << endl;

    

    return 0;
}
