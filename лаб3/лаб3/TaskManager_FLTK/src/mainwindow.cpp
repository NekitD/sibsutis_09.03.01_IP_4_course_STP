#include "mainwindow.h"

#include <FL/fl_ask.H>
#include <FL/fl_message.H>
#include <FL/Fl_Multiline_Input.H>

#include <sstream>

namespace
{
    MainWindow* currentWindow = nullptr;

    std::string choiceText(Fl_Choice& choice)
    {
        if (choice.value() < 0 || choice.text() == nullptr)
        {
            return "";
        }
        return choice.text();
    }
}


MainWindow::MainWindow()
    : window(1050, 700, "Task Manager - C++ Templates Lab"),
      projectLabel(20, 30, 210, 25, "Projects"),
      tasksLabel(250, 30, 780, 25, "Tasks"),
      projectList(20, 60, 210, 390),
      taskList(250, 60, 780, 300),

      titleLabel(250, 375, 280, 25, "Title"),
      descriptionLabel(545, 375, 485, 25, "Description"),
      titleInput(250, 400, 280, 32),
      descriptionInput(545, 400, 485, 32),

      searchLabel(250, 445, 250, 25, "Search"),
      statusChoiceLabel(515, 445, 145, 25, "Status"),
      priorityChoiceLabel(675, 445, 145, 25, "Priority"),
      searchInput(250, 470, 250, 32),
      statusChoice(515, 470, 145, 32),
      priorityChoice(675, 470, 145, 32),

      statusFilterLabel(250, 515, 180, 25, "Status filter"),
      priorityFilterLabel(445, 515, 180, 25, "Priority filter"),
      statusFilter(250, 540, 180, 32),
      priorityFilter(445, 540, 180, 32),

      addTaskButton(840, 465, 140, 32, "Add"),
      updateTaskButton(840, 505, 140, 32, "Update"),
      deleteTaskButton(840, 545, 140, 32, "Delete"),
      clearButton(650, 540, 160, 32, "Clear"),

      addProjectButton(20, 470, 210, 32, "Add project"),
      deleteProjectButton(20, 510, 210, 32, "Delete project"),
      statistics(20, 600, 1010, 40, "")
{
    currentWindow = this;

    window.resizable(window);

    projectLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    tasksLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    titleLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    descriptionLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    searchLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    statusChoiceLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    priorityChoiceLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    statusFilterLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    priorityFilterLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

    projectList.type(FL_HOLD_BROWSER);
    taskList.type(FL_HOLD_BROWSER);

    statusChoice.add("Todo");
    statusChoice.add("In Progress");
    statusChoice.add("Done");
    statusChoice.value(0);

    priorityChoice.add("Low");
    priorityChoice.add("Medium");
    priorityChoice.add("High");
    priorityChoice.value(1);

    statusFilter.add("All statuses");
    statusFilter.add("Todo");
    statusFilter.add("In Progress");
    statusFilter.add("Done");
    statusFilter.value(0);

    priorityFilter.add("All priorities");
    priorityFilter.add("Low");
    priorityFilter.add("Medium");
    priorityFilter.add("High");
    priorityFilter.value(0);

    projectList.callback(onProjectSelected, this);
    taskList.callback(onTaskSelected, this);
    addTaskButton.callback(onAddTask, this);
    updateTaskButton.callback(onUpdateTask, this);
    deleteTaskButton.callback(onDeleteTask, this);
    clearButton.callback(onClear, this);
    addProjectButton.callback(onAddProject, this);
    deleteProjectButton.callback(onDeleteProject, this);
    searchInput.callback(onFilterChanged, this);
    statusFilter.callback(onFilterChanged, this);
    priorityFilter.callback(onFilterChanged, this);

    refreshProjects();
    refreshTasks();
}

void MainWindow::show()
{
    window.show();
}

void MainWindow::refreshProjects()
{
    projectList.clear();

    std::vector<Project> projects = manager.GetProjects();
    for (int i = 0; i < static_cast<int>(projects.size()); i++)
    {
        projectList.add(projects[i].getName().c_str());
    }

    if (!projects.empty())
    {
        projectList.select(1);
    }
}

