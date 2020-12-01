//
//  Bracelet.hpp
//  ObstacleRepresentationGenerator
//
//  Created by Chris Hartman on 2/24/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef Bracelet_hpp
#define Bracelet_hpp
#include <vector>
#include <iostream>

class Bracelet {
    friend std::ostream& operator<<(std::ostream &, const Bracelet &);
public:
    Bracelet(int n=0);
    Bracelet(std::vector<int> v);
    bool next();
    int operator[](size_t) const;
    int & operator[](size_t);
    std::vector<int>& rawVector();
    const std::vector<int>& rawVector() const;
    size_t size() const;
private:
    bool canonical();
    std::vector<int> _v;
};


#endif /* Bracelet_hpp */
