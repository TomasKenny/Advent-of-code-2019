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

class Computer{
public:
    void Run();
    
    void SetValue(int pos, int val){
        m_program[pos] = val;
    }

    void SetProgram(const std::vector<int> & program){
        m_program = program;
    }

    int GetValueAt(int pos){ return m_program[pos]; }

private:
    std::vector<int> m_program;
};

void Computer::Run(){
    int ptr = 0;
    bool halt = false;
    while(true){
        switch(m_program[ptr]){
            case 1:
                m_program[m_program[ptr+3]] = m_program[m_program[ptr+1]] + m_program[m_program[ptr+2]];
                break;
            case 2: 
                m_program[m_program[ptr+3]] = m_program[m_program[ptr+1]] * m_program[m_program[ptr+2]];
                break;
            case 99:
                halt = true;
        }
        if(halt){
            break;
        }
        ptr += 4;
    }
}

int main(){
    std::ifstream myfile;
    myfile.open("c:\\Users\\naplava\\Desktop\\input.txt");
    

    std::vector<int> program;
    int number;
     while(myfile >> number){
        program.push_back(number);
        if(myfile.peek() == ','){
            myfile.ignore(1);
        }
    }

    int noun, verb;
    for(noun = 0; noun <= 99; noun++){
        for(verb = 0; verb <= 99; verb++){
            Computer c;
            c.SetProgram(program);
            c.SetValue(1, noun);
            c.SetValue(2, verb);
            c.Run();
            if(c.GetValueAt(0) == 19690720){
                goto end;
            }
        }
    }
    
end:
    printf("show me: %d\n", noun * 100 + verb);
    getchar();
    return 0;
}