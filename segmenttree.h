#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H
#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <complex>
#include <cmath>
#include <numeric>
#include <climits>
#include <bitset>
#include <functional>
#include <random>
#include <ctime>
using namespace std;
#define int long long
const int pinf = 2e18;
int gcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return (b, a % b);
}

template <class T>
class segmenttree
{
private:
    int n;
    T valueForNodes;
    vector<T> st, a;
    T(*combinerFunction) (T x, T y);

public:
    segmenttree(int n, vector<T> a, T val, T *(combinerFunction)(T x, T y))
    {
        this->n = n;
        this->a = a;
        this->valueForNodes = val;
        this->st = vector<int>(4 * n + 10, val);
        this->combinerFunction = combinerFunction;
        buildtree(1, 1, n, a);
    }
    T query(int qs, int qe)
    {
        return query1(1, 1, n, qs, qe, a);
    }
    void update(int idx, T val)
    {
        update1(1, 1, n, idx, val, a);
    }

private:
    void buildtree(int si, int ss, int se, vector<T> &a)
    {
        if (ss > se)
        {
            return;
        }
        if (ss == se)
        {
            st[si] = a[ss];
            return;
        }
        int mid = ss + (se - ss) / 2;
        buildtree(2 * si, ss, mid, a);
        buildtree(2 * si + 1, mid + 1, se, a);
        st[si] = combinerFunction(st[2 * si], st[2 * si + 1]);
    }
    template <class T>
    query1(int si, int ss, int se, int qs, int qe, vector<T> &a)
    {
        if (ss > se)
        {
            return this->valueForNodes;
        }
        if (ss > qe || se < qs)
        {
            return this->valueForNodes;
        }
        if (ss >= qs && se <= qe)
        {
            return st[si];
        }
        int mid = (ss + se) / 2;
        return combinerFunction(query1(2 * si, ss, mid, qs, qe, a), query1(2 * si + 1, mid + 1, se, qs, qe, a));
    }
    void update1(int si, int ss, int se, int idx, T val, vector<int> &a)
    {
        if (ss > se)
        {
            return;
        }
        if (ss > idx || se < idx)
        {
            return;
        }
        if (ss == se)
        {
            st[si] = val;
            return;
        }
        int mid = (ss + se) / 2;
        update1(2 * si, ss, mid, idx, val, a);
        update1(2 * si + 1, mid + 1, se, idx, val, a);
        st[si] = combinerFunction(st[2 * si], st[2 * si + 1]);
    }
};

template <class T>
class sumsegtree
{
private:
    int n;
    vector<T> st, lazy, a;

public:
    sumsegtree(int n, vector<T> a)
    {
        st = vector<T>(4 * n + 15, 0);
        lazy = vector<T>(4 * n + 15, 0);
        this->a = a;
        this->n = n;
        build1(1, 1, n, a);
    }
    T query(int qs, int qe)
    {
        return query1(1, 1, n, qs, qe, a);
    }
    void update(int qs, int qe, T val)
    {
        update1(1, 1, n, qs, qe, val, a);
    }

private:
    void build1(int si, int ss, int se, vector<T> &a)
    {
        if (ss > se)
            return;
        if (ss == se)
        {
            st[si] = a[ss];
            return;
        }
        int mid = (ss + se) / 2;
        build1(2 * si, ss, mid, a);
        build1(2 * si + 1, mid + 1, se, a);
        st[si] = (st[2 * si] + st[2 * si + 1]);
    }
    T query1(int si, int ss, int se, int qs, int qe, vector<T> &a)
    {
        if (ss > se)
            return 0;
        if (lazy[si] != 0)
        {
            T dx = lazy[si];
            lazy[si] = 0;
            st[si] += (dx * (se - ss + 1));
            if (ss != se)
            {
                lazy[2 * si] += dx;
                lazy[2 * si + 1] += dx;
            }
        }
        if (ss > qe || se < qs)
        {
            return 0;
        }
        if (ss >= qs && se <= qe)
        {
            return st[si];
        }
        int mid = (ss + se) / 2;
        return (query1(2 * si, ss, mid, qs, qe, a) + query1(2 * si + 1, mid + 1, se, qs, qe, a));
    }
    void update1(int si, int ss, int se, int qs, int qe, T val, vector<T> &a)
    {
        if (ss > se)
            return;
        if (lazy[si] != 0)
        {
            T dx = lazy[si];
            lazy[si] = 0;
            st[si] += (dx * (se - ss + 1));
            if (ss != se)
            {
                lazy[2 * si] += dx;
                lazy[2 * si + 1] += dx;
            }
        }
        if (ss > qe || se < qs)
        {
            return;
        }
        if (ss >= qs && se <= qe)
        {
            st[si] += (se - ss + 1) * val;
            if (ss != se)
            {
                lazy[2 * si] += val;
                lazy[2 * si + 1] += val;
            }
            return;
        }
        int mid = (ss + se) / 2;
        update1(2 * si, ss, mid, qs, qe, val, a);
        update1(2 * si + 1, mid + 1, se, qs, qe, val, a);
        st[si] = st[2 * si] + st[2 * si + 1];
    }
};

template <class T>
class minsegtree
{
private:
    int n;
    vector<T> st, a;

public:
    minsegtree(int n, vector<T> a)
    {
        st = vector<T>(4 * n + 5, 0);
        this->n = n;
        this->a = a;
        build1(1, 1, n, a);
    }
    T query(int qs, int qe)
    {
        return query1(1, 1, n, qs, qe, a);
    }
    void update(int idx, T val)
    {
        return update1(1, 1, n, idx, val, a);
    }

private:
    void build1(int si, int ss, int se, vector<T> &a)
    {
        if (ss > se)
            return;
        if (ss == se)
        {
            st[si] = a[ss];
            return;
        }
        int mid = (ss + se) / 2;
        build1(2 * si, ss, mid, a);
        build1(2 * si + 1, mid + 1, se, a);
        st[si] = min(st[2 * si], st[2 * si + 1]);
    }
    T query1(int si, int ss, int se, int qs, int qe, vector<T> &a)
    {
        if (ss > se)
            return pinf;
        if (ss >= se && se <= qe)
        {
            return st[si];
        }
        int mid = (ss + se) / 2;
        return min(query1(2 * si, ss, mid, qs, qe, a), query1(2 * si + 1, mid + 1, se, qs, qe, a));
    }
    void update1(int si, int ss, int se, int idx, T val, vector<T> &a)
    {
        if (ss > se)
            return;
        if (ss == se)
        {
            st[si] = val;
            return;
        }
        int mid = (ss + se) / 2;
        update1(2 * si, ss, mid, idx, val, a);
        update1(2 * si + 1, mid + 1, se, idx, val, a);
        st[si] = min(st[2 * si], st[2 * si + 1]);
    }
};

template <class T>
class maxsegtree
{
private:
    int n;
    vector<T> st, a;

public:
    maxsegtree(int n, vector<T> a)
    {
        st = vector<T>(4 * n + 5, 0);
        this->n = n;
        this->a = a;
        build1(1, 1, n, a);
    }
    T query(int qs, int qe)
    {
        return query1(1, 1, n, qs, qe, a);
    }
    void update(int idx, int val)
    {
        return update1(1, 1, n, idx, val, a);
    }

private:
    void build1(int si, int ss, int se, vector<int> &a)
    {
        if (ss > se)
            return;
        if (ss == se)
        {
            st[si] = a[ss];
            return;
        }
        int mid = (ss + se) / 2;
        build1(2 * si, ss, mid, a);
        build1(2 * si + 1, mid + 1, se, a);
        st[si] = max(st[2 * si], st[2 * si + 1]);
    }
    T query1(int si, int ss, int se, int qs, int qe, vector<T> &a)
    {
        if (ss > se)
            return -pinf;
        if (ss > qe || se < qs)
        {
            return INT_MIN;
        }
        if (ss >= qs && se <= qe)
        {
            return st[si];
        }
        int mid = (ss + se) / 2;
        return max(query1(2 * si, ss, mid, qs, qe, a), query1(2 * si + 1, mid + 1, se, qs, qe, a));
    }
    void update1(int si, int ss, int se, int idx, T val, vector<T> &a)
    {
        if (ss > se)
            return;
        if (ss > idx || se < idx)
        {
            return;
        }
        if (ss == se)
        {
            st[idx] = val;
            return;
        }
        int mid = (ss + se) / 2;
        update1(2 * si, ss, mid, qs, qe, val, a);
        update1(2 * si + 1, mid + 1, se, qs, qe, val, a);
        st[si] = max(st[2 * si], st[2 * si + 1]);
    }
};

class gcdsegtree
{
private:
    int n;
    vector<int> st, a;

public:
    gcdsegtree(int n, vector<int> a)
    {
        st = vector<int>(4 * n + 15, 0);
        this->n = n;
        this->a = a;
        build1(1, 1, n, a);
    }
    int query(int qs, int qe)
    {
        return query1(1, 1, n, qs, qe, a);
    }
    void update(int idx, int val)
    {
        return update1(1, 1, n, idx, val, a);
    }

private:
    void build1(int si, int ss, int se, vector<int> &a)
    {
        if (ss > se)
            return;
        if (ss == se)
        {
            st[si] = a[ss];
            return;
        }
        int mid = (ss + se) / 2;
        build1(2 * si, ss, mid, a);
        build1(2 * si + 1, mid + 1, se, a);
        st[si] = gcd(st[2 * si], st[2 * si + 1]);
    }
    int query1(int si, int ss, int se, int qs, int qe, vector<int> &a)
    {
        if (ss > se)
            return 0;
        if (ss > qe || se < qs)
        {
            return 0;
        }
        if (ss >= qs && se <= qe)
        {
            return st[si];
        }
        int mid = (ss + se) / 2;
        return gcd(query1(2 * si, ss, mid, qs, qe, a), query1(2 * si + 1, mid + 1, se, qs, qe, a));
    }
    void update1(int si, int ss, int se, int idx, int val, vector<int> &a)
    {
        if (ss > se)
            return;
        if (ss > idx || se < idx)
        {
            return;
        }
        if (ss == idx && se == idx)
        {
            st[si] = val;
            return;
        }
        int mid = (ss + se) / 2;
        update1(2 * si, ss, mid, idx, val, a);
        update1(2 * si + 1, mid + 1, se, idx, val, a);
        st[si] = gcd(st[2 * si], st[2 * si + 1]);
    }
};

#endif
