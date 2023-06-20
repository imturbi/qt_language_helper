#ifndef KBMLANG_H
#define KBMLANG_H

#include <QObject>

class KbmLang : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int hint MEMBER m_hint NOTIFY hintChanged)
    Q_PROPERTY(QString message MEMBER m_message NOTIFY messageChanged)
    Q_PROPERTY(QString fileWord MEMBER m_fileWord NOTIFY fileWordChanged)
    Q_PROPERTY(QStringList languageList MEMBER m_languageList NOTIFY languageListChanged)

public:
    enum KbmHintLang {
        KBM_HINT_LANG_NONE,
        KBM_HINT_LANG_OPEN_LANG_FAILED,
        KBM_HINT_LANG_OPEN_TRANS_FAILED,
        KBM_HINT_LANG_INVALID_LANG_FILE,
        KBM_HINT_LANG_INVALID_TRANS_FILE,
        KBM_HINT_LANG_OK,
    };
    Q_ENUM(KbmHintLang)

private:
    int m_hint;
    QString m_message;
    QString m_fileWord;
    QStringList m_languageList;

    void setHint(int hint);
    void setMessage(QString msg);
    void setFileWord(QString name);


public:
    explicit KbmLang(QObject *parent = nullptr);

    Q_INVOKABLE bool parse(QString lang);
    Q_INVOKABLE bool replace(QString src, QString lang, QString olang, QString tlang);
    Q_INVOKABLE bool getLanguageList(QString lang);

signals:
    void hintChanged();
    void messageChanged();
    void fileWordChanged();
    void languageListChanged();

public slots:
};

#endif // KBMLANG_H
