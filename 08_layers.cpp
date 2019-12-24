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


int main(){
    std::ifstream myfile;
    myfile.open("c:\\Users\\Tom\\Desktop\\input.txt");
	std::string input;
	std::getline(myfile, input);

	std::vector<std::vector<char>> layers;
	std::vector<char> layer;
	const int itemsInLayer = 25 * 6;
	const int layerWidth = 25;

	for(int i = 0; i < input.size(); i++){
		if((i % itemsInLayer) == 0){
			layer.clear();
		}
		layer.push_back(input[i]);
		if((i % itemsInLayer) == (itemsInLayer - 1)){
			layers.push_back(layer);
		}
	}
    
	//part 1
	int index = 0;
	int minZeroCount = itemsInLayer;

	for(int i = 0; i < layers.size(); i++){
		int zeroCount = std::count(layers[i].begin(), layers[i].end(), '0'); 
		if(zeroCount < minZeroCount){
			minZeroCount = zeroCount;
			index = i;
		}
	}

	int val = std::count(layers[index].begin(), layers[index].end(), '1') *  
		std::count(layers[index].begin(), layers[index].end(), '2');
	
	printf("product: %d\n", val);

	//part 2
	for(int i = 0; i < itemsInLayer; i++){
		char finalColor;
		bool exitLoop = false;
		
		for(int j = 0; j < layers.size(); j++){
			switch(layers[j][i]){
				case '0':
					finalColor = ' ';
					exitLoop = true;
					break;

				case '1':
					finalColor = 'O';
					exitLoop = true;
					break;

				default:
					break;
			}
			if(exitLoop){
				break;
			}
		}
		
		putchar(finalColor);
		if((i + 1) % layerWidth == 0){
			putchar('\n');
		}
	}

    getchar();
    return 0;
}
