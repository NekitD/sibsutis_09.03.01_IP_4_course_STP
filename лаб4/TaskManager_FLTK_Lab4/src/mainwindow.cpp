#include "mainwindow.h"

#include <FL/fl_ask.H>
#include <FL/Fl_Multiline_Input.H>

#include <sstream>
#include <cstdlib>

MainWindow::MainWindow()
    : window(1050, 700, "Task Manager - Lab 4"),

      projectLabel(20, 30, 210, 25, "Projects"),
      projectList(20, 60, 210, 320),
      progressLabel(20, 395, 210, 20, "Progress: -"),
      projectProgress(20, 420, 210, 24, ""),
      projectStatsLabel(20, 455, 210, 80, ""),
      addProjectButton(20, 550, 210, 32, "Add project"),
      deleteProjectButton(20, 590, 210, 32, "Delete project"),

      tasksLabel(250, 30, 780, 25, "Tasks"),
      taskList(250, 60, 780, 250),
      taskStatsLabel(250, 315, 780, 25, ""),

      titleLabel(250, 350, 260, 20, "Title"),
      titleInput(250, 370, 260, 28),
      descriptionLabel(520, 350, 510, 20, "Description"),
      descriptionInput(520, 370, 510, 28),

      priorityChoiceLabel(250, 405, 150, 20, "Priority"),
      priorityChoice(250, 425, 150, 28),
      statusChoiceLabel(415, 405, 150, 20, "Status"),
      statusChoice(415, 425, 150, 28),
      projectChoiceLabel(580, 405, 220, 20, "Project"),
      projectChoice(580, 425, 220, 28),

      searchLabel(250, 460, 200, 20, "Search"),
      searchInput(250, 480, 200, 28),
      statusFilterLabel(465, 460, 170, 20, "Status filter"),
      statusFilter(465, 480, 170, 28),
      priorityFilterLabel(650, 460, 170, 20, "Priority filter"),
      priorityFilter(650, 480, 170, 28),

      addTaskButton(250, 525, 145, 32, "Add"),
      updateTaskButton(405, 525, 145, 32, "Update"),
      deleteTaskButton(560, 525, 145, 32, "Delete"),
      moveTaskButton(715, 525, 145, 32, "Move to project"),
      clearButton(870, 525, 160, 32, "Clear"),

      statistics(20, 580, 1010, 100, "")
{
    window.resizable(window);

    projectLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    progressLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    projectStatsLabel.align(FL_ALIGN_LEFT | FL_ALIGN_TOP | FL_ALIGN_INSIDE);
    tasksLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    taskStatsLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    titleLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    descriptionLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    priorityChoiceLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    statusChoiceLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    projectChoiceLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    searchLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    statusFilterLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    priorityFilterLabel.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    statistics.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

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

    projectProgress.minimum(0);
    projectProgress.maximum(100);
    projectProgress.value(0);
    projectProgress.selection_color(FL_GRAY);

    projectList.callback(onProjectSelected, this);
    taskList.callback(onTaskSelected, this);
    addTaskButton.callback(onAddTask, this);
    updateTaskButton.callback(onUpdateTask, this);
    deleteTaskButton.callback(onDeleteTask, this);
    moveTaskButton.callback(onMoveTask, this);
    clearButton.callback(onClearForm, this);
    addProjectButton.callback(onAddProject, this);
    deleteProjectButton.callback(onDeleteProject, this);
    searchInput.callback(onFilterChanged, this);
    statusFilter.callback(onFilterChanged, this);
    priorityFilter.callback(onFilterChanged, this);
    // projectChoice.callback НЕ регистрируем — это поле формы, а не фильтр.

    refreshProjects();
    refreshProjectChoice();
    refreshTasks();
    refreshStatistics();
    updateProgressDisplay(selectedProjectId());
}

void MainWindow::show()
{
    window.show();
}

std::string MainWindow::choiceText(Fl_Choice& c)
{
    if (c.value() < 0 || c.text() == nullptr)
    {
        return "";
    }
    return c.text();
}

