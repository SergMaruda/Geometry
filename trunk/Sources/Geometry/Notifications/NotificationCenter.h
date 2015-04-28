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

typedef std::shared_ptr<ISubscription> TSubscriptionPtr;
typedef std::vector<TSubscriptionPtr> TSubscriptions;

class NotificationCenter
  {
  public:
    typedef std::function<void (IUIObject*)> TNotificationFunc;
    typedef std::vector<TNotificationFunc> TObservers;

    static NotificationCenter& Instance();

    TSubscriptionPtr Subscribe(ENotification, const TNotificationFunc&);
    void Subscribe(TSubscriptions&, ENotification, const TNotificationFunc&);

    template<class TObserver>
    TSubscriptionPtr Subscribe(ENotification i_ntf, TObserver* ip_observer, void (TObserver::*ip_func)(IUIObject*))
      {
      return Subscribe(i_ntf, std::bind1st(std::mem_fun(ip_func), ip_observer));
      }

    template<class TObserver>
    void Subscribe(TSubscriptions& o_subscriptions, ENotification i_ntf, TObserver* ip_observer, void (TObserver::*ip_func)(IUIObject*))
      {
      return Subscribe(o_subscriptions, i_ntf, std::bind1st(std::mem_fun(ip_func), ip_observer));
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
