#pragma once
#include <vector>
#include <random>
#include <unordered_map>
#include <utility>

using std::default_random_engine;
using std::enable_if_t;
using std::forward;
using std::pair;
using std::swap;
using std::vector;
using std::uniform_int_distribution;
using std::unordered_map;

class RandomizedSet {
private:
    vector<long long> v;
    unordered_map<long long, long long> m;
    default_random_engine gen;
    uniform_int_distribution<long long> uniform;

public:
    void insert(long long val) {
        if (m.count(val)) return;
        m[val] = v.size();
        v.push_back(val);
    }

    void remove(long long val) {
        if (m.count(val) == 0) return;
        long long back = v.back();
        swap(v[m[val]], v.back());
        m[back] = m[val];
        m.erase(val);
        v.pop_back();
    }

    long long random() {
        return v[uniform(gen, decltype(uniform)::param_type(0, v.size() - 1))];
    }

    int size() {
        return m.size();
    }
};

template<class, class=void>
struct is_isRoot_defined
    : std::false_type {};

template<class T>
struct is_isRoot_defined<T, decltype(void(std::declval<T>().isRoot()))>
    : std::true_type {};

template<class, class=void>
struct is_setRoot_defined
    : std::false_type {};

template<class T>
struct is_setRoot_defined<T, decltype(void(std::declval<T>().setRoot(false)))>
    : std::true_type {};

template <class T>
class Pool {
private:
    RandomizedSet rs;
    typename std::conditional<is_isRoot_defined<T>::value, RandomizedSet, nullptr_t>::type root_rs;
    long long count = 0;
    unordered_map<long long, T> pool;
    vector<long long> gc;

public:
    long long size() {
        return pool.size();
    }

    template <class T_ = T>
    std::enable_if_t<is_isRoot_defined<T_>::value, long long> rootTeamSize() {
        return root_rs.size();
    }

    template <class T_>
    std::enable_if_t<is_isRoot_defined<T_>::value> insert(T_&& val) {
        if (val.isRoot()) {
            root_rs.insert(count);
        }
        rs.insert(count);
        val.setId(count);
        pool.insert({count, forward<T_>(val)});
        count++;
    }

    template <class T_>
    std::enable_if_t<!is_isRoot_defined<T_>::value> insert(T_&& val) {
        rs.insert(count);
        val.setId(count);
        pool.insert({count, forward<T_>(val)});
        count++;
    }

    template <class T_ = T>
    std::enable_if_t<is_setRoot_defined<T_>::value> addRootTeam(long long id) {
        pool[id].setRoot(true);
        root_rs.insert(id);
    }

    pair<long long, T&> random() {
        long long id = rs.random();
        return {id, pool[id]};
    }

    template <class T_ = T>
    std::enable_if_t<is_isRoot_defined<T_>::value, pair<long long, T&>> randomRootTeam() {
        long long id = root_rs.random();
        return {id, pool[id]};
    }

    T& get(long long id) {
        return pool[id];
    }

    void remove(long long id) {
        if constexpr(is_isRoot_defined<T>::value) {
            root_rs.remove(id);
        }
        gc.push_back(id);
        rs.remove(id);
    }

    template <class T_ = T>
    std::enable_if_t<is_setRoot_defined<T_>::value> removeRootTeam(long long id) {
        pool[id].setRoot(false);
        root_rs.remove(id);
    }

    void maintain() {
        for (auto id : gc) {
            pool.erase(id);
        }
        gc.clear();
    }
};
