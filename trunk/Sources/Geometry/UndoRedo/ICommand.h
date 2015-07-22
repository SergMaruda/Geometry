#pragma once

namespace UR
  {
  struct ICommand
    {
    virtual bool Do() = 0;
    virtual ~ICommand() = 0 {};
    };
  }