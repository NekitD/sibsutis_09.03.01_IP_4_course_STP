#include "project.h"

Project::Project() : id(0), name("")
{
}

Project::Project(int id, std::string name) : id(id), name(name)
{
}

int Project::getId() { return id; }
std::string Project::getName() { return name; }
void Project::setName(std::string name) { this->name = name; }
