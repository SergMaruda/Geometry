#pragma once
#include "Notifications.h"
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <set>


struct IUIObject;

struct IConnection
  {
  virtual ~IConnection() = 0 {};
  };

class NotificationCenter
  {
  public:
    typedef std::function<void (IUIObject*)> TNotificationFunc;
    typedef std::vector<TNotificationFunc> TObservers;
    typedef std::shared_ptr<IConnection> TConnectionPtr;

    static NotificationCenter& Instance();

    TConnectionPtr AddObserver(ENotification, const TNotificationFunc&);
    void Notify(ENotification, IUIObject* ip_sender);

  private:
    NotificationCenter();
    NotificationCenter(const NotificationCenter&);
    friend class Connection;
    
    std::map<IConnection*, TNotificationFunc> m_functorid;
    std::map<IConnection*, ENotification> m_connection_notification;
    std::map<ENotification, std::set<IConnection*>> m_observers;
  };