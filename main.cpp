#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <chrono>
using namespace std::literals;
using Graph = vector<vector<int>>;
#include <cassert>
#include <algorithm>
using std::find;
using std::random_shuffle;
#include <utility>
using std::pair;
#include <random>
#include <algorithm>
using std::next_permutation;
#include "Bracelet.hpp"

#define NO_PUSHING_IN 0
#define MAX_SWEEP_BLOCKED_EDGES 0

class Interval
{
public:
    Interval(unsigned long l, unsigned long r):_left(l),_right(r){
    }
    unsigned long l()const{return _left;}
    unsigned long r()const{return _right;}
private:
    unsigned long _left;
    unsigned long _right;
};

void gspPrint(const Graph& g)
{
    for (int v=0;v<g.size();++v) {
        cout << char(v+'A') << " --> ";
        for (const auto &i:g[v])
            cout << char(i+'A') << " ";
        cout << endl;
    }
    cout << endl;
}

template <typename T>
ostream & operator<<(ostream &out, const vector<T>& v) {
    out << "[ ";
    for(auto i:v) out << i << " ";
    return out << "]";
}

ostream & operator<<(ostream &out, const vector<pair<Bracelet,vector<char>>>& v) {
    out << "---------------------" << endl;
    for(const auto &i:v) {
        const auto &p = i.first;
        const auto &verticesPushedIn = i.second;
        for(int ii=0;ii<p.size();++ii){
            out << p[ii];
            if (verticesPushedIn[p[ii]]) cout << "'";
            out << " ";
        }
        out << endl;
    }
    return out << "---------------------" << endl;
}

ostream & operator<<(ostream &out, const Interval& i) {
    return out << "(" << i.l() << "," << i.r() << ")";
}

ostream & operator<<(ostream &out, const Graph& g) {
    for (int v=0;v<g.size();++v) {
        out << v << " --> ";
        for (const auto &i:g[v])
            out << i << " ";
        out << endl;
    }
    return out;
}

Graph powerOfCycle(int n, int k)
{
    Graph adjacencyLists(n);
    for(int ii=0;ii<n;++ii)
        for(int jj=n-k;jj<=n+k;++jj)
            if(jj%n != 0)
                adjacencyLists[ii].push_back((ii+jj)%n);
    for (auto &l : adjacencyLists)
        sort(l.begin(),l.end());
    return adjacencyLists;
}

vector<Graph> readGraphFile(string fileName)
{
    vector<Graph> retval;
    ifstream fin(fileName);
    int numVertices;
    while (fin>>numVertices) {
        Graph adjacencyLists(numVertices);
        for(int thisVertex=0;thisVertex<numVertices;++thisVertex) {
            int neighbor;
            fin >> neighbor;
            while (neighbor != -1) {
                adjacencyLists[thisVertex].push_back(neighbor);
                fin >> neighbor;
                }
        }
        retval.push_back(adjacencyLists);
    }
    return retval;
}

bool adjacent(const Graph &g,int x, int y)
{
    return find(g[x].begin(),g[x].end(),y) != g[x].end();
}

Interval findInterval(const Graph &g, const Bracelet &p, int vertexPos) {
    const auto N = p.size();

    auto cw=0ul;
    while(true) {
        int thisVertex = p[vertexPos];
        int thatVertex = p[(vertexPos+cw)%N];
        if (adjacent(g, thisVertex, thatVertex))
            break;
        ++cw;
    }
    assert(cw != 0);

    auto ccw=N;
    while(true) {
        int thisVertex = p[vertexPos];
        int thatVertex = p[(vertexPos+ccw)%N];
        if (adjacent(g, thisVertex, thatVertex))
            break;
        --ccw;
    }
    assert(ccw != N);

    return {(vertexPos+cw)%N,(vertexPos+ccw)%N};
}

