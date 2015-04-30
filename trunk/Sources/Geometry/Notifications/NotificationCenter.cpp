#include "NotificationCenter.h"

class Subscription: public ISubscription
  {
  public:

  ~Subscription()
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
TSubscriptionPtr NotificationCenter::Subscribe( ENotification i_ntf, const TNotificationFunc& i_handler)
  {
  auto p_connection = new Subscription;
  m_functorid[p_connection] = i_handler;
  m_connection_notification[p_connection] = i_ntf;

  m_observers[i_ntf].insert(p_connection);
  return TSubscriptionPtr(p_connection);
  }

void NotificationCenter::Subscribe( TSubscriptions& o_subscriptions, ENotification i_ntf, const TNotificationFunc& i_handler)
  {
  o_subscriptions.push_back(Subscribe(i_ntf, i_handler));
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
