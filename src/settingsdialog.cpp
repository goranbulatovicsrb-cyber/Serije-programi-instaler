#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent, const QString &currentKey)
    : QDialog(parent)
{
    setWindowTitle("Podešavanja — IMDB API Ključ");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumWidth(500);
    setupUi(currentKey);
}

void SettingsDialog::setupUi(const QString &currentKey) {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    // Header
    auto *header = new QWidget;
    header->setObjectName("dialogHeader");
    auto *hlay = new QHBoxLayout(header);
    auto *title = new QLabel("⚙  PODEŠAVANJA");
    title->setStyleSheet("color:#fff; font-size:14px; font-weight:bold; letter-spacing:2px;");
    hlay->addWidget(title);
    mainLayout->addWidget(header);

    auto *content = new QWidget;
    content->setStyleSheet("background:#12121f; padding:20px;");
    auto *cLayout = new QVBoxLayout(content);
    cLayout->setSpacing(14);

    // OMDb info
    auto *infoBox = new QWidget;
    infoBox->setStyleSheet("background:#1a1a2e; border:1px solid #2a2a45; border-radius:8px; padding:14px;");
    auto *infoLayout = new QVBoxLayout(infoBox);
    auto *infoTitle = new QLabel("📡  OMDb API — IMDB Ocjene");
    infoTitle->setStyleSheet("color:#6c63ff; font-size:13px; font-weight:bold;");
    auto *infoText = new QLabel(
        "Da bi aplikacija automatski uvozila IMDB ocjene, žanrove, opise i ostale\n"
        "informacije o serijama, potreban je besplatan OMDb API ključ.\n\n"
        "Besplatan nalog daje 1000 zahtjeva dnevno — dovoljno za cijelu kolekciju."
    );
    infoText->setStyleSheet("color:#9999cc; font-size:12px;");
    infoText->setWordWrap(true);

    auto *linkBtn = new QPushButton("🌐  Registruj se besplatno na omdbapi.com");
    linkBtn->setObjectName("btnImdb");
    linkBtn->setMinimumHeight(32);
    connect(linkBtn, &QPushButton::clicked, [](){
        QDesktopServices::openUrl(QUrl("https://www.omdbapi.com/apikey.aspx"));
    });

    infoLayout->addWidget(infoTitle);
    infoLayout->addWidget(infoText);
    infoLayout->addWidget(linkBtn);
    cLayout->addWidget(infoBox);

    // API key input
    auto *keyLabel = new QLabel("API KLJUČ:");
    keyLabel->setObjectName("labelTitle");
    cLayout->addWidget(keyLabel);

    m_keyInput = new QLineEdit(currentKey);
    m_keyInput->setPlaceholderText("Unesite vaš OMDb API ključ ovdje...");
    m_keyInput->setMinimumHeight(38);
    m_keyInput->setEchoMode(QLineEdit::Password);
    cLayout->addWidget(m_keyInput);

    auto *showHide = new QCheckBox("  Prikaži API ključ");
    showHide->setStyleSheet("color:#9999cc; font-size:12px;");
    connect(showHide, &QCheckBox::toggled, [this](bool show){
        m_keyInput->setEchoMode(show ? QLineEdit::Normal : QLineEdit::Password);
    });
    cLayout->addWidget(showHide);

    if (!currentKey.isEmpty()) {
        auto *statusLbl = new QLabel("✅  API ključ je već postavljen");
        statusLbl->setStyleSheet("color:#2ed573; font-size:12px;");
        cLayout->addWidget(statusLbl);
    }

    mainLayout->addWidget(content, 1);

    // Buttons
    auto *btnWidget = new QWidget;
    btnWidget->setStyleSheet("background:#0d0d1a; border-top:1px solid #2a2a45; padding:10px 16px;");
    auto *btnLayout = new QHBoxLayout(btnWidget);
    auto *cancelBtn = new QPushButton("Otkaži");
    cancelBtn->setMinimumHeight(36);
    auto *saveBtn = new QPushButton("💾  Sačuvaj");
    saveBtn->setObjectName("btnPrimary");
    saveBtn->setMinimumHeight(36);
    saveBtn->setMinimumWidth(120);
    btnLayout->addStretch();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    mainLayout->addWidget(btnWidget);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, this, &QDialog::accept);
}

QString SettingsDialog::apiKey() const { return m_keyInput->text().trimmed(); }

QString SettingsDialog::loadApiKey() {
    QSettings s("SeriesIndexer", "SeriesIndexer");
    return s.value("omdb_api_key", "").toString();
}

void SettingsDialog::saveApiKey(const QString &key) {
    QSettings s("SeriesIndexer", "SeriesIndexer");
    s.setValue("omdb_api_key", key);
}
