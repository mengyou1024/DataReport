#ifndef QWAVEVIEW_H
#define QWAVEVIEW_H

#include <QJsonObject>
#include <QPainter>
#include <QQuickItem>
#include <QQuickPaintedItem>

class QWaveView : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
public:
    QWaveView()  = default;
    ~QWaveView() = default;
public slots:

protected:
    void paint(QPainter *painter) override;

private:
};

#endif // QWAVEVIEW_H
