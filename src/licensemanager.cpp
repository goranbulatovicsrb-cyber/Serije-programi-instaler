#include "licensemanager.h"
#include <QDateTime>
#include <QSettings>
#include <QCryptographicHash>
#include <QDebug>

// ─── SECRET KEY ─────────────────────────────────────────────
// Change this to your own unique string before distributing!
// Keep it secret — it's embedded in the binary.
const QString LicenseManager::SECRET_KEY  = "SI2025-FilmoviSerije-XK7mP9qRR";
const QString LicenseManager::VALID_CHARS = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
//                                           ^ no I,O,1,0 — avoid confusion

// ─────────────────────────────────────────────────────────────

LicenseManager& LicenseManager::instance() {
    static LicenseManager inst;
    return inst;
}

LicenseManager::LicenseManager() {}

// ─── CORE ALGORITHM ──────────────────────────────────────────
// A serial is valid when:
//   1. Exactly 20 chars after stripping dashes
//   2. All chars are in VALID_CHARS
//   3. Group checksums match (positions 4,9,14,19 encode check values)
//   4. Global HMAC checksum matches
// ─────────────────────────────────────────────────────────────

QString LicenseManager::computeChecksum(const QString &stripped) {
    // HMAC-like: hash( stripped[0..17] + SECRET_KEY )
    // Use first 18 chars as data, last 2 chars are the checksum
    QString data = stripped.left(18) + SECRET_KEY;
    QByteArray hash = QCryptographicHash::hash(
        data.toUtf8(), QCryptographicHash::Sha256);

    // Take bytes 0 and 1, map to VALID_CHARS
    int i0 = (unsigned char)hash[0] % VALID_CHARS.length();
    int i1 = (unsigned char)hash[1] % VALID_CHARS.length();
    return QString(VALID_CHARS[i0]) + QString(VALID_CHARS[i1]);
}

bool LicenseManager::validateSerial(const QString &serial) {
    QString s = stripSerial(serial).toUpper();

    // Length check
    if (s.length() != 20) return false;

    // Valid chars check
    for (QChar c : s) {
        if (!VALID_CHARS.contains(c)) return false;
    }

    // Group structure check:
    // Each group of 5: chars 0-3 are data, char 4 is group checksum
    // Group checksum = (sum of first 4 char indices in VALID_CHARS) % VALID_CHARS.length()
    for (int g = 0; g < 4; g++) {
        int base = g * 5;
        int sum = 0;
        for (int i = 0; i < 4; i++) {
            sum += VALID_CHARS.indexOf(s[base + i]);
        }
        int expected = sum % VALID_CHARS.length();
        int actual   = VALID_CHARS.indexOf(s[base + 4]);
        if (expected != actual) return false;
    }

    // Global HMAC checksum check (last 2 chars)
    QString checksum = computeChecksum(s);
    if (s[18] != checksum[0] || s[19] != checksum[1]) return false;

    return true;
}

QString LicenseManager::formatSerial(const QString &raw) {
    QString r = raw.toUpper();
    if (r.length() != 20) return r;
    return r.left(5) + "-" + r.mid(5,5) + "-" + r.mid(10,5) + "-" + r.mid(15,5);
}

QString LicenseManager::stripSerial(const QString &serial) {
    QString s = serial;
    s.remove("-").remove(" ");
    return s.toUpper();
}

// ─── ACTIVATION ──────────────────────────────────────────────

bool LicenseManager::isActivated() const {
    QSettings s("SeriesIndexer", "License");
    QString saved = s.value("serial", "").toString();
    if (saved.isEmpty()) return false;
    return validateSerial(saved);
}

LicenseManager::Status LicenseManager::activate(const QString &serial) {
    QString stripped = stripSerial(serial);
    if (!validateSerial(stripped)) return InvalidSerial;

    QSettings s("SeriesIndexer", "License");
    s.setValue("serial", stripped);
    s.setValue("activated_at", QDateTime::currentDateTime().toString(Qt::ISODate));
    return Activated;
}

QString LicenseManager::savedSerial() const {
    QSettings s("SeriesIndexer", "License");
    QString raw = s.value("serial", "").toString();
    if (raw.length() != 20) return "";
    // Mask last two groups: XXXXX-XXXXX-*****-*****
    return raw.left(5) + "-" + raw.mid(5,5) + "-*****-*****";
}

void LicenseManager::deactivate() {
    QSettings s("SeriesIndexer", "License");
    s.remove("serial");
    s.remove("activated_at");
}
