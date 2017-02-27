# Тестовые задачи для itsk

    tasks/ - теоретические задачи
    well_regression/ - регрессия дебита скважин на С++
      data/ - входные и выходные данные программы
      doc/ - описание алгоритма, результатов
      
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

    results_o1.txt
    results_o2.txt
    results_w1.txt
    results_w2.txt

где /[ow/]/[12/] - тип продукта (o - oil, w - water), 1,2 - тип ф-ии спада.


