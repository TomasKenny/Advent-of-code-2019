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

class PlanetSystem{
public:
    void AddOrbit(const std::string & object, const std::string & parent){
        m_orbits[object] = parent;
    }
    void RemoveOrbit(const std::string & object){
        auto it = m_orbits.find(object);
        if(it != m_orbits.end()){
            m_orbits.erase(it);
        }
    }

    int GetParentsCount(const std::string & object) const;
    std::vector<std::string> GetAllObjects() const;
    void GetPathToRoot(const std::string & object, std::vector<std::string> & path) const;

private:
    std::map<std::string, std::string> m_orbits;  // <object, object's parent>
};

std::vector<std::string> PlanetSystem::GetAllObjects() const {
    std::set<std::string> objectsSet;
    for(const auto & obj: m_orbits){
        objectsSet.insert(obj.first);
        objectsSet.insert(obj.second);
    }
    return std::vector<std::string>(objectsSet.begin(), objectsSet.end());
}

int PlanetSystem::GetParentsCount(const std::string & object) const {
    auto it = m_orbits.find(object);
    if(it == m_orbits.end()){
        return 0;
    }
    return GetParentsCount(it->second) + 1;
}

void PlanetSystem::GetPathToRoot(const std::string & object, std::vector<std::string> & path) const {
    auto it = m_orbits.find(object);
    if(it == m_orbits.end()){
        return;
    }
    path.push_back(it->second);
    GetPathToRoot(it->second, path);
}

int main(){
    std::ifstream myfile;
    myfile.open("c:\\Users\\naplava\\Desktop\\input.txt");

    PlanetSystem system;
    std::string line;
    while(std::getline(myfile, line)){
        std::string object = line.substr(line.find(')') + 1);
        std::string parent = line.substr(0, line.find(')'));
        system.AddOrbit(object, parent);
    }

    int orbitsSum = 0;
    for(const auto & obj: system.GetAllObjects()){
        orbitsSum += system.GetParentsCount(obj);

    }

    std::vector<std::string> path, path2;
    system.GetPathToRoot("YOU", path);
    system.GetPathToRoot("SAN", path2);

    //remove common path
    std::string lastParent;
    while(path[path.size() - 1] == path2[path2.size() - 1]){
        lastParent = path[path.size() - 1];
        path.erase(path.end() - 1);
        path2.erase(path2.end() - 1);
    }

    //fake the root node
    system.RemoveOrbit(lastParent);
    int pathLength = system.GetParentsCount("YOU") + system.GetParentsCount("SAN") - 2;

    printf("orbits sum: %d\n", orbitsSum);
    printf("path from me to Santa: %d\n", pathLength);

    getchar();
    return 0;
}
