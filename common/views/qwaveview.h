#ifndef QWAVEVIEW_H
#define QWAVEVIEW_H

#include <QJsonObject>
#include <QPainter>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QTimer>

class QWaveView : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
public:
    QWaveView();
    ~QWaveView() = default;

    bool             displayAxis      = true;       ///< 显示坐标轴
    bool             displayLines     = true;       ///< 显示线条
    int              axisWidth        = 35;         ///< 坐标轴宽度
    bool             displayGridlines = true;       ///< 显示网格线
    QPointF          abscissaRange    = {0, 100};   ///< 横坐标范围
    QPointF          ordinateRange    = {0, 100};   ///< 纵坐标范围
    QPointF          drawMap          = {400, 255}; ///< 绘图映射范围
    int              sequence         = 0;          ///< 图像序号
    QVector<uint8_t> lineData         = {};         ///< 曲线数据

    Q_INVOKABLE void start(void);
    Q_INVOKABLE void pause(void);
    Q_INVOKABLE void reseum(void);
    Q_INVOKABLE void stop(void);

    QPointF getOrdinateRange() const;
    void    setOrdinateRange(QPointF newOrdinateRange);
    QPointF getAbscissaRange() const;
    void    setAbscissaRange(QPointF newAbscissaRange);
    bool    getDisplayAxis() const;
    void    setDisplayAxis(bool newDisplayAxis);
    bool    getDisplayGridlines() const;
    void    setDisplayGridlines(bool newDisplayGridlines);
    QPointF getDrawMap() const;
    void    setDrawMap(QPointF newDrawMap);
    int     getAxisWidth() const;
    void    setAxisWidth(int newAxisWidth);
    int     getSequence() const;
    void    setSequence(int newSequence);

    QVector<uint8_t> getLineData() const;
    void             setLineData(const QVector<uint8_t> &newLineData);

    bool getDisplayLines() const;
    void setDisplayLines(bool newDisplayLines);

signals:
    void ordinateRangeChanged();
    void abscissaRangeChanged();
    void displayAxisChanged();
    void displayGridlinesChanged();
    void drawMapChanged();
    void lineDataChanged();
    void axisWidthChanged();
    void sequenceChanged();

    void displayLinesChanged();

private:
    QTimer m_flushTimer{};
    int    m_batch = 0; ///< 批次

    QRect getDrawable(void);
    void  drawAxias(QPainter *painter);
    void  drawGridlines(QPainter *painter);

    virtual void     drawLines(QPainter *painter);
    virtual QPointF &mapDot(QPointF &dot);

    Q_PROPERTY(QPointF ordinateRange READ getOrdinateRange WRITE setOrdinateRange NOTIFY ordinateRangeChanged)
    Q_PROPERTY(QPointF abscissaRange READ getAbscissaRange WRITE setAbscissaRange NOTIFY abscissaRangeChanged)
    Q_PROPERTY(bool displayAxis READ getDisplayAxis WRITE setDisplayAxis NOTIFY displayAxisChanged)
    Q_PROPERTY(bool displayGridlines READ getDisplayGridlines WRITE setDisplayGridlines NOTIFY displayGridlinesChanged)
    Q_PROPERTY(QPointF drawMap READ getDrawMap WRITE setDrawMap NOTIFY drawMapChanged)
    Q_PROPERTY(QVector<uint8_t> lineData READ getLineData WRITE setLineData NOTIFY lineDataChanged)
    Q_PROPERTY(int axisWidth READ getAxisWidth WRITE setAxisWidth NOTIFY axisWidthChanged)
    Q_PROPERTY(int sequence READ getSequence WRITE setSequence NOTIFY sequenceChanged)

    Q_PROPERTY(bool displayLines READ getDisplayLines WRITE setDisplayLines NOTIFY displayLinesChanged)

protected:
    void paint(QPainter *painter) override;
};

#endif // QWAVEVIEW_H
