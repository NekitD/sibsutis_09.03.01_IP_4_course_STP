#include "main_window.h"
#include "dialogs.h"
#include "theme.h"
#include <FL/fl_ask.H>
#include <sstream>

MainWindow::MainWindow(Database& db)
    : Fl_Double_Window(1260, 620, "ПВЗ — управление заказами"), db_(db)
{
    color(Theme::BG());
    buildUI();
    refresh();
    end();
}

void MainWindow::buildUI() {
    header_ = new Fl_Box(0, 0, w(), 44);
    header_->box(FL_FLAT_BOX);
    header_->color(Theme::ACCENT());
    header_->label("  ПВЗ  •  Управление заказами");
    header_->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    header_->labelfont(FL_HELVETICA_BOLD);
    header_->labelsize(16);
    header_->labelcolor(FL_WHITE);

    auto* toolbar = new Fl_Box(0, 44, w(), 56);
    toolbar->box(FL_FLAT_BOX);
    toolbar->color(Theme::SURFACE());

    search_ = new Fl_Input(0, 0, 0, 0);
    search_->textsize(14);
    search_->when(FL_WHEN_CHANGED);
    search_->callback(cbRefresh, this);

    filter_ = new Fl_Choice(0, 0, 0, 0);
    filter_->add("Все|К выдаче|Выдан|Отменён");
    filter_->value(0);
    filter_->textsize(13);
    filter_->callback(cbFilter, this);

    btn_refresh_ = new Fl_Button(0, 0, 0, 0, "Обновить");
    btn_refresh_->callback(cbRefresh, this);
    btn_refresh_->labelsize(13);

    btn_add_ = new Fl_Button(0, 0, 0, 0, "+  Принять");
    btn_add_->callback(cbAdd, this);
    btn_add_->labelsize(13);
    btn_add_->labelfont(FL_HELVETICA_BOLD);
    btn_add_->color(Theme::ACCENT());
    btn_add_->labelcolor(FL_WHITE);

    btn_issue_ = new Fl_Button(0, 0, 0, 0, "Выдать");
    btn_issue_->callback(cbIssue, this);
    btn_issue_->labelsize(13);

    btn_cancel_ = new Fl_Button(0, 0, 0, 0, "Отменить");
    btn_cancel_->callback(cbCancel, this);
    btn_cancel_->labelsize(13);

    btn_report_ = new Fl_Button(0, 0, 0, 0, "Отчёт");
    btn_report_->callback(cbReport, this);
    btn_report_->labelsize(13);

    btn_cleanup_ = new Fl_Button(0, 0, 0, 0, "Очистить историю");
    btn_cleanup_->callback(cbCleanup, this);
    btn_cleanup_->labelsize(13);
    btn_cleanup_->tooltip("Удалить все выданные и отменённые заказы");

    auto makeCol = [&](const char* t) {
        auto* b = new Fl_Box(0, 0, 0, 0, t);
        b->box(FL_FLAT_BOX);
        b->color(Theme::HEADER_BG());
        b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        b->labelfont(FL_HELVETICA_BOLD);
        b->labelsize(12);
        b->labelcolor(Theme::TEXT_MUTED());
        return b;
    };
    col_code_   = makeCol("  Код");
    col_cust_   = makeCol("  Телефон");
    col_phone_  = makeCol("  Артикул");
    col_cell_   = makeCol("  Ячейка");
    col_status_ = makeCol("  Статус");
    col_date_   = makeCol("  Поступил");

    list_ = new Fl_Hold_Browser(0, 0, 0, 0);
    list_->box(FL_DOWN_BOX);
    list_->color(Theme::SURFACE());
    list_->textsize(14);
    list_->textcolor(Theme::TEXT());
    list_->column_char('\t');

    static const int col_widths[] = {120, 160, 180, 100, 130, 180, 0};
    list_->column_widths(col_widths);
    list_->has_scrollbar(Fl_Browser_::VERTICAL);

    status_ = new Fl_Box(0, 0, 0, 0, "Готово");
    status_->box(FL_FLAT_BOX);
    status_->color(Theme::SURFACE());
    status_->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    status_->labelsize(12);
    status_->labelcolor(Theme::TEXT_MUTED());

    layout();
}

