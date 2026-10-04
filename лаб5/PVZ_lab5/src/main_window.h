#pragma once
#include "db.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Box.H>
#include <vector>

class MainWindow : public Fl_Double_Window {
public:
    MainWindow(Database& db);
    void refresh();

private:
    Database& db_;

    Fl_Box*          header_;
    Fl_Input*        search_;
    Fl_Choice*       filter_;
    Fl_Button*       btn_refresh_;
    Fl_Button*       btn_add_;
    Fl_Button*       btn_issue_;
    Fl_Button*       btn_cancel_;
    Fl_Button*       btn_report_;
    Fl_Button*       btn_cleanup_;

    Fl_Box*          col_code_;
    Fl_Box*          col_cust_;
    Fl_Box*          col_phone_;
    Fl_Box*          col_cell_;
    Fl_Box*          col_status_;
    Fl_Box*          col_date_;
    Fl_Hold_Browser* list_;
    Fl_Box*          status_;

    std::vector<Order> orders_;

    void buildUI();
    void rebuild();
    void setStatus(const std::string& txt, Fl_Color color);
    void doIssue();
    void doCancel();
    void doCleanup();
    void openAddWindow();
    void openReportWindow();

    static void cbRefresh(Fl_Widget*, void*);
    static void cbFilter(Fl_Widget*, void*);
    static void cbAdd(Fl_Widget*, void*);
    static void cbIssue(Fl_Widget*, void*);
    static void cbCancel(Fl_Widget*, void*);
    static void cbReport(Fl_Widget*, void*);
    static void cbCleanup(Fl_Widget*, void*);

    void resize(int X, int Y, int W, int H) override;
    void layout();
};