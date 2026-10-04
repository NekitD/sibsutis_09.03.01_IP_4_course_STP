#pragma once
#include "db.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Browser.H>
#include <functional>
#include <string>

class AddOrderWindow : public Fl_Double_Window {
public:
    using SuccessCb = std::function<void(const std::string& code)>;
    AddOrderWindow(Database& db, const SuccessCb& on_success);

private:
    Database&   db_;
    SuccessCb   on_success_;
    Fl_Input*   contact_;
    Fl_Input*   cell_;
    Fl_Box*     error_;
    Fl_Button*  btn_ok_;
    Fl_Button*  btn_cancel_;

    static void cbOk(Fl_Widget*, void*);
    static void cbCancel(Fl_Widget*, void*);
    void doAdd();
};

class ReportWindow : public Fl_Double_Window {
public:
    explicit ReportWindow(Database& db);

private:
    Database&   db_;
    Fl_Tabs*    tabs_;
    Fl_Browser* list_accepted_;
    Fl_Browser* list_issued_;
    Fl_Browser* list_cancelled_;
    Fl_Box*     summary_;

    void loadAll();
    void fill(Fl_Browser* browser, const std::vector<Order>& orders);
    void setupBrowser(Fl_Browser* b);
};