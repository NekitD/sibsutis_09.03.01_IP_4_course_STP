#pragma once
#include <string>
#include <stdexcept>

struct Track
{
    std::string title;
    std::string artist;
    int durationSec;

    bool operator==(const Track& other) const
    {
        return title == other.title && artist == other.artist;
    }
    bool operator!=(const Track& other) const
    {
        return !(*this == other);
    }
};

class PlaylistException : public std::runtime_error
{
public:
    explicit PlaylistException(const std::string& message)
        : std::runtime_error(message) {}
};

class Playlist
{
private:
    Track* tracks;
    int capacity;
    int count;

public:
    // Конструктор.
    explicit Playlist(int capacity);

    // Конструктор копирования, оператор присваивания, деструктор.
    Playlist(const Playlist& other);
    Playlist& operator=(const Playlist& other);
    ~Playlist();

    // Свойства.
    int Capacity() const { return capacity; }
    int Count() const { return count; }

    // Индексатор.
    const Track& operator[](int i) const;
    Track& operator[](int i);

    // Равенство.
    bool operator==(const Playlist& other) const;
    bool operator!=(const Playlist& other) const { return !(*this == other); }

    // Методы.
    void Add(const Track& track);
    void Merge(const Playlist& other);
    void RemoveTracksOf(const Playlist& other);
    int TotalDuration() const;
    int FindByArtist(const std::string& artist) const;
    std::string ToString() const;

private:
    bool Contains(const Track& track) const;
};
