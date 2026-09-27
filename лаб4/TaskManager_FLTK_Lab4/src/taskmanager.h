#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <string>
#include <vector>

#include "task.h"
#include "project.h"
#include "repository.h"

class TaskManager
{
private:
    Repository<Task> taskRepository;
    Repository<Project> projectRepository;

    int nextTaskId;
    int nextProjectId;

public:
    TaskManager();

    void AddTask(std::string title,
                 std::string description,
                 std::string priority,
                 std::string status,
                 int projectId);

    void UpdateTask(int index,
                    std::string title,
                    std::string description,
                    std::string priority,
                    std::string status);

    void DeleteTask(int index);
    std::vector<Task> GetTasks();

    bool MoveTaskToProject(int taskIndex, int projectId);

    int CountTasksInProject(int projectId);
    int CountTasksInProjectByStatus(int projectId, std::string status);
    double CompletionPercent(int projectId);

    void AddProject(std::string name);
    void DeleteProject(int index);
    std::vector<Project> GetProjects() const;
};

#endif