bool inIntervals(const Bracelet &p, const vector<Interval> intervals, int xPos, int yPos)
{
    const auto N = p.size();
    bool yInx = false;
    for(auto ii=intervals[xPos].l();(ii%N) != (intervals[xPos].r()+1)%N;++ii)
        if(p[yPos]==p[ii%N]) {
            yInx = true;
            break;
        }
    bool xIny = false;
    for(auto ii=intervals[yPos].l();(ii%N) != (intervals[yPos].r()+1)%N;++ii)
        if(p[xPos]==p[ii%N]) {
            xIny = true;
            break;
        }
    return yInx && xIny;
}

int count=0;

void printSolution(const vector<int> &p, const Graph &g, const vector<char> &verticesPushedIn) {
    const auto N = p.size();
        cout << "ID: " << count << endl << g << endl;
        for(int ii=0;ii<N;++ii){
            cout << p[ii];
            if (verticesPushedIn[p[ii]]) cout << "'";
            cout << " ";
        }
        cout << "\n----------------------------\n";
}

vector<Interval> findIntervals( const Graph &g, const Bracelet &p)
{
    vector<Interval> intervals;
    for(int vertexPos = 0; vertexPos<p.size(); ++vertexPos)
        intervals.push_back(findInterval(g,p,vertexPos));
    return intervals;
}
//TODO: maybe use types to differentiate between positions around cycle
//and vertex number? (so ii and p[ii] are different types.)
bool blocked(const Graph &g, const Bracelet &p, int xx, int yy)
{
    const auto N = p.size();
    assert (xx<yy);
    int maxReachable = xx;
    for(int start = xx; start <= maxReachable; ++start)
        for(int ii=maxReachable+1;ii<=yy;++ii)
            if(adjacent(g, p[start%N], p[ii%N])) {
                if (ii==yy)
                    return true;
                else
                    maxReachable = ii;
            }
    return maxReachable==yy;
}

//pair<int,vector<char>> goodOrdering(const Bracelet &p, const Graph &g) {
//    assert(p.size()==g.size());
//    const auto N = p.size();
//    auto intervals=findIntervals(g, p);
//    
//    int numVerticesPushedIn = 0;
//    vector<char> verticesPushedIn(N);
//    int badEdges=0;
//    for(int ii=0;ii<N;++ii)
//        for(int jj=ii+1;jj<N;++jj) {
//            if(!adjacent(g, p[ii], p[jj]) && inIntervals(p, intervals, ii, jj)) {
//                auto distance = std::min<int>(jj-ii,ii+(int)N-jj);
//                if (distance == 2) {
//                    auto start = (jj == ii+2) ? (ii+N-1)%N : jj-1;
//                    auto before = p[start];
//                    auto first = p[(start+1)%N];
//                    auto second = p[(start+2)%N];
//                    auto third = p[(start+3)%N];
//                    auto after = p[(start+4)%N];
//                    
//                    if(verticesPushedIn[second])
//                        continue;
//                    if(!verticesPushedIn[first] &&!verticesPushedIn[third]) {
//                        verticesPushedIn[second]=true;
//                        ++numVerticesPushedIn;
//                        continue;
//                    }
//                    if(verticesPushedIn[first] && verticesPushedIn[third]) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    if(verticesPushedIn[before] && verticesPushedIn[first]) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    if(verticesPushedIn[third] && verticesPushedIn[after]) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    if(verticesPushedIn[first] && adjacent(g, before, third)) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    if(verticesPushedIn[third] && adjacent(g, first, after)) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    verticesPushedIn[second]=true;
//                    ++numVerticesPushedIn;
//                    continue;
//                }
//            
//                if (distance == 3) {
//                    auto start = (jj == ii+3) ? ii : jj;
//                    auto first = p[start];
//                    auto second = p[(start+1)%N];
//                    auto third = p[(start+2)%N];
//                    auto fourth = p[(start+3)%N];
//                    
//                    if(!adjacent(g,second,third) && !adjacent(g,first,third) && !adjacent(g,second,fourth))
//                        continue;
//
//                    if(verticesPushedIn[first]) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    if(verticesPushedIn[fourth]) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    if (adjacent(g,first,third) && (!adjacent(g,first,second) || !adjacent(g,second,third))) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    if (adjacent(g,second,fourth) && (!adjacent(g,third,fourth) || !adjacent(g,second,third))) {
//                        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                            badEdges += 1;
//                        continue;
//                    }
//                    
//                    verticesPushedIn[second]=true;
//                    verticesPushedIn[third]=true;
//                    numVerticesPushedIn += 2;
//                    continue;
//                    }
//                if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)N))
//                    badEdges += 1;
//                }
//        }
////    if(badEdges==0)  printSolution(p.rawVector(), g, verticesPushedIn);
//    return {badEdges,verticesPushedIn};
//}

