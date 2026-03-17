#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QTextEdit>
#include "database.h"

class SeriesDialog : public QDialog {
    Q_OBJECT
public:
    explicit SeriesDialog(QWidget *parent = nullptr, const SeriesData &data = SeriesData{});
    SeriesData getData() const;

private slots:
    void onAccept();

private:
    void setupUi(const SeriesData &data);
    QWidget* createFormRow(const QString &label, QWidget *widget);

    QLineEdit  *m_name;
    QLineEdit  *m_size;
    QLineEdit  *m_year;
    QLineEdit  *m_hardDisk;
    QComboBox  *m_category;
    QCheckBox  *m_watched;
    QLabel     *m_imdbRatingLabel;
    QLabel     *m_genreLabel;
    QLabel     *m_directorLabel;
    QTextEdit  *m_plotLabel;
    SeriesData  m_original;
    bool        m_editing;
};
