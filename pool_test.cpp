#include <iostream>
#include <cassert>
#include "pool.h"

using namespace std;

int main() {
    Pool<int> pool;
    for (int i = 0; i < 100; ++i) {
        pool.insert(i);
    }
    for (int i = 0; i < 1000; ++i) {
        int r = pool.random().second;
        assert(r >= 0 and r < 100);
    }
    assert(pool.pool.size() == 100);
    for (int id = 0; id < 100; ++id) {
        if (id == 50) continue;
        pool.remove(id);
    }
    for (int i = 0; i < 1000; ++i) {
        int r = pool.random().second;
        assert(pool.pool[r] == 50);
    }
    for (int i = 0; i < 50; ++i) {
        pool.insert(i);
    }
    for (int i = 0; i < 1000; ++i) {
        int r = pool.random().second;
        assert(r >= 0 and r <= 50);
    }
    pool.maintain();
    assert(pool.pool.size() == 51);
}
