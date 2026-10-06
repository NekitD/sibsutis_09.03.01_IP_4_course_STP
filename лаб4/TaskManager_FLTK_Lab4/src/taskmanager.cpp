#include "taskmanager.h"

// ============================================================
//  TODO: реализуйте ВСЕ методы TaskManager по спецификации.
//
//  ----- Базовые операции (были в лабораторной №3) -----
//
//  Конструктор:
//    nextTaskId    = 1
//    nextProjectId = 1
//    Репозитории пусты по умолчанию.
//
//  AddProject(name):
//    - создать Project(nextProjectId, name);
//    - увеличить nextProjectId на 1;
//    - добавить в projectRepository.
//
//  DeleteProject(index):
//    - если index вне диапазона — выйти;
//    - взять id проекта;
//    - УДАЛИТЬ ВСЕ ЗАДАЧИ ЭТОГО ПРОЕКТА (обход С КОНЦА,
//      иначе индексы сдвинутся после Remove);
//    - удалить проект из projectRepository.
//
//  GetProjects():
//    - вернуть projectRepository.GetAll().
//
//  AddTask(title, description, priority, status, projectId):
//    - создать Task(nextTaskId, title, projectId);
//    - увеличить nextTaskId на 1;
//    - установить description, priority, status;
//    - добавить в taskRepository.
//
//  UpdateTask(index, title, description, priority, status):
//    - если index вне диапазона — выйти;
//    - взять задачу по индексу;
//    - поменять title, description, priority, status
//      (id и projectId НЕ трогать!);
//    - положить обратно через taskRepository.Update.
//
//  DeleteTask(index):
//    - taskRepository.Remove(index).
//
//  GetTasks():
//    - вернуть taskRepository.GetAll().
//
//  ----- Новые операции (лабораторная №4) -----
//
//  MoveTaskToProject(taskIndex, projectId):
//    - если taskIndex вне диапазона — вернуть false;
//    - если projectId != 0 и такого проекта нет — вернуть false;
//    - взять задачу, вызвать SetProjectId(projectId);
//    - обновить через taskRepository.Update;
//    - вернуть true.
//    id задачи НЕ меняется.
//
//  CountTasksInProject(projectId):
//    - посчитать задачи, у которых GetProjectId() == projectId.
//
//  CountTasksInProjectByStatus(projectId, status):
//    - посчитать задачи проекта с указанным статусом.
//
//  CompletionPercent(projectId):
//    - если задач в проекте нет — вернуть 0.0;
//    - иначе 100.0 * (число задач со статусом "Done") / (всего задач).
// ============================================================

TaskManager::TaskManager() : nextProjectId(1), nextTaskId(1) {

}

void TaskManager::AddTask(std::string title, std::string description,
    std::string priority, std::string status, int projId) {

    Task new_task(nextTaskId, title, projId);
    new_task.SetDescription(description);
    new_task.SetPriority(priority);
    new_task.SetStatus(status);
    taskRepository.Add(new_task);

    nextTaskId++;

}

void TaskManager::UpdateTask(int index, std::string title, std::string description, std::string priority, std::string status)
{

    std::vector<Task> tasks = taskRepository.GetAll();
    if(index < 0 || index >= static_cast<int>(tasks.size()))
    {
        return;
    }

    Task task = tasks[index];
    task.SetTitle(title);
    task.SetDescription(description);
    task.SetPriority(priority);
    task.SetStatus(status);

    taskRepository.Update(index, task);
}

void TaskManager::DeleteTask(int index) {
    taskRepository.Remove(index);
}

std::vector<Task> TaskManager::GetTasks() const {
    return taskRepository.GetAll();
}

void TaskManager::AddProject(std::string name) {
    Project new_proj(nextProjectId, name);
    projectRepository.Add(new_proj);
    nextProjectId++;
}

void TaskManager::DeleteProject(int index)
{
    std::vector<Project> projects = projectRepository.GetAll();
    if(index < 0 || index >= static_cast<int>(projects.size()))
    {
        return;
    }

    int projectId = projects[index].getId();

    std::vector<Task> tasks = taskRepository.GetAll();
    for(int i = static_cast<int>(tasks.size()) - 1; i >= 0; i--)
    {
        if(tasks[i].GetProjectId() == projectId)
        {
            taskRepository.Remove(i);
        }
    }

    projectRepository.Remove(index);
}

std::vector<Project> TaskManager::GetProjects() const {
    return projectRepository.GetAll();
}

Project TaskManager::FindProject(int index) {
    return projectRepository.Find(index);
}

bool TaskManager::MoveTaskToProject(int taskIndex, int projectId)
{
    std::vector<Task> tasks = taskRepository.GetAll();

    if(taskIndex < 0 || taskIndex >= static_cast<int>(tasks.size())|| projectId != 0)
    {
        return false;
    }

    std::vector<Project> projects = projectRepository.GetAll();
    for(size_t i = 0; i < projects.size(); i++)
    {
        if(projects[i].getId() == projectId)
        {
            Task task = tasks[taskIndex];
            task.SetProjectId(projectId);
            taskRepository.Update(taskIndex, task);
            return true;
        }
    }
     return false;  
}

int TaskManager::CountTasksInProject(int projectId) {
    std::vector<Task> tasks = taskRepository.GetAll();
    int count = 0;
    for (std::vector<Task>::iterator task = tasks.begin(); task != tasks.end(); task++) {
        if (task->GetProjectId() == projectId) {
            count++;
        }
    }
    return count;
}

int TaskManager::CountTasksInProjectByStatus(int projectId, std::string status) {
    std::vector<Task> tasks = taskRepository.GetAll();
    int count = 0;
    for (std::vector<Task>::iterator task = tasks.begin(); task != tasks.end(); task++) {
        if (task->GetProjectId() == projectId && task->GetStatus() == status) {
            count++;
        }
    }
    return count;
}

double TaskManager::CompletionPercent(int projectId)
{
    int common = CountTasksInProject(projectId);
    if(common == 0)
    {
        return 0.0;
    }
    return 100.0 * CountTasksInProjectByStatus(projectId, "Done") / common;
}