int MainWindow::selectedProjectId()
{
    int row = projectList.value();
    if (row <= 1)
    {
        return 0;
    }

    std::vector<Project> projects = manager.GetProjects();
    int idx = row - 2;
    if (idx < 0 || idx >= static_cast<int>(projects.size()))
    {
        return 0;
    }
    return projects[idx].getId();
}

int MainWindow::selectedTaskIndex()
{
    int row = taskList.value();
    if (row <= 0)
    {
        return -1;
    }

    const char* txt = taskList.text(row);
    if (txt == nullptr)
    {
        return -1;
    }

    std::string s(txt);
    size_t pos = s.find(" | ");
    if (pos == std::string::npos)
    {
        return -1;
    }

    int id = std::atoi(s.substr(0, pos).c_str());

    std::vector<Task> tasks = manager.GetTasks();
    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        if (tasks[i].GetId() == id)
        {
            return i;
        }
    }
    return -1;
}

void MainWindow::refreshProjects()
{
    int prevRow = projectList.value();
    if (prevRow < 1) prevRow = 0;

    projectList.clear();

    {
        std::vector<Task> all = manager.GetTasks();
        int unassigned = 0;
        for (int i = 0; i < static_cast<int>(all.size()); i++)
        {
            if (all[i].GetProjectId() == 0) unassigned++;
        }

        std::stringstream line;
        line << "(no project)  [" << unassigned << "]";
        projectList.add(line.str().c_str());
    }

    std::vector<Project> projects = manager.GetProjects();
    for (int i = 0; i < static_cast<int>(projects.size()); i++)
    {
        int pid = projects[i].getId();
        int total = manager.CountTasksInProject(pid);
        int done = manager.CountTasksInProjectByStatus(pid, "Done");
        double percent = manager.CompletionPercent(pid);

        std::stringstream line;
        line << projects[i].getName() << "  [";
        if (total == 0)
        {
            line << "0/0]  -";
        }
        else
        {
            line << done << "/" << total << "]  "
                 << static_cast<int>(percent + 0.5) << "%";
        }
        projectList.add(line.str().c_str());
    }

    int maxRow = projectList.size();
    int selectRow = 1;
    if (prevRow >= 1 && prevRow <= maxRow)
    {
        selectRow = prevRow;
    }
    else if (maxRow >= 2)
    {
        selectRow = 2;
    }
    projectList.select(selectRow);

    refreshProjectChoice();
}

void MainWindow::refreshProjectChoice()
{
    std::string currentText = choiceText(projectChoice);

    projectChoice.clear();
    projectChoice.add("(no project)");

    std::vector<Project> projects = manager.GetProjects();
    for (int i = 0; i < static_cast<int>(projects.size()); i++)
    {
        projectChoice.add(projects[i].getName().c_str());
    }

    int found = 0;
    for (int i = 0; i < static_cast<int>(projects.size()); i++)
    {
        if (projects[i].getName() == currentText)
        {
            found = i + 1;
            break;
        }
    }
    projectChoice.value(found);
}

void MainWindow::refreshTasks()
{
    taskList.clear();

    int row = projectList.value();
    int selectedPid = selectedProjectId();
    bool showOnlyUnassigned = (row == 1);
    bool showAll = (row <= 0);

    std::string search = searchInput.value() ? searchInput.value() : "";
    std::string selectedStatus = choiceText(statusFilter);
    std::string selectedPriority = choiceText(priorityFilter);

    std::vector<Task> tasks = manager.GetTasks();
    int shown = 0;

    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        if (showOnlyUnassigned)
        {
            if (tasks[i].GetProjectId() != 0) continue;
        }
        else if (!showAll)
        {
            if (tasks[i].GetProjectId() != selectedPid) continue;
        }

        std::string title = tasks[i].GetTitle();
        if (!search.empty() && title.find(search) == std::string::npos)
        {
            continue;
        }

        std::string status = tasks[i].GetStatus();
        if (selectedStatus != "All statuses" && status != selectedStatus)
        {
            continue;
        }

        std::string priority = tasks[i].GetPriority();
        if (selectedPriority != "All priorities" && priority != selectedPriority)
        {
            continue;
        }

        std::stringstream line;
        line << tasks[i].GetId() << " | " << title
             << " | " << priority << " | " << status;
        taskList.add(line.str().c_str());
        shown++;
    }

    int scope = static_cast<int>(tasks.size());
    if (showOnlyUnassigned)
    {
        scope = 0;
        for (int i = 0; i < static_cast<int>(tasks.size()); i++)
        {
            if (tasks[i].GetProjectId() == 0) scope++;
        }
    }
    else if (!showAll)
    {
        scope = manager.CountTasksInProject(selectedPid);
    }

    std::stringstream info;
    info << "Shown: " << shown << " of " << scope;
    taskStatsLabel.copy_label(info.str().c_str());
}