void MainWindow::layout() {
    int W = w(), H = h();
    header_->resize(0, 0, W, 44);

    const int Y = 44 + 12;
    const int BH = 32;
    const int LEFT = 12;
    const int RIGHT = 12;
    const int GAP = 8;

    int x = LEFT;
    search_->resize(x, Y, 280, BH);   x += 280 + GAP;
    filter_->resize(x, Y, 130, BH);   x += 130 + GAP;
    btn_refresh_->resize(x, Y, 100, BH);

    int r = W - RIGHT;
    btn_cleanup_->resize(r - 160, Y, 160, BH);  r -= 160 + GAP;
    btn_report_ ->resize(r - 100, Y, 100, BH);  r -= 100 + GAP;
    btn_cancel_ ->resize(r - 100, Y, 100, BH);  r -= 100 + GAP;
    btn_issue_  ->resize(r - 100, Y, 100, BH);  r -= 100 + GAP;
    btn_add_    ->resize(r - 120, Y, 120, BH);

    const int HDR_Y = Y + BH + 12;
    const int HDR_H = 26;
    int cw[] = {120, 160, 180, 100, 130, 180};
    int cx = LEFT;
    col_code_  ->resize(cx, HDR_Y, cw[0], HDR_H); cx += cw[0];
    col_cust_  ->resize(cx, HDR_Y, cw[1], HDR_H); cx += cw[1];
    col_phone_ ->resize(cx, HDR_Y, cw[2], HDR_H); cx += cw[2];
    col_cell_  ->resize(cx, HDR_Y, cw[3], HDR_H); cx += cw[3];
    col_status_->resize(cx, HDR_Y, cw[4], HDR_H); cx += cw[4];
    col_date_  ->resize(cx, HDR_Y, cw[5], HDR_H);

    int list_y = HDR_Y + HDR_H;
    int list_h = H - list_y - 26;
    list_->resize(LEFT, list_y, W - LEFT - RIGHT, list_h);

    status_->resize(0, H - 26, W, 26);

    redraw();
}

void MainWindow::resize(int X, int Y, int W, int H) {
    Fl_Double_Window::resize(X, Y, W, H);
    layout();
}

void MainWindow::refresh() { rebuild(); }

void MainWindow::rebuild() {
    list_->clear();

    const char* q = search_->value();
    auto all = db_.listOrders(q ? q : "");

    std::string want;
    switch (filter_->value()) {
        case 1: want = "ready";     break;
        case 2: want = "issued";    break;
        case 3: want = "cancelled"; break;
        default: want = "";         break;
    }

    orders_.clear();
    for (const auto& o : all) {
        if (want.empty() || o.status == want)
            orders_.push_back(o);
    }

    for (const auto& o : orders_) {
        Fl_Color c;
        if (o.status == "issued")         c = Theme::SUCCESS();
        else if (o.status == "cancelled") c = Theme::DANGER();
        else                              c = Theme::ACCENT();

        std::string phone_disp   = o.phone.empty()   ? "—" : o.phone;
        std::string article_disp = o.article.empty() ? "—" : o.article;
        std::string cell_disp    = o.cell.empty()    ? "—" : o.cell;

        std::ostringstream row;
        row << "@C" << (unsigned int)c
            << o.code << "\t"
            << phone_disp << "\t"
            << article_disp << "\t"
            << cell_disp << "\t"
            << Theme::statusRu(o.status) << "\t"
            << o.created_at;

        list_->add(row.str().c_str());
    }

    std::ostringstream s;
    s << "  Готово  •  Заказов: " << orders_.size();
    setStatus(s.str(), Theme::TEXT_MUTED());
}

void MainWindow::setStatus(const std::string& txt, Fl_Color color) {
    status_->copy_label(txt.c_str());
    status_->labelcolor(color);
    status_->redraw();
}

