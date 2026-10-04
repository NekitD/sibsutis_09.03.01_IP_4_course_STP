#include "dialogs.h"
#include "theme.h"
#include <sstream>
#include <iomanip>

// ═══════════════════════════════════════════════════════════════
// AddOrderWindow
// ═══════════════════════════════════════════════════════════════
AddOrderWindow::AddOrderWindow(Database& db, const SuccessCb& cb)
    : Fl_Double_Window(460, 260, "Приёмка заказа"), db_(db), on_success_(cb)
{
    color(Theme::SURFACE());
    set_modal();

    auto* hdr = new Fl_Box(0, 0, 460, 44);
    hdr->box(FL_FLAT_BOX);
    hdr->color(Theme::ACCENT());
    hdr->label("Новый заказ");
    hdr->labelfont(FL_HELVETICA_BOLD);
    hdr->labelsize(16);
    hdr->labelcolor(FL_WHITE);

    // ── Телефон или артикул ───────────────────────────────────
    auto* l1 = new Fl_Box(20, 62, 200, 26, "Телефон или артикул:");
    l1->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    l1->labelsize(14);
    l1->labelcolor(Theme::TEXT());

    contact_ = new Fl_Input(200, 62, 240, 30);
    contact_->textsize(14);
    contact_->tooltip("С этикетки: номер получателя или трек-номер посылки");

    auto* hint = new Fl_Box(200, 94, 240, 20,
        "Например: +79990001122  или  ART-123456789");
    hint->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    hint->labelsize(11);
    hint->labelcolor(Theme::TEXT_MUTED());

    // ── Ячейка ────────────────────────────────────────────────
    auto* l2 = new Fl_Box(20, 124, 200, 26, "Ячейка хранения:");
    l2->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    l2->labelsize(14);
    l2->labelcolor(Theme::TEXT());

    cell_ = new Fl_Input(200, 124, 240, 30);
    cell_->textsize(14);
    cell_->tooltip("Куда положили посылку. Можно оставить пустым");

    auto* hint2 = new Fl_Box(200, 156, 240, 20,
        "Например: A1, B12, полка-3");
    hint2->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    hint2->labelsize(11);
    hint2->labelcolor(Theme::TEXT_MUTED());

    // ── Ошибка ────────────────────────────────────────────────
    error_ = new Fl_Box(20, 180, 420, 22, "");
    error_->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    error_->labelsize(12);
    error_->labelcolor(Theme::DANGER());

    // ── Кнопки ────────────────────────────────────────────────
    btn_cancel_ = new Fl_Button(220, 212, 110, 34, "Отмена");
    btn_cancel_->callback(cbCancel, this);
    btn_cancel_->labelsize(14);

    btn_ok_ = new Fl_Button(340, 212, 100, 34, "Принять");
    btn_ok_->callback(cbOk, this);
    btn_ok_->labelsize(14);
    btn_ok_->labelfont(FL_HELVETICA_BOLD);
    btn_ok_->color(Theme::ACCENT());
    btn_ok_->labelcolor(FL_WHITE);

    end();
}

void AddOrderWindow::cbCancel(Fl_Widget*, void* self) {
    static_cast<AddOrderWindow*>(self)->hide();
}

void AddOrderWindow::cbOk(Fl_Widget*, void* self) {
    static_cast<AddOrderWindow*>(self)->doAdd();
}

void AddOrderWindow::doAdd() {
    std::string contact = contact_->value() ? contact_->value() : "";
    std::string cell    = cell_->value()    ? cell_->value()    : "";

    if (contact.size() < 5) {
        error_->copy_label("Введите телефон или артикул посылки");
        return;
    }
    if (cell.size() > 30) {
        error_->copy_label("Название ячейки слишком длинное (максимум 30)");
        return;
    }

    try {
        std::string code = db_.addOrder(contact, cell);
        if (on_success_) on_success_(code);
        hide();
    } catch (const std::exception& e) {
        error_->copy_label(e.what());
    }
}

// ═══════════════════════════════════════════════════════════════
// ReportWindow
// ═══════════════════════════════════════════════════════════════
static const int REPORT_W = 900;
static const int REPORT_H = 560;

ReportWindow::ReportWindow(Database& db)
    : Fl_Double_Window(REPORT_W, REPORT_H, "Отчёт по заказам"), db_(db)
{
    color(Theme::SURFACE());

    auto* hdr = new Fl_Box(0, 0, REPORT_W, 44);
    hdr->box(FL_FLAT_BOX);
    hdr->color(Theme::ACCENT());
    hdr->label("Отчёт по заказам");
    hdr->labelfont(FL_HELVETICA_BOLD);
    hdr->labelsize(16);
    hdr->labelcolor(FL_WHITE);

    const int TABS_X = 10;
    const int TABS_Y = 54;
    const int TABS_W = REPORT_W - 20;
    const int TABS_H = REPORT_H - 54 - 46;

    tabs_ = new Fl_Tabs(TABS_X, TABS_Y, TABS_W, TABS_H);

    auto makePage = [&](const char* title, Fl_Browser*& out_browser) {
        auto* page = new Fl_Group(TABS_X + 5, TABS_Y + 25,
                                  TABS_W - 10, TABS_H - 30, title);
        out_browser = new Fl_Browser(TABS_X + 10, TABS_Y + 30,
                                     TABS_W - 20, TABS_H - 40);
        setupBrowser(out_browser);
        page->end();
    };

    makePage("Принято",   list_accepted_);
    makePage("Выдано",    list_issued_);
    makePage("Отменено",  list_cancelled_);

    tabs_->end();

    summary_ = new Fl_Box(15, REPORT_H - 42, REPORT_W - 160, 28, "");
    summary_->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    summary_->labelsize(13);
    summary_->labelcolor(Theme::TEXT_MUTED());

    auto* btn_close = new Fl_Button(REPORT_W - 130, REPORT_H - 42, 120, 32, "Закрыть");
    btn_close->callback([](Fl_Widget*, void* w) {
        static_cast<Fl_Window*>(w)->hide();
    }, this);
    btn_close->labelsize(14);

    end();

    loadAll();
}

void ReportWindow::setupBrowser(Fl_Browser* b) {
    b->box(FL_DOWN_BOX);
    b->color(Theme::SURFACE());
    b->textsize(13);
    b->textcolor(Theme::TEXT());
    b->column_char('\t');

    static const int widths[] = {120, 180, 180, 100, 130, 180, 0};
    b->column_widths(widths);
    b->has_scrollbar(Fl_Browser::VERTICAL);
}

void ReportWindow::fill(Fl_Browser* browser,
                        const std::vector<Order>& orders)
{
    browser->clear();
    for (const auto& o : orders) {
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

        browser->add(row.str().c_str());
    }
}

void ReportWindow::loadAll() {
    try {
        auto all       = db_.listByStatus("");
        auto issued    = db_.listByStatus("issued");
        auto cancelled = db_.listByStatus("cancelled");

        fill(list_accepted_,  all);
        fill(list_issued_,    issued);
        fill(list_cancelled_, cancelled);

        std::ostringstream s;
        s << "Принято: " << all.size()
          << "   •   Выдано: " << issued.size()
          << "   •   Отменено: " << cancelled.size();
        summary_->copy_label(s.str().c_str());
    } catch (const std::exception& e) {
        summary_->copy_label((std::string("Ошибка: ") + e.what()).c_str());
    }
}