using Edge = pair<int,int>;

class Status {
public:
    Status(size_t n):_n(n),_pushedIn(n) {}
    void markAsBadOrSweepBlocked(const Graph &g, const Bracelet &p, int ii, int jj) {
        if (blocked(g, p, ii, jj) && blocked(g,p,jj,ii+(int)_n))
            _badEdges.push_back(Edge(p[ii],p[jj]));
        else
            _sweepBlockedEdges.push_back(Edge(p[ii],p[jj]));
        
    }
    bool solved() {
        return _badEdges.empty() && _sweepBlockedEdges.size() <= MAX_SWEEP_BLOCKED_EDGES;
    }
    vector<Edge> _sweepBlockedEdges;
    vector<Edge> _badEdges;
    vector<char> _pushedIn;
    size_t _n;
};

bool operator<(const Status &lhs, const Status &rhs) {//less than == better
    if (lhs._badEdges.size() < rhs._badEdges.size())
        return true;
    else if (lhs._badEdges.size() == rhs._badEdges.size() && lhs._sweepBlockedEdges.size()<rhs._sweepBlockedEdges.size())
        return true;
    else
        return false;
}

bool operator==(const Status &lhs, const Status &rhs) {
    return lhs._badEdges.size()==rhs._badEdges.size() && lhs._sweepBlockedEdges.size()==rhs._sweepBlockedEdges.size();
}

Status testBracelet(const Bracelet &p, const Graph &g) {
    assert(p.size()==g.size());
    const auto N = p.size();
    Status s(N);
    auto intervals=findIntervals(g, p);
    
    for(int ii=0;ii<N;++ii)
        for(int jj=ii+1;jj<N;++jj) {
            if(!adjacent(g, p[ii], p[jj]) && inIntervals(p, intervals, ii, jj)) {
                if (NO_PUSHING_IN) {
                   s.markAsBadOrSweepBlocked(g,p,ii,jj);
                   continue;
                }
               auto distance = std::min<int>(jj-ii,ii+(int)N-jj);

                if (distance == 2) {
                    auto start = (jj == ii+2) ? (ii+N-1)%N : jj-1;
                    auto before = p[start];
                    auto first = p[(start+1)%N];
                    auto second = p[(start+2)%N];
                    auto third = p[(start+3)%N];
                    auto after = p[(start+4)%N];
                    
                    if(s._pushedIn[second])
                        continue;
                    if(!s._pushedIn[first] &&!s._pushedIn[third]) {
                        s._pushedIn[second]=true;
                        continue;
                    }
                    if(s._pushedIn[first] && s._pushedIn[third]) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    if(s._pushedIn[before] && s._pushedIn[first]) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    if(s._pushedIn[third] && s._pushedIn[after]) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    if(s._pushedIn[first] && adjacent(g, before, third)) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    if(s._pushedIn[third] && adjacent(g, first, after)) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    s._pushedIn[second]=true;
                    continue;
                }
                
                if (distance == 3) {
                    auto start = (jj == ii+3) ? ii : jj;
                    auto first = p[start];
                    auto second = p[(start+1)%N];
                    auto third = p[(start+2)%N];
                    auto fourth = p[(start+3)%N];
                    
                    if(s._pushedIn[first] || s._pushedIn[fourth]) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    
                    if (adjacent(g,first,second) && adjacent(g,first,third) && !adjacent(g,second,third) && adjacent(g,second,fourth) && adjacent(g,third,fourth)) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }

                    if (!adjacent(g,first,second) && adjacent(g,first,third) && !adjacent(g,second,third) && adjacent(g,second,fourth) && adjacent(g,third,fourth)) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    
                    if (!adjacent(g,first,second) && adjacent(g,first,third) && adjacent(g,second,third) && adjacent(g,second,fourth) && !adjacent(g,third,fourth)) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    
                    if (adjacent(g,first,second) && adjacent(g,first,third) && !adjacent(g,second,third) && adjacent(g,second,fourth) && !adjacent(g,third,fourth)) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    
                    if (!adjacent(g,first,second) && adjacent(g,first,third) && !adjacent(g,second,third) && adjacent(g,second,fourth) && !adjacent(g,third,fourth)) {
                        s.markAsBadOrSweepBlocked(g,p,ii,jj);
                        continue;
                    }
                    
                    s._pushedIn[second]=true;
                    s._pushedIn[third]=true;
                    continue;
                }
                s.markAsBadOrSweepBlocked(g,p,ii,jj);
            }
        }
