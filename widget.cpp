#include "widget.h"
#include "ui_widget.h"
#include <QSettings>
#include <QDebug>
#include <QWebView>
#include <QWebFrame>
#include <QWebPage>
#include <QWebElementCollection>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QFileInfo>
#include <QNetworkReply>
#include <QTimer>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QWebSettings * setting = ui->webView->settings();
    setting->setAttribute(QWebSettings::AutoLoadImages, false);
    setting->setAttribute(QWebSettings::JavascriptEnabled, false);

    connect(ui->webView, SIGNAL(loadProgress(int)),this, SLOT(onProgress(int)));
    connect(ui->webView, SIGNAL(loadFinished(bool)),this, SLOT(onLoadFinished(bool)));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_PushButton_clicked()
{
    QUrl url = ui->lineEdit->text();
    ui->webView->load(url);
}

void Widget::onProgress(int progress)
{
    QString title = QString::number(progress)+"%";
    this->setWindowTitle(title);
}


void Widget::onLoadFinished(bool ok)
{
    qDebug() << "load finished" << ok;
    QWebElementCollection elements = ui->webView->page()->mainFrame()->findAllElements("body > div > div.left > ul > li > div > a > img");

    for(int i=0; i < elements.count(); ++i)
    {
        const QWebElement &ele = elements.at(i);
        QString url = ele.attribute("src");
        QImage image = downloadImage(url);

        QFileInfo info(url);
        QString fileName = info.fileName();

        image.save(fileName);

        qDebug() << "save image" << fileName;

        QEventLoop loop;

        QTimer::singleShot(5*1000, &loop, SLOT(quit()));

        loop.exec();

    }

    elements = ui->webView->page()->mainFrame()->findAllElements("body > div > div.left > ul > li > div > a > img");
    if(elements.count() <= 0)
    {
        qDebug() << "no next page";
        return;
    }
    QString nextPageUrl = "https://www.ivsky.com/tupian/ziranfengguang/" + elements.at(0).attribute("href");
    ui->webView->load(QUrl(nextPageUrl));

}

QImage Widget::downloadImage(const QString &url)
{
    QNetworkAccessManager mgr;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply * reply = mgr.get(request);

    QEventLoop loop;

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QByteArray data = reply->readAll();

    QImage image = QImage::fromData(data);

    return image;
}
