#include <gtest/gtest.h>
#include "Playlist.h"


//конструктор
//ошибка
TEST(PlaylistTests, Constructor_Expected_Exception_OnZeroCapacity) {
    EXPECT_THROW(Playlist p(0), PlaylistException);
}
//ошибка
TEST(PlaylistConstructor, ErrorCapacityThrows) {
    EXPECT_THROW(Playlist p(-5), PlaylistException);
}
//успешное создание
TEST(PlaylistConstructor, PositiveCapacityCreatesEmpty) {
    Playlist p(3);
    EXPECT_EQ(p.Capacity(), 3);
    EXPECT_EQ(p.Count(), 0);
}

//индексатор
//i > Count-1 - ошибка
TEST(PlaylistTests, Indexer_Expected_Exception_OnOutOfRange) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    EXPECT_THROW(p[5], PlaylistException);
}
//i < 0 - ошибка
TEST(PlaylistIndexer, OutOfRangeLowThrows) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    EXPECT_THROW(p[-1], PlaylistException);
}
//корректный индекс - вернет трек
TEST(PlaylistIndexer, ValidIndexReturnsTrack) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    EXPECT_EQ(p[0].title, "Song A");
    EXPECT_EQ(p[0].artist, "Artist A");
    EXPECT_EQ(p[0].durationSec, 200);
}
//прямая перезапись (не const)
TEST(PlaylistIndexer, WriteViaNonConstIndexer) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    p[0] = { "Song B", "Artist B", 100 };
    EXPECT_EQ(p[0].title, "Song B");
    EXPECT_EQ(p[0].artist, "Artist B");
    EXPECT_EQ(p[0].durationSec, 100);
}
//const версия - обращ к несущ индексу(т к пустой)
TEST(PlaylistIndexer, ConstIndexerOutOfRangeThrows) {
    const Playlist p(2);
    EXPECT_THROW(p[0], PlaylistException);
}

//add
//дубликат - ошибка
TEST(PlaylistTests, Add_Expected_Exception_OnDuplicate) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    EXPECT_THROW(p.Add({ "Song A", "Artist A", 250 }), PlaylistException);
}
//переполнение - ошибка
TEST(PlaylistTests, Add_Expected_Exception_OnOverflow) {
    Playlist p(1);
    p.Add({ "Song A", "Artist A", 200 });
    EXPECT_THROW(p.Add({ "Song B", "Artist B", 180 }), PlaylistException);
}
//другой артист - добавлено
TEST(PlaylistAdd, Add_OtherArtist_NotDuplicate) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    EXPECT_NO_THROW(p.Add({ "Song A", "Artist B", 200 }));
    EXPECT_EQ(p.Count(), 2);
}
//успешное добавление
TEST(PlaylistAdd, Add_Successful) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    EXPECT_EQ(p.Count(), 1);
    p.Add({ "Song B", "Artist B", 100 });
    EXPECT_EQ(p.Count(), 2);
}

//оператор ==
//разное count - false
TEST(PlaylistEquality, CountReturnsFalse) {
    Playlist a(3), b(3);
    a.Add({ "Song A", "Artist A", 200 });
    EXPECT_FALSE(a == b);
}
//одинаковое сount, разные треки - false.
TEST(PlaylistEquality, TracksReturnsFalse) {
    Playlist a(2), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    b.Add({ "Song B", "Artist B", 200 });
    EXPECT_FALSE(a == b);
}
//одинаковые листы
TEST(PlaylistTests, Equal_SamePlaylists_ReturnsTrue) {
    Playlist a(2);
    a.Add({ "Song A", "Artist A", 200 });
    Playlist b(2);
    b.Add({ "Song A", "Artist A", 200 });
    EXPECT_TRUE(a == b);
}

//totalduration
//успешная сумма длительностей
TEST(PlaylistTests, TotalDuration_ReturnsSumOfTracks) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    p.Add({ "Song B", "Artist B", 100 });
    EXPECT_EQ(p.TotalDuration(), 300);
}
//пустой лист - 0
TEST(PlaylistTotalDuration, EmptyPlaylistReturnsZero) {
    Playlist p(3);
    EXPECT_EQ(p.TotalDuration(), 0);
}

