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