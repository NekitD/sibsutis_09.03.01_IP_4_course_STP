#include "db.h"
#include "main_window.h"
#include "theme.h"
#include <FL/Fl.H>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
    Theme::apply();

    // Строка подключения задаётся ТОЛЬКО через переменную окружения.
    // В коде дефолтных значений нет — так студент не запустит
    // приложение со случайной базой.
    const char* env = std::getenv("PICKUP_DB");
    if (!env || !*env) {
        std::cerr
            << "Ошибка: переменная окружения PICKUP_DB не задана.\n\n"
            << "Запустите приложение так:\n\n"
            << "  PICKUP_DB=\"host=localhost dbname=pickup "
               "user=pickup_user password=pickup_pass\" ./pickup_point\n\n";
        return 1;
    }

    try {
        Database db(env);
        MainWindow win(db);
        win.show(argc, argv);
        return Fl::run();
    } catch (const std::exception& e) {
        std::cerr << "DB error: " << e.what() << "\n";
        return 1;
    }
}