void MainWindow::refreshStatistics()
{
    std::vector<Task> tasks = manager.GetTasks();
    std::vector<Project> projects = manager.GetProjects();

    int total = static_cast<int>(tasks.size());
    int todo = 0, inProgress = 0, done = 0, noProject = 0;

    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        if (tasks[i].GetStatus() == "Todo") todo++;
        else if (tasks[i].GetStatus() == "In Progress") inProgress++;
        else if (tasks[i].GetStatus() == "Done") done++;

        if (tasks[i].GetProjectId() == 0) noProject++;
    }

    std::stringstream s;
    s << "Projects: " << projects.size()
      << "    Tasks: " << total
      << "    Todo: " << todo
      << "    In Progress: " << inProgress
      << "    Done: " << done
      << "    Without project: " << noProject;
    statistics.copy_label(s.str().c_str());
}

void MainWindow::updateProgressDisplay(int projectId)
{
    if (projectId == 0)
    {
        projectProgress.value(0);
        projectProgress.selection_color(FL_GRAY);

        if (projectList.value() == 1)
        {
            std::vector<Task> tasks = manager.GetTasks();
            int unassigned = 0;
            for (int i = 0; i < static_cast<int>(tasks.size()); i++)
            {
                if (tasks[i].GetProjectId() == 0) unassigned++;
            }

            progressLabel.copy_label("Unassigned tasks");

            std::stringstream s;
            s << "Total: " << unassigned;
            projectStatsLabel.copy_label(s.str().c_str());
        }
        else
        {
            progressLabel.copy_label("Progress: -");
            projectStatsLabel.copy_label("");
        }

        projectProgress.redraw();
        progressLabel.redraw();
        projectStatsLabel.redraw();
        return;
    }

    double percent = manager.CompletionPercent(projectId);
    projectProgress.value(percent);

    if (percent < 30.0)
    {
        projectProgress.selection_color(FL_RED);
    }
    else if (percent < 70.0)
    {
        projectProgress.selection_color(FL_YELLOW);
    }
    else
    {
        projectProgress.selection_color(FL_GREEN);
    }

    int total = manager.CountTasksInProject(projectId);
    int todo = manager.CountTasksInProjectByStatus(projectId, "Todo");
    int inProgress = manager.CountTasksInProjectByStatus(projectId, "In Progress");
    int done = manager.CountTasksInProjectByStatus(projectId, "Done");

    std::stringstream label;
    label << "Progress: " << static_cast<int>(percent + 0.5) << "%";
    progressLabel.copy_label(label.str().c_str());

    std::stringstream stats;
    stats << "Todo: " << todo << "\n"
          << "In Progress: " << inProgress << "\n"
          << "Done: " << done << "\n"
          << "Total: " << total;
    projectStatsLabel.copy_label(stats.str().c_str());

    projectProgress.redraw();
    progressLabel.redraw();
    projectStatsLabel.redraw();
}

void MainWindow::clearForm()
{
    titleInput.value("");
    descriptionInput.value("");
    priorityChoice.value(1);
    statusChoice.value(0);
    taskList.deselect();
}

