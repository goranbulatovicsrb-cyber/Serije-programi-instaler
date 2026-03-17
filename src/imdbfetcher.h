#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQueue>
#include <QString>
#include <QTimer>

struct ImdbResult {
    int     seriesId = 0;
    bool    success = false;
    QString error;
    double  rating = 0.0;
    QString imdbId;
    QString genre;
    QString director;
    QString actors;
    QString plot;
    QString country;
    QString language;
    QString awards;
    QString posterUrl;
    QString title;
    QString year;
};

struct FetchRequest {
    int     seriesId;
    QString name;
    QString year;
};

class ImdbFetcher : public QObject {
    Q_OBJECT
public:
    explicit ImdbFetcher(QObject *parent = nullptr);
    ~ImdbFetcher();

    void setApiKey(const QString &key);
    QString apiKey() const { return m_apiKey; }
    bool hasApiKey() const { return !m_apiKey.isEmpty(); }

    // Fetch single series
    void fetchSingle(int seriesId, const QString &name, const QString &year = QString());

    // Fetch batch (queued)
    void fetchBatch(const QList<FetchRequest> &requests);
    void cancelBatch();

    bool isBusy() const { return m_pending > 0; }
    int  pendingCount() const { return m_pending; }

signals:
    void resultReady(ImdbResult result);
    void batchProgress(int done, int total);
    void batchComplete();
    void errorOccurred(QString msg);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void processNextQueue();

private:
    QString buildUrl(const QString &name, const QString &year) const;
    ImdbResult parseReply(int id, const QByteArray &data);

    QNetworkAccessManager *m_nam;
    QString m_apiKey;
    QQueue<FetchRequest> m_queue;
    int m_pending = 0;
    int m_batchTotal = 0;
    int m_batchDone  = 0;
    QTimer *m_rateLimitTimer;
    bool m_cancelled = false;
    static constexpr int MAX_CONCURRENT = 3;
    int m_concurrent = 0;
};
