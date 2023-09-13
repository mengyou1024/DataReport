#include "qwaveview.h"

QWaveView::QWaveView() {
    m_flushTimer.setInterval(10);
    connect(&m_flushTimer, &QTimer::timeout, this, [this]() { update(); });
    start();
}

void QWaveView::paint(QPainter *painter) {
    painter->fillRect(QRect(0, 0, width(), height()), Qt::black);
    painter->setFont(QFont("楷体", 12, 2));
    drawAxias(painter);
    drawGridlines(painter);
    painter->setPen(QColor(51, 153, 229));
    painter->setFont(QFont("楷体", 18, 4));
    painter->drawText(QRect(0, getDrawable().bottom(), axisWidth, axisWidth), Qt::AlignCenter, QString::number(sequence));
    drawLines(painter);
    static qint64 lastTime = QDateTime::currentMSecsSinceEpoch();
    static int    i        = 0;
    static qreal  fps      = 0;
    painter->setFont(QFont("楷体", 12, 2));
    if (++i == 10) {
        fps      = 1000.0 * 10 / (QDateTime::currentMSecsSinceEpoch() - lastTime);
        lastTime = QDateTime::currentMSecsSinceEpoch();
        i        = 0;
    }
    QString s = QString("FPS:%1").arg(QString::number(fps, 'f', 1));
    painter->drawText(QPoint{axisWidth + 5, 15}, s);
}

QPointF QWaveView::getAbscissaRange() const {
    return abscissaRange;
}

void QWaveView::setAbscissaRange(QPointF newAbscissaRange) {
    if (abscissaRange == newAbscissaRange)
        return;
    abscissaRange = newAbscissaRange;
    emit abscissaRangeChanged();
}

bool QWaveView::getDisplayAxis() const {
    return displayAxis;
}

void QWaveView::setDisplayAxis(bool newDisplayAxis) {
    if (displayAxis == newDisplayAxis)
        return;
    displayAxis = newDisplayAxis;
    emit displayAxisChanged();
}

bool QWaveView::getDisplayGridlines() const {
    return displayGridlines;
}

void QWaveView::setDisplayGridlines(bool newDisplayGridlines) {
    if (displayGridlines == newDisplayGridlines)
        return;
    displayGridlines = newDisplayGridlines;
    emit displayGridlinesChanged();
}

QPointF QWaveView::getDrawMap() const {
    return drawMap;
}

void QWaveView::setDrawMap(QPointF newDrawMap) {
    if (drawMap == newDrawMap)
        return;
    drawMap = newDrawMap;
    emit drawMapChanged();
}

int QWaveView::getAxisWidth() const {
    return axisWidth;
}

void QWaveView::setAxisWidth(int newAxisWidth) {
    if (axisWidth == newAxisWidth)
        return;
    axisWidth = newAxisWidth;
    emit axisWidthChanged();
}

int QWaveView::getSequence() const {
    return sequence;
}

void QWaveView::setSequence(int newSequence) {
    if (sequence == newSequence)
        return;
    sequence = newSequence;
    emit sequenceChanged();
}

bool QWaveView::getDisplayLines() const {
    return displayLines;
}

void QWaveView::setDisplayLines(bool newDisplayLines) {
    if (displayLines == newDisplayLines)
        return;
    displayLines = newDisplayLines;
    emit displayLinesChanged();
}

void QWaveView::start() {
    setDisplayLines(true);
    m_batch = 0;
    update();
    m_flushTimer.start();
}

void QWaveView::pause() {
    m_flushTimer.stop();
}

void QWaveView::reseum() {
    m_flushTimer.start();
}

void QWaveView::stop() {
    setDisplayLines(false);
    m_batch = 0;
    m_flushTimer.stop();
    update();
}

QVector<uint8_t> QWaveView::getLineData() const {
    return lineData;
}

void QWaveView::setLineData(const QVector<uint8_t> &newLineData) {
    if (lineData == newLineData)
        return;
    lineData = newLineData;
    emit lineDataChanged();
}

QRect QWaveView::getDrawable() {
    if (displayAxis) {
        return QRect(axisWidth + 1, 0, width() - axisWidth - 1, height() - axisWidth);
    }
    return QRect(0, 0, width(), height());
}

