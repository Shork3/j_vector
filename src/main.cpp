#include <iostream>
#include "j_vector.hpp"
#include <vector>
int main()
{
    just::j_vector<int> jvec {1,2,3,4,5};
    jvec.push_back(6);
    jvec.push_back(7);
    jvec.push_back(8);
    jvec.push_back(9);
    jvec.push_back(10);

    for(auto itr = jvec.cbegin(); itr != jvec.cend(); ++itr)
    {
        std::cout<<*itr<<std::endl;
    }

    return 0;
}