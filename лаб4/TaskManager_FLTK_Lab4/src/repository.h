#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>

template <class T>
class Repository
{
private:
    std::vector<T> items;

public:
    void Add(T item)
    {
        items.push_back(item);
    }

    T Find(int id) {
        return items.at(id);
    }

    void Remove(int index)
    {
        if (index >= 0 && index < static_cast<int>(items.size()))
        {
            items.erase(items.begin() + index);
        }
    }

    void Update(int index, T item)
    {
        if (index >= 0 && index < static_cast<int>(items.size()))
        {
            items[index] = item;
        }
    }

    int Size()
    {
        return static_cast<int>(items.size());
    }

    std::vector<T> GetAll() const
    {
        return items;
    }

    void Clear()
    {
        items.clear();
    }
};

#endif