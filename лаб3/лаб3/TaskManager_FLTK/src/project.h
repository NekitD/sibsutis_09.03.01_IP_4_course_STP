#ifndef PROJECT_H
#define PROJECT_H

#include <string>

class Project
{
private:
    int id;
    std::string name;

public:
    Project();
    Project(int id, std::string name);

    int getId();
    std::string getName();

    void setName(std::string name);
};

#endif
