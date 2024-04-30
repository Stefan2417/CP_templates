struct Dinic {
    const int MX = 1 << 30;

    struct edge {
        int to, c, fl;

        edge() : to(0), c(0), fl(0) {
        }

        edge(int to, int c, int fl) : to(to), c(c), fl(fl) {

        }
    };

    vector<edge> edges;

    int n;
    vector<vector<int>> g;
    vector<int> used;
    vector<int> pt;
    vector<int> dist;
    int T = 1;

    Dinic(int n_) {
        n = n_;
        g.resize(n);
        used.resize(n);
        pt.resize(n);
        dist.resize(n);
    }

    bool bfs(int s, int lb) {
        fill(dist.begin(), dist.end(), -1);
        dist[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int i: g[v]) {
                if (dist[edges[i].to] == -1 && edges[i].c - edges[i].fl >= lb) {
                    dist[edges[i].to] = dist[v] + 1;
                    q.push(edges[i].to);
                }
            }
        }
        return dist[n - 1] != -1;
    }

    int dfs(int v, int push, int lb) {
        if (v == n - 1) return push;
        used[v] = T;
        for (int i = pt[v]; i < g[v].size(); i++) {
            if (used[edges[g[v][i]].to] != T && dist[v] + 1 == dist[edges[g[v][i]].to] &&
                edges[g[v][i]].c - edges[g[v][i]].fl >= lb) {
                int add = dfs(edges[g[v][i]].to, min(push, edges[g[v][i]].c - edges[g[v][i]].fl), lb);
                if (add > 0) {
                    edges[g[v][i]].fl += add;
                    edges[g[v][i] ^ 1].fl -= add;
                    return add;
                }
            }
            pt[v]++;
        }
        return 0;
    }

    void make_edge(int a, int b, int cap, bool dir = true) {
        g[a].emplace_back(edges.size());
        edges.emplace_back(b, cap, 0);
        g[b].emplace_back(edges.size());
        edges.emplace_back(a, (dir ? 0 : cap), 0);
    }

    long long build_flow(int upper = 1) {
        long long res = 0;
        while (upper >= 1) {
            while (bfs(0, upper)) {
                for (int i = 0; i < n; i++) pt[i] = 0;
                while (true) {
                    T++;
                    int cur = dfs(0, MX, upper);
                    res += cur;
                    if (cur == 0) break;
                }
            }
            upper >>= 1;
        }
        return res;
    }

    struct dflow {
        int fl;
        vector<int> edges;
    };

    vector<int> st;

    int dfsik(int v, int f) {
        if (v == n - 1) return f;
        for (int id: g[v]) {
            if (edges[id].fl > 0 && edges[id].c > 0) {
                st.pb(id / 2);
                int add = dfsik(edges[id].to, min(f, edges[id].fl));
                edges[id].fl -= add;
                return add;
            }
        }
        return 0;
    }

    vector<dflow> decompose() {
        vector<dflow> ans;
        while (true) {
            st.clear();
            int cur = dfsik(0, MOD);
            if (cur == 0) break;
            ans.pb({cur, st});
        }
        return ans;
    }

    void dfs_coloring(int v) {
        used[v] = 1;
        for (auto i: g[v]) {
            if (!used[edges[i].to] && edges[i].fl < edges[i].c)
                dfs_coloring(edges[i].to);
        }
    }

    vector<int> build_cut() {
        fill(used.begin(), used.end(), 0);
        dfs_coloring(0);
        vector<int> res;
        for (int i = 0; i < n; i++)
            for (auto j: g[i]) {
                if (used[i] && !used[edges[j].to]) {
                    res.pb(j / 2);
                }
            }
        sort(res.begin(), res.end());
        return res;
    }

};
