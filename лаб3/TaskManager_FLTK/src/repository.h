#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>

template <class T>
class Repository
{
private:
    std::vector<T> items;

public:
    void Add(T item);
    void Remove(int index);
    void Update(int index, T item);
    int Size() const;
    std::vector<T> GetAll() const;
    void Clear();
};

#endif