void MainWindow::refreshTasks()
{
    taskList.clear();

    std::vector<Task> tasks = manager.GetTasks();
    std::string search = searchInput.value();
    std::string selectedStatus = statusFilter.text() == nullptr ? "All statuses" : statusFilter.text();
    std::string selectedPriority = priorityFilter.text() == nullptr ? "All priorities" : priorityFilter.text();

    int shown = 0;
    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        std::string title = tasks[i].GetTitle();
        std::string status = tasks[i].GetStatus();
        std::string priority = tasks[i].GetPriority();

        bool matchesSearch = search.empty() || title.find(search) != std::string::npos;
        bool matchesStatus = selectedStatus == "All statuses" || status == selectedStatus;
        bool matchesPriority = selectedPriority == "All priorities" || priority == selectedPriority;

        if (matchesSearch && matchesStatus && matchesPriority)
        {
            std::stringstream line;
            line << tasks[i].GetId() << " | " << title << " | " << priority << " | " << status;
            taskList.add(line.str().c_str());
            shown++;
        }
    }

    std::stringstream info;
    info << "Tasks: " << tasks.size() << "    Shown: " << shown
         << "    Projects: " << manager.GetProjects().size();
    statistics.copy_label(info.str().c_str());
}

int MainWindow::selectedTaskIndex()
{
    int row = taskList.value();
    if (row <= 0)
    {
        return -1;
    }

    std::string selected = taskList.text(row);
    std::vector<Task> tasks = manager.GetTasks();
    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        std::stringstream prefix;
        prefix << tasks[i].GetId() << " | ";
        if (selected.find(prefix.str()) == 0)
        {
            return i;
        }
    }

    return -1;
}

void MainWindow::loadSelectedTask()
{
    int index = selectedTaskIndex();
    if (index < 0)
    {
        return;
    }

    std::vector<Task> tasks = manager.GetTasks();
    titleInput.value(tasks[index].GetTitle().c_str());
    descriptionInput.value(tasks[index].GetDescription().c_str());

    if (tasks[index].GetStatus() == "Todo") statusChoice.value(0);
    if (tasks[index].GetStatus() == "In Progress") statusChoice.value(1);
    if (tasks[index].GetStatus() == "Done") statusChoice.value(2);

    if (tasks[index].GetPriority() == "Low") priorityChoice.value(0);
    if (tasks[index].GetPriority() == "Medium") priorityChoice.value(1);
    if (tasks[index].GetPriority() == "High") priorityChoice.value(2);
}

void MainWindow::clearTaskFields()
{
    titleInput.value("");
    descriptionInput.value("");
    statusChoice.value(0);
    priorityChoice.value(1);
    taskList.deselect();
}

void MainWindow::addTaskFromForm()
{
    if (std::string(titleInput.value()).empty())
    {
        fl_alert("Enter a task title.");
        return;
    }

    manager.AddTask(titleInput.value(), descriptionInput.value(),
                    choiceText(priorityChoice), choiceText(statusChoice));
    refreshTasks();
    clearTaskFields();
}

void MainWindow::updateTaskFromForm()
{
    int index = selectedTaskIndex();
    if (index < 0)
    {
        fl_alert("Select a task first.");
        return;
    }

    manager.UpdateTask(index, titleInput.value(), descriptionInput.value(),
                       choiceText(priorityChoice), choiceText(statusChoice));
    refreshTasks();
}

void MainWindow::deleteSelectedTask()
{
    int index = selectedTaskIndex();
    if (index < 0)
    {
        fl_alert("Select a task first.");
        return;
    }

    manager.DeleteTask(index);
    refreshTasks();
    clearTaskFields();
}

void MainWindow::addProjectFromDialog()
{
    const char* name = fl_input("Project name:", "New project");
    if (name != nullptr && std::string(name).empty() == false)
    {
        manager.AddProject(name);
        refreshProjects();
        refreshTasks();
    }
}

void MainWindow::deleteSelectedProject()
{
    int row = projectList.value();
    if (row <= 0)
    {
        fl_alert("Select a project first.");
        return;
    }

    manager.DeleteProject(row - 1);
    refreshProjects();
    refreshTasks();
}

void MainWindow::updateFilter()
{
    refreshTasks();
}

void MainWindow::onProjectSelected(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);

    int row = window->projectList.value();
    if (row > 0)
    {
        const char* name = window->projectList.text(row);

        std::stringstream info;
        info << "Selected project: " << (name == nullptr ? "" : name)
             << "    Tasks: " << window->manager.GetTasks().size()
             << "    Projects: " << window->manager.GetProjects().size();

        window->statistics.copy_label(info.str().c_str());
    }
}

void MainWindow::onTaskSelected(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->loadSelectedTask();
}

void MainWindow::onAddTask(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->addTaskFromForm();
}

void MainWindow::onUpdateTask(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->updateTaskFromForm();
}

void MainWindow::onDeleteTask(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->deleteSelectedTask();
}

void MainWindow::onClear(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->clearTaskFields();
}

void MainWindow::onAddProject(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->addProjectFromDialog();
}

void MainWindow::onDeleteProject(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->deleteSelectedProject();
}

void MainWindow::onFilterChanged(Fl_Widget*, void* data)
{
    MainWindow* window = static_cast<MainWindow*>(data);
    window->updateFilter();
}
