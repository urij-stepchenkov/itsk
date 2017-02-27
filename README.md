h1 Тестовые задачи для itsk

    tasks/ - теоретические задачи
    well_regression/ - регрессия дебита скважин на С++
      data/ - входные и выходные данные программы
      doc/ - описание алгоритма, резулбтатов
      
h2 Программа well_regression

Сборка утилитой CMake:
    cd well_regression
    cmake . ; make

Запуск:
    ./well_regression IFILE product_type func_type OFILE
При запуске без параметров выводит справку.

