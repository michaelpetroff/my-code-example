#include <iostream>
#include <map>
#include <string>

struct Node {
    std::map<char, Node> children;
    std::map<char, Node*> aut;
    bool is_terminal = false;
    Node* parent = nullptr, *suf = nullptr, *term = nullptr;
    char parent_c = 0;
    int toSearch = 0;

    Node() = default;
};

Node root;

void add(const std::string& input) {
    Node* step = &root;
    for (char c : input) {
        step->children[c].parent = step;
        step->children[c].parent_c = c;
        step = &step->children[c];
    }
    step->is_terminal = true;
}

Node* go(Node* nd, char c);

Node* get_suf(Node* nd) {
    if (!nd->suf) {
        if (!nd->parent || !nd->parent->parent)
            nd->suf = &root;
        else
            nd->suf = go(get_suf(nd->parent), nd->parent_c);
    }
    return nd->suf;
}

Node* go(Node* nd, char c) {
    if (nd->aut.find(c) == nd->aut.end()) {
        if (nd->children.find(c) != nd->children.end())
            nd->aut[c] = &nd->children[c];
        else
            nd->aut[c] = go(get_suf(nd), c);
    }
    return nd->aut[c];
}

Node* get_term(Node* nd) {
    if (!nd->term) {
        Node* step = get_suf(nd);
        while (step->parent && !step->is_terminal) step = get_suf(step);
        nd->term = step;
    }
    return nd->term;
}

bool is_loop(Node* cur) {
    if (cur->toSearch == 1) { cur->toSearch = 0; return true; }
    if (cur->toSearch == 2 || cur->is_terminal || get_term(cur)->parent) return false;
    cur->toSearch = 1;
    bool res = is_loop(go(cur, '0')) || is_loop(go(cur, '1'));
    cur->toSearch = 2;
    return res;
}

int main() {
    size_t n, max_length = 0;
    std::string name;
    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> name;
        if (name.length() > max_length) max_length = name.length();
        add(name);
    }
    std::cout << (is_loop(&root) ? "TAK\n" : "NIE\n");
}
