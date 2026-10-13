# DataConverter — учебный проект для лабораторной по CI/CD

DataConverter — готовый учебный CLI-проект для конвертации табличных данных между CSV, JSON и YAML. В этой лабораторной студенты настраивают GitHub Actions


# Примечания от студентов:
Чтобы сборка работала путь к проекту не должен содержать кириллицы.

Сборка:
mkdir build
cd build
cmake ..
cmake --build .

Запуск тестов:
Из корня проекта
./build/Debug/dataconv_tests.exe