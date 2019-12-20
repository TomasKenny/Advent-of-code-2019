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

class IOProvider{
public:
    virtual int Input() = 0;
    virtual void Output(int val) = 0;
};

class IOProviderImpl: public IOProvider{
public:
    virtual int Input() override {
        return Get();
    }
    virtual void Output(int val) override {
        Set(val);
    }

public:
    void Set(int v){ m_values.push(v); } 
    
    int Get(){ 
        int value = m_values.top();
        m_values.pop();
        return value;
    }

private:
    std::stack<int> m_values;
};


class Computer{
public:
    Computer(IOProvider & io): m_io(io){}

    enum ParamMode {
        POSITION,
        IMMEDIATE
    };

    enum Opcode{
        ADD = 1,
        MULTIPLY = 2,
        INPUT = 3,
        OUTPUT = 4,
        JUMP_IF_TRUE = 5,
        JUMP_IF_FALSE = 6,
        LT = 7,
        EQ = 8,
        HALT = 99
    };

    void Run();

    void SetValue(int pos, int val){
        m_program[pos] = val;
    }

    void SetProgram(const std::vector<int> & program){
        m_program = program;
    }

    int GetValueAt(int pos){ return m_program[pos]; }

private:
    void ParseInstruction(int code, Opcode & opcode, ParamMode & param1mode, ParamMode & param2mode, ParamMode & param3mode);

    int Input(){ return m_io.Input(); }

    void Output(int val){ m_io.Output(val); }

    int GetOperand(ParamMode paramMode, int val){
        return (paramMode == IMMEDIATE) ? m_program[val] : m_program[m_program[val]];
    }

    IOProvider & m_io;
    std::vector<int> m_program;
};

void Computer::ParseInstruction(int code, Opcode & opcode, ParamMode & param1mode, ParamMode & param2mode, ParamMode & param3mode)
{
    char buffer[10] = {0};
    sprintf_s(buffer, "%05d", code);
    opcode = (Opcode)atoi(buffer + 3);
    param1mode = (ParamMode)(buffer[2] - '0');
    param2mode = (ParamMode)(buffer[1] - '0');
    param3mode = (ParamMode)(buffer[0] - '0');
}

void Computer::Run(){
    int ptr = 0;
    Opcode opcode; 
    ParamMode param1mode, param2mode, param3mode;
    bool halt = false;

    while(true){
        ParseInstruction(m_program[ptr], opcode, param1mode, param2mode, param3mode);
        switch(opcode){
        default:
            throw std::runtime_error("unknown opcode");

        case ADD:
            m_program[m_program[ptr+3]] = GetOperand(param1mode, ptr + 1) + GetOperand(param2mode, ptr + 2);
            ptr += 4;
            break;

        case MULTIPLY: 
            m_program[m_program[ptr+3]] = GetOperand(param1mode, ptr + 1) * GetOperand(param2mode, ptr + 2);
            ptr += 4;
            break;

        case JUMP_IF_TRUE:
            if(GetOperand(param1mode, ptr + 1) != 0){
                ptr = GetOperand(param2mode, ptr + 2);
            }
            else{
                ptr += 3;
            }
            break;

        case JUMP_IF_FALSE:
            if(GetOperand(param1mode, ptr + 1) == 0){
                ptr = GetOperand(param2mode, ptr + 2);
            }
            else{
                ptr += 3;
            }
            break;

        case LT: 
            m_program[m_program[ptr+3]] = (GetOperand(param1mode, ptr + 1) < GetOperand(param2mode, ptr + 2)) ? 1 : 0;
            ptr += 4;
            break;

        case EQ: 
            m_program[m_program[ptr+3]] = (GetOperand(param1mode, ptr + 1) == GetOperand(param2mode, ptr + 2)) ? 1 : 0;
            ptr += 4;
            break;

        case INPUT:
            m_program[m_program[ptr+1]] = Input();
            ptr += 2;
            break;

        case OUTPUT:
            Output(m_program[m_program[ptr+1]]);
            ptr += 2;
            break;

        case HALT:
            halt = true;
        }
        
        if(halt){
            break;
        }

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

    std::vector<int> val = {0, 1, 2, 3, 4};
    std::vector<int> ampSetting;
    int maxThrust = 0;
    
    do{
        //printf("%d%d%d%d%d\n", val[4], val[3], val[2], val[1], val[0]);
        IOProviderImpl io;
        io.Set(0);

        for(int i = 0; i < 5; i++){
            Computer c(io);
            io.Set(val[i]);
            c.SetProgram(program);
            c.Run();
        }

        int thrust = io.Get();
        if(thrust > maxThrust){
            printf("%d: %d%d%d%d%d\n", thrust, val[0], val[1], val[2], val[3], val[4]);
            maxThrust = thrust;
            ampSetting = val;
        }
    }while(std::next_permutation(val.begin(), val.end()));

    printf("max trust: %d\nfor combination: %d%d%d%d%d\n", 
        maxThrust, ampSetting[0], ampSetting[1], ampSetting[2], ampSetting[3], ampSetting[4]);

    return 0;
}
