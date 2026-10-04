--
Сборка:
mkdir build
cd build
cmake ..
cmake --build . --config Release
--
Запуск (Windows):
cd build\Releases
$env:PICKUP_DB = "host=localhost dbname=pickup user=pickup_user password=pickup_pass"
.\pickup_point.exe
--
Если вы сидите на Windows для сборки придётся помучаться с настройками и возможно даже изменить исходный CMake файл как сделал я.