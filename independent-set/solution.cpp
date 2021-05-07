#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

std::vector<std::set<short>> graph;

struct SizeCompare {
    bool operator()(short lhs, short rhs) const {
        if (graph[lhs].size() != graph[rhs].size())
            return graph[lhs].size() < graph[rhs].size();
        return lhs < rhs;
    }
};

bool Try(std::map<short, std::set<short>*, SizeCompare>::const_reverse_iterator cur,
         std::set<short> included, short s, int step, short max_step) {
    if (included.size() == s) return true;
    if (step - included.size() > max_step - s) return false;
    bool t1 = Try(next(cur), included, s, step + 1, max_step);
    if (t1) return true;
    bool isInd = true;
    for (short neigh : *(cur->second))
        if (included.find(neigh) != included.end()) {
            isInd = false; break;
        }
    if (!isInd) return false;
    included.insert(cur->first);
    return Try(next(cur), included, s, step + 1, max_step);
}

int main() {
    short v, s, e;
    std::cin >> v >> s >> e;
    if (s > v) {std::cout << "NO"; return 0;}
    graph.resize(v);
    std::map<short, std::set<short>*, SizeCompare> traversal;
    for (short i = 0; i < e; ++i) {
        int first, second;
        std::cin >> first >> second;
        --first, --second;
        graph[first].insert(second);
        graph[second].insert(first);
    }
    for (short i = 0; i < v; ++i) {
        traversal[i] = &graph[i];
    }
    std::set<short> included;
    std::cout << (Try(traversal.rbegin(), included, s, 0, v) ? "YES" : "NO");
}
