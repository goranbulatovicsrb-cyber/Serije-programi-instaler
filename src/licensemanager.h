#pragma once
#include <QString>
#include <QObject>

// ─────────────────────────────────────────────────────────────
//  SeriesIndexer License System
//
//  Serial format:  XXXXX-XXXXX-XXXXX-XXXXX  (4 groups x 5 chars)
//  Valid chars:    A-Z, 0-9  (uppercase)
//  Algorithm:      Group checksums + global hash with secret key
// ─────────────────────────────────────────────────────────────

class LicenseManager : public QObject {
    Q_OBJECT
public:
    static LicenseManager& instance();

    enum Status {
        NotActivated,
        Activated,
        InvalidSerial,
        AlreadyActivated
    };

    // Check on startup — returns true if activated
    bool isActivated() const;

    // Validate + save serial
    Status activate(const QString &serial);

    // Returns saved serial (masked: XXXXX-XXXXX-*****-*****)
    QString savedSerial() const;

    // Clear activation (for testing)
    void deactivate();

    // Static validator — can be called from keygen too
    static bool validateSerial(const QString &serial);

    // Format raw 20 chars to XXXXX-XXXXX-XXXXX-XXXXX
    static QString formatSerial(const QString &raw);

    // Remove dashes
    static QString stripSerial(const QString &serial);

private:
    LicenseManager();
    static QString computeChecksum(const QString &stripped);
    static const QString SECRET_KEY;
    static const QString VALID_CHARS;
};
