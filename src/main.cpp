#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include <QDebug>
#include <QTimer>

#include "mainwindow.h"
#include "licensemanager.h"
#include "licensedialog.h"

int main(int argc, char *argv[]) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app(argc, argv);
    app.setApplicationName("SeriesIndexer");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("SeriesIndexer");
    app.setStyle(QStyleFactory::create("Fusion"));

    // Load QSS stylesheet
    QFile styleFile(":/resources/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
        styleFile.close();
    }

    // ── License check ──────────────────────────────────────
    if (!LicenseManager::instance().isActivated()) {
        LicenseDialog licDlg(nullptr, true);
        // If user closes dialog without activating → quit
        QObject::connect(&licDlg, &QDialog::rejected, &app, &QApplication::quit);
        licDlg.show();

        // Run event loop just for license dialog
        // When accepted → continue; when rejected → quit called above
        if (licDlg.exec() != QDialog::Accepted) {
            return 0;
        }

        // Double-check activation was saved
        if (!LicenseManager::instance().isActivated()) {
            return 0;
        }
    }
    // ───────────────────────────────────────────────────────

    MainWindow w;
    w.show();
    return app.exec();
}
