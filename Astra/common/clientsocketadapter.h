#ifndef CLIENTSOCKETADAPTER_H
# define CLIENTSOCKETADAPTER_H

# include "../common/adapterinterface.h"

class ClientSocketAdapter : public AdapterInterface {
  Q_OBJECT
public:
  explicit ClientSocketAdapter(QObject *parent);
};

#endif // CLIENTSOCKETADAPTER_H
