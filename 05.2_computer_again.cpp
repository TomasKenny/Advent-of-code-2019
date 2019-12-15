#include "stdafx.h"
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
	
	int Input(){ return 5; }
	
	void Output(int val){ printf("%d\n", val); }
	
	int GetOperand(ParamMode paramMode, int val){
		return (paramMode == IMMEDIATE) ? m_program[val] : m_program[m_program[val]];
	}

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
    myfile.open("c:\\Users\\Tom\\Desktop\\input.txt");
    

    std::vector<int> program;
    int number;
     while(myfile >> number){
        program.push_back(number);
        if(myfile.peek() == ','){
            myfile.ignore(1);
        }
    }

    
    Computer c;
    c.SetProgram(program);
    c.Run();
       
    
    getchar();
    return 0;
}