void MainWindow::loadTaskToForm(int taskIndex)
{
    std::vector<Task> tasks = manager.GetTasks();
    if (taskIndex < 0 || taskIndex >= static_cast<int>(tasks.size()))
    {
        return;
    }

    const Task& t = tasks[taskIndex];
    titleInput.value(t.GetTitle().c_str());
    descriptionInput.value(t.GetDescription().c_str());

    std::string status = t.GetStatus();
    if (status == "Todo") statusChoice.value(0);
    else if (status == "In Progress") statusChoice.value(1);
    else if (status == "Done") statusChoice.value(2);

    std::string priority = t.GetPriority();
    if (priority == "Low") priorityChoice.value(0);
    else if (priority == "Medium") priorityChoice.value(1);
    else if (priority == "High") priorityChoice.value(2);

    std::vector<Project> projects = manager.GetProjects();
    int row = 0;
    if (t.GetProjectId() != 0)
    {
        for (int i = 0; i < static_cast<int>(projects.size()); i++)
        {
            if (projects[i].getId() == t.GetProjectId())
            {
                row = i + 1;
                break;
            }
        }
    }
    projectChoice.value(row);
}

void MainWindow::addTaskFromForm()
{
    std::string title = titleInput.value() ? titleInput.value() : "";
    if (title.empty())
    {
        fl_alert("Enter a task title.");
        return;
    }

    int projectId = 0;
    int pIdx = projectChoice.value();
    std::vector<Project> projects = manager.GetProjects();
    if (pIdx > 0 && pIdx - 1 < static_cast<int>(projects.size()))
    {
        projectId = projects[pIdx - 1].getId();
    }

    manager.AddTask(title,
                    descriptionInput.value() ? descriptionInput.value() : "",
                    choiceText(priorityChoice),
                    choiceText(statusChoice),
                    projectId);

    refreshProjects();
    refreshProjectChoice();

    if (projectId == 0)
    {
        projectList.select(1);
    }
    else
    {
        for (int i = 0; i < static_cast<int>(projects.size()); i++)
        {
            if (projects[i].getId() == projectId)
            {
                projectList.select(i + 2);
                break;
            }
        }
    }

    refreshTasks();
    refreshStatistics();
    updateProgressDisplay(selectedProjectId());
    clearForm();
}

void MainWindow::updateTaskFromForm()
{
    int index = selectedTaskIndex();
    if (index < 0)
    {
        fl_alert("Select a task first.");
        return;
    }

    std::string title = titleInput.value() ? titleInput.value() : "";
    if (title.empty())
    {
        fl_alert("Enter a task title.");
        return;
    }

    manager.UpdateTask(index,
                       title,
                       descriptionInput.value() ? descriptionInput.value() : "",
                       choiceText(priorityChoice),
                       choiceText(statusChoice));

    int pIdx = projectChoice.value();
    int newPid = 0;
    std::vector<Project> projects = manager.GetProjects();
    if (pIdx > 0 && pIdx - 1 < static_cast<int>(projects.size()))
    {
        newPid = projects[pIdx - 1].getId();
    }

    std::vector<Task> tasks = manager.GetTasks();
    if (index < static_cast<int>(tasks.size()) &&
        tasks[index].GetProjectId() != newPid)
    {
        manager.MoveTaskToProject(index, newPid);
    }

    refreshProjects();
    refreshTasks();
    refreshStatistics();
    updateProgressDisplay(selectedProjectId());
    clearForm();
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

    refreshProjects();
    refreshTasks();
    refreshStatistics();
    updateProgressDisplay(selectedProjectId());
    clearForm();
}

void MainWindow::moveSelectedTask()
{
    int index = selectedTaskIndex();
    if (index < 0)
    {
        fl_alert("Select a task first.");
        return;
    }

    int pIdx = projectChoice.value();
    int newPid = 0;
    std::vector<Project> projects = manager.GetProjects();
    if (pIdx > 0 && pIdx - 1 < static_cast<int>(projects.size()))
    {
        newPid = projects[pIdx - 1].getId();
    }

    std::vector<Task> tasks = manager.GetTasks();
    if (index < static_cast<int>(tasks.size()) &&
        tasks[index].GetProjectId() == newPid)
    {
        fl_alert("Task is already in the selected project.");
        return;
    }

    if (!manager.MoveTaskToProject(index, newPid))
    {
        fl_alert("Failed to move task.");
        return;
    }

    refreshProjects();
    refreshProjectChoice();

    if (newPid == 0)
    {
        projectList.select(1);
    }
    else
    {
        for (int i = 0; i < static_cast<int>(projects.size()); i++)
        {
            if (projects[i].getId() == newPid)
            {
                projectList.select(i + 2);
                break;
            }
        }
    }

    refreshTasks();
    refreshStatistics();
    updateProgressDisplay(selectedProjectId());
    clearForm();
}

