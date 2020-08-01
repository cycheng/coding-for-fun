// https://www.facebook.com/codingcompetitions/hacker-cup/2020/qualification-round/problems/D2
#include <bits/stdc++.h>
using namespace std;

struct cost {
  uint64_t totalcost = 0;
  // index to tree node.
  int idx = 0;
  cost() = default;
  cost(uint64_t c, int idx) : totalcost(c), idx(idx) {}
};

struct node {
  int parent = 0;
  int child = 0;
  int sibling = 0;
  // if the node is a branch node, then which main node it belongs to (under
  // which main node's subtree)?
  int main = 0;
  // If the node is main node, dist = A to this node.
  // If the node is branch node, dist = this node to main node
  int dist = 0;
  // node(int p, int s) : parent(p), sibling(s) {}
  node() = default;
};

#if 0
void calculate_dist(const vector<node> &tree, vector<int> &dist,
                    vector<bool> &visited, int a, int b) {
  vector<int> worklist;
  fill(visited.begin(), visited.end(), false);
  worklist.reserve(tree.size());
  worklist.push_back(a);
  // never visit null.
  visited[0] = true;
  // bfs build.
  int curdist = 0, levelindex = 1;
  for (int i = 0; i < worklist.size(); ++i) {
    if (i == levelindex) {
      levelindex = worklist.size();
      curdist++;
    }
    const int nid = worklist[i];
    const node &n = tree[nid];
    dist[nid] = curdist;
    visited[nid] = true;

    const int pid = tree[nid].parent;
    if (!visited[pid])
      worklist.push_back(pid);

    int cid = n.child;
    while (cid) {
      if (!visited[cid])
        worklist.push_back(cid);
      cid = tree[cid].sibling;
    }
  }
}
#endif

void set_dist_for_branch_descendants(const int main, vector<node> &tree,
                                     vector<bool> &visited, int start) {
  // bfs
  vector<int> worklist;
  // heuristic
  worklist.reserve(16);
  worklist.push_back(start);

  int dist = 1, levelindex = 1;
  for (int i = 0; i < (int)worklist.size(); ++i) {
    if (i == levelindex) {
      levelindex = worklist.size();
      dist++;
    }

    int id = worklist[i];
    tree[id].main = main;
    tree[id].dist = dist;
    visited[id] = true;

    const int pid = tree[id].parent;
    if (!visited[pid])
      worklist.push_back(pid);

    int cid = tree[id].child;
    while (cid) {
      if (!visited[cid])
        worklist.push_back(cid);
      cid = tree[cid].sibling;
    }
  }
}

void find_main_node_and_cal_dist(const vector<bool> &is_main_node,
                                 vector<node> &tree, vector<bool> &visited,
                                 int a, int b) {
  fill(visited.begin(), visited.end(), false);
  // never visit null, and node a is of course on the main road.
  visited[0] = true;

  int curmain = a; //, prevmain = 0;
  int dist = 0;
  while (curmain != b) {
    int nextmain = 0;
    assert(is_main_node[curmain]);

    tree[curmain].dist = dist;
    tree[curmain].main = curmain;
    visited[curmain] = true;

    int pid = tree[curmain].parent;
    if (is_main_node[pid]) {
      // if (pid != prevmain)
      if (!visited[pid])
        nextmain = pid;
    } else {
      set_dist_for_branch_descendants(curmain, tree, visited, pid);
    }

    int cid = tree[curmain].child;
    while (cid) {
      if (is_main_node[cid]) {
        if (!visited[cid]) {
          assert(nextmain == 0);
          nextmain = cid;
        }
      } else {
        set_dist_for_branch_descendants(curmain, tree, visited, cid);
      }
      cid = tree[cid].sibling;
    }
    assert(nextmain);
    curmain = nextmain;
    ++dist;
  }
  visited[b] = true;
  tree[b].main = b;
  tree[b].dist = dist;
}

