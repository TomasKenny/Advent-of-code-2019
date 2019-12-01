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

__int64 GetFuel(__int64 mass){
	return mass / 3 - 2;
}

__int64 GetAdditionalFuel(__int64 mass){
	__int64 additionalFuel = 0;
	while(mass > 0){
		mass = std::max(GetFuel(mass), (__int64)0);
		additionalFuel += mass;
	}
	return additionalFuel;
}


int main(){
	std::ifstream myfile;
	myfile.open("c:\\Users\\Tom\\Desktop\\input.txt");
	
	__int64 mass, fuelSum = 0, additionalFuelSum = 0;
	
	while(myfile >> mass){
		fuelSum += GetFuel(mass);
		additionalFuelSum += GetAdditionalFuel(mass);
	}

	printf("fuel sum: %lld\n", fuelSum);
	printf("additional fuel sum: %lld\n", additionalFuelSum);
	getchar();
	return 0;
}
