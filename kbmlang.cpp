#include "kbmlang.h"

#include <QtXml>
#include <QDomDocument>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;



KbmLang::KbmLang(QObject *parent) : QObject(parent)
{
    m_hint = KBM_HINT_LANG_NONE;

}

void KbmLang::setHint(int hint)
{
    m_hint = hint;
    emit hintChanged();
}

void KbmLang::setMessage(QString msg)
{
    m_message = msg;
    emit messageChanged();
}

void KbmLang::setFileWord(QString name)
{
    m_fileWord = name;
    emit fileWordChanged();
}

bool KbmLang::parse(QString lang)
{
    QFile file(lang);
    QFileInfo fileinfo(lang);

    qDebug() << "kbm lang parse " << lang;
    if (!file.open(QFileDevice::ReadOnly)) {
        qDebug() << " failed to open " << lang;
        setMessage(QString("failed to open: %1").arg(lang));
        setHint(KBM_HINT_LANG_OPEN_LANG_FAILED);
        return false;
    }
    file.close();

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "invalid xml file " << lang;
        setMessage(QString("not XML file: %1").arg(lang));
        setHint(KBM_HINT_LANG_INVALID_LANG_FILE);
        return false;
    }

    QDomDocumentType docType = doc.doctype();
    qDebug() << "docType: " << docType.name();
    if(docType.name() != "TS") {
        setMessage(QString("docType is not TS."));
        setHint(KBM_HINT_LANG_INVALID_LANG_FILE);
        return false;
    }

    QDomElement root = doc.documentElement();

    if(root.isNull() || root.nodeName() != "TS") {
        setMessage(QString("invalid root element"));
        setHint(KBM_HINT_LANG_INVALID_LANG_FILE);
        return false;
    }

    QString xf = QString("%1/%2.xlsx").arg(fileinfo.path()).arg(fileinfo.baseName());
    if(QFile::exists(xf)) {
        for(int i = 1; i < 10000; i++) {
            xf = QString("%1/%2_%3.xlsx").arg(fileinfo.path()).arg(fileinfo.baseName()).arg(i);
            if(!QFile::exists(xf)) {
                break;
            }
        }
    }

    QXlsx::Document xlsx(xf);


    int count=1;
    QDomNode context = root.firstChild();
    while(!context.isNull()) {
        QDomNode node = context.firstChild();
        while(!node.isNull()) {
            if(node.nodeName() == "message") {
                QDomNode node1 = node.firstChild();
                while(!node1.isNull()) {
                    if(node1.nodeName() == "source") {
                        QDomNode node2 = node1.firstChild();
                        if(node2.isText()) {
                            QDomText src = node2.toText();
                            qDebug() << "src text: " << src.nodeValue();
                            xlsx.write(QString("A%1").arg(count++), src.nodeValue());
                        }
                    }

                    node1 = node1.nextSibling();
                }
            }

            node = node.nextSibling();
        }

        context = context.nextSibling();
    }

    xlsx.save();
    setFileWord(xf);
    setMessage(QString("提取翻译文字完成"));

    return true;
}

bool KbmLang::getLanguageList(QString lang)
{
    QXlsx::Document xlsx(lang);
    int i = 1;

    m_languageList.clear();

    while(1) {
        Cell *cell = xlsx.cellAt(1, i++);
        if(!cell) break;
        m_languageList.append(cell->value().toString());
    }

    emit languageListChanged();

    return true;
}

bool KbmLang::replace(QString src, QString lang, QString olang, QString tlang)
{
    int col1 = 0, col2 = 0;
    QFile file(src);
    QXlsx::Document xlsx(lang);

    qDebug() << "kbm lang merge " << lang;
    if (!file.open(QFileDevice::ReadWrite)) {
        qDebug() << " failed to open " << lang;
        setMessage(QString("failed to open: %1").arg(lang));
        setHint(KBM_HINT_LANG_OPEN_LANG_FAILED);
        return false;
    }
    file.close();

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "invalid xml file " << lang;
        setMessage(QString("not XML file: %1").arg(lang));
        setHint(KBM_HINT_LANG_INVALID_LANG_FILE);
        return false;
    }

    QDomDocumentType docType = doc.doctype();
    qDebug() << "docType: " << docType.name();
    if(docType.name() != "TS") {
        setMessage(QString("docType is not TS."));
        setHint(KBM_HINT_LANG_INVALID_LANG_FILE);
        return false;
    }

    QDomElement root = doc.documentElement();

    if(root.isNull() || root.nodeName() != "TS") {
        setMessage(QString("invalid root element"));
        setHint(KBM_HINT_LANG_INVALID_LANG_FILE);
        return false;
    }

    while(1) {
        Cell *cell = xlsx.cellAt(1, ++col1);
        if(!cell) {
            qDebug() << "no found orig lang: " << olang;
            setMessage(QString("没有源语言:%1").arg(olang));
            return false;
        }

        if(cell->value().toString() == olang) {
            break;
        }
    }

    while(1) {
        Cell *cell = xlsx.cellAt(1, ++col2);
        if(!cell) {
            qDebug() << "no found target lang: " << tlang;
            setMessage(QString("没有翻译语言:%1").arg(tlang));
            return false;
        }

        if(cell->value().toString() == tlang) {
            break;
        }
    }

    qDebug() << "col1: " << col1 << ", col2: " << col2;

    int count=1;
    QDomText stext;
    QDomText ttext;

    QDomNode context = root.firstChild();
    while(!context.isNull()) {
        QDomNode node = context.firstChild();
        while(!node.isNull()) {
            qDebug() << "node: " << node.nodeName();
            if(node.nodeName() == "message") {
                stext.clear();
                ttext.clear();
                count++;
                QString dest1 = xlsx.cellAt(count, col1)->value().toString();
                QString dest2 = xlsx.cellAt(count, col2)->value().toString();
                qDebug() << "dest1: " << dest1;
                qDebug() << "dest2: " << dest2;

                QDomNodeList list = node.childNodes();
                for(int i = 0; i < list.size(); i++) {
                    QDomNode n = list.at(i);
                    if(!n.isElement()) continue;
                    if(n.nodeName() == "source") {
                        QDomNode node1 = n.firstChild();
                        stext = node1.toText();
                    } else if(n.nodeName() == "translation") {
                        if(stext.nodeValue() != dest1) {
                            setMessage(QString("unmatch line %1 at row %2").arg(count).arg(olang));
                            setHint(KBM_HINT_LANG_INVALID_LANG_FILE);
                            return false;
                        }

                        if(n.hasAttributes()) {
                            qDebug() << "delete translation attribute: type";
                            n.attributes().removeNamedItem("type");
                        }

                        if(n.firstChild().isNull()) {
                            QDomText node1 = doc.createTextNode(dest2);
                            n.appendChild(node1);
                        } else {
                            QDomNode node1 = n.firstChild();
                            n.firstChild().setNodeValue(dest2);
                            QDomNode node2 = n.firstChild();
                            n.replaceChild(node2, node1);
                        }
                    }
                }
            }

            node = node.nextSibling();
        }

        context = context.nextSibling();
    }

    qDebug() << "to save file " << src;
    QFile xfile(src);
    if( !xfile.open( QFile::WriteOnly | QFile::Truncate) ) {
        setMessage(QString("failed to open dest file %1").arg(src));
        return false;
    }

    QTextStream ts(&xfile);
    ts.reset();
    ts.setCodec("utf-8");
    doc.save(ts, 4, QDomNode::EncodingFromDocument);
    xfile.close();
    setMessage(QString("merge language finished"));

    return true;
}
