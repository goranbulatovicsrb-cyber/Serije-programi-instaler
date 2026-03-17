#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QList>
#include <QVariantMap>

struct SeriesData {
    int id = 0;
    QString name;
    QString size;
    QString year;
    QString hardDisk;
    QString category;     // "DOMACE" or "STRANE"
    double imdbRating = 0.0;
    QString imdbId;
    QString genre;
    QString director;
    QString actors;
    QString plot;
    QString country;
    QString language;
    QString awards;
    QString posterUrl;
    bool watched = false;
    QString createdAt;
    QString updatedAt;
};

class Database : public QObject {
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool open(const QString &path);
    void close();
    bool isOpen() const;

    // CRUD
    int  addSeries(const SeriesData &s);
    bool updateSeries(const SeriesData &s);
    bool deleteSeries(int id);
    bool deleteAllSeries();

    // Read
    QList<SeriesData> getAllSeries() const;
    QList<SeriesData> searchSeries(const QString &term,
                                    const QString &category = QString(),
                                    const QString &hardDisk = QString(),
                                    double minRating = 0.0,
                                    double maxRating = 10.0) const;
    SeriesData getSeriesById(int id) const;
    SeriesData getSeriesByName(const QString &name) const;

    // IMDB update
    bool updateImdb(int id, double rating, const QString &imdbId,
                    const QString &genre, const QString &director,
                    const QString &actors, const QString &plot,
                    const QString &country, const QString &language,
                    const QString &awards, const QString &posterUrl);

    // Smart import: add new, update existing if changed
    int importSeries(const QList<SeriesData> &list, bool updateExisting = false);

    // Stats
    int countAll() const;
    int countByCategory(const QString &cat) const;
    double totalSizeGB() const;
    QStringList allHardDisks() const;

    QString lastError() const { return m_lastError; }

private:
    bool createTables();
    SeriesData rowToSeries(QSqlQuery &q) const;
    mutable QString m_lastError;
    QSqlDatabase m_db;
};
