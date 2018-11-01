#pragma once

#include "api/mediastreaminterface.h"
#include "api/notifier.h"
#include <rtc_base/refcountedobject.h>

class TitanTrackSourceInterface
    : public rtc::RefCountedObject<webrtc::MediaSourceInterface> {

 protected:
  ~TitanTrackSourceInterface() override = default;
};

class TitanTrackSource : public webrtc::Notifier<TitanTrackSourceInterface> {
  public:
    TitanTrackSource() = default;


    SourceState state() const override { return state_; }
    bool remote() const override { return remote_; }

private:
    SourceState state_;
    const bool remote_ = false;
};
