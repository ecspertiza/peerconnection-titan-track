#include "pch.h"

#include "TitanMediaTrackInterface.h"


TitanTrack::TitanTrack(const std::string& id, TitanTrackSourceInterface* titan_source)
    : MediaStreamTrack<TitanTrackInterface>(id), _titanSource(titan_source) 
{
  _titanSource->RegisterObserver(this);
}

bool TitanTrack::set_enabled(bool enable)
{
  return MediaStreamTrack<TitanTrackInterface>::set_enabled(enable);
}

std::string TitanTrack::kind() const
{
  return kVideoKind;
}

void TitanTrack::OnChanged()
{
  if (_titanSource->state() == webrtc::MediaSourceInterface::kEnded) {
    set_state(kEnded);
  } else {
    set_state(kLive);
  }
}

void TitanTrack::AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
                                 const rtc::VideoSinkWants& wants) 
{
  VideoSourceBase::AddOrUpdateSink(sink, wants);
  rtc::VideoSinkWants modified_wants = wants;
  modified_wants.black_frames = !enabled();
  _titanSource->AddOrUpdateSink(sink, modified_wants);
}

void TitanTrack::RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) 
{
  VideoSourceBase::RemoveSink(sink);
  _titanSource->RemoveSink(sink);
}