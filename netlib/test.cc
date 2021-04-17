#include <stdio.h>
#include <stdlib.h>
#include <iostream>
struct dp {
    int k;
    int l;
};

#include <vector>

void make_heap(std::vector<int> &vec) {
    int len = vec.size();
    for(int i = len / 2 - 1; i >= 0; --i) {
        int s = i;
        int temp = vec[i];
        for(int j = s*2+1; j < len; j = j*2+1) {
            if(j+1 < len && vec[j] > vec[j+1])
                ++j;
            if(vec[j] >= temp)
                break;
            vec[s] = vec[j];
            s = j;
        }
        vec[s] = temp;
    }
}

int main()
{
    std::vector<int> vec = {6,2,7,33,4,9,8,0,56,4,4,1};
    make_heap(vec);
    for(auto k : vec) {
        std::cout << k << " ";
    }
    std::cout << "\n";
}