#include "repository.h"

template <class T>
void Repository<T>::Add(T item) {
    items.push_back(item);
}

template <class T>
void Repository<T>::Remove(int index) {
    items.erase(index);
}

template <class T>
void Repository<T>::Update(int index, T item) {
    items[index] = item;
}

template <class T>
int Repository<T>::Size() const {
    return items.size();
}

template <class T>
std::vector<T> Repository<T>::GetAll() const {
    return items;
}

template <class T>
void Repository<T>::Clear() {
    items.clear();
}