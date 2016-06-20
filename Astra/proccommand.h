#ifndef PROCCOMMAND_H
#define PROCCOMMAND_H

#include <QObject>
#include "state.h"

class ProcCommand : public QObject
{
    Q_OBJECT
public:
    explicit ProcCommand(QObject *parent = 0);

signals:
    void signal_status();

public slots:
    //void slot_connect();
    //void slot_disconnect();
    void slot_status();
    //void slot_reset();
    //void slot_read();
    //void slot_write();

private:
    State *state;
};

#endif // PROCCOMMAND_H
