# Тестовые задачи для itsk

    tasks/ - теоретические задачи
    well_regression/ - регрессия дебита скважин на С++
      data/ - входные и выходные данные программы
      doc/ - описание алгоритма, результаты
      
# Программа well_regression

Сборка утилитой CMake:

    cd well_regression
    cmake . ; make

Запуск (при запуске без параметров выводит справку):

    ./well_regression IFILE product_type func_type OFILE

Для запуска всех имеющихся тестов:

    ./test.sh

Исходные данные по скважинам берутся из: 

    data/well_data.txt

Результаты сохраняются в файлы:

    data/results_o1.txt
    data/results_o2.txt
    data/results_w1.txt
    data/results_w2.txt

где [ow][12] - тип продукта (o - oil, w - water), 1,2 - тип ф-ии спада.

Тестировалось на CentOS 7.3.1611 64-bit со стандартными для этого дистрибутива версиями пакетов CMake, make, gcc.

