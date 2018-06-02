#pragma once

#include <QObject>

class ProcessingFinishedParams;


class IRequestSender
{
public:
  virtual void responseSendRequest(int32_t id) = 0;
  virtual void informationProcessingFinishedValue(const ProcessingFinishedParams& value) = 0;
};
