# Лабораторная работа №2  
## Интеграция механизма аллокации памяти с фиксированными блоками на C++ в пользовательское SAT-приложение

---

# 1. Постановка задачи

В рамках лабораторной работы необходимо:

- интегрировать пользовательский аллокатор памяти в SAT-решатель;
- обеспечить выделение памяти через `Allocator`;
- реализовать возможность изменения стратегии выбора переменной ветвления;
- провести статический и динамический анализ проекта.

---

# 2. Используемые технологии

- C++17
- Qt
- Allocator  
- PVS-Studio
- Valgrind

---

# 3. Архитектура проекта

Основные компоненты проекта:

- `BBV` — битовый вектор;
- `BoolInterval` — представление булевого интервала;
- `BoolEquation` — SAT-задача и правила упрощения;
- `NodeBoolTree` — узел дерева поиска;
- `IBranchingStrategy` — интерфейс стратегии ветвления;
- `StrategyLessCommon` — реализация стратегии выбора переменной;
- `Allocator` — пользовательский аллокатор памяти.

---

# 4. Интеграция Allocator

В проект интегрирован пользовательский аллокатор памяти с фиксированными блоками.

Allocator использует механизм free-list:

- освобожденные блоки памяти не возвращаются системе;
- блоки сохраняются в списке свободной памяти;
- при следующем выделении память переиспользуется.

Это позволяет:

- уменьшить количество обращений к heap;
- снизить фрагментацию памяти;
- ускорить создание большого количества объектов одинакового размера.

---

## Поддерживаемые режимы работы

### HEAP_BLOCKS

```cpp
Allocator alloc(sizeof(T), 0);
```

- память выделяется из heap по мере необходимости;
- освобожденные блоки переиспользуются через free-list.

---

### HEAP_POOL

```cpp
Allocator alloc(sizeof(T), N);
```

- заранее создается пул памяти;
- после создания пула отсутствуют дополнительные обращения к heap.

---

### STATIC_POOL

```cpp
char buffer[sizeof(T) * N];
Allocator alloc(sizeof(T), N, buffer);
```

- используется внешний статический буфер;
- полностью отсутствует динамическое выделение памяти.

---

## Использование Allocator в проекте

Для основных объектов SAT-решателя используются отдельные аллокаторы:

```cpp
Allocator allocBoolInterval(32, 200);
Allocator allocBoolEquation(64, 100);
Allocator allocNodeBoolTree(32, 100);
Allocator allocCNF(1024, 1);
Allocator allocStrategy(8, 1);
```

Создание объектов выполняется через placement new:

```cpp
void* mem = allocBoolInterval.Allocate(sizeof(BoolInterval));
CNF[i] = new (mem) BoolInterval(strv.toUtf8().data());
```

Освобождение памяти выполняется вручную:

```cpp
CNF[i]->~BoolInterval();
allocBoolInterval.Deallocate(CNF[i]);
```

---

# 5. Реализация паттерна Strategy

Для выбора переменной ветвления реализован паттерн «Стратегия».

## Интерфейс стратегии

```cpp
class IBranchingStrategy
{
public:
    virtual ~IBranchingStrategy() = default;
    virtual int chooseColumn(BoolEquation& equation) = 0;
};
```

## Реализация стратегии

```cpp
class StrategyLessCommon : public IBranchingStrategy
{
public:
    int chooseColumn(BoolEquation& equation) override;
};
```

Стратегия выбирает столбец с минимальным количеством символов `'-'`.

Подключение стратегии к SAT-решателю:

```cpp
IBranchingStrategy* _strategy;
```

Выбор переменной выполняется через делегирование:

```cpp
return _strategy->chooseColumn(*this);
```

Это позволяет изменять алгоритм ветвления без изменения основной логики SAT-решателя.

---

# 6. Структура проекта

```text
Allocator/
    Allocator.cpp
    Allocator.h
    DataTypes.h

SAT_DPLL/
    BBV.cpp
    BBV.h
    boolinterval.cpp
    boolinterval.h
    boolequation.cpp
    boolequation.h
    strategylesscommon.cpp
    strategylesscommon.h
    IBranchingStrategy.h
    NodeBoolTree.h

main.cpp
Strategy_Allocation_SAT_lab2.pro
```

---

# 7. Анализ проекта

## PVS-Studio

В ходе статического анализа были обнаружены:

- потенциальные утечки памяти;
- предупреждения о работе с динамической памятью;
- ошибки копирования объектов;
- неинициализированные данные.

Отчет:

```text
PVSReport.json
```

---

## Valgrind

В ходе динамического анализа были обнаружены:

- mismatched new/delete;
- утечки памяти;
- ошибки освобождения памяти.

Отчет:

```text
ReportVal.txt
```

Большая часть предупреждений относится к исходной реализации SAT-проекта.

---
