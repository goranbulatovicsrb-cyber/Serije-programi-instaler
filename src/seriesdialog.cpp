#include "seriesdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFont>

SeriesDialog::SeriesDialog(QWidget *parent, const SeriesData &data)
    : QDialog(parent), m_original(data), m_editing(data.id > 0)
{
    setMinimumWidth(520);
    setModal(true);
    setupUi(data);
}

QWidget* SeriesDialog::createFormRow(const QString &label, QWidget *widget) {
    QWidget *row = new QWidget;
    QHBoxLayout *l = new QHBoxLayout(row);
    l->setContentsMargins(0, 0, 0, 0);
    QLabel *lbl = new QLabel(label);
    lbl->setObjectName("labelTitle");
    lbl->setFixedWidth(110);
    l->addWidget(lbl);
    l->addWidget(widget, 1);
    return row;
}

void SeriesDialog::setupUi(const SeriesData &data) {
    setWindowTitle(m_editing ? "Uredi Seriju" : "Dodaj Novu Seriju");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Header
    auto *header = new QWidget;
    header->setObjectName("dialogHeader");
    auto *hlay = new QHBoxLayout(header);
    auto *titleLbl = new QLabel(m_editing ? "✏  UREDI SERIJU" : "＋  DODAJ NOVU SERIJU");
    QFont tf; tf.setPointSize(14); tf.setBold(true);
    titleLbl->setFont(tf);
    titleLbl->setStyleSheet("color: #ffffff; letter-spacing: 2px;");
    hlay->addWidget(titleLbl);
    mainLayout->addWidget(header);

    // Form area
    auto *formWidget = new QWidget;
    formWidget->setStyleSheet("background-color: #12121f; padding: 8px;");
    auto *formLayout = new QVBoxLayout(formWidget);
    formLayout->setSpacing(10);
    formLayout->setContentsMargins(20, 16, 20, 16);

    m_name = new QLineEdit(data.name);
    m_name->setPlaceholderText("Unesite naziv serije...");
    formLayout->addWidget(createFormRow("NAZIV:", m_name));

    m_category = new QComboBox;
    m_category->addItem("🌍  Strana serija", "STRANE");
    m_category->addItem("🇷🇸  Domaća serija", "DOMACE");
    int idx = (data.category == "DOMACE") ? 1 : 0;
    m_category->setCurrentIndex(idx);
    formLayout->addWidget(createFormRow("KATEGORIJA:", m_category));

    m_year = new QLineEdit(data.year);
    m_year->setPlaceholderText("npr. (2020-2023)");
    formLayout->addWidget(createFormRow("GODINA:", m_year));

    m_size = new QLineEdit(data.size);
    m_size->setPlaceholderText("npr. 14.4 GB");
    formLayout->addWidget(createFormRow("VELIČINA:", m_size));

    m_hardDisk = new QLineEdit(data.hardDisk);
    m_hardDisk->setPlaceholderText("npr. WD 5 TB");
    formLayout->addWidget(createFormRow("HARD DISK:", m_hardDisk));

    m_watched = new QCheckBox("  Označeno kao gledano");
    m_watched->setChecked(data.watched);
    m_watched->setStyleSheet("color: #e0e0f0; font-size: 13px; padding: 4px;");
    formLayout->addWidget(m_watched);

    // IMDB info (read-only, only shown when editing)
    if (m_editing && data.imdbRating > 0) {
        auto *sep = new QFrame;
        sep->setFrameShape(QFrame::HLine);
        sep->setStyleSheet("color: #2a2a45;");
        formLayout->addWidget(sep);

        auto *imdbLabel = new QLabel("─── IMDB INFORMACIJE ───");
        imdbLabel->setObjectName("labelTitle");
        imdbLabel->setAlignment(Qt::AlignCenter);
        formLayout->addWidget(imdbLabel);

        m_imdbRatingLabel = new QLabel(QString("⭐  %1 / 10").arg(data.imdbRating, 0, 'f', 1));
        m_imdbRatingLabel->setStyleSheet("color: #f5c518; font-size: 16px; font-weight: bold;");
        formLayout->addWidget(createFormRow("OCJENA:", m_imdbRatingLabel));

        if (!data.genre.isEmpty()) {
            m_genreLabel = new QLabel(data.genre);
            m_genreLabel->setStyleSheet("color: #9999cc;");
            m_genreLabel->setWordWrap(true);
            formLayout->addWidget(createFormRow("ŽANR:", m_genreLabel));
        }

        if (!data.director.isEmpty()) {
            m_directorLabel = new QLabel(data.director);
            m_directorLabel->setStyleSheet("color: #9999cc;");
            formLayout->addWidget(createFormRow("REŽISER:", m_directorLabel));
        }

        if (!data.plot.isEmpty()) {
            m_plotLabel = new QTextEdit(data.plot);
            m_plotLabel->setReadOnly(true);
            m_plotLabel->setMaximumHeight(80);
            m_plotLabel->setStyleSheet("color: #aaaacc; font-size: 11px;");
            formLayout->addWidget(createFormRow("OPIS:", m_plotLabel));
        }
    }

    mainLayout->addWidget(formWidget, 1);

    // Buttons
    auto *btnWidget = new QWidget;
    btnWidget->setStyleSheet("background-color: #0d0d1a; border-top: 1px solid #2a2a45; padding: 8px 16px;");
    auto *btnLayout = new QHBoxLayout(btnWidget);
    auto *cancelBtn = new QPushButton("Otkaži");
    auto *saveBtn   = new QPushButton(m_editing ? "💾  Sačuvaj" : "➕  Dodaj Seriju");
    saveBtn->setObjectName("btnPrimary");
    saveBtn->setMinimumWidth(140);
    saveBtn->setMinimumHeight(36);
    cancelBtn->setMinimumHeight(36);
    btnLayout->addStretch();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    mainLayout->addWidget(btnWidget);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn,   &QPushButton::clicked, this, &SeriesDialog::onAccept);
}

void SeriesDialog::onAccept() {
    if (m_name->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Greška", "Naziv serije je obavezno polje!");
        m_name->setFocus();
        return;
    }
    accept();
}

SeriesData SeriesDialog::getData() const {
    SeriesData s = m_original;
    s.name     = m_name->text().trimmed();
    s.size     = m_size->text().trimmed();
    s.year     = m_year->text().trimmed();
    s.hardDisk = m_hardDisk->text().trimmed();
    s.category = m_category->currentData().toString();
    s.watched  = m_watched->isChecked();
    return s;
}
