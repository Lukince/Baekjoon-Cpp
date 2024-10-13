#include <iostream>
#include <vector>
#include <algorithm>

#define ll long long

using namespace std;

struct pos_t {
    constexpr pos_t() : pos_t(0, 0) { }
    constexpr pos_t(int a, int b)
        : x(a), y(b) { }
    ll x, y;

    constexpr bool cmp(pos_t pt) {
        if (this->x == pt.x) return this->y <= pt.y;
        else return this->x <= pt.x;
    }

    constexpr bool operator==(pos_t p) { return this->x == p.x && this->y == p.y; }
};

constexpr ll ccw(pos_t s, pos_t a, pos_t b) {
    return 1LL * (a.x - s.x) * (b.y - s.y) - 1LL * (b.x - s.x) * (a.y - s.y);
}

template<typename ForwardIterator>
vector<pos_t> cvh(ForwardIterator _s, ForwardIterator _e) {
    vector<pos_t> arr(_s, _e);

    sort(arr.begin(), arr.end(), [](const pos_t& p1, const pos_t& p2) {
        if (p1.x != p2.x) return p1.x < p2.x;
        else return p1.y < p2.y;
    });

    vector<pos_t> top, bottom;
    
    for (auto b = arr.begin(), e = arr.end(); b != e; b++) {
        while (top.size() >= 2 && ccw(top[top.size() - 2], top[top.size() - 1], *b) >= 0) top.pop_back();

        top.push_back(*b);
    }

    for (auto b = arr.rbegin(), e = arr.rend(); b != e; b++) {
        while (bottom.size() >= 2 && ccw(bottom[bottom.size() - 2], bottom[bottom.size() - 1], *b) >= 0) bottom.pop_back();

        bottom.push_back(*b);
    }

    top.pop_back();
    top.insert(top.end(), bottom.begin(), bottom.end() - 1);

    return top;
}

constexpr int zip(ll res) { return res < 0 ? -1 : !!res; }

constexpr bool iscross(pos_t a, pos_t b, pos_t x, pos_t y) {
    int p = zip(ccw(y, x, a)) * zip(ccw(y, x, b)),
        q = zip(ccw(a, b, y)) * zip(ccw(a, b, x));
    
    if (!p & !q) {
        if (!a.cmp(b)) swap(a, b);
        if (!x.cmp(y)) swap(x, y);

        return a.cmp(y) && x.cmp(b);
    }

    return p <= 0 && q <= 0;
}

bool in(const vector<pos_t>& v, pos_t p) {
    for (int i = 0; i < v.size(); i++) {
        pos_t p1 = v[i], p2 = v[(i + 1) % v.size()];

        if (ccw(p1, p2, p) > 0) { return false; }
    }

    return true;
}

bool loop = true;

bool solve() {
    int n, m; cin >> n >> m;

    if (n == 0 && m == 0) { exit(0); }

    vector<pos_t> _b, _w;

    for (int i = 0; i < n; i++) {
        int a, b, c, d; cin >> a >> b >> c >> d;

        _b.emplace_back(a, b);
        _b.emplace_back(c, d);
        _b.emplace_back(a, d);
        _b.emplace_back(c, b);
    }

    for (int i = 0; i < m; i++) {
        int a, b, c, d; cin >> a >> b >> c >> d;

        _w.emplace_back(a, b);
        _w.emplace_back(c, d);
        _w.emplace_back(a, d);
        _w.emplace_back(c, b);
    }

    vector<pos_t>
        b = cvh(_b.begin(), _b.end()),
        w = cvh(_w.begin(), _w.end());
    
    for (int i = 0; i < b.size(); i++) {
        pos_t b1 = b[i], b2 = b[(i + 1) % b.size()];
        for (int j = 0; j < w.size(); j++) {
            pos_t w1 = w[j], w2 = w[(j + 1) % w.size()];

            if (iscross(b1, b2, w1, w2)) {
                return false;
            }
        }
    }

    bool i1 = in(b, w[0]), i2 = in(w, b[0]);

    return !(i1 || i2);
}

int main() {
    cin.tie(nullptr); ios_base::sync_with_stdio(false);

    int n = 1;
    while (loop) {
        bool c = solve();
        cout << "Case " << n << ": It is " << (c ? "" : "not ") << "possible to separate the two groups of vendors.\n\n";
        n++;
    }
}