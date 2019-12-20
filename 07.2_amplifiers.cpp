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

class IOConnector{
public:
    IOConnector(int ampCount){
        m_inputs.insert(m_inputs.begin(), ampCount, std::vector<int>());
        m_inputs[0].push_back(0);
    }

    int Input(int ampIndex){
        int val = m_inputs[ampIndex][0];
        m_inputs[ampIndex].erase(m_inputs[ampIndex].begin());
        return val;
    }

    void Output(int ampIndex, int val){
        int targetAmpIdx = (ampIndex + 1) % m_inputs.size();
        m_inputs[targetAmpIdx].push_back(val);
    }

    void SetAsFirst(int ampIndex, int val){
        m_inputs[ampIndex].insert(m_inputs[ampIndex].begin(), val);
    }

    int ReadLastOutput(int ampIndex) const {
        int targetAmpIdx = (ampIndex + 1) % m_inputs.size();
        return m_inputs[targetAmpIdx][m_inputs[targetAmpIdx].size() - 1];
    }

private:
    std::vector<std::vector<int>> m_inputs;
};


class Computer{
public:
    Computer(int id, IOConnector & io): m_id(id), m_io(io){}

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

    void Run(bool haltOnOutput = false);

    void SetValue(int pos, int val){
        m_program[pos] = val;
    }

    void SetProgram(const std::vector<int> & program){
        m_program = program;
    }

    int GetValueAt(int pos){ return m_program[pos]; }
    
    bool IsHalted(){ return isHalted; }

private:
    void ParseInstruction(int code, Opcode & opcode, ParamMode & param1mode, ParamMode & param2mode, ParamMode & param3mode);

    int Input(){ return m_io.Input(m_id); }

    void Output(int val){ m_io.Output(m_id, val); }

    int GetOperand(ParamMode paramMode, int val){
        return (paramMode == IMMEDIATE) ? m_program[val] : m_program[m_program[val]];
    }

    IOConnector & m_io;
    std::vector<int> m_program;
    int m_ptr = 0;
    bool isHalted = false;
    int m_id;
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

void Computer::Run(bool pauseOnOutput){
    
    Opcode opcode; 
    ParamMode param1mode, param2mode, param3mode;
    bool pause = false;

    if(isHalted){
        return;
    }

    while(true){
        ParseInstruction(m_program[m_ptr], opcode, param1mode, param2mode, param3mode);
        switch(opcode){
        default:
            throw std::runtime_error("unknown opcode");

        case ADD:
            m_program[m_program[m_ptr+3]] = GetOperand(param1mode, m_ptr + 1) + GetOperand(param2mode, m_ptr + 2);
            m_ptr += 4;
            break;

        case MULTIPLY: 
            m_program[m_program[m_ptr+3]] = GetOperand(param1mode, m_ptr + 1) * GetOperand(param2mode, m_ptr + 2);
            m_ptr += 4;
            break;

        case JUMP_IF_TRUE:
            if(GetOperand(param1mode, m_ptr + 1) != 0){
                m_ptr = GetOperand(param2mode, m_ptr + 2);
            }
            else{
                m_ptr += 3;
            }
            break;

        case JUMP_IF_FALSE:
            if(GetOperand(param1mode, m_ptr + 1) == 0){
                m_ptr = GetOperand(param2mode, m_ptr + 2);
            }
            else{
                m_ptr += 3;
            }
            break;

        case LT: 
            m_program[m_program[m_ptr+3]] = (GetOperand(param1mode, m_ptr + 1) < GetOperand(param2mode, m_ptr + 2)) ? 1 : 0;
            m_ptr += 4;
            break;

        case EQ: 
            m_program[m_program[m_ptr+3]] = (GetOperand(param1mode, m_ptr + 1) == GetOperand(param2mode, m_ptr + 2)) ? 1 : 0;
            m_ptr += 4;
            break;

        case INPUT:
            m_program[m_program[m_ptr+1]] = Input();
            m_ptr += 2;
            break;

        case OUTPUT:
            Output(m_program[m_program[m_ptr+1]]);
            m_ptr += 2;
            if(pauseOnOutput){
                pause = true;
            }
            break;

        case HALT:
            isHalted = true;
        }
        
        if(isHalted || pause){
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

    std::vector<int> val = {5,6,7,8,9};
    std::vector<int> ampSetting;
    int maxThrust = 0;
    
    do{
        //printf("%d%d%d%d%d\n", val[4], val[3], val[2], val[1], val[0]);
    
        IOConnector io(5);
        Computer c[5] = {{0, io}, {1, io}, {2, io}, {3, io}, {4, io}};
        for(auto & comp: c){ comp.SetProgram(program); }

        for(int i = 0; i < 5; i++){
            io.SetAsFirst(i, (val[i]));
        }

        while(true){
            for(int i = 0; i < 5; i++){
                c[i].Run(true);
            }
            if(c[4].IsHalted()){
                break;
            }
        }

        int thrust = io.ReadLastOutput(4);
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