//    printSolution(p.rawVector(), g, s._pushedIn);
    return s;
}


//bool hasDominating(const Graph &g)
//{
//    for(const auto &v:g)
//        if(v.size()==g.size()-1)
//            return true;
//    return false;
//}
//

bool hasDegree2VertexInTriangle(const Graph &g)
{
    for(const auto &v:g)
        if (v.size()==2 && adjacent(g,v[0],v[1]))
            return true;
    return false;
}

Graph removeDegree2VertexInTriangle(const Graph &g)
{
   for(int i=0;i<g.size();++i)
      if (g[i].size()==2 && adjacent(g,g[i][0],g[i][1])) {
         auto result = g;
         result.erase(result.begin()+i);
         for(auto &k:result)
            for(int v=0;v<k.size();++v) {
               if (k[v]==i)
                  k.erase(k.begin()+v--);
               else if (k[v]>i)
                  --k[v];
            }
         return result;
      }
   return g;
}

bool hasAdjacentVerticesWithCommonNeighborhood(const Graph &g)
{
    for(int ii=0;ii<g.size();++ii)
        for(int jj=ii+1;jj<g.size();++jj) {
            auto x = g[ii];
            auto y = g[jj];
            if (x.size() != y.size())
                continue;
            auto where = lower_bound(x.begin(),x.end(),jj);
            if (where == x.end() || *where != jj)
                continue;
            if (where != x.end() && *where==jj) {
                x.erase(where);
                y.erase(lower_bound(y.begin(),y.end(),ii));
            }
            if (x==y) {
                return true;
            }

        }
    return false;
}

void glennFormat(const Graph &g, ostream &gout)
{
    gout << g.size() << endl;
    for(const auto &v : g) {
        for(auto i:v)
            gout << i << " ";
        gout << -1 << endl;
    }
}


//std::random_device r;
//std::default_random_engine e1(r());
//
//bool testGraphRandomPermutations(const Graph &g)
//{
//    const auto n=g.size();
//    static auto fewestBadEdges=100*n*(n-1)/2;
//    
//    std::uniform_int_distribution<int> uniform_dist(0,(int)g.size()-1);
//
//    
//    if (hasDegree2VertexInTriangle(g)) {
//        cout << "Has degree 2 vertex in triangle" << endl;
//        return true;
//    }
//    
//    if (hasAdjacentVerticesWithCommonNeighborhood(g)) {
//        cout << "Has AdjacentVerticesWithCommonNeighborhood" << endl;
//        return true;
//    }
//    
//    int solutions=0;
//    static vector<pair<Bracelet,vector<char>>> bestBracelets{{Bracelet((int)n),vector<char>(n)}};
//    auto stuff =goodOrdering(bestBracelets[0].first, g);
//    auto & badEdges = stuff.first;
//    auto & verticesPushedIn = stuff.second;
//    auto pCount = 0ul;
//    auto localBestBadEdges = 9999999;
//    Bracelet v((int)n);
//    random_shuffle(v.rawVector().begin(), v.rawVector().end());
//    
//    do {
//        pCount++;
//        if(pCount%20000==0) cout << '.' << std::flush;
////        if(pCount==100000) return false;
//        Bracelet w(v);
//        auto x = uniform_dist(e1);
//        for(int ii=0;ii<5;++ii)
//            std::swap(w[x],w[uniform_dist(e1)]);
//        x = uniform_dist(e1);
//        for(int ii=0;ii<2;++ii)
//            std::swap(w[x],w[uniform_dist(e1)]);
//        
//        stuff =goodOrdering(w, g);
//
//        if (badEdges==0) {
//            solutions++;
//            cout << "SOLVED!!!" << endl;
//            cout << "Found a new permutations with " << badEdges << " bad edges: " << endl;
//            bestBracelets = {{w,verticesPushedIn}};
//            fewestBadEdges = badEdges;
//            printSolution(w.rawVector(), g, verticesPushedIn);
//            testBracelet(w, g);
//            return true;
//        }
//        if(badEdges<=localBestBadEdges) {
//            v=w;
//            localBestBadEdges = badEdges;
//        }
//        
//        if(badEdges<=fewestBadEdges) {
//            cout << "Found a new permutations with " << badEdges << " bad edges: " << endl;
//            bestBracelets = {{w,verticesPushedIn}};
//            fewestBadEdges = badEdges;
//            cout << bestBracelets << endl;
//            cout << " Or, showing bad edges: " << endl;
//            testBracelet(w,g);
//        }
//    } while (1);
//}

