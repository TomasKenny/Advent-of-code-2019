#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <set>
#include <deque>
#include <map>
#include <regex>
#include "assert.h"
#include <string>
#include <stack>

bool IsPasswordOk(int passwordInt){
    char password[10] = {0};
    sprintf_s(password, "%d", passwordInt);
    
    if(password[0] > password[1] ||
        password[1] > password[2] ||
        password[2] > password[3] ||
        password[3] > password[4] ||
        password[4] > password[5])
    {
        return false;
    }

    if(password[0] == password[1] ||
        password[1] == password[2] ||
        password[2] == password[3] ||
        password[3] == password[4] ||
        password[4] == password[5])
    {
        return true;
    }
    return false;
}

bool IsPasswordOkVer2(int passwordInt){
    char password[10] = {0};
    sprintf_s(password, "%d", passwordInt);

    if(password[0] > password[1] ||
        password[1] > password[2] ||
        password[2] > password[3] ||
        password[3] > password[4] ||
        password[4] > password[5])
    {
        return false;
    }

    for(int i = 0; i < (6 - 1); i++){
        if((password[i] == password[i + 1]) && 
            (((i + 2) > (6 - 1)) || (password[i] != password[i + 2])))
        {
            return true;
        }

        //skip the rest of the block
        int blockBegin = i;
        while(i < 5 && (password[i] == password[i+1])){
            i++;
        }
    }
    return false;
}

int main(){
    int counter = 0;
    for(int i = 231832; i <= 767346; i++){
        if(IsPasswordOkVer2(i)){
            counter++;
        }
    }
    
    printf("show me: %d\n", counter);

    getchar();
    return 0;
}