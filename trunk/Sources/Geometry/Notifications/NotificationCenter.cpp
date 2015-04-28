#include "NotificationCenter.h"

class Connection: public ISubscription
  {
  public:

  ~Connection()
    {
    auto& nfc = NotificationCenter::Instance();
    nfc.m_functorid.erase(this);
    auto notification = nfc.m_connection_notification[this];
    nfc.m_observers[notification].erase(this);
    nfc.m_connection_notification.erase(this);
    }

  };

//--------------------------------------------------------------------------------------------------
NotificationCenter& NotificationCenter::Instance()
  {
  static NotificationCenter instance;
  return instance;
  }

//--------------------------------------------------------------------------------------------------
NotificationCenter::TSubscriptionPtr NotificationCenter::Subscribe( ENotification i_notification, const TNotificationFunc& functor)
  {
  auto p_connection = new Connection;
  m_functorid[p_connection] = functor;
  m_connection_notification[p_connection] = i_notification;

  m_observers[i_notification].insert(p_connection);
  return TSubscriptionPtr(p_connection);
  }

//--------------------------------------------------------------------------------------------------
void NotificationCenter::Notify( ENotification i_notification, IUIObject* ip_sender )
  {
  auto& handlers = m_observers[i_notification];
  for(auto i = handlers.begin(); i != handlers.end(); ++i)
    {
    m_functorid[*i](ip_sender);
    }
  }

//--------------------------------------------------------------------------------------------------
NotificationCenter::NotificationCenter()
  {
  }