void MainWindow::addProjectFromDialog()
{
    const char* name = fl_input("Project name:", "New project");
    if (name == nullptr || std::string(name).empty())
    {
        return;
    }

    manager.AddProject(name);

    refreshProjects();
    refreshProjectChoice();
    refreshTasks();
    refreshStatistics();
    updateProgressDisplay(selectedProjectId());
}

void MainWindow::deleteSelectedProject()
{
    int row = projectList.value();
    if (row <= 1)
    {
        fl_alert("Select a project first.");
        return;
    }

    std::vector<Project> projects = manager.GetProjects();
    int idx = row - 2;
    if (idx < 0 || idx >= static_cast<int>(projects.size()))
    {
        return;
    }

    int projectId = projects[idx].getId();
    std::string name = projects[idx].getName();
    int count = manager.CountTasksInProject(projectId);

    std::stringstream msg;
    msg << "Delete project \"" << name << "\"?";
    if (count > 0)
    {
        msg << "\nTogether with it " << count << " task(s) will be deleted.";
    }

    int answer = fl_choice("%s", "Cancel", "Delete", nullptr, msg.str().c_str());
    if (answer != 1)
    {
        return;
    }

    manager.DeleteProject(idx);

    refreshProjects();
    refreshProjectChoice();
    refreshTasks();
    refreshStatistics();
    updateProgressDisplay(selectedProjectId());
    clearForm();
}

void MainWindow::onProjectSelected(Fl_Widget*, void* data)
{
    MainWindow* w = static_cast<MainWindow*>(data);

    int row = w->projectList.value();
    int pid = w->selectedProjectId();

    if (row <= 1)
    {
        w->projectChoice.value(0);
    }
    else
    {
        std::vector<Project> projects = w->manager.GetProjects();
        int idx = row - 2;
        if (idx >= 0 && idx < static_cast<int>(projects.size()))
        {
            w->projectChoice.value(idx + 1);
        }
    }

    w->clearForm();
    w->refreshTasks();
    w->updateProgressDisplay(pid);
}

void MainWindow::onTaskSelected(Fl_Widget*, void* data)
{
    MainWindow* w = static_cast<MainWindow*>(data);
    int index = w->selectedTaskIndex();
    if (index >= 0)
    {
        w->loadTaskToForm(index);
    }
}

void MainWindow::onAddTask(Fl_Widget*, void* data)
{
    static_cast<MainWindow*>(data)->addTaskFromForm();
}

void MainWindow::onUpdateTask(Fl_Widget*, void* data)
{
    static_cast<MainWindow*>(data)->updateTaskFromForm();
}

void MainWindow::onDeleteTask(Fl_Widget*, void* data)
{
    static_cast<MainWindow*>(data)->deleteSelectedTask();
}

void MainWindow::onMoveTask(Fl_Widget*, void* data)
{
    static_cast<MainWindow*>(data)->moveSelectedTask();
}

void MainWindow::onClearForm(Fl_Widget*, void* data)
{
    static_cast<MainWindow*>(data)->clearForm();
}

void MainWindow::onAddProject(Fl_Widget*, void* data)
{
    static_cast<MainWindow*>(data)->addProjectFromDialog();
}

void MainWindow::onDeleteProject(Fl_Widget*, void* data)
{
    static_cast<MainWindow*>(data)->deleteSelectedProject();
}

void MainWindow::onFilterChanged(Fl_Widget*, void* data)
{
    static_cast<MainWindow*>(data)->refreshTasks();
}