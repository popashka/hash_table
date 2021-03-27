#include<iostream>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <cstdlib>
#include <functional>

class hsh {
public:
    size_t operator()(int key) {
        return 1;
    }
};

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
public:
    Hash hasher;
    const int N = 10007;
    int md = 10007;
    size_t sz = 0;
    std::vector<std::vector <std::pair <const KeyType, ValueType>>> v;
    //std::vector<std::vector <const std::pair <KeyType, const ValueType>>> cv;
    std::vector <std::pair <const KeyType, ValueType>> vv;
    //std::vector <const std::pair <const KeyType, ValueType>> cvv;
    std::vector <int> id;

    //Constructor




    HashMap(Hash h) : hasher(h), id(N, -1) {
    }

    HashMap() : id(N,  -1){
    }


    HashMap(std::vector<std::pair< KeyType, ValueType>> data) : id(N, -1) {
        for (auto x: data) {
            insert(x);
        }
    }
    HashMap(std::vector<std::pair< const KeyType, ValueType>> data) : id(N, -1) {
        for (auto x: data) {
            insert(x);
        }
    }
    template <typename Iter>
    HashMap(Iter it1, Iter it2) : id(N, -1){
        while (it1 != it2) {
            insert(*it1);
            it1++;
        }
    }
    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> lst) : id(N, -1){
        for (auto x: lst) {
            insert(x);
        }
    }

    template <typename Iter>
    HashMap(Iter it1, Iter it2, Hash h) : id(N, -1), hasher(h){
        while (it1 != it2) {
            insert(*it1);
            it1++;
        }
    }
    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> lst, Hash h) : hasher(h), id(N, -1){
        for (auto x: lst) {
            insert(x);
        }
    }

    //End-of-constructor

    void erase(KeyType key) {
        if (find(key) == end()) {
            return;
        }
        size_t num = hasher(key);
        num = num % md;
        if (num < 0) {
            num += md;
        }
        int was = num;
        num = id[num];
        sz--;
        //cout << "NUM " << num << endl;
        std::vector <std::pair <const KeyType, ValueType>> w = {};
        std::vector <std::pair <const KeyType, const ValueType>> cw = {};
        for (int i = 0; i < v[num].size(); i++) {
            if (v[num][i].first != key) {
                w.push_back(v[num][i]);
                //cw.push_back(cv[num][i]);
            }
        }
        //v[num].clear();
        //cv[num].clear();
        if (w.size() > 0){
            v[num].clear();
            for (int i = 0; i < w.size(); i++) {
                v[num].push_back(w[i]);
            }
        } else {
            if (num == v.size() - 1) {
                v.pop_back();
                id[was] = -1;
            } else {
                v[num].clear();
                id[was] = -1;
                size_t nm = hasher(v[v.size() - 1][0].first);
                nm = nm % md;
                id[nm] = num;
                for (int i = 0; i < v[v.size() - 1].size(); i++) {
                    v[num].push_back(v[v.size() - 1][i]);
                }
                v.pop_back();
                //swap(cv[num], cv.back());
                //cv.pop_back();
                //cout << v.size() << endl;
            }
        }
    }

    struct iterator{
        /*int lf(const KeyType p) {
            if (p == 1 || p == 3) {
                return 0;
            } else {
                return 1;
            }
        }*/
        iterator() = default;
        friend HashMap;
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::pair<const KeyType, ValueType>;
        using pointer           = std::pair<const KeyType, ValueType>*;
        using reference         = std::pair<const KeyType, ValueType>&;

        int lf(KeyType key) {
            size_t num = hasher(key);
            num = num % md;
            return idd[num];
        }

        iterator(pointer ptr, std::vector<std::vector <std::pair <const KeyType, ValueType>>> &v, std::vector<int> &iddd, Hash &hasher) : m_ptr(ptr),
                                        w(&v), idd(iddd), hasher(hasher){}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        iterator operator-() {m_ptr++; return *this;}

        iterator& operator=(const iterator &other) {
            m_ptr = other.m_ptr;
            return *this;
        }

        //iterator begin() { return iterator(&*(v[0].begin()), v); }
        //iterator end()   { return iterator(&*(v[v.size()-1].end()), v); }

        iterator& operator++() {
            //cout << &*((*w)[(*w).size() - 1].end()) << endl;
            int k = lf(m_ptr->first);
            //cout << k << endl;
            if (m_ptr != &(*w)[k].back()) {
                m_ptr++;
                return *this;
            }
            if (k == (*w).size() - 1) {
                m_ptr++;
                return *this;
            }
            m_ptr = &*(*w)[k+1].begin();
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this; ++(*this); return tmp;
        }

        friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
        Hash hasher;
        std::vector <std::vector <std::pair <const KeyType, ValueType>>>* w;
        std::vector <int> idd;
        int md = 10007;
    };


    iterator begin() {
        if (empty()) {
            return end();
        }
        return iterator(&*(v[0].begin()), v, id, hasher);
    }
    iterator end() {
        if (empty()) {
            return iterator(&*vv.end(), v, id, hasher);
        }
        return iterator(&*(v[v.size() - 1].end()), v, id, hasher);
    }

    struct const_iterator  {
        /*int lf(const KeyType p) {
            if (p == 1 || p == 3) {
                return 0;
            } else {
                return 1;
            }
        }*/
        const_iterator() = default;
        friend HashMap;
        using iterator_category = std::forward_iterator_tag;
        //using difference_type   = std::ptrdiff_t;
        using value_type        = const std::pair<const KeyType, ValueType>;
        using pointer           = value_type*;
        using reference         = value_type&;

        int lf(KeyType key) {
            size_t num = hasher(key);
            num = num % md;
            return idd[num];
        }

        const_iterator(const std::pair<const KeyType, ValueType>* ptr,
                       const std::vector<std::vector < std::pair <const KeyType, ValueType>>> &v, const std::vector<int> &iddd,
                       const Hash &hasher) : pt(ptr),
        w(&v), idd(iddd), hasher(hasher){
            m_ptr = pt;
        }

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        const_iterator& operator=(const const_iterator &other) {
            m_ptr = other.m_ptr;
            return *this;
        }

        //iterator begin() { return iterator(&*(v[0].begin()), v); }
        //iterator end()   { return iterator(&*(v[v.size()-1].end()), v); }

        const_iterator& operator++() {
            //cout << &*((*w)[(*w).size() - 1].end()) << endl;
            int k = lf(m_ptr->first);
            //cout << k << endl;
            if (m_ptr != &(*w)[k].back()) {
                m_ptr++;
                return *this;
            }
            if (k == (*w).size() - 1) {
                m_ptr++;
                return *this;
            }
            m_ptr = &*(*w)[k+1].begin();
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this; ++(*this); return tmp;
        }

        friend bool operator== (const const_iterator& a, const const_iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const const_iterator& a, const const_iterator& b) { return a.m_ptr != b.m_ptr; };

    private:

        const std::pair<const KeyType, ValueType>* pt;
        pointer m_ptr;
        const std::vector <std::vector <std::pair <const KeyType, ValueType>>>* w;
        const std::vector <int> idd;
        Hash hasher;
        int md = 10007;

    };


    const_iterator begin() const {
        if (sz == 0) {
            return end();
        }
        return const_iterator(&*v[0].begin(), v, id, hasher);
    }
    const_iterator end() const {
        if (sz == 0) {
            const_iterator(&*v[0].end(), v, id, hasher);
        }
        return const_iterator(&*(v[v.size() - 1].end()), v, id, hasher);
    }

    const_iterator find(const KeyType key) const {
        size_t num = hasher(key);
        num = num % md;
        if (id[num] == -1) {
            return end();
        }
        num = id[num];
        for (int i = 0; i < v[num].size(); i++) {
            if (v[num][i].first == key) {
                return const_iterator(&v[num][i], v, id, hasher);
            }
        }
        return end();
    }

    iterator find(const KeyType key) {
        size_t num = hasher(key);
        num = num % md;
        if (id[num] == -1) {
            return  end();
        }
        num = id[num];
        for (int i = 0; i < v[num].size(); i++) {
            if (v[num][i].first == key) {
                return iterator(&v[num][i], v, id, hasher);
            }
        }
        return end();
    }

    void insert(std::pair <const KeyType, ValueType> p) {
        std::pair <const KeyType, const ValueType> pp = p;
        if (find(p.first) != end()) {
            return;
        }
        sz++;
        KeyType key = p.first;
        size_t num = hasher(key);
        num = num % md;
        if (id[num] == -1) {
            id[num] = v.size();
            v.push_back({{p}});
            //cv.push_back({{pp}});
        } else {
            v[id[num]].push_back(p);
            //cv[id[num]].push_back(pp);
        }
    }

    size_t size() const {
        return sz;
    }
    bool empty() const {
        if (sz != 0) {
            return 0;
        } else {
            return 1;
        }
    }

    void clear() {
        sz = 0;
        for (int i = 0; i < v.size(); i++) {
            KeyType k = v[i][0].first;
            size_t num = hasher(k);
            num %= md;
            id[num] = -1;
        }
        v.clear();
        //cv.clear();
    }

    class default_t {
    public:
        operator ValueType() const { return ValueType(); }
    };



    ValueType& operator[](KeyType key) {
        default_t const empty = default_t();

        size_t num = hasher(key);
        num %= md;
        if (id[num] != -1) {
            num = id[num];
            for (int i = 0; i < v[num].size(); i++) {
                if (v[num][i].first == key) {
                    return v[num][i].second;
                }
            }
        }
        insert({key, empty});
        auto it = find(key);
        return (*it).second;
    }

    const ValueType& at(KeyType key) const {
        size_t num = hasher(key);
        num %= md;
        if (id[num] != -1) {
            num = id[num];
            for (int i = 0; i < v[num].size(); i++) {
                if (v[num][i].first == key) {
                    return v[num][i].second;
                }
            }
        }
        throw std::out_of_range{"No such elem!!"};
    }

    Hash hash_function() const{
        return hasher;
    }

    HashMap& operator=(const HashMap &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        for (int i = 0; i < other.v.size(); i++) {
            v.push_back(other.v[i]);
            //cv.push_back(other.cv[i]);
            KeyType k = v[i][0].first;
            size_t num = hasher(k);
            num %= md;
            id[num] = i;
        }
        sz = other.sz;
        hasher = other.hasher;
        return *this;
    }

};

int main() {

    /*ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);*/
    //freopen("input.txt", "r", stdin);
    int n;
    std::cin >> n;
    HashMap <int, int> map;
    for (int i = 1; i <= n; i++) {
        char c;
        std::cin >> c;
        if (c == '+') {
            int k, v;
            std::cin >> k >> v;
            map.insert({k, v});
        }
        if (c == '-') {
            int k;
            std::cin >> k;
            map.erase(k);
        }
        if (c == '?') {
            int k;
            std::cin >> k;
            auto it = map.find(k);
            if (it != map.end()) {
                std::cout << it->second << "\n";
            } else {
                std::cout << -1 << "\n";
            }
        }
        if (c == '!') {
            map.clear();
        }
        if (c == '<') {
            for (auto it = map.begin(); it != map.end(); it++) {
                std::cout << it->first << " " << it->second << "\n";
            }
        }
    }

    std::cout << map.size() << "\n";
    return 0;
}



