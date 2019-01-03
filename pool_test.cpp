#include <iostream>
#include <cassert>
#include "pool.h"

using namespace std;

class Foo {
private:
    int val, id;
public:
    Foo() {}
    Foo(int v): val(v) {}
    int getVal() const {
        return val;
    }
    void setId(int v) {
        id = v;
    }
};

class HasRoot {
private:
    int val;
    bool root;
    int id;
public:
    HasRoot() {}
    HasRoot(int v, bool r): val(v), root(r) {}
    bool isRoot() const {
        return root;
    }
    int getVal() const {
        return val;
    }
    void setRoot(bool r) {
        root = r;
    }
    void setId(int v) {
        id = v;
    }
};

int main() {
    Pool<Foo> pool;
    for (int i = 0; i < 100; ++i) {
        pool.insert(Foo(i));
    }
    for (int i = 0; i < 1000; ++i) {
        auto r = pool.random().second.getVal();
        assert(r >= 0 and r < 100);
    }
    assert(pool.size() == 100);
    for (int id = 0; id < 100; ++id) {
        if (id == 50) continue;
        pool.remove(id);
    }
    for (int i = 0; i < 1000; ++i) {
        auto r = pool.random().second.getVal();
        assert(pool.get(r).getVal() == 50);
    }
    for (int i = 0; i < 50; ++i) {
        pool.insert(Foo(i));
    }
    for (int i = 0; i < 1000; ++i) {
        auto r = pool.random().second.getVal();
        assert(r >= 0 and r <= 50);
    }
    pool.maintain();
    assert(pool.size() == 51);
    for (int i = 0; i < 51; ++i) {
        auto r = pool.random().first;
        pool.remove(r);
    }
    pool.maintain();
    assert(pool.size() == 0);

    Pool<HasRoot> root_pool;
    root_pool.insert(HasRoot(0, true));
    for (int i = 1; i < 10; ++i) {
        auto tmp = HasRoot(i, false);
        root_pool.insert(tmp);
    }
    for (int i = 0; i < 1000; ++i) {
        int r = root_pool.random().second.getVal();
        assert(r >= 0 and r <= 9);
    }
    for (int i = 0; i < 1000; ++i) {
        int r = root_pool.randomRootTeam().second.getVal();
        assert(r == 0);
    }
    root_pool.addRootTeam(9);
    root_pool.removeRootTeam(0);
    for (int i = 0; i < 1000; ++i) {
        int r = root_pool.randomRootTeam().second.getVal();
        assert(r == 9);
    }
}
