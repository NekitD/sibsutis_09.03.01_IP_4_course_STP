#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Progress.H>

#include <string>

#include "taskmanager.h"

class MainWindow
{
private:
    TaskManager manager;

    Fl_Window   window;

    Fl_Box      projectLabel;
    Fl_Browser  projectList;
    Fl_Box      progressLabel;
    Fl_Progress projectProgress;
    Fl_Box      projectStatsLabel;
    Fl_Button   addProjectButton;
    Fl_Button   deleteProjectButton;

    Fl_Box      tasksLabel;
    Fl_Browser  taskList;
    Fl_Box      taskStatsLabel;

    Fl_Box      titleLabel;
    Fl_Input    titleInput;
    Fl_Box      descriptionLabel;
    Fl_Input    descriptionInput;

    Fl_Box      priorityChoiceLabel;
    Fl_Choice   priorityChoice;
    Fl_Box      statusChoiceLabel;
    Fl_Choice   statusChoice;
    Fl_Box      projectChoiceLabel;
    Fl_Choice   projectChoice;

    Fl_Box      searchLabel;
    Fl_Input    searchInput;
    Fl_Box      statusFilterLabel;
    Fl_Choice   statusFilter;
    Fl_Box      priorityFilterLabel;
    Fl_Choice   priorityFilter;

    Fl_Button   addTaskButton;
    Fl_Button   updateTaskButton;
    Fl_Button   deleteTaskButton;
    Fl_Button   moveTaskButton;
    Fl_Button   clearButton;

    Fl_Box      statistics;

    void refreshProjects();
    void refreshTasks();
    void refreshProjectChoice();
    void refreshStatistics();
    void updateProgressDisplay(int projectId);

    int selectedProjectId();
    int selectedTaskIndex();

    void clearForm();
    void loadTaskToForm(int taskIndex);

    void addTaskFromForm();
    void updateTaskFromForm();
    void deleteSelectedTask();
    void moveSelectedTask();
    void addProjectFromDialog();
    void deleteSelectedProject();

    std::string choiceText(Fl_Choice& c);

    static void onAddTask(Fl_Widget*, void*);
    static void onUpdateTask(Fl_Widget*, void*);
    static void onDeleteTask(Fl_Widget*, void*);
    static void onMoveTask(Fl_Widget*, void*);
    static void onClearForm(Fl_Widget*, void*);
    static void onAddProject(Fl_Widget*, void*);
    static void onDeleteProject(Fl_Widget*, void*);
    static void onProjectSelected(Fl_Widget*, void*);
    static void onTaskSelected(Fl_Widget*, void*);
    static void onFilterChanged(Fl_Widget*, void*);

public:
    MainWindow();
    void show();
};

#endif