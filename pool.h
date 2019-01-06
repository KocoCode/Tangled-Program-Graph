#pragma once
#include <vector>
#include <random>
#include <unordered_map>
#include <utility>

using std::default_random_engine;
using std::forward;
using std::pair;
using std::swap;
using std::vector;
using std::uniform_int_distribution;
using std::unordered_map;

class RandomizedSet {
private:
    vector<int> v;
    unordered_map<int, int> m;
    default_random_engine gen;
    uniform_int_distribution<int> uniform;

public:
    void insert(int val) {
        if (m.count(val)) return;
        m[val] = v.size();
        v.push_back(val);
    }

    void remove(int val) {
        if (m.count(val) == 0) return;
        int back = v.back();
        swap(v[m[val]], v.back());
        m[back] = m[val];
        m.erase(val);
        v.pop_back();
    }

    int random() {
        return v[uniform(gen, decltype(uniform)::param_type(0, v.size() - 1))];
    }

    int size() {
        return m.size();
    }

    bool find(int id) {
        return m.find(id) != m.end();
    }

    vector<int>::const_iterator begin() const {
        return v.begin();
    }

    vector<int>::const_iterator end() const {
        return v.end();
    }
};

template <class T>
class BidderPool {
private:
    RandomizedSet rs;
    int count = 0;
    unordered_map<int, T> pool;

public:
    int size() {
        return pool.size();
    }

    template <class T_ = T>
    int insert(T_&& val) {
        rs.insert(count);
        val.setId(count);
        pool.insert({count, forward<T_>(val)});
        return count++;
    }

    void remove(int id) {
        rs.remove(id);
        pool.erase(id);
    }

    int random() {
        return rs.random();
    }

    T& get(int id) {
        return pool[id];
    }

    typename unordered_map<int, T>::iterator begin() {
        return pool.begin();
    }

    typename unordered_map<int, T>::iterator end() {
        return pool.end();
    }
};

template <class T>
class TeamPool {
private:
    RandomizedSet rs, root_rs;
    int count = 0;
    unordered_map<int, T> pool;

public:
    int size() {
        return pool.size();
    }

    int rootSize() {
        return root_rs.size();
    }

    template <class T_ = T>
    int insert(T_&& val) {
        rs.insert(count);
        val.setId(count);
        pool.insert({count, forward<T_>(val)});
        return count++;
    }

    int random() {
        return rs.random();
    }

    int randomRootTeam() {
        return root_rs.random();
    }

    T& get(int id) {
        return pool[id];
    }

    void remove(int id) {
        rs.remove(id);
        root_rs.remove(id);
        pool.erase(id);
    }

    void addRootTeam(int id) {
        root_rs.insert(id);
    }

    void removeRootTeam(int id) {
        root_rs.remove(id);
    }

    const RandomizedSet& teams() {
        return rs;
    }

    const RandomizedSet& rootTeams() {
        return root_rs;
    }
};
