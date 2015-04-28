#pragma once
#include "Notifications.h"
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <set>


struct IUIObject;

struct ISubscription
  {
  virtual ~ISubscription() = 0 {};
  };

class NotificationCenter
  {
  public:
    typedef std::function<void (IUIObject*)> TNotificationFunc;
    typedef std::vector<TNotificationFunc> TObservers;
    typedef std::shared_ptr<ISubscription> TSubscriptionPtr;

    static NotificationCenter& Instance();

    TSubscriptionPtr AddObserver(ENotification, const TNotificationFunc&);

    template<class TObserver>
    TSubscriptionPtr AddObserver(ENotification i_ntf, TObserver* ip_observer, void (TObserver::*ip_func)(IUIObject*))
      {
      return AddObserver(i_ntf, std::bind1st(std::mem_fun(ip_func), ip_observer));
      }

    void Notify(ENotification, IUIObject* ip_sender);

  private:
    NotificationCenter();
    NotificationCenter(const NotificationCenter&);
    friend class Connection;
    
    std::map<ISubscription*, TNotificationFunc> m_functorid;
    std::map<ISubscription*, ENotification> m_connection_notification;
    std::map<ENotification, std::set<ISubscription*>> m_observers;
  };