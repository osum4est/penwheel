#ifndef PEN_WHEEL_WHEEL_H
#define PEN_WHEEL_WHEEL_H

#include <QDialog>
#include <QThread>
#include <utils/mouse_events.h>

QT_BEGIN_NAMESPACE
namespace Ui { class wheel; }
QT_END_NAMESPACE

class wheel : public QDialog, public mouse_event_handler{
Q_OBJECT

    Ui::wheel *_ui;
    bool _active;

public:
    explicit wheel(QWidget *parent = nullptr);
    ~wheel() override;

    void mouse_moved(const QPointF &pos) override;
    void mouse_down(const Qt::MouseButton &button) override;
    void mouse_up(const Qt::MouseButton &button) override;

private:
    void move_window(const QPointF pos);
};

#endif
