#pragma once
#include "pch.h"

#include "TitanMediaSourceInterface.h"
#include <iostream>
#include <thread>
#include <api/video/i420_buffer.h>


class Timer {
  std::thread th;
  bool running = false;

 public:
  typedef std::chrono::milliseconds Interval;
  typedef std::function<void(void)> Timeout;

  void start(const Interval& interval, const Timeout& timeout) {
    running = true;

    th = std::thread([=]() {
      while (running == true) {
        std::this_thread::sleep_for(interval);
        timeout();
      }
    });
  }

  void stop() { running = false; }
};

Timer* timer;

TitanTrackSource::TitanTrackSource(bool changes, bool remote)
    : remote_(remote) {
  if (changes == true) {
    timer = new Timer;
    timer->start(std::chrono::milliseconds(1000),
                 [this] { this->CompleteFrame(); });
  }
}

void TitanTrackSource::AddOrUpdateSink(
    rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
    const rtc::VideoSinkWants& wants) {
  VideoSourceBase::AddOrUpdateSink(sink, wants);
  std::cout << __FUNCTION__ << std::endl;
}

void TitanTrackSource::RemoveSink(
    rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) {
  VideoSourceBase::RemoveSink(sink);
  std::cout << __FUNCTION__ << std::endl;
}

int timestamp = 0;

uint8_t red[25] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t green[25] = {0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t blue[25] = {0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int type = 0;

void TitanTrackSource::CompleteFrame()
{
  for (auto& sink_pair : sink_pairs()) {

      
      rtc::scoped_refptr<webrtc::I420Buffer> buffer(
          webrtc::I420Buffer::Create(5, 5));
      buffer->InitializeData();

      if (type == 0)
      {
        memcpy((void*)buffer->DataY(), red, 25);
        std::cout << "send red" << std::endl;
        type++;
      }else if (type == 1)
      {
        memcpy((void*)buffer->DataY(), green, 25);
        std::cout << "send green" << std::endl;
        type++;
      }else if (type == 2)
      {
        memcpy((void*)buffer->DataY(), blue, 25);
        std::cout << "send blue" << std::endl;
        type = 0;
      }
      
      const uint8_t *data = buffer->DataY();

      time_t ltime;
      time(&ltime);

      timestamp += rtc::kNumMicrosecsPerSec / 30;

      // timestamp = ltime;
      // std::cout << "timestamp = " << timestamp;
      sink_pair.sink->OnFrame(
          webrtc::VideoFrame(buffer, webrtc::kVideoRotation_0, timestamp));
      this->FireOnChanged();
  }
}