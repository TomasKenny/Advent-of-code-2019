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

using namespace std;

struct Coord {
	Coord(): x(0), y(0){} 
	Coord(int x, int y): x(x), y(y){}

	int x;
	int y;

	bool operator<(const Coord & other) const {
		if(x == other.x){
			return y < other.y;
		}
		else{
			return x < other.x;
		}
	}
};

struct CoordData{
	CoordData(): coordValue(0), wire1distance(INT_MAX), wire2distance(INT_MAX){} 
	int coordValue;
	int wire1distance;
	int wire2distance;
};

int ManhattanDist(const Coord & first, const Coord & second){
	return abs(first.x - second.x) + abs(first.y - second.y);
}

class Grid{
public:
	enum Direction{
		UP, DOWN, LEFT, RIGHT
	};

	static const int WIRE1_MASK = 0x1;
	static const int WIRE2_MASK = 0x2;
	static const int BOTH_WIRES = WIRE1_MASK | WIRE2_MASK;

	void ResetPos(){ m_currentPos = Coord(); }
	void ResetDistance(){ m_currentDistance = 0; }
	void Travel(Direction direction, int dist, int wireMask);
	std::vector<Coord> GetIntersections() const;
	Coord GetLowestDelayIntersection(int & delay) const;
	void SetCoordDistance(int distance, int x, int y, int wireMask);

private:
	Coord m_currentPos;
	int m_currentDistance;
	std::map<Coord, CoordData> m_points;
};

void Grid::SetCoordDistance(int distance, int x, int y, int wireMask){
	if(wireMask == WIRE1_MASK){
		m_points[Coord(x, y)].wire1distance = std::min(m_points[Coord(x, y)].wire1distance, distance);
	}
	else{
		m_points[Coord(x, y)].wire2distance = std::min(m_points[Coord(x, y)].wire2distance, distance);
	}
}

void Grid::Travel(Direction direction, int dist, int wireMask){
	switch(direction){
		case RIGHT:
			for(int x = m_currentPos.x + 1; x <= m_currentPos.x + dist; x++){
				m_points[Coord(x, m_currentPos.y)].coordValue |= wireMask;
				SetCoordDistance(++m_currentDistance, x, m_currentPos.y, wireMask);
			}
			m_currentPos.x += dist;
			break;

		case LEFT:
			for(int x = m_currentPos.x - 1; x >= m_currentPos.x - dist; x--){
				m_points[Coord(x, m_currentPos.y)].coordValue |= wireMask;
				SetCoordDistance(++m_currentDistance, x, m_currentPos.y, wireMask);
			}
			m_currentPos.x -= dist;
			break;
		
		case UP:
			for(int y = m_currentPos.y + 1; y <= m_currentPos.y + dist; y++){
				m_points[Coord(m_currentPos.x, y)].coordValue |= wireMask;
				SetCoordDistance(++m_currentDistance, m_currentPos.x, y, wireMask);
			}
			m_currentPos.y += dist;
			break;

		case DOWN:
			for(int y = m_currentPos.y - 1; y >= m_currentPos.y - dist; y--){
				m_points[Coord(m_currentPos.x, y)].coordValue |= wireMask;
				SetCoordDistance(++m_currentDistance, m_currentPos.x, y, wireMask);
			}
			m_currentPos.y -= dist;
			break;
	}
}

void tokenize(const string& str, vector<string>& tokens, const string& delimiters = ",")
{
  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);

  // Find first non-delimiter.
  string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos) {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));

    // Skip delimiters.
    lastPos = str.find_first_not_of(delimiters, pos);

    // Find next non-delimiter.
    pos = str.find_first_of(delimiters, lastPos);
  }
}

std::vector<Coord> Grid::GetIntersections() const {
	std::vector<Coord> intersections;
	for(std::map<Coord, CoordData>::const_iterator it = m_points.begin(); it != m_points.end(); it++){
		if((it->second.coordValue & BOTH_WIRES) == BOTH_WIRES){
			intersections.push_back(it->first);
		}
	}
	return intersections;
}

Coord Grid::GetLowestDelayIntersection(int & lowestDelay) const {
	Coord intersection;
	lowestDelay = INT_MAX;

	for(std::map<Coord, CoordData>::const_iterator it = m_points.begin(); it != m_points.end(); it++){
		if((it->second.coordValue & BOTH_WIRES) == BOTH_WIRES){
			int delay = it->second.wire1distance + it->second.wire2distance;
			if(delay < lowestDelay){
				intersection = it->first;
				lowestDelay = delay;
			}
		}
	}
	return intersection;
}

int main(){
	std::ifstream myfile;
	myfile.open("c:\\Users\\Tom\\Desktop\\input.txt");
	
	std::string wire[2];
	int wireMasks[2] = { Grid::WIRE1_MASK, Grid::WIRE2_MASK };
	getline(myfile, wire[0]);
	getline(myfile, wire[1]);


	std::vector<std::string> directions;
	Grid g;

	for(int j = 0; j < 2; j++){
		directions.clear();
		tokenize(wire[j], directions);
		g.ResetDistance();

		for(int i = 0; i < directions.size(); i++){
			switch(directions[i][0]){
				case 'U':
					g.Travel(Grid::UP, atoi(&directions[i][1]), wireMasks[j]);
					break;
				case 'D':
					g.Travel(Grid::DOWN, atoi(&directions[i][1]), wireMasks[j]);
					break;
				case 'L':
					g.Travel(Grid::LEFT, atoi(&directions[i][1]), wireMasks[j]);
					break;
				case 'R':
					g.Travel(Grid::RIGHT, atoi(&directions[i][1]), wireMasks[j]);
					break;
			}
		}
		g.ResetPos();
	}

	std::vector<Coord> intersections = g.GetIntersections();

	int distance = INT_MAX;

	for(int i = 0; i < intersections.size(); i++){
		int currentDistance = ManhattanDist(Coord(), intersections[i]);
		if(currentDistance < distance){
			distance = currentDistance;
		}
	}

	int delay;
	Coord intersection = g.GetLowestDelayIntersection(delay);

	printf("closest intersection: %d\n", distance);
	printf("intersection with lowest delay: [%d, %d], distance: %d\n", intersection.x, intersection.y, delay);
	getchar();
	return 0;
}
