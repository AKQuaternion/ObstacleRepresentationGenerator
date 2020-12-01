//
//  Bracelet.cpp
//  ObstacleRepresentationGenerator
//
//  Created by Chris Hartman on 2/24/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#include "Bracelet.hpp"
#include <numeric>
using std::iota;

Bracelet::Bracelet(int n):_v(n)
{
    iota(_v.begin(),_v.end(),0);
}

Bracelet::Bracelet(std::vector<int> v):_v(v){
}

bool Bracelet::next(){
    auto second = std::next(_v.begin());
    do
        if(!next_permutation(second, end(_v)))
            return false;
    while (!canonical());
    
    return true;
}

bool Bracelet::canonical () {
    auto onePosition=-1;
    auto twoPosition=-1;
    for(auto ii=0;ii<_v.size();++ii) {
        if(_v[ii] == 1)
            onePosition = ii;
        if(_v[ii] == 2)
            twoPosition = ii;
    }
    return onePosition<twoPosition;
}

size_t Bracelet::size() const
{
    return _v.size();
}

std::vector<int>& Bracelet::rawVector()
{
    return _v;
}

const std::vector<int>& Bracelet::rawVector() const
{
    return _v;
}

int Bracelet::operator[](size_t i) const
{
    return _v[i];
}

int & Bracelet::operator[](size_t i)
{
    return _v[i];
}

std::ostream& operator<<(std::ostream &out, const Bracelet &b)
{
    out << "[ ";
    for(auto i:b._v) out << i << " ";
    return out << "]";
}
