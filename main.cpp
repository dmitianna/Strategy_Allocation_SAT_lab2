#include <iostream>
#include <chrono>
#include "Allocator.h"

class MyClass
{
private:
    int id;
    float value;

public:
    MyClass(int _id, float _value) : id(_id), value(_value) {}
    ~MyClass() {}
};

using Clock = std::chrono::high_resolution_clock;

//new/delete
void testDefaultNew(size_t N)
{
    auto start = Clock::now();
    for (size_t i = 0; i < N; ++i)
    {
        MyClass* obj = new MyClass(i, i * 1.0f);
        delete obj;
    }
    auto end = Clock::now();

    std::cout << "new/delete: "<< std::chrono::duration<double, std::milli>(end - start).count()<< " ms\n";
}

// allocator (heap pool)
// Аллокатор выделяет новые блоки памяти
void testAllocator(size_t N)
{
    Allocator alloc(sizeof(MyClass), N);
    MyClass** arr = new MyClass*[N];

    // первый проход
    auto start = Clock::now();

    for (size_t i = 0; i < N; ++i) {
        void* mem = alloc.Allocate(sizeof(MyClass));
        arr[i] = new (mem) MyClass(i, i * 1.0f);
    }

    auto end = Clock::now();

    std::cout << "allocator first try: "<< std::chrono::duration<double, std::milli>(end - start).count()<< " ms\n";

    // освобождение
    // объекты уничтожаются, память возвращается в free-list
    for (size_t i = 0; i < N; ++i) {
        arr[i]->~MyClass();
        alloc.Deallocate(arr[i]);
    }

    // reuse
    // Память берётся из free-list
    start = Clock::now();

    for (size_t i = 0; i < N; ++i) {
        void* mem = alloc.Allocate(sizeof(MyClass));
        arr[i] = new (mem) MyClass(i, i * 2.0f);
    }

    end = Clock::now();

    std::cout << "allocator reuse from free-list: "<< std::chrono::duration<double, std::milli>(end - start).count()<< " ms\n";
    for (size_t i = 0; i < N; ++i){
        arr[i]->~MyClass();
        alloc.Deallocate(arr[i]);
    }

    delete[] arr;
}
int main()
{
    const size_t N = 500000;
    testDefaultNew(N);
    testAllocator(N);
    return 0;
}
