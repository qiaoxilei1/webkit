#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QString>
#include <QImage>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_PushButton_clicked();
    void onProgress(int progress);
    void onLoadFinished(bool ok);

private:
    //下载一张图片
    QImage downloadImage(const QString &url);

private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
