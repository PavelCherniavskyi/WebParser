#pragma once

#include <QObject>


class IRequestSender
{
public:
  void responseSendRequest(int32_t id);
};
