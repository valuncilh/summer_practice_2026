# summer_practice_2026

# Решение задач
## Структура
Решения расположены в директориях `taskN`.

## task1
Параллельная сортировка слиянием на C (pthread).

### Сборка и запуск
В директории `task1`:
- `make` — сборка исполняемого файла `task1`. Поддерживает переменные `CFLAGS` и `LDFLAGS`.
- `make clean` — удаление артефактов сборки.
- `make test` — быстрый тест работы программы.

Пример запуска:
```bash
echo "3 2 1" | ./task1 2
> 1 2 3
```
## task2
Дуплексный эхо чат меджу процессами через именованный канал (FIFO)
- Сборка: `cd task2 && make`
- Запуск (в двух терминалах):
  ```bash
  ./task2 chat.fifo
  ```
## task3
Сортировка строк текстового файла различными способами с фильтрацией пустых строк.

### Сборка и запуск
В директории `task3`:
*   `make` — сборка исполняемого файла `task3`.

### Параметры запуска:
`./task3 <input_file> <output_file> <sort_type>`

### Типы сортировки:
*   `plain` — побайтовая сортировка (чувствительна к регистру).
*   `lex` — лексикографическая (без учета регистра).
*   `rplain` — обратная побайтовая.
*   `rlex` — обратная лексикографическая.

Пример запуска:
```bash
echo -e "3\n\n2\na" > in.txt
./task3 in.txt out.txt plain
cat out.txt
# Output:
# 2
# 3
# a
```
## task4
Парсер аргументов командной строки с поддержкой коротких опций -m, -c, -s, -t и длинной --elbrus=значение (разрешённые значения: 1c+, 2c+, 2c3, 4c, 8c, 16c), а также произвольных не-опций.

### Сборка и запуск
В директории task4:
- `make` — сборка task4
- `make clean` — очистка

Примеры запуска:

```bash
./task4 -tm -s --elbrus=16c file0 --elbrus=2c3 file1
# Short options: 't' 'm' 's'
# Long options: 'elbrus=16c' 'elbrus=2c3'
# Non options: 'file0' 'file1'

./task4 -tm -x --elbrus=8c
# Incorrect option: 'x'
```

## task5
программа на языке С, которая выполняет open, выводит возвращаемую ошибку и завершается с
соответствующим кодом.

### Сборка и запуск
В директории task5:
- `make` — сборка task5
- `make clean` — очистка

Пример запуска:
```bash
>Can’t open file: No such file or directory.
>Errno = ENOENT
```

## task6
программa на С для получения справочной информации о файлах с
помощью системного вызова stat.

### Сборка и запуск
В директории task6:
- `make` — сборка task6
- `make clean` — очистка

Пример запуска:
```bash
./task6 Makefile
  File: Makefile
  Size: 170   Blocks: 8    IO Block: 4096    regular file
Device: 2050    Inode: 811433   Links: 1
Access: (0644/-rw-r--r--) Uid: ( 1000/   dev_v)   Gid: ( 1000/   dev_v)
  Access: 2026-04-22 22:35:49.461300757 +0300
  Modify: 2026-04-22 22:35:49.449300795 +0300
  Change: 2026-04-22 22:35:49.449300795 +0300
```
## task7
Программа на С выполняет открытие общего файла на чтение. После открытия
файла создать параллельный процесс с помощью fork. В обоих процессах свои
файлы для записи, чтение информации из общего файла и копирование ее в собственные
выходные файлы. По окончании копирования, вывод на экран содержимое полученных
файлов.

### Сборка и запуск
В директории task7:
- `make` — сборка task7
- `make clean` — очистка

Пример запуска:
```bash
./task7 test_file
>Child:
Tis time, my friend, 'tis time! The heart to peace aspires:
Day follows day; the rolling stream of hours
Crumbles the banks of being, and you and I
Had thought to live, and yet, behold, we die.

Though joy for ever flees, peace stays and concentration.
For long now has it been my consolation,
Hard-driven slave, to plan rebellious flight
To some far sanctuary of work and chaste delight.

>Parent:
Tis time, my friend, 'tis time! The heart to peace aspires:
Day follows day; the rolling stream of hours
Crumbles the banks of being, and you and I
Had thought to live, and yet, behold, we die.

Though joy for ever flees, peace stays and concentration.
For long now has it been my consolation,
Hard-driven slave, to plan rebellious flight
To some far sanctuary of work and chaste delight.
```

## task8
Обработка сигналов SIGINT при умножении матриц на C.
Программа перемножает матрицы 100x100 с задержкой. Демонстрирует два способа перехвата сигнала: через signal() и sigaction(). При первом Ctrl+C выводит текущие индексы циклов, при втором — завершает работу.

### Сборка и запуск
В директории task8:

- `make` — сборка исполняемого файла task8.
- `make clean` — удаление артефактов сборки.

Пример запуска (вариант с --signal):
```bash
./task8 --signal 
# Нажмите Ctrl-C во время выполнения
> 12 45
> 45 89
> 12 89
# Нажмите Ctrl-C еще раз для выхода
```

## task9
Поиск максимальной возрастающей подпоследовательности на C++.
Находит непрерывную возрастающую последовательность чисел максимальной длины. Если таких несколько, выбирает ту, у которой больше сумма элементов.

### Сборка и запуск
В директории task9:

- `make` — сборка исполняемого файла task9.
- `make clean` — удаление артефактов сборки.

Пример запуска:
```bash 
echo "1 2 3 0 4 5 6 0" | ./task9
> 0 4 5 6
```

## task10
Парсер логических выражений и таблица истинности на C++ (MVC).
Строит таблицу истинности для выражений с переменными A-Z и операторами AND, OR, NOT. Поддерживает скобки. Реализовано с использованием паттерна MVC.

Сборка и запуск
В директории task10:

- `make` — сборка исполняемого файла task10 (требуется компилятор с поддержкой C++17).
- `make clean` — удаление артефактов сборки.

Пример запуска:
```bash 
echo "A AND B" | ./task10
> A B Result
> 0 0 0
> 0 1 0
> 1 0 0
> 1 1 1
```