ostream & operator<<(ostream &os, const vector<Edge> &v) {
    for(auto e:v)
        os << "(" << e.first << "->" << e.second << ") ";
    return os;
}

bool testGraph(const Graph &g2) {
   auto g=removeDegree2VertexInTriangle(g2);
   if (hasAdjacentVerticesWithCommonNeighborhood(g))
      return true;
//   if (hasDegree2VertexInTriangle(g))
//      return true;


    Bracelet v((int)g.size());
    vector<pair<Bracelet,Status>> bestBracelets{{v,testBracelet(v, g)}};
    
    do {
        auto status = testBracelet(v, g);
        
        if (status.solved()) {
//            printSolution(v.rawVector(), g, status._pushedIn);
            return true;
        }
        
        if(status<bestBracelets[0].second)
            bestBracelets = {{v,status}};
        else if (status==bestBracelets[0].second)
            bestBracelets.push_back({v,status});
    } while (v.next());

    auto firstStatus = bestBracelets[0].second;
    if (firstStatus._badEdges.empty()) {
        cout << "Too many sweep blocked edges: " << firstStatus._sweepBlockedEdges << endl;
        printSolution(bestBracelets[0].first.rawVector(), g, firstStatus._pushedIn);
    }
    return false;
}

int testGraphsOfSize(int n)
{
    int badGraphs = 0;
    auto filename = "graphs_conn_d2_order"s + char('0'+n) + ".txt";
//   filename = "machineBad" + filename;
   auto graphs = readGraphFile(filename);

    cout << "Testing " << graphs.size() << " graphs of size " << n << endl;
    ofstream fout("humanBad"+filename);
    ofstream gout("machineBad"+filename);
    for (const auto &g:graphs) {
        ++count;
        if(count%1000==0)
            cout << count << endl;
//        cout << "ID: " << count << endl;
//        gspPrint(g);
        if (!testGraph(g)) {
            badGraphs++;
            fout << "ID: " << badGraphs << endl << g << "had no solutions." << endl;
            fout << "----------------------\n";
            cout << "ID: " << count << endl << g << "had no solutions." << endl;
//            cout << bestBracelets << endl;
            cout << "----------------------\n";
            glennFormat(g,gout);
        }
    }
   cout << badGraphs << " graphs of size " << n << " were bad.\n";
    return badGraphs;
}

