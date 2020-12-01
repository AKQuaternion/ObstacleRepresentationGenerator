////
////  DodecahedronFunctions.cpp
////  ObstacleRepresentationGenerator
////
////  Created by Chris Hartman on 4/16/17.
////  Copyright © 2017 Chris Hartman. All rights reserved.
////
//
//#include "DodecahedronFunctions.hpp"
//
//
//void tryDbCaPermutations() {
//    //Dodec Antipodal to Circle Reflection
//    auto graphs = readGraphFile("dodecahedron.txt");
//    auto dod = graphs[0];
//    cout << dod << endl;
//    
//    vector<pair<int,int>> AntipodalEdges{{0,19},{1,15},{17,3},{5,10},{2,16},{4,18},{6,11},{7,12},{8,13},{9,14}};
//    Bracelet permutation{10};
//    
//    int fewestBadEdges=9999999;
//    Bracelet bestBracelet;
//    int numPerms=0;
//    do {
//        if(++numPerms%1000==0)
//            cout << numPerms << endl;
//        for(int bits=0;bits<(1<<10);++bits) {
//            vector<int> p;
//            for(int i=0;i<10;++i)
//                if (bits&(1<<i)) {
//                    p.push_back(AntipodalEdges[permutation[i]].first);
//                } else {
//                    p.push_back(AntipodalEdges[permutation[i]].second);
//                }
//            for(int i=9;i>=0;--i)
//                if (bits&(1<<i)) {
//                    p.push_back(AntipodalEdges[permutation[i]].second);
//                } else {
//                    p.push_back(AntipodalEdges[permutation[i]].first);
//                }
//            Bracelet b(p);
//            auto stuff = goodOrdering(p, dod);
//            auto & badEdges = stuff.first;
//            //            auto & verticesPushedIn = stuff.second;
//            
//            if (badEdges==0) {
//                cout << "SOLVED!!!" << endl << dod << endl << bestBracelet << endl;
//                return;
//            }
//            
//            if(badEdges<fewestBadEdges) {
//                cout << "Found a new permutations with " << badEdges << " bad edges: " << endl;
//                bestBracelet=b;
//                fewestBadEdges = badEdges;
//                cout << bestBracelet << endl;
//                cout << " Or, showing bad edges: " << endl;
//                goodOrderingPrint(bestBracelet,dod);
//            }
//            
//        }
//    } while (permutation.next());
//}
//
//void tryDbCbPermutations() {
//    //Dodec Antipodal to Circle Antipodal
//    auto graphs = readGraphFile("dodecahedron.txt");
//    auto dod = graphs[0];
//    cout << dod << endl;
//    
//    vector<pair<int,int>> AntipodalEdges{{0,19},{1,15},{17,3},{5,10},{2,16},{4,18},{6,11},{7,12},{8,13},{9,14}};
//    Bracelet permutation{10};
//    
//    int fewestBadEdges=9999999;
//    Bracelet bestBracelet;
//    int numPerms=0;
//    do {
//        if(++numPerms%1000==0)
//            cout << numPerms << endl;
//        for(int bits=0;bits<(1<<10);++bits) {
//            vector<int> p;
//            for(int i=0;i<10;++i)
//                if (bits&(1<<i)) {
//                    p.push_back(AntipodalEdges[permutation[i]].first);
//                } else {
//                    p.push_back(AntipodalEdges[permutation[i]].second);
//                }
//            for(int i=0;i<10;++i)
//                if (bits&(1<<i)) {
//                    p.push_back(AntipodalEdges[permutation[i]].second);
//                } else {
//                    p.push_back(AntipodalEdges[permutation[i]].first);
//                }
//            Bracelet b(p);
//            auto stuff = goodOrdering(p, dod);
//            auto & badEdges = stuff.first;
//            //            auto & verticesPushedIn = stuff.second;
//            
//            if (badEdges==0) {
//                cout << "SOLVED!!!" << endl << dod << endl << bestBracelet << endl;
//                return;
//            }
//            
//            if(badEdges<=fewestBadEdges) {
//                cout << "Found a new permutations with " << badEdges << " bad edges: " << endl;
//                bestBracelet=b;
//                fewestBadEdges = badEdges;
//                cout << bestBracelet << endl;
//                cout << " Or, showing bad edges: " << endl;
//                goodOrderingPrint(bestBracelet,dod);
//            }
//            
//        }
//    } while (permutation.next());
//}
//
//void tryDaCaPermutations() {
//    //Dodec Rotational to Circle Reflection
//    // Finds nothing better than my massaged drawing.
//    auto graphs = readGraphFile("dodecahedron.txt");
//    auto dod = graphs[0];
//    cout << dod << endl;
//    
//    vector<pair<int,int>> RotationalEdges{{4,3},{1,14},{11,6},{10,16},{19,8},{15,9},{17,18},{13,0},{12,7},{2,5}};
//    Bracelet permutation{10};
//    
//    int fewestBadEdges=9999999;
//    Bracelet bestBracelet;
//    int numPerms=0;
//    do {
//        if(++numPerms%1000==0)
//            cout << numPerms << endl;
//        for(int bits=0;bits<(1<<10);++bits) {
//            vector<int> p;
//            for(int i=0;i<10;++i)
//                if (bits&(1<<i)) {
//                    p.push_back(RotationalEdges[permutation[i]].first);
//                } else {
//                    p.push_back(RotationalEdges[permutation[i]].second);
//                }
//            for(int i=9;i>=0;--i)
//                if (bits&(1<<i)) {
//                    p.push_back(RotationalEdges[permutation[i]].second);
//                } else {
//                    p.push_back(RotationalEdges[permutation[i]].first);
//                }
//            Bracelet b(p);
//            auto stuff = goodOrdering(p, dod);
//            auto & badEdges = stuff.first;
//            //            auto & verticesPushedIn = stuff.second;
//            
//            if (badEdges==0) {
//                cout << "SOLVED!!!" << endl << dod << endl << b << endl;
//                return;
//            }
//            
//            if(badEdges<=fewestBadEdges) {
//                cout << "Found a new permutations with " << badEdges << " bad edges: " << endl;
//                bestBracelet=b;
//                fewestBadEdges = badEdges;
//                cout << bestBracelet << endl;
//                cout << " Or, showing bad edges: " << endl;
//                goodOrderingPrint(bestBracelet,dod);
//            }
//            
//        }
//    } while (permutation.next());
//}
//
//void tryDaCbPermutations() {
//    //Dodec Rotational to Circle Antipodal
//    auto graphs = readGraphFile("dodecahedron.txt");
//    auto dod = graphs[0];
//    cout << dod << endl;
//    
//    vector<pair<int,int>> RotationalEdges{{4,3},{1,14},{11,6},{10,16},{19,8},{15,9},{17,18},{13,0},{12,7},{2,5}};
//    Bracelet permutation{10};
//    
//    int fewestBadEdges=9999999;
//    Bracelet bestBracelet;
//    int numPerms=0;
//    do {
//        if(++numPerms%1000==0)
//            cout << numPerms << endl;
//        for(int bits=0;bits<(1<<10);++bits) {
//            vector<int> p;
//            for(int i=0;i<10;++i)
//                if (bits&(1<<i)) {
//                    p.push_back(RotationalEdges[permutation[i]].first);
//                } else {
//                    p.push_back(RotationalEdges[permutation[i]].second);
//                }
//            for(int i=0;i<10;++i)
//                if (bits&(1<<i)) {
//                    p.push_back(RotationalEdges[permutation[i]].second);
//                } else {
//                    p.push_back(RotationalEdges[permutation[i]].first);
//                }
//            auto stuff = goodOrdering(p, dod);
//            auto & badEdges = stuff.first;
//            //            auto & verticesPushedIn = stuff.second;
//            
//            if (badEdges==0) {
//                cout << "SOLVED!!!" << endl << dod << endl << bestBracelet << endl;
//                return;
//            }
//            
//            if(badEdges<=fewestBadEdges) {
//                cout << "Found a new permutations with " << badEdges << " bad edges: " << endl;
//                bestBracelet=p;
//                fewestBadEdges = badEdges;
//                cout << bestBracelet << endl;
//                cout << " Or, showing bad edges: " << endl;
//                goodOrderingPrint(bestBracelet,dod);
//            }
//            
//        }
//    } while (permutation.next());
//}
//
//void tryFivefold() {
//    auto graphs = readGraphFile("dodecahedron.txt");
//    auto dod = graphs[0];
//    cout << dod << endl;
//    
//    vector<vector<int>> orbits{{1,9,8,7,0},{2,10,17,6,4},{11,18,16,5,3},{12,19,15,14,13}};
//    Bracelet permutation{4};
//    
//    int fewestBadEdges=9999999;
//    Bracelet bestBracelet;
//    int numPerms=0;
//    do {
//        if(++numPerms%1000==0)
//            cout << numPerms << endl;
//        for(int s0=0;s0<5;s0++)
//            for(int s1=0;s1<5;s1++)
//                for(int s2=0;s2<5;s2++)
//                    for(int s3=0;s3<5;s3++) {
//                        vector<int> p;
//                        for (int xx=0;xx<5;xx++) {
//                            p.push_back(orbits[permutation[0]][(s0+xx)%5]);
//                            p.push_back(orbits[permutation[1]][(s1+xx)%5]);
//                            p.push_back(orbits[permutation[2]][(s2+xx)%5]);
//                            p.push_back(orbits[permutation[3]][(s3+xx)%5]);
//                        }
//                        auto stuff = goodOrdering(p, dod);
//                        auto & badEdges = stuff.first;
//                        
//                        if (badEdges==0) {
//                            cout << "SOLVED!!!" << endl << dod << endl << bestBracelet << endl;
//                            return;
//                        }
//                        
//                        if(badEdges<fewestBadEdges) {
//                            cout << "Found a new permutations with " << badEdges << " bad edges: " << endl;
//                            bestBracelet=p;
//                            fewestBadEdges = badEdges;
//                            cout << p << endl;
//                            cout << " Or, showing bad edges: " << endl;
//                            goodOrderingPrint(bestBracelet,dod);
//                        }
//                    }
//    } while (permutation.next());
//}
//
//
//
//void tryDodecahedronMatchingPermutations() {
//    auto graphs = readGraphFile("dodecahedron.txt");
//    auto dod = graphs[0];
//    cout << dod << endl;
//    
//    vector<pair<int,int>> matchingEdges{{0,4},{1,9},{2,3},{5,14},{6,7},{8,17},{10,11},{12,13},{15,16},{18,19}};
//    Bracelet permutation{int(matchingEdges.size())};
//    
//    int fewestBadEdges=9999999;
//    Bracelet bestBracelet;
//    int numPerms=0;
//    do {
//        if(++numPerms%1000==0)
//            cout << numPerms << endl;
//        for(int bits=0;bits<(1<<10);++bits) {
//            vector<int> p;
//            for(int i=0;i<10;++i)
//                if (bits&(1<<i)) {
//                    p.push_back(matchingEdges[permutation[i]].first);
//                    p.push_back(matchingEdges[permutation[i]].second);
//                } else {
//                    p.push_back(matchingEdges[permutation[i]].second);
//                    p.push_back(matchingEdges[permutation[i]].first);
//                }
//            Bracelet b(p);
//            auto stuff = goodOrdering(p, dod);
//            auto & badEdges = stuff.first;
//            //            auto & verticesPushedIn = stuff.second;
//            
//            if (badEdges==0) {
//                cout << "SOLVED!!!" << endl << dod << endl << bestBracelet << endl;
//                return;
//            }
//            
//            if(badEdges<fewestBadEdges) {
//                cout << "Found a new permutations with " << badEdges << " bad edges: " << endl;
//                bestBracelet=b;
//                fewestBadEdges = badEdges;
//                cout << bestBracelet << endl;
//                cout << " Or, showing bad edges: " << endl;
//                goodOrderingPrint(bestBracelet,dod);
//            }
//            
//        }
//    } while (permutation.next());
//}
