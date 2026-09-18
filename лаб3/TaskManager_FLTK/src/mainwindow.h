#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Box.H>
#include <string>

#include "taskmanager.h"

class MainWindow
{
private:
    TaskManager manager;

    Fl_Window window;
    Fl_Box projectLabel;
    Fl_Box tasksLabel;
    Fl_Box titleLabel;
    Fl_Box descriptionLabel;
    Fl_Box searchLabel;
    Fl_Box statusChoiceLabel;
    Fl_Box priorityChoiceLabel;
    Fl_Box statusFilterLabel;
    Fl_Box priorityFilterLabel;

    Fl_Browser projectList;
    Fl_Browser taskList;

    Fl_Input titleInput;
    Fl_Input descriptionInput;
    Fl_Input searchInput;

    Fl_Choice statusChoice;
    Fl_Choice priorityChoice;
    Fl_Choice statusFilter;
    Fl_Choice priorityFilter;

    Fl_Button addTaskButton;
    Fl_Button updateTaskButton;
    Fl_Button deleteTaskButton;
    Fl_Button clearButton;
    Fl_Button addProjectButton;
    Fl_Button deleteProjectButton;

    Fl_Box statistics;

    void refreshProjects();
    void refreshTasks();
    void clearTaskFields();
    void loadSelectedTask();
    int selectedTaskIndex();

    static void onProjectSelected(Fl_Widget*, void* data);
    static void onTaskSelected(Fl_Widget*, void* data);
    static void onAddTask(Fl_Widget*, void* data);
    static void onUpdateTask(Fl_Widget*, void* data);
    static void onDeleteTask(Fl_Widget*, void* data);
    static void onClear(Fl_Widget*, void* data);
    static void onAddProject(Fl_Widget*, void* data);
    static void onDeleteProject(Fl_Widget*, void* data);
    static void onFilterChanged(Fl_Widget*, void* data);

    void addTaskFromForm();
    void updateTaskFromForm();
    void deleteSelectedTask();
    void addProjectFromDialog();
    void deleteSelectedProject();
    void updateFilter();

public:
    MainWindow();
    void show();
};

#endif
