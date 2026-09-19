#include "task.h";


Task::Task(int _id, std::string _title): : id(_id), title(_title), description(""), status("Todo"), priority("Medium") {}

int Task::GetId() const{
	return id;
}

std::string Task::GetTitle() const {
	return title;
}

std::string Task::GetDescription() const {
	return description;
}

std::string Task::GetStatus() const {
	return status;
}

std::string Task::GetPriority() const {
	return priority;
}

void Task::SetTitle(std::string _title) {
	title = _title;
}

void Task::SetDescription(std::string _description) {
	description = _description;
}

void Task::SetStatus(std::string _status) {
	if(_status == "Todo" || _status == "In Progress" || _status == "Done")
	{
        status = _status;
    }
}

void Task::SetPriority(std::string _priority) {
	if(_priority == "Low" || _priority == "Medium" || _priority == "High")
	{
        priority = _priority;
    }
}
