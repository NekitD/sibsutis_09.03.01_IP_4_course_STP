#include "taskmanager.h"

TaskManager::TaskManager() {

}

void TaskManager::AddTask(std::string title, std::string description, 
    std::string priority, std::string status) {

    Task new_task(taskRepository.Size(), title);
    new_task.SetDescription(description);
    new_task.SetPriority(priority);
    new_task.SetStatus(status);
    taskRepository.Add(new_task);

}

void TaskManager::UpdateTask(int index, std::string title, std::string description,
    std::string priority, std::string status) {

    Task new_task(index, title);
    new_task.SetDescription(description);
    new_task.SetPriority(priority);
    new_task.SetStatus(status);

    taskRepository.Update(index, new_task);

}

void TaskManager::DeleteTask(int index) {
    taskRepository.Remove(index);
}

std::vector<Task> TaskManager::GetTasks() const{
    return taskRepository.GetAll();
}

void TaskManager::AddProject(std::string name) {
    Project new_proj(projectRepository.Size(), name);
    projectRepository.Add(new_proj);
}

void TaskManager::DeleteProject(int index) {
    projectRepository.Remove(index);
}

std::vector<Project> TaskManager::GetProjects() const{
    return projectRepository.GetAll();
}