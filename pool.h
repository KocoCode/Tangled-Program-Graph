#include <vector>
#include <random>
#include <unordered_map>
#include <utility>

using std::default_random_engine;
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
        m[val] = v.size();
        v.push_back(val);
    }

    void remove(long long val) {
        long long back = v.back();
        swap(v[m[val]], v.back());
        m[back] = m[val];
        m.erase(val);
        v.pop_back();
    }

    long long random() {
        return v[uniform(gen, decltype(uniform)::param_type(0, v.size() - 1))];
    }
};

template <class T>
class Pool {
private:
    RandomizedSet rs;
    long long count = 0;
    vector<long long> gc;

public:
    unordered_map<long long, T> pool;

    long long insert(const T& val) {
        rs.insert(count);
        pool.insert({count, val});
        return count++;
    }

    pair<long long, T> random() {
        long long id = rs.random();
        return {id, pool[id]};
    }

    void remove(long long id) {
        gc.push_back(id);
        rs.remove(id);
    }

    void maintain() {
        for (auto id : gc) {
            pool.erase(id);
        }
        gc.clear();
    }
};