void MainWindow::cbRefresh(Fl_Widget*, void* self) {
    static_cast<MainWindow*>(self)->rebuild();
}
void MainWindow::cbFilter(Fl_Widget*, void* self) {
    static_cast<MainWindow*>(self)->rebuild();
}
void MainWindow::cbAdd(Fl_Widget*, void* self) {
    static_cast<MainWindow*>(self)->openAddWindow();
}
void MainWindow::cbIssue(Fl_Widget*, void* self) {
    static_cast<MainWindow*>(self)->doIssue();
}
void MainWindow::cbCancel(Fl_Widget*, void* self) {
    static_cast<MainWindow*>(self)->doCancel();
}
void MainWindow::cbReport(Fl_Widget*, void* self) {
    static_cast<MainWindow*>(self)->openReportWindow();
}
void MainWindow::cbCleanup(Fl_Widget*, void* self) {
    static_cast<MainWindow*>(self)->doCleanup();
}

void MainWindow::openAddWindow() {
    auto* w = new AddOrderWindow(db_, [this](const std::string& code) {
        setStatus("  Принят заказ " + code, Theme::SUCCESS());
        rebuild();
    });
    w->show();
}

void MainWindow::openReportWindow() {
    auto* w = new ReportWindow(db_);
    w->show();
}

void MainWindow::doIssue() {
    int idx = list_->value();
    if (idx <= 0 || idx > (int)orders_.size()) {
        fl_alert("Выберите заказ в списке.");
        return;
    }
    const auto& o = orders_[idx - 1];
    if (o.status != "ready") {
        fl_alert("Заказ нельзя выдать (статус: %s).", Theme::statusRu(o.status));
        return;
    }

    std::string what;
    if (!o.phone.empty() && !o.article.empty())
        what = "Телефон: " + o.phone + "\nАртикул: " + o.article;
    else if (!o.phone.empty())
        what = "Телефон: " + o.phone;
    else
        what = "Артикул: " + o.article;

    if (!o.cell.empty())
        what += "\nЯчейка: " + o.cell;

    if (fl_choice("Выдать заказ %s?\n%s",
                  "Отмена", "Выдать", nullptr,
                  o.code.c_str(), what.c_str()) == 1) {
        if (db_.issueOrder(o.code))
            setStatus("  Выдан " + o.code, Theme::SUCCESS());
        else
            setStatus("  Не удалось выдать " + o.code, Theme::DANGER());
        rebuild();
    }
}

void MainWindow::doCancel() {
    int idx = list_->value();
    if (idx <= 0 || idx > (int)orders_.size()) {
        fl_alert("Выберите заказ в списке.");
        return;
    }
    const auto& o = orders_[idx - 1];
    if (o.status != "ready") {
        fl_alert("Отменить можно только заказ «К выдаче».");
        return;
    }
    if (fl_choice("Отменить заказ %s?", "Нет", "Да", nullptr,
                  o.code.c_str()) == 1) {
        if (db_.cancelOrder(o.code))
            setStatus("  Отменён " + o.code, Theme::WARNING());
        else
            setStatus("  Не удалось отменить " + o.code, Theme::DANGER());
        rebuild();
    }
}

void MainWindow::doCleanup() {
    try {
        int n = db_.countClosedOrders();
        if (n == 0) {
            fl_alert("Нет выданных или отменённых заказов.");
            return;
        }
        if (fl_choice("Удалить %d заказ(ов) со статусами "
                      "«Выдан» и «Отменён»?\n\n"
                      "Это действие нельзя отменить.",
                      "Отмена", "Удалить", nullptr, n) == 1) {
            int deleted = db_.deleteClosedOrders();
            std::ostringstream s;
            s << "  Удалено заказов: " << deleted;
            setStatus(s.str(), Theme::TEXT_MUTED());
            rebuild();
        }
    } catch (const std::exception& e) {
        fl_alert("Ошибка: %s", e.what());
    }
}