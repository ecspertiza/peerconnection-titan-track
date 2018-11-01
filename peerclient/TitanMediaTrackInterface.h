#pragma once

#include "TitanMediaSourceInterface.h"

#include "api/mediastreaminterface.h"
#include <rtc_base/refcountedobject.h>
#include "pc/mediastreamtrack.h"

class TitanTrackInterface : public rtc::RefCountedObject<webrtc::MediaStreamTrackInterface>
{

};


class TitanTrack : public webrtc::MediaStreamTrack<TitanTrackInterface>,
                   public webrtc::ObserverInterface 
{
 public:
  TitanTrack(const std::string& id, TitanTrackSourceInterface* titan_source);

  bool set_enabled(bool enable) override;
  std::string kind() const override;

  void OnChanged() override;


private:
  TitanTrackSourceInterface* _titanSource;
};