void QWaveView::drawAxias(QPainter *painter) {
    if (displayAxis) {
        auto drawAble = getDrawable();
        // 横轴
        for (int i = 0; i <= 50; i++) {
            QLineF       li;
            qreal        x  = drawAble.x() + ((drawAble.width() - 1) / 50.0) * i;
            qreal        y  = drawAble.bottom() + 1;
            QColor       c  = QColor(0, 255, 0);
            QString      s  = QString::number((abscissaRange.y() - abscissaRange.x()) / 50.0 * i, 'f', 1);
            QFontMetrics fm = painter->fontMetrics();
            painter->setPen(c);
            if (i % 10 == 0) {
                li = {x, y, x, y + 15};
                if (i == 0) {
                    c = QColor(255, 0, 0);
                    painter->setPen(c);
                }
                QRect fmRect   = fm.boundingRect(s);
                QRect drawRect = {};
                if (i != 50) {
                    drawRect = QRect(QPoint(li.p2().x() + 5, li.p2().y() - 5), QSize(fmRect.width(), fmRect.height()));
                } else {
                    drawRect = QRect(QPoint(li.p2().x() - fmRect.width() - 5, li.p2().y() - 5), QSize(fmRect.width(), fmRect.height()));
                }
                painter->drawText(drawRect, Qt::AlignCenter, s);
            } else if (i % 5 == 0) {
                li = {x, y, x, y + 10};
            } else {
                li = {x, y, x, y + 5};
            }
            painter->drawLine(li);
        }
        // 纵轴(x相同, y变化)
        for (int i = 0; i <= 50; i++) {
            QLineF       li;
            qreal        x  = drawAble.left();
            qreal        y  = drawAble.height() / 50.0 * i;
            QColor       c  = QColor(0, 255, 0);
            QString      s  = QString::number((ordinateRange.y() - ordinateRange.x()) / 50.0 * (50 - i), 'f', 1);
            QFontMetrics fm = painter->fontMetrics();
            painter->setPen(c);
            if (i % 10 == 0) {
                li = {x - 15, y, x, y};
                if (i == 50) {
                    c = QColor(255, 0, 0);
                    painter->setPen(c);
                }
                painter->save();
                QRect fmRect   = fm.boundingRect(s);
                QRect drawRect = QRect(QPoint(0, 0), QSize(fmRect.width(), fmRect.height()));
                if (i != 0) {
                    painter->translate(li.p1().x() + 5, li.p1().y() - 5 - fmRect.width());
                } else {
                    painter->translate(li.p1().x() + 5, li.p1().y() + 5);
                }
                painter->rotate(90);
                painter->drawText(drawRect, Qt::AlignCenter, s);
                painter->restore();
            } else if (i % 5 == 0) {
                li = {x - 10, y, x, y};
            } else {
                li = {x - 5, y, x, y};
            }
            painter->drawLine(li);
        }
    }
}

void QWaveView::drawGridlines(QPainter *painter) {
    if (displayGridlines) {
        QRect drawAble = getDrawable();
        // 绘制坐标轴
        // 虚线的画笔
        QVector<qreal> dashes;
        qreal          space = 4;
        dashes << 1 << space << 1 << space;
        QPen pen;
        pen.setBrush(Qt::yellow);
        pen.setDashPattern(dashes);
        QPointF grid[24] = {};
        // 横向
        qreal temp;
        for (int32_t i = 0; i < 12; i += 2) {
            temp        = qreal(drawAble.height()) / 5 * i / 2 + qreal(drawAble.y());
            grid[i]     = {qreal(drawAble.left()), temp};
            grid[i + 1] = {qreal(drawAble.right()), temp};
        }
        // 纵向
        for (int32_t i = 0; i < 12; i += 2) {
            temp             = qreal(drawAble.width() - 1) / 5.0 * i / 2.0 + qreal(drawAble.x());
            grid[12 + i]     = {temp, qreal(drawAble.top())};
            grid[12 + i + 1] = {temp, qreal(drawAble.bottom())};
        }
        painter->setPen(pen);
        painter->drawLines(grid, 12);
    }
}

void QWaveView::drawLines(QPainter *painter) {
    if (displayLines) {
        if (m_batch++ == 0) {
            QPointF dot1 = {200, 100};
            QPointF dot2 = {399, 100};
            painter->drawLine(mapDot(dot1), mapDot(dot2));
        } else {
            m_batch      = 0;
            QPointF dot1 = {200, 200};
            QPointF dot2 = {399, 200};
            painter->drawLine(mapDot(dot1), mapDot(dot2));
        }
    }
}

QPointF &QWaveView::mapDot(QPointF &dot) {
    QRect drawAble = getDrawable();
    qreal width    = drawAble.width();
    qreal height   = drawAble.height();
    dot.setX(drawAble.left() + (dot.x() / drawMap.x()) * width);
    dot.setY(drawAble.bottom() - (dot.y() / drawMap.y() * height));
    return dot;
}

QPointF QWaveView::getOrdinateRange() const {
    return ordinateRange;
}

void QWaveView::setOrdinateRange(QPointF newOrdinateRange) {
    if (ordinateRange == newOrdinateRange)
        return;
    ordinateRange = newOrdinateRange;
    emit ordinateRangeChanged();
}
