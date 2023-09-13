#ifndef QFRAMELESSWINDOW_H
#define QFRAMELESSWINDOW_H

#include <QPoint>
#include <QQuickWindow>
#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>

class QFramelessWindow : public QQuickWindow {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit QFramelessWindow(QWindow *parent = nullptr);
    ~QFramelessWindow() override = default;
    bool inMaximumButton() const;
public slots:
    void setInMaximumButton(bool newInMaximumButton);

signals:
    void inMaximumButtonChanged();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    bool event(QEvent *event) override;

private:
    static constexpr int RESIZE_EDGE_THRESHOLD = 5;
    void                 setWindowsStyle(void);
    int                  resizeWindow(HWND hwnd, const QPoint &mouse_pos);
    bool                 mInMaximumButton = false;

    Q_PROPERTY(bool inMaximumButton READ inMaximumButton WRITE setInMaximumButton NOTIFY inMaximumButtonChanged)
};

#endif // QFRAMELESSWINDOW_H
