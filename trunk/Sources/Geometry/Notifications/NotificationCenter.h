#pragma once
#include "Notifications.h"
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include "..\Primitives\IUIObject.h"

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

    static TSubscriptionPtr Subscribe(ENotification, const TNotificationFunc&);
    static void Subscribe(TSubscriptions&, ENotification, const TNotificationFunc&);

    template<class TObserver>
    static TSubscriptionPtr Subscribe(ENotification i_ntf, TObserver* ip_observer, void (TObserver::*ip_func)(IUIObject*))
      {
      return Subscribe(i_ntf, std::bind1st(std::mem_fun(ip_func), ip_observer));
      }

    template<class TObserver>
    static void Subscribe(TSubscriptions& o_subscriptions, ENotification i_ntf, TObserver* ip_observer, void (TObserver::*ip_func)(IUIObject*))
      {
      return Subscribe(o_subscriptions, i_ntf, std::bind1st(std::mem_fun(ip_func), ip_observer));
      }


    static void Notify(ENotification, IUIObject* ip_sender);

  private:
    NotificationCenter();
    ~NotificationCenter();
    NotificationCenter(const NotificationCenter&);
    static NotificationCenter& Instance();
    friend class Subscription;
    
    std::map<ISubscription*, TNotificationFunc> m_functorid;
    std::map<ISubscription*, ENotification> m_connection_notification;
    std::map<ENotification, std::set<ISubscription*>> m_observers;
    bool m_blocked;
  };
