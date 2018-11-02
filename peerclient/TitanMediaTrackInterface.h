#pragma once

#include "TitanMediaSourceInterface.h"

#include "api/mediastreaminterface.h"
#include <rtc_base/refcountedobject.h>
#include "pc/mediastreamtrack.h"

class TitanTrackInterface : public rtc::RefCountedObject<webrtc::VideoTrackInterface> 
{
public:
  TitanTrackInterface() = default;
  void AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
                       const rtc::VideoSinkWants& wants) override {}
  void RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) override {}
};


class TitanTrack : public webrtc::MediaStreamTrack<TitanTrackInterface>,
                   public rtc::VideoSourceBase,
                   public webrtc::ObserverInterface 
{
 public:
  TitanTrack(const std::string& id, TitanTrackSourceInterface* titan_source);

  bool set_enabled(bool enable) override;
  std::string kind() const override;

  void OnChanged() override;

  webrtc::VideoTrackSourceInterface* GetSource() const override {
    return _titanSource;
  }

  void AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
                       const rtc::VideoSinkWants& wants) override;
  void RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) override;

private:
 rtc::scoped_refptr<TitanTrackSourceInterface> _titanSource;
};
