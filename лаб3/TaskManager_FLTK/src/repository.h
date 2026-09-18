#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>

template <class T>
class Repository
{
private:
    std::vector<T> items;

public:
    void Repository::Add(T item) {
        items.push_back(item);
    }
    void Repository::Remove(int index) {
        items.erase(items.begin() + index);
    }
    void Repository::Update(int index, T item) {
        items[index] = item;
    }
    int Repository::Size() const {
        return items.size();
    }
    std::vector<T> Repository<T>::GetAll() const {
        return items;
    }
    void Repository::Clear() {
        items.clear();
    }
};

#endif
