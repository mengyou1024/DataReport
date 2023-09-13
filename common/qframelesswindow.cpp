#include "qframelesswindow.h"

QFramelessWindow::QFramelessWindow(QWindow *parent)
    : QQuickWindow(parent) {
    setFlags(flags() | Qt::WindowType::Window | Qt::WindowType::FramelessWindowHint);
    setWindowsStyle();
}

bool QFramelessWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
    MSG *msg = reinterpret_cast<MSG *>(message);
    switch (msg->message) {
        case WM_NCCALCSIZE: {
            *result = 0;
            return true;
        }
        case WM_NCHITTEST: {
            if (inMaximumButton()) {
                *result = HTMAXBUTTON;
                qDebug() << "inMaximumButton";
                return true;
            }
            QPoint mouse_pos(GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam));
            *result = resizeWindow(msg->hwnd, mouse_pos);
            if (*result != 0) {
                return true;
            } else {
                return false;
            }
        }
        default: {
            return QWindow::nativeEvent(eventType, message, result);
        }
    }
}

bool QFramelessWindow::event(QEvent *event) {
    if (event->type() == QEvent::Type::WindowStateChange) {
        auto ev = static_cast<QWindowStateChangeEvent *>(event);
        if (ev->oldState() == Qt::WindowNoState && windowState() == Qt::WindowMaximized) {
        } else if (ev->oldState() == Qt::WindowMaximized && windowState() == Qt::WindowNoState) {
        }
    }
    return QQuickWindow::event(event);
}

void QFramelessWindow::setWindowsStyle() {
    auto  hwnd  = reinterpret_cast<HWND>(winId());
    DWORD style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME /*| WS_CAPTION |WS_MAXIMIZEBOX | WS_MINIMIZEBOX*/);
    const MARGINS shadow = {1, 1, 1, 1};
    DwmExtendFrameIntoClientArea(hwnd, &shadow);
}

int QFramelessWindow::resizeWindow(HWND hwnd, const QPoint &mouse_pos) {
    int  result      = 0;
    RECT window_rect = {0};
    GetWindowRect(hwnd, &window_rect);
    bool allow_resize_width  = minimumWidth() != maximumWidth();
    bool allow_resize_height = minimumHeight() != maximumHeight();
    int  mouse_x             = mouse_pos.x();
    int  mouse_y             = mouse_pos.y();
    if (allow_resize_width) {
        if (mouse_x >= window_rect.left - RESIZE_EDGE_THRESHOLD && mouse_x <= window_rect.left + RESIZE_EDGE_THRESHOLD) {
            result = HTLEFT;
        } else if (mouse_x >= window_rect.right - RESIZE_EDGE_THRESHOLD && mouse_x <= window_rect.right + RESIZE_EDGE_THRESHOLD) {
            result = HTRIGHT;
        }
    }
    if (allow_resize_height) {
        if (mouse_y > window_rect.top - RESIZE_EDGE_THRESHOLD && mouse_y <= window_rect.top + RESIZE_EDGE_THRESHOLD) {
            result = HTTOP;
        } else if (mouse_y > window_rect.bottom - RESIZE_EDGE_THRESHOLD && mouse_y <= window_rect.bottom + RESIZE_EDGE_THRESHOLD) {
            result = HTBOTTOM;
        }
    }
    if (allow_resize_width && allow_resize_height) {
        if (mouse_x >= window_rect.left - RESIZE_EDGE_THRESHOLD && mouse_x <= window_rect.left + RESIZE_EDGE_THRESHOLD &&
            mouse_y > window_rect.top - RESIZE_EDGE_THRESHOLD && mouse_y <= window_rect.top + RESIZE_EDGE_THRESHOLD) {
            result = HTTOPLEFT;
        } else if (mouse_x >= window_rect.right - RESIZE_EDGE_THRESHOLD && mouse_x <= window_rect.right + RESIZE_EDGE_THRESHOLD &&
                   mouse_y > window_rect.top - RESIZE_EDGE_THRESHOLD && mouse_y <= window_rect.top + RESIZE_EDGE_THRESHOLD) {
            result = HTTOPRIGHT;
        } else if (mouse_x >= window_rect.left - RESIZE_EDGE_THRESHOLD && mouse_x <= window_rect.left + RESIZE_EDGE_THRESHOLD &&
                   mouse_y > window_rect.bottom - RESIZE_EDGE_THRESHOLD && mouse_y <= window_rect.bottom + RESIZE_EDGE_THRESHOLD) {
            result = HTBOTTOMLEFT;
        } else if (mouse_x >= window_rect.right - RESIZE_EDGE_THRESHOLD && mouse_x <= window_rect.right + RESIZE_EDGE_THRESHOLD &&
                   mouse_y > window_rect.bottom - RESIZE_EDGE_THRESHOLD && mouse_y <= window_rect.bottom + RESIZE_EDGE_THRESHOLD) {
            result = HTBOTTOMRIGHT;
        }
    }
    return result;
}

bool QFramelessWindow::inMaximumButton() const {
    return mInMaximumButton;
}

void QFramelessWindow::setInMaximumButton(bool newInMaximumButton) {
    if (mInMaximumButton == newInMaximumButton)
        return;
    mInMaximumButton = newInMaximumButton;
    emit inMaximumButtonChanged();
}
