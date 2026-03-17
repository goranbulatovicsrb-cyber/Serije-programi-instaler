#pragma once
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QString>

// Column indices in the main table model
enum TableCol {
    COL_ID = 0,
    COL_NAME,
    COL_CATEGORY,
    COL_YEAR,
    COL_SIZE,
    COL_HDD,
    COL_RATING,
    COL_GENRE,
    COL_WATCHED,
    COL_COUNT
};

class SeriesFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit SeriesFilterModel(QObject *parent = nullptr);

    void setSearch(const QString &text);
    void setCategoryFilter(const QString &cat);  // "" = all
    void setHddFilter(const QString &hdd);       // "" = all
    void setMinRating(double min);
    void setMaxRating(double max);
    void setWatchedFilter(int state); // -1=all, 0=unwatched, 1=watched

protected:
    bool filterAcceptsRow(int srcRow, const QModelIndex &srcParent) const override;

private:
    QString m_search;
    QString m_category;
    QString m_hdd;
    double  m_minRating = 0.0;
    double  m_maxRating = 10.0;
    int     m_watchedState = -1;
};
