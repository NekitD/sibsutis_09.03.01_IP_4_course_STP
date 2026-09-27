#ifndef TASK_H
#define TASK_H

#include <string>

class Task
{
private:
    int id;
    std::string title;
    std::string description;
    std::string status;
    std::string priority;

public:
    Task(int _id, std::string _title);

    int GetId() const;
    std::string GetTitle() const;
    std::string GetDescription() const;
    std::string GetStatus() const;
    std::string GetPriority() const;

    void SetTitle(std::string _title);
    void SetDescription(std::string _description);
    void SetStatus(std::string _status);
    void SetPriority(std::string _priority);
};

#endif
