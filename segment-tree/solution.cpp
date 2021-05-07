#include <cmath>
#include <iostream>
#include <vector>

int left(int i) { return 2 * i + 1; }
int right(int i) { return 2 * i + 2; }

struct Node {
    long val = 0, mark = 0, length = 0;
    Node() = default;
    long sum() const { return val + mark * length; }

    Node operator+(const Node& other) const {
        Node res;
        res.val = sum() + other.sum();
        res.length = length + other.length;
        return res;
    }
};

void update(int cur, int reql, int reqr, long val, int indl, int indr, int newN, std::vector<Node>& tree) {
    if (indl >= reqr || reql >= indr) return;
    if (indl >= reql && reqr >= indr) { tree[cur].mark += val; return; }
    tree[left(cur)].mark += tree[cur].mark;
    tree[right(cur)].mark += tree[cur].mark;
    tree[cur].mark = 0;
    int mid = (indl + indr) / 2;
    update(left(cur), reql, reqr, val, indl, mid, newN, tree);
    update(right(cur), reql, reqr, val, mid, indr, newN, tree);
    tree[cur] = tree[left(cur)] + tree[right(cur)];
}

long compute(int cur, int reql, int reqr, int indl, int indr, std::vector<Node>& tree) {
    if (indl >= reqr || reql >= indr) return 0;
    if (indl >= reql && reqr >= indr) return tree[cur].sum();
    tree[left(cur)].mark += tree[cur].mark;
    tree[right(cur)].mark += tree[cur].mark;
    tree[cur] = tree[left(cur)] + tree[right(cur)];
    int mid = (indl + indr) / 2;
    return compute(left(cur), reql, reqr, indl, mid, tree) +
    compute(right(cur), reql, reqr, mid, indr, tree);
}

int main() {
    int q, req, leftr, rightr, n;
    long summand;
    std::cin >> n;
    int k0 = std::ceil(std::log2(n));
    int newN = 1 << k0;
    std::vector<long> data(newN);
    std::vector<Node> tree(2 * newN - 1);
    for (int i = 0; i < n; ++i) std::cin >> data[i];
    for (int i = newN - 1; i < 2 * newN - 1; ++i) {
        tree[i].val = data[i - newN + 1];
        tree[i].length = 1;
    }
    for (int i = newN - 2; i >= 0; --i) tree[i] = tree[left(i)] + tree[right(i)];
    std::cin >> q;
    for (int i = 0; i < q; ++i) {
        std::cin >> req >> leftr >> rightr;
        if (req == 1) {
            std::cout << compute(0, leftr, rightr + 1, 0, newN, tree) << '\n';
        } else {
            std::cin >> summand;
            update(0, leftr, rightr + 1, summand, 0, newN, newN, tree);
        }
    }
}