int testGlennGraphs()
{
    int badGraphs = 0;
//    string fname = "bad_order9_planar-ggc_minus_cmh.txt";
    string fname = "bad_graphs_conn_d2_order9_planar.txt";
//    string fname = "bad_order8_push2.txt";
//    string fname = "graphs_conn_d2_order9_planar.txt";
//    string fname = "interesting8.txt";
    auto graphs = readGraphFile(fname);
    ofstream gout("bad_"+fname);
    cout << "Testing " << graphs.size() << " graphs." << endl;
    for (const auto &g:graphs) {
        ++count;
        if(count%1000==0)
            cout << count << endl;
//        cout << "ID: " << count << endl;
//        gspPrint(g);
        if (!testGraph(g)) {
            badGraphs++;
            cout << "ID: " << count << endl << g << "had no solutions." << endl;
            //            cout << bestBracelets << endl;
            cout << "----------------------\n";
            glennFormat(g,gout);
        }
    }
//    testBracelet(vector<int>{0,1,8,4,7,6,3,5,2}, graphs.back());
    return badGraphs;
}


// http://users.cecs.anu.edu.au/~bdm/data/graphs.html
int main() {
//    auto graphs = readGraphFile("gformat9.txt");
//    auto g13 = graphs[5];
//    cout << g13 << endl;
//    Bracelet b({0,1,2,3,5,6,4,7});
//    findIntervals(g13, b);
//    cout << blocked(g13,b,7,9) << endl;
//    return 0;
//    tryFivefold();
//    tryDaCaPermutations();
//    tryDaCbPermutations();
    
//    auto graphs = readGraphFile("dodecahedron.txt");
//    auto dod = graphs[0];
//    cout << dod << endl;
//    while (!testGraphRandomPermutations(dod))
//        ;
//    return 0;

//    vector<int> badGraphs;
//    for (int ii=4;ii<=9;++ii)
//        count=0,badGraphs.push_back(testGraphsOfSize(ii));

//    badGraphs.push_back(testGraphsOfSize(9));
//    cout << badGraphs << endl;
    
//    testGraphRandomPermutations(powerOfCycle(17,6));
//    goodOrderingPrint(Bracelet{{0,1,2,3,12,13,14,15,4,5,6,7,16,17,18,8,9,10,11}},powerOfCycle(19, 7));
//    cout << powerOfCycle(19, 7) << endl;

//    goodOrderingPrint(Bracelet{{0,1,2,6,7,8,3,4,5}},powerOfCycle(9, 2));

//    auto graphs = readGraphFile("gformat8.txt");
//    auto g5 = graphs[1];
//    cout << g5 << endl;
//    Bracelet b({0,4,5,7,1,6,2,3});
////    findIntervals(g5, b);
////    cout << blocked(g5,b,7,9) << endl;
//    testBracelet(b, g5);

    auto numBad = testGlennGraphs();
   cout << numBad << " bad graphs.\n";
//   testGraphsOfSize(3);
//   testGraphsOfSize(4);
//   testGraphsOfSize(5);
//   testGraphsOfSize(6);
//   testGraphsOfSize(7);
//   testGraphsOfSize(8);
    return 0;
}

/*
 .SOLVED!!!
 0 --> 1 2 3 4 5 6 11 12 13 14 15 16
 1 --> 0 2 3 4 5 6 7 12 13 14 15 16
 2 --> 0 1 3 4 5 6 7 8 13 14 15 16
 3 --> 0 1 2 4 5 6 7 8 9 14 15 16
 4 --> 0 1 2 3 5 6 7 8 9 10 15 16
 5 --> 0 1 2 3 4 6 7 8 9 10 11 16
 6 --> 0 1 2 3 4 5 7 8 9 10 11 12
 7 --> 1 2 3 4 5 6 8 9 10 11 12 13
 8 --> 2 3 4 5 6 7 9 10 11 12 13 14
 9 --> 3 4 5 6 7 8 10 11 12 13 14 15
 10 --> 4 5 6 7 8 9 11 12 13 14 15 16
 11 --> 0 5 6 7 8 9 10 12 13 14 15 16
 12 --> 0 1 6 7 8 9 10 11 13 14 15 16
 13 --> 0 1 2 7 8 9 10 11 12 14 15 16
 14 --> 0 1 2 3 8 9 10 11 12 13 15 16
 15 --> 0 1 2 3 4 9 10 11 12 13 14 16
 16 --> 0 1 2 3 4 5 10 11 12 13 14 15
 
 [ 12 13 14 4 5 6 7 15 16 0 8 9 10 1 2 3 11 ]
*/
