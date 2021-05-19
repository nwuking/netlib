
#include <string>
#include <vector>
#include <iostream>


void Split(const std::string &str, char ch, std::vector<std::string> &vec) {
    int len = str.length();
    int start = 0;

    for(int i = 0; i < len; ++i) {
        if(str[i] != ch) {
            start = i;
            
            for(int j = i+1; j < len; ++j) {
                if(str[j] == ch) {
                    std::string temp(str.begin()+start, str.begin()+j);
                    vec.push_back(temp);
                    i = j;
                    break;
                }
                if(str[j] !=ch && j == len-1) {
                    std::string temp(str.begin()+start, str.end());
                    vec.push_back(temp);
                    i = j;
                }
                
             }
        }
    }
   
}

#include <string.h>

#include <sys/types.h>
#include <stddef.h>

int atoi(char *pData) {
    int op = 1;
    char *p = pData;
    if(*p == '-') {
        op = -1;
        ++p;
    }
    if(*p == '+')
        ++p;
    int res = 0;
    while(*p >= '0' && *p <= '9') {
        int t = *p - '0';
        if(op == 1) {
            if(INT32_MAX - res*10 < t) {
                std::cout << "+error" << std::endl;
                break;
            }
            res = res * 10 + t;
        }
        else {
            if(INT32_MIN - res*10  > t*(-1)) {
                std::cout << "-error" << std::endl;
                break;
            }
            
            res = res * 10 - t;
        }
        ++p;
    }
    return res;
}

#include <time.h>

#include <cstdlib>

int func() {
    std::cout << "func(int) " <<std::endl;
    return 9;
}



class oopp;
#include <vector>
int main() {
    
    std::cout << sizeof(long) << "\n" << sizeof(long long) << std::endl;
    int t = 2;
    const int t1 = 2;
    static_cast<long>(t);
    static_cast<long>(t1);
    return 0;
}