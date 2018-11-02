#pragma once

#include <api/mediastreaminterface.h>
#include <api/notifier.h>
#include <media/base/mediachannel.h>
#include <media/base/videosourcebase.h>
#include <rtc_base/refcountedobject.h>

class FrameBuffer : rtc::RefCountedObject<webrtc::VideoFrameBuffer> 
{
  public:
    FrameBuffer();


    // This function specifies in what pixel format the data is stored in.
    Type type() const override { return Type::kNative; }

    // The resolution of the frame in pixels. For formats where some planes are
    // subsampled, this is the highest-resolution plane.
    int width() const override  { return 10; }
    int height() const override  { return 10; }

};

class TitanTrackSourceInterface
    : public rtc::RefCountedObject<
          webrtc::Notifier<webrtc::VideoTrackSourceInterface>> {
 public:
  TitanTrackSourceInterface() = default;
};

class TitanTrackSource : public TitanTrackSourceInterface,
                         public rtc::VideoSourceBase {
 public:
  TitanTrackSource(bool changes = false, bool remote = false);

  SourceState state() const override { return state_; }
  bool remote() const override { return remote_; }

  bool is_screencast() const override { return false; }
  rtc::Optional<bool> needs_denoising() const override { return rtc::nullopt; }

  bool GetStats(Stats* stats) override { return false; }

  void AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
                       const rtc::VideoSinkWants& wants) override;
  void RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) override;

 private:
  rtc::ThreadChecker worker_thread_checker_;
  rtc::VideoSourceInterface<webrtc::VideoFrame>* source_;
  std::vector<rtc::VideoSinkInterface<webrtc::VideoFrame>*> _container;

  cricket::VideoOptions options_;
  SourceState state_;
  const bool remote_;

  const rtc::scoped_refptr<webrtc::VideoFrameBuffer> buffer;

  void CompleteFrame();
};
