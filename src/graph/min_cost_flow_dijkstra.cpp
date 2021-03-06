//ダイクストラ

typedef pair<int, int > P; // firstは最短距離,secondは頂点の番号

// 辺を表す構造体（行き先、容量、コスト、逆辺）
struct edge { int to, cap, cost, rev; };

int V; // 頂点数
vector<edge> G[MAX_V]; // グラフの隣接リスト表現
int h[MAX_V]; // ポテンシャル
int dist[MAX_V]; // 最短距離
int prevv[MAX_V], preve[MAX_V]; // 直前の頂点と辺

// fromからtoへ向かう容量cap, コストcostの辺をグラフに追加する
void add_edge(int from, int to, int cap, int cost) {
    G[from].push_back((edge){to, cap, cost, G[to].size()});
    G[to].push_back((edge){from,0,-cost,G[from].size() - 1});
}

// sからtへの流量fの最小費用流を求める
// 流せない場合は-1を返す
int min_cost_flow(int s, int t, int f) {
    int res = 0;
    fill(h, h + V, 0); // hを初期化
    while(f > 0) {
        // ダイクストラ法を用いてhを更新する
        priority_queue<P, vector<P>, greater<P> > que;
        fill(dist, dist + V, INF);
        dist[s] = 0;
        que.push(P(0, s));
        while (!que.empty()) {
            P p = que.top(); que.pop();
            int v = p.second;
            if (dist[v] < p.first) continue;
            for(int i=0;i<G[v].size();i++) {
                edge &e = G[v][i];
                if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                    dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                    prevv[e.to] = v;
                    preve[e.to] = i;
                    que.push(P(dist[e.to], e.to));
                }
            }
        }
        if (dist[t] == INF) {
            // これ以上流せない
            return -1;
        }
        for(int v = 0; v < V; v++) h[v] += dist[v];

        // s-t間最短路に沿って目一杯流す
        int d = f;
        for(int v = t; v != s; v = prevv[v]) {
            d = min(d, G[prevv[v]][preve[v]].cap);
        }
        f -= d;
        res += d * h[t];
        for(int v = t; v != s; v = prevv[v]) {
            edge &e = G[prevv[v]][preve[v]];
            e.cap -= d;
            G[v][e.rev].cap += d;
        }
    }
    return res;
}