// road_marker[i] = 'true' means node i is on the main road.
//                  'false' means on the branch road.
void mark_road(const vector<node> &tree, vector<bool> &road_marker,
               vector<bool> &visited, int a, int b) {
  fill(visited.begin(), visited.end(), false);
  // never visit null, and node a is of course on the main road.
  visited[0] = true;
  road_marker[a] = true;

  // <current node, who (worklist id) visit this node>
  vector<pair<int, int>> worklist;
  worklist.reserve(tree.size());
  worklist.emplace_back(a, 0);
  // bfs.
  int curdist = 0, levelindex = 1;
  int b_in_worklist = 0;
  for (int i = 0; i < (int)worklist.size(); ++i) {
    if (i == levelindex) {
      levelindex = worklist.size();
      curdist++;
    }
    const int nid = worklist[i].first;
    if (nid == b) {
      b_in_worklist = i;
      break;
    }
    const node &n = tree[nid];
    visited[nid] = true;

    const int pid = tree[nid].parent;
    if (!visited[pid])
      worklist.emplace_back(pid, i);

    int cid = n.child;
    while (cid) {
      if (!visited[cid])
        worklist.emplace_back(cid, i);
      cid = tree[cid].sibling;
    }
  }

  assert(b_in_worklist != 0);
  // because a_in_worklist is '0'
  while (b_in_worklist != 0) {
    int nid = worklist[b_in_worklist].first;
    road_marker[nid] = true;
    b_in_worklist = worklist[b_in_worklist].second;
  }
}

int64_t solve() {
  int n, m, a, b;
  cin >> n >> m >> a >> b;

  // Build the tree.
  // 0 is null.
  vector<int> costs(n + 1);
  vector<node> tree(n + 1);
  for (int i = 1; i <= n; ++i) {
    int parent;
    cin >> parent >> costs[i];
    tree[i].parent = parent;
    if (tree[parent].child == 0)
      tree[parent].child = i;
    else {
      int child = tree[parent].child;
      tree[i].sibling = child;
      tree[parent].child = i;
    }
  }

  vector<bool> visited(n + 1);

  // // Build distance in two directions: A -> B and B <- A
  // vector<int> // dista_to_b(n + 1) ,
  //     distb_to_a(n + 1);
  // // calculate_dist(tree, dista_to_b, a, b);
  // calculate_dist(tree, distb_to_a, visited, b, a);

  // Mark main and branch path.
  vector<bool> is_main_road(n + 1);
  mark_road(tree, is_main_road, visited, a, b);

  find_main_node_and_cal_dist(is_main_road, tree, visited, a, b);

  auto greaterCost = [](const cost &a, const cost &b) {
    return a.totalcost > b.totalcost;
  };
  // top is the smallest cost.
  priority_queue<cost, vector<cost>, decltype(greaterCost)> pq(greaterCost);
  uint64_t curcost = 0;
  // traverse a to b in bfs way.
  {
    // <nid, current m>
    vector<pair<int, int>> worklist;
    fill(visited.begin(), visited.end(), false);
    visited[0] = true;

    worklist.reserve(tree.size());

    worklist.emplace_back(a, m + 1);
    // never visit null.
    visited[0] = true;
    // bfs build.
    int // curdist = 0,
        levelindex = 1;
    for (int i = 0; i < (int)worklist.size(); ++i) {
      if (i == levelindex) {
        levelindex = worklist.size();
      }

      const int nid = worklist[i].first;
      int curm = worklist[i].second - 1;
      const node &n = tree[nid];
      visited[nid] = true;

      // tree, distb_to_a, is_main_road
      if (curm < 0) {
        int dist = 0;
        while (!pq.empty()) {
          const int tid = pq.top().idx;
          // calculate distance from tid -> nid
          int t_dist_to_main = 0;
          if (!is_main_road[tid])
            t_dist_to_main = tree[tid].dist;

          int n_dist_to_main = 0;
          if (!is_main_road[nid])
            n_dist_to_main = tree[nid].dist;

          if (tree[tid].main == tree[nid].main)
            dist = abs(t_dist_to_main - n_dist_to_main);
          else
            dist = t_dist_to_main + n_dist_to_main;

          dist += abs(tree[tree[tid].main].dist - tree[tree[nid].main].dist);

          if (dist <= m)
            break;

          if (!is_main_road[nid])
            break;
          pq.pop();
        }

        if (pq.empty())
          return -1;
        curcost = pq.top().totalcost;
        curm = m - dist;
      }

      // cout << " curcost " << curcost << endl;

      if (nid == b)
        return curcost;

      if (costs[nid] && curm >= 0)
        pq.emplace(curcost + costs[nid], nid);

      const int pid = tree[nid].parent;
      if (!visited[pid])
        worklist.emplace_back(pid, curm);

      int cid = n.child;
      while (cid) {
        if (!visited[cid])
          worklist.emplace_back(cid, curm);
        cid = tree[cid].sibling;
      }
    }
  }
  return curcost;
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    cout << "Case #" << i << ": " << solve();
    cout << endl;
  }
  return 0;
}