//tostring
//пустой лист - пустая строка
TEST(PlaylistToString, EmptyPlaylistReturnsEmptyString) {
    Playlist p(2);
    EXPECT_EQ(p.ToString(), "");
}
//граничный
TEST(PlaylistToString, LimitingCase) {
    Playlist p(1);
    p.Add({ "Song", "Artist", 61 });
    EXPECT_EQ(p.ToString(), "Artist - Song (1:01)\n");
}
//корректный вывод строк
TEST(PlaylistToString, FormatsTracksCorrectly) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    p.Add({ "Song B", "Artist B", 65 });
    std::string expected =
        "Artist A - Song A (3:20)\n"
        "Artist B - Song B (1:05)\n";
    EXPECT_EQ(p.ToString(), expected);
}

//копирование
//копия объекта
TEST(PlaylistCopy, AssignmentDeepCopy) {
    Playlist a(2), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    b = a;
    EXPECT_EQ(a, b);
}
// Тесты для Merge, RemoveTracksOf и FindByArtist — самостоятельно

//merge
//больше максимума - ошибка
TEST(PlaylistMerge, InsufficientCapacityThrows) {
    Playlist a(2), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    a.Add({ "Song B", "Artist B", 100 });
    b.Add({ "Song C", "Artist C", 150 });
    EXPECT_THROW(a.Merge(b), PlaylistException);
}
//успешное без дубликатов
TEST(PlaylistMerge, SuccessfulMerge) {
    Playlist a(3), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    b.Add({ "Song B", "Artist B", 100 });
    b.Add({ "Song C", "Artist C", 150 });
    a.Merge(b);
    EXPECT_EQ(a.Count(), 3);
    EXPECT_EQ(a[1].title, "Song B");
    EXPECT_EQ(a[2].title, "Song C");
}
//дубликат - все, кроме него
TEST(PlaylistMerge, DuplicatesSkipped) {
    Playlist a(3), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    b.Add({ "Song A", "Artist A", 300 });
    b.Add({ "Song B", "Artist B", 100 });
    a.Merge(b);
    EXPECT_EQ(a.Count(), 2);
    EXPECT_EQ(a[1].title, "Song B");
}
//слияение с пустым
TEST(PlaylistMerge, MergeEmpty) {
    Playlist a(2), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    a.Merge(b);
    EXPECT_EQ(a.Count(), 1);
}

//remocetracksof
//не найдено треков для удаления
TEST(PlaylistRemove, NoTracks) {
    Playlist a(2), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    b.Add({ "Song B", "Artist B", 100 });
    EXPECT_THROW(a.RemoveTracksOf(b), PlaylistException);
}
//удаление повтора
TEST(PlaylistRemove, SuccesfulRemoval) {
    Playlist a(4), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    a.Add({ "Song B", "Artist B", 100 });
    a.Add({ "Song C", "Artist C", 150 });
    b.Add({ "Song B", "Artist B", 100 });
    a.RemoveTracksOf(b);
    EXPECT_EQ(a.Count(), 2);
    EXPECT_EQ(a[0].title, "Song A");
    EXPECT_EQ(a[1].title, "Song C");
}
//удаление всех
TEST(PlaylistRemove, RemoveAllTracks) {
    Playlist a(2), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    a.Add({ "Song B", "Artist B", 100 });
    b.Add({ "Song A", "Artist A", 200 });
    b.Add({ "Song B", "Artist B", 100 });
    a.RemoveTracksOf(b);
    EXPECT_EQ(a.Count(), 0);
}
//пустой лист b
TEST(PlaylistRemove, RemoveEmpty) {
    Playlist a(2), b(2);
    a.Add({ "Song A", "Artist A", 200 });
    EXPECT_THROW(a.RemoveTracksOf(b), PlaylistException);
}

//findbyartist
//не найден трек исполнителя
TEST(PlaylistFind, NotFound) {
    Playlist p(2);
    p.Add({ "Song A", "Artist A", 200 });
    EXPECT_THROW(p.FindByArtist("Unknown"), PlaylistException);
}
//пустой лист
TEST(PlaylistFind, FindInEmptyPlaylistThrows) {
    Playlist p(2);
    EXPECT_THROW(p.FindByArtist("Artist"), PlaylistException);
}
//успех - вернуло первый найденный
TEST(PlaylistFind, ReturnsFirstMatchingIndex) {
    Playlist p(3);
    p.Add({ "Song A", "Artist X", 200 });
    p.Add({ "Song B", "Artist Y", 100 });
    p.Add({ "Song C", "Artist X", 150 });
    EXPECT_EQ(p.FindByArtist("Artist X"), 0);
    EXPECT_EQ(p.FindByArtist("Artist Y"), 1);
}
