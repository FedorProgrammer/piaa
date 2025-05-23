# Лабораторная работа 1

**Вариант:** 2и  
**Тема:** Итеративный бэктрекинг. Исследование времени выполнения от размера квадрата.

## Структура лабораторной

* `benchmark` - директория, содержащая файл для подсчета времени работы алгоритма.
* `include` - директория, содержащая заголовочные файлы.
* `src` - директория, содержащая файлы с исходным кодом.
* `visual-output` - директория, содержащая визуальную часть (т.е. замощенные квадраты).

Для визуализации использовались библиотеки matplotplusplus (https://github.com/alandefreitas/matplotplusplus) и ffmpeg (https://ffmpeg.org/documentation.html)

## Команды Makefile

* `make` или `make all` - сборка проекта.
* `make run` - запуск основной программы.
* `make run_benchmark` - запуск тестов на время (и их запись в файл benchmark.json). 
* `make run_visualize` - визуализация результатов из файла benchmark.json.
* `make run_steps` - визуализация поэтапной работы алгоритма.
* `make clean` - удаление всех объектных файлов.
* `make clean_visuals` - удаление выходных визуализаций.
* `make deep_clean` - полная очистка всех сгенерированных файлов.