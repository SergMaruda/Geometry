#pragma once
#include "Notifications.h"
#include "NotificationCenter.h"


class Observer
  {
  public:
    void Subscribe(ENotification i_ntf, const NotificationCenter::TNotificationFunc& i_func)
      {
      m_subscriptions.push_back(NotificationCenter::Subscribe(i_ntf, i_func));
      }
    
    template<class TObserver>
    void Subscribe(ENotification i_ntf, TObserver* ip_observer, void (TObserver::*ip_func)(IUIObject*))
      {
      Subscribe(i_ntf, std::bind1st(std::mem_fun(ip_func), ip_observer));
      }
  protected:
    TSubscriptions m_subscriptions;
  };