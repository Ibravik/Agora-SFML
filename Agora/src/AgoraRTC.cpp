#include "AgoraRTC.h"

namespace agoraYCE
{
#if defined(_WIN32)
#include <dwmapi.h>
#endif

  void EventHandler::SetOnWarning(std::function<void(const WarnInfo& _info)> _callback)
  {
    m_onWarning = std::move(_callback);
  }

  void EventHandler::SetOnError(std::function<void(const ErrorInfo& _info)> _callback)
  {
    m_onError = std::move(_callback);
  }

  void EventHandler::SetOnJoinChannelSuccess(std::function<void(const JoinChannelInfo& _info)> _callback)
  {
    m_onJoinChannelSuccess = std::move(_callback);
  }

  void EventHandler::SetOnLeaveChannel(std::function<void(const CallStatsInfo& _info)> _callback)
  {
    m_onLeaveChannel = std::move(_callback);
  }

  void EventHandler::SetOnClientRoleChanged(std::function<void(const RoleChangedInfo& _info)> _callback)
  {
    m_onClientRoleChanged = std::move(_callback);
  }

  void EventHandler::SetOnConnectionLost(std::function<void()> _callback)
  {
    m_onConnectionLost = std::move(_callback);
  }

  void EventHandler::SetOnRtcStats(std::function<void(const CallStatsInfo& _info)> _callback)
  {
    m_onRtcStats = std::move(_callback);
  }

  void EventHandler::SetOnStreamMessageError(std::function<void(const StreamErrorInfo& _info)> _callback)
  {
    m_onStreamMessageError = std::move(_callback);
  }

  void EventHandler::SetOnStreamInjectedStatus(std::function<void(const StreamStatusInfo& _info)> _callback)
  {
    m_onStreamInjectedStatus = std::move(_callback);
  }

  void EventHandler::SetOnUserJoined(std::function<void(const UserJoinInfo& _info)> _callback)
  {
    m_onUserJoined = std::move(_callback);
  }

  void EventHandler::SetOnActiveSpeaker(std::function<void(const unsigned int uid)> _callback)
  {
    m_onActiveSpeaker = std::move(_callback);
  }

  void EventHandler::SetOnRtmpStreamingStateChanged(std::function<void(const StreamingStateChangedInfo& _info)> _callback)
  {
    m_onRtmpStreamingStateChanged = std::move(_callback);
  }

  void EventHandler::SetOnRtmpStreamingEvent(std::function<void(const StreamingEventInfo& _info)> _callback)
  {
    m_onRtmpStreamingEvent = std::move(_callback);
  }

  void EventHandler::SetOnUserOffline(std::function<void(const UserOfflineInfo& _info)> _callback)
  {
    m_onUserOffline = std::move(_callback);
  }

  void EventHandler::SetOnRemoteVideoStats(std::function<void(const RemoteVideoStatsInfo& _info)> _callback)
  {
    m_onRemoteVideoStats = std::move(_callback);
  }

  void EventHandler::SetOnLocalVideoStats(std::function<void(const LocalVideoStatsInfo& _info)> _callback)
  {
    m_onLocalVideoStats = std::move(_callback);
  }

  void EventHandler::onWarning(int warn, const char* msg)
  {
    if (m_onWarning != nullptr)
    {
      WarnInfo info;
      info.warn = warn;
      if (msg != nullptr)
      {
        info.msg = std::string(msg);
      }

      m_onWarning(info);
    }
  }

  void EventHandler::onError(int err, const char* msg)
  {
    if (m_onError != nullptr)
    {
      ErrorInfo info;
      info.err = err;
      if (msg != nullptr)
      {
        info.msg = std::string(msg);
      }

      m_onError(info);
    }
  }

  void EventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
  {
    if (m_onJoinChannelSuccess != nullptr)
    {
      JoinChannelInfo info;

      info.uid = uid;
      info.elapsed = elapsed;
      if (channel != nullptr)
      {
        info.channel = std::string(channel);
      }

      m_onJoinChannelSuccess(info);
    }
  }

  void EventHandler::onLeaveChannel(const RtcStats& stats)
  {
    if (m_onLeaveChannel != nullptr)
    {
      CallStatsInfo info;
      info.duration = stats.duration;
      info.txBytes = stats.txBytes;
      info.rxBytes = stats.rxBytes;
      info.txAudioBytes = stats.txAudioBytes;
      info.txVideoBytes = stats.txVideoBytes;
      info.rxAudioBytes = stats.rxAudioBytes;
      info.rxVideoBytes = stats.rxVideoBytes;
      info.txKBitRate = stats.txKBitRate;
      info.rxKBitRate = stats.rxKBitRate;
      info.rxAudioKBitRate = stats.rxAudioKBitRate;
      info.txAudioKBitRate = stats.txAudioKBitRate;
      info.rxVideoKBitRate = stats.rxVideoKBitRate;
      info.txVideoKBitRate = stats.txVideoKBitRate;
      info.lastmileDelay = stats.lastmileDelay;
      info.txPacketLossRate = stats.txPacketLossRate;
      info.rxPacketLossRate = stats.rxPacketLossRate;
      info.userCount = stats.userCount;
      info.cpuAppUsage = stats.cpuAppUsage;
      info.cpuTotalUsage = stats.cpuTotalUsage;
      info.gatewayRtt = stats.gatewayRtt;
      info.memoryAppUsageRatio = stats.memoryAppUsageRatio;
      info.memoryTotalUsageRatio = stats.memoryTotalUsageRatio;
      info.memoryAppUsageInKbytes = stats.memoryAppUsageInKbytes;

      m_onLeaveChannel(info);
    }
  }

  void EventHandler::onClientRoleChanged(CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole)
  {
    if (m_onClientRoleChanged != nullptr)
    {
      RoleChangedInfo info;
      info.oldRole = oldRole;
      info.newRole = newRole;

      m_onClientRoleChanged(info);
    }
  }

  void EventHandler::onConnectionLost()
  {
    if (m_onConnectionLost != nullptr)
    {
      m_onConnectionLost();
    }
  }

  void EventHandler::onRtcStats(const RtcStats& stats)
  {
    if (m_onRtcStats != nullptr)
    {
      CallStatsInfo info;
      info.duration = stats.duration;
      info.txBytes = stats.txBytes;
      info.rxBytes = stats.rxBytes;
      info.txAudioBytes = stats.txAudioBytes;
      info.txVideoBytes = stats.txVideoBytes;
      info.rxAudioBytes = stats.rxAudioBytes;
      info.rxVideoBytes = stats.rxVideoBytes;
      info.txKBitRate = stats.txKBitRate;
      info.rxKBitRate = stats.rxKBitRate;
      info.rxAudioKBitRate = stats.rxAudioKBitRate;
      info.txAudioKBitRate = stats.txAudioKBitRate;
      info.rxVideoKBitRate = stats.rxVideoKBitRate;
      info.txVideoKBitRate = stats.txVideoKBitRate;
      info.lastmileDelay = stats.lastmileDelay;
      info.txPacketLossRate = stats.txPacketLossRate;
      info.rxPacketLossRate = stats.rxPacketLossRate;
      info.userCount = stats.userCount;
      info.cpuAppUsage = stats.cpuAppUsage;
      info.cpuTotalUsage = stats.cpuTotalUsage;
      info.gatewayRtt = stats.gatewayRtt;
      info.memoryAppUsageRatio = stats.memoryAppUsageRatio;
      info.memoryTotalUsageRatio = stats.memoryTotalUsageRatio;
      info.memoryAppUsageInKbytes = stats.memoryAppUsageInKbytes;

      m_onRtcStats(info);
    }
  }

  void EventHandler::onStreamMessageError(uid_t uid, int streamId, int code, int missed, int cached)
  {
    if (m_onStreamMessageError != nullptr)
    {
      StreamErrorInfo info;
      info.uid = uid;
      info.streamId = streamId;
      info.code = code;
      info.missed = missed;
      info.cached = cached;

      m_onStreamMessageError(info);
    }
  }

  void EventHandler::onStreamInjectedStatus(const char* url, uid_t uid, int status)
  {
    if (m_onStreamInjectedStatus != nullptr)
    {
      StreamStatusInfo info;
      info.uid = uid;
      info.status = status;
      if (url != nullptr)
      {
        info.url = std::string(url);
      }

      m_onStreamInjectedStatus(info);
    }
  }

  void EventHandler::onUserJoined(uid_t uid, int elapsed)
  {
    if (m_onUserJoined != nullptr)
    {
      UserJoinInfo info;
      info.uid = uid;
      info.elapsed = elapsed;

      m_onUserJoined(info);
    }
  }

  void EventHandler::onActiveSpeaker(uid_t uid)
  {
    if (m_onActiveSpeaker != nullptr)
    {
      m_onActiveSpeaker(uid);
    }
  }

  void EventHandler::onRtmpStreamingStateChanged(const char* url, RTMP_STREAM_PUBLISH_STATE state, RTMP_STREAM_PUBLISH_ERROR errCode)
  {
    if (m_onRtmpStreamingStateChanged != nullptr)
    {
      StreamingStateChangedInfo info;
      info.state = state;
      info.errCode = errCode;
      if (url != nullptr)
      {
        info.url = std::string(url);
      }

      m_onRtmpStreamingStateChanged(info);
    }
  }

  void EventHandler::onRtmpStreamingEvent(const char* url, RTMP_STREAMING_EVENT eventCode)
  {
    if (m_onRtmpStreamingEvent != nullptr)
    {
      StreamingEventInfo info;
      info.eventCode = eventCode;
      if (url != nullptr)
      {
        info.url = std::string(url);
      }

      m_onRtmpStreamingEvent(info);
    }
  }

  void EventHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
  {
    if (m_onUserOffline != nullptr)
    {
      UserOfflineInfo info;
      info.uid = uid;
      info.reason = reason;

      m_onUserOffline(info);
    }
  }

  void EventHandler::onRemoteVideoStats(const RemoteVideoStats& stats)
  {
    if (m_onRemoteVideoStats != nullptr)
    {
      RemoteVideoStatsInfo info;
      info.uid = stats.uid;
      info.delay = stats.delay;
      info.width = stats.width;
      info.height = stats.height;
      info.receivedBitrate = stats.receivedBitrate;
      info.decoderOutputFrameRate = stats.decoderOutputFrameRate;
      info.rendererOutputFrameRate = stats.rendererOutputFrameRate;
      info.packetLossRate = stats.packetLossRate;
      info.totalFrozenTime = stats.totalFrozenTime;
      info.frozenRate = stats.frozenRate;
      info.totalActiveTime = stats.totalActiveTime;
      info.publishDuration = stats.publishDuration;

      m_onRemoteVideoStats(info);
    }
  }

  void EventHandler::onLocalVideoStats(const LocalVideoStats& stats)
  {
    if (m_onLocalVideoStats != nullptr)
    {
      LocalVideoStatsInfo info;
      info.sentBitrate = stats.sentBitrate;
      info.sentFrameRate = stats.sentFrameRate;
      info.encoderOutputFrameRate = stats.encoderOutputFrameRate;
      info.rendererOutputFrameRate = stats.rendererOutputFrameRate;
      info.targetBitrate = stats.targetBitrate;
      info.targetFrameRate = stats.targetFrameRate;
      info.qualityAdaptIndication = stats.qualityAdaptIndication;
      info.encodedBitrate = stats.encodedBitrate;
      info.encodedFrameWidth = stats.encodedFrameWidth;
      info.encodedFrameHeight = stats.encodedFrameHeight;
      info.encodedFrameCount = stats.encodedFrameCount;
      info.codecType = stats.codecType;
      info.txPacketLossRate = stats.txPacketLossRate;
      info.captureFrameRate = stats.captureFrameRate;

      m_onLocalVideoStats(info);
    }
  }

  void VideoFrameObserver::SetOnCaptureVideoFrameCallback(std::function<void(const VideoBuffer& _bufferInfo)> _callback)
  {
    m_OnCaptureLocalFrame = std::move(_callback);
  }

  void VideoFrameObserver::SetOnRenderVideoFrameCallback(std::function<void(const VideoBuffer& _bufferInfo, const unsigned int _clientID)> _callback)
  {
    m_OnCaptureRemoteFrame = std::move(_callback);
  }

  bool VideoFrameObserver::onCaptureVideoFrame(VideoFrame& _videoFrame)
  {
    // Check if the buffer is a valid one
    if (_videoFrame.yBuffer == nullptr ||
      _videoFrame.yStride <= 0 ||
      _videoFrame.width <= 0 ||
      _videoFrame.height <= 0)
    {
      return false;
    }

    // set the needed info for render
    VideoBuffer videoBuffer;
    videoBuffer.width = _videoFrame.width;
    videoBuffer.height = _videoFrame.height;
    videoBuffer.yStride = _videoFrame.yStride;
    videoBuffer.yBuffer = _videoFrame.yBuffer;

    // send the buffer to a better place :C
    if (m_OnCaptureLocalFrame)
    {
      m_OnCaptureLocalFrame(videoBuffer);
    }
    return true;
  }

  bool VideoFrameObserver::onRenderVideoFrame(unsigned int _clientID, VideoFrame& _videoFrame)
  {
    // Check if the buffer is a valid one
    if (_videoFrame.yBuffer == nullptr ||
      _videoFrame.yStride <= 0 ||
      _videoFrame.width <= 0 ||
      _videoFrame.height <= 0)
    {
      return false;
    }

    // set the needed info for render
    VideoBuffer videoBuffer;
    videoBuffer.width = _videoFrame.width;
    videoBuffer.height = _videoFrame.height;
    videoBuffer.yStride = _videoFrame.yStride;
    videoBuffer.yBuffer = _videoFrame.yBuffer;

    // send the buffer to a better place :C
    if (m_OnCaptureRemoteFrame)
    {
      m_OnCaptureRemoteFrame(videoBuffer, _clientID);
    }
    return true;
  }

  AgoraRTC::AgoraRTC()
  {

  }

  AgoraRTC::~AgoraRTC()
  {
#if (ENABLE_AGORA_RTC == 0)
    return;
#endif

    if (m_RtcEngine)
    {
      m_RtcEngine->leaveChannel();
    }

    if (m_AudioPlaybackCollection)
    {
      m_AudioPlaybackCollection->release();
    }

    if (m_AudioRecordingCollection)
    {
      m_AudioRecordingCollection->release();
    }

    if (m_VideoRecordingCollection)
    {
      m_VideoRecordingCollection->release();
    }

    if (m_AudioDeviceManager)
    {
      m_AudioDeviceManager->release();
    }

    if (m_VideoDeviceManager)
    {
      m_VideoDeviceManager->release();
    }

    if (m_MediaEngine)
    {
      m_MediaEngine->release();
    }

    if (m_RtcEngine)
    {
      m_RtcEngine->release();
    }
  }

  void AgoraRTC::UpdateAvailableDevices()
  {
#if (ENABLE_AGORA_RTC == 0)
    return;
#endif

    // create the video device collection
    m_VideoRecordingCollection = m_VideoDeviceManager->enumerateVideoDevices();
    if(m_VideoRecording.empty())
    {
      m_VideoRecording = std::string("Not Set");
      m_VideoRecording = GetDeviceID(GetCurrentDevice(eDEVICE_TYPE::kVideoRecording), eDEVICE_TYPE::kVideoRecording);
    }

    // create the audio recording device collection
    m_AudioRecordingCollection = m_AudioDeviceManager->enumerateRecordingDevices();
    if (m_AudioRecording.empty())
    {
      m_AudioRecording = std::string("Not Set");
      m_AudioRecording = GetDeviceID(GetCurrentDevice(eDEVICE_TYPE::kAudioRecording), eDEVICE_TYPE::kAudioRecording);
    }

    // create the audio playback device collection
    m_AudioPlaybackCollection = m_AudioDeviceManager->enumeratePlaybackDevices();
    if (m_AudioPlayback.empty())
    {
      m_AudioPlayback = std::string("Not Set");
      m_AudioPlayback = GetDeviceID(GetCurrentDevice(eDEVICE_TYPE::kAudioPlayback), eDEVICE_TYPE::kAudioPlayback);
    }

    // set a default screen
    if (m_ScreenDevice.deviceName.empty())
    {
      m_ScreenDevice.deviceName = std::string("Not Set");
      auto screens = GetDeviceList(eDEVICE_TYPE::kScreen);
      if (!screens.empty())
      {
        SetCurrentDeviceByID(screens.at(0), eDEVICE_TYPE::kScreen);
      }
    }

    // set a default window
    if (m_Window == nullptr)
    {
#if defined(_WIN32)
      m_Window = HWND(1);
      auto windows = GetDeviceList(eDEVICE_TYPE::kWindow);
      if (!windows.empty())
      {
        SetCurrentDeviceByID(windows.at(0), eDEVICE_TYPE::kWindow);
      }
#endif
    }
  }

  int AgoraRTC::StartUp(const std::string& _appID)
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // create the agora engine object 
    m_RtcEngine = createAgoraRtcEngine();
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }
    int result = 0;

    // create the agora engine context
    RtcEngineContext ctx;
    ctx.appId = _appID.c_str();
    ctx.eventHandler = &m_EventHandler;
    result = m_RtcEngine->initialize(ctx);
    if (result < 0)
    {
      return result;
    }

    // force to enable the video module and the audio module
    m_RtcEngine->enableVideo();
    m_RtcEngine->enableAudio();

    // create the media engine
    if (!m_MediaEngine.queryInterface(m_RtcEngine, agora::AGORA_IID_MEDIA_ENGINE))
    {
      return -1;
    }

    // register the video observer
    result = m_MediaEngine->registerVideoFrameObserver(&m_VideoObserver);
    if (result < 0)
    {
      return result;
    }

    // create the video device manager
    if (!m_VideoDeviceManager.queryInterface(m_RtcEngine, agora::AGORA_IID_VIDEO_DEVICE_MANAGER))
    {
      return -1;
    }

    // create the audio device engine
    if (!m_AudioDeviceManager.queryInterface(m_RtcEngine, agora::AGORA_IID_AUDIO_DEVICE_MANAGER))
    {
      return -1;
    }
    return 0;
  }

  std::string AgoraRTC::GetSDKVersion() const
  {
#if (ENABLE_AGORA_RTC == 0)
    return std::string();
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return "No version detected";
    }

    // get string with version info
    int build = 0;
    return std::string(m_RtcEngine->getVersion(&build));
  }

  bool AgoraRTC::GetStatus() const
  {
#if (ENABLE_AGORA_RTC == 0)
    return true;
#endif

    return m_RtcEngine != nullptr;
  }

  std::vector<std::string> AgoraRTC::GetDeviceList(const eDEVICE_TYPE _deviceType)
  {
#if (ENABLE_AGORA_RTC == 0)
    return std::vector<std::string>();
#endif

    std::vector<std::string> output;

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return output;
    }
    UpdateAvailableDevices();

    // avoid all if the device collection is not ready
    if ((_deviceType == eDEVICE_TYPE::kVideoRecording && m_VideoRecordingCollection == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioRecording && m_AudioRecordingCollection == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioPlayback && m_AudioPlaybackCollection == nullptr))
    {
      return output;
    }
    int result = 0;

    // Get the device list length detected by agora
    int lenght = 0;
    if (_deviceType == eDEVICE_TYPE::kVideoRecording)
    {
      lenght = m_VideoRecordingCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
    {
      lenght = m_AudioRecordingCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
    {
      lenght = m_AudioPlaybackCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kScreen)
    {
#if defined(_WIN32)
      while (lenght >= 0)
      {
        DISPLAY_DEVICEA device;
        device.cb = sizeof(device);
        if (EnumDisplayDevicesA(nullptr, lenght++, &device, 0))
        {
          if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
          {
            continue;
          }
          output.push_back(std::string(device.DeviceName));
        }
        else
        {
          lenght = -1;
        }
      }
#endif
    }
    else if (_deviceType == eDEVICE_TYPE::kWindow)
    {
#if defined(_WIN32)
      std::vector<HWND> windList;
      ::EnumWindows(&AgoraRTC::WndEnumProc, (LPARAM)&windList);
      for (size_t i = 0; i < windList.size(); i++)
      {
        char winName[255]{};
        ::GetWindowTextA(windList.at(i), winName, 255);
        output.push_back(std::string(winName));
      }
#endif
    }

    // Push the device list
    for (int i = 0; i < lenght; ++i)
    {
      char name[MAX_DEVICE_ID_LENGTH]{};
      char id[MAX_DEVICE_ID_LENGTH]{};
      result = -1;

      if (_deviceType == eDEVICE_TYPE::kVideoRecording)
      {
        result = m_VideoRecordingCollection->getDevice(i, name, id);
      }
      else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
      {
        result = m_AudioRecordingCollection->getDevice(i, name, id);
      }
      else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
      {
        result = m_AudioPlaybackCollection->getDevice(i, name, id);
      }

      if (result >= 0)
      {
        output.push_back(std::string(name));
      }
    }
    return output;
  }

  std::string AgoraRTC::GetDeviceID(const std::string& _deviceName, const eDEVICE_TYPE _deviceType)
  {
#if (ENABLE_AGORA_RTC == 0)
    return std::string();
#endif
    std::string output;

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return output;
    }
    UpdateAvailableDevices();

    // avoid all if the device collection is not ready
    if ((_deviceType == eDEVICE_TYPE::kVideoRecording && m_VideoRecordingCollection == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioRecording && m_AudioRecordingCollection == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioPlayback && m_AudioPlaybackCollection == nullptr))
    {
      return output;
    }
    int result = 0;

    // Get the device list length detected by agora
    int lenght = 0;
    if (_deviceType == eDEVICE_TYPE::kVideoRecording)
    {
      lenght = m_VideoRecordingCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
    {
      lenght = m_AudioRecordingCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
    {
      lenght = m_AudioPlaybackCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kScreen)
    {
#if defined(_WIN32)
      std::vector<std::string> screens = GetDeviceList(eDEVICE_TYPE::kScreen);
      for (size_t i = 0; i < screens.size(); i++)
      {
        if (_deviceName == screens.at(i))
        {
          return _deviceName;
        }
      }
#endif
    }
    else if (_deviceType == eDEVICE_TYPE::kWindow)
    {
#if defined(_WIN32)
      std::vector<HWND> windList;
      ::EnumWindows(&AgoraRTC::WndEnumProc, (LPARAM)&windList);
      for (size_t i = 0; i < windList.size(); i++)
      {
        char winName[255]{};
        ::GetWindowTextA(windList.at(i), winName, 255);
        if (_deviceName == std::string(winName))
        {
          return _deviceName;
        }
      }
#endif
    }

    // find the device in the list
    for (int i = 0; i < lenght; ++i)
    {
      char name[MAX_DEVICE_ID_LENGTH]{};
      char id[MAX_DEVICE_ID_LENGTH]{};
      result = -1;

      if (_deviceType == eDEVICE_TYPE::kVideoRecording)
      {
        result = m_VideoRecordingCollection->getDevice(i, name, id);
      }
      else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
      {
        result = m_AudioRecordingCollection->getDevice(i, name, id);
      }
      else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
      {
        result = m_AudioPlaybackCollection->getDevice(i, name, id);
      }

      if (result >= 0 &&
        _deviceName == std::string(name))
      {
        output = std::string(id);
      }
    }
    return output;
  }

  std::string AgoraRTC::GetDeviceName(const std::string& _deviceID, const eDEVICE_TYPE _deviceType)
  {
#if (ENABLE_AGORA_RTC == 0)
    return std::string();
#endif
    std::string output;

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return output;
    }
    UpdateAvailableDevices();

    // avoid all if the device collection is not ready
    if ((_deviceType == eDEVICE_TYPE::kVideoRecording && m_VideoRecordingCollection == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioRecording && m_AudioRecordingCollection == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioPlayback && m_AudioPlaybackCollection == nullptr))
    {
      return output;
    }
    int result = 0;

    // Get the device list length detected by agora
    int lenght = 0;
    if (_deviceType == eDEVICE_TYPE::kVideoRecording)
    {
      lenght = m_VideoRecordingCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
    {
      lenght = m_AudioRecordingCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
    {
      lenght = m_AudioPlaybackCollection->getCount();
    }
    else if (_deviceType == eDEVICE_TYPE::kScreen)
    {
#if defined(_WIN32)
      std::vector<std::string> screens = GetDeviceList(eDEVICE_TYPE::kScreen);
      for (size_t i = 0; i < screens.size(); i++)
      {
        if (_deviceID == screens.at(i))
        {
          return _deviceID;
        }
      }
#endif
    }
    else if (_deviceType == eDEVICE_TYPE::kWindow)
    {
#if defined(_WIN32)
      std::vector<HWND> windList;
      ::EnumWindows(&AgoraRTC::WndEnumProc, (LPARAM)&windList);
      for (size_t i = 0; i < windList.size(); i++)
      {
        char winName[255]{};
        ::GetWindowTextA(windList.at(i), winName, 255);
        if (_deviceID == std::string(winName))
        {
          return _deviceID;
        }
      }
#endif
    }

    // find the device in the list
    for (int i = 0; i < lenght; ++i)
    {
      char name[MAX_DEVICE_ID_LENGTH]{};
      char id[MAX_DEVICE_ID_LENGTH]{};
      result = -1;

      if (_deviceType == eDEVICE_TYPE::kVideoRecording)
      {
        result = m_VideoRecordingCollection->getDevice(i, name, id);
      }
      else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
      {
        result = m_AudioRecordingCollection->getDevice(i, name, id);
      }
      else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
      {
        result = m_AudioPlaybackCollection->getDevice(i, name, id);
      }

      if (result >= 0 &&
        _deviceID == std::string(id))
      {
        output = std::string(name);
      }
    }
    return output;
  }

  std::string AgoraRTC::GetCurrentDevice(const eDEVICE_TYPE _deviceType)
  {
#if (ENABLE_AGORA_RTC == 0)
    return std::string();
#endif
    std::string output;

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return output;
    }
    UpdateAvailableDevices();

    // avoid all if the device collection is not ready
    if ((_deviceType == eDEVICE_TYPE::kVideoRecording && m_VideoDeviceManager.get() == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioRecording && m_AudioDeviceManager.get() == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioPlayback && m_AudioDeviceManager.get() == nullptr))
    {
      return output;
    }
    int result = 0;

    // get the current device id
    char id[MAX_DEVICE_ID_LENGTH]{};
    if (_deviceType == eDEVICE_TYPE::kVideoRecording)
    {
      result = m_VideoDeviceManager->getDevice(id);
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
    {
      result = m_AudioDeviceManager->getRecordingDevice(id);
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
    {
      result = m_AudioDeviceManager->getPlaybackDevice(id);
    }
    else if (_deviceType == eDEVICE_TYPE::kScreen)
    {
      return m_ScreenDevice.deviceName;
    }
    else if (_deviceType == eDEVICE_TYPE::kWindow)
    {
#if defined(_WIN32)
      char winName[255]{};
      ::GetWindowTextA(m_Window, winName, 255);
      return std::string(winName);
#endif
    }

    if (result >= 0)
    {
      output = GetDeviceName(std::string(id), _deviceType);
    }
    return output;
  }

  float AgoraRTC::GetAudioRecordingVolume() const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0.0f;
#endif

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr || m_AudioDeviceManager.get() == nullptr)
    {
      return 0.0f;
    }

    int volume = 0;
    m_AudioDeviceManager->getRecordingDeviceVolume(&volume);
    return volume / 255.0f;
  }

  float AgoraRTC::GetAudioPlaybackVolume() const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0.0f;
#endif

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr || m_AudioDeviceManager.get() == nullptr)
    {
      return 0.0f;
    }
    int volume = 0;
    m_AudioDeviceManager->getPlaybackDeviceVolume(&volume);
    return volume / 255.0f;
  }

  int AgoraRTC::SetCurrentDeviceByID(const std::string& _deviceID, const eDEVICE_TYPE _deviceType)
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }
    UpdateAvailableDevices();

    // avoid all if the device collection is not ready
    if (_deviceID.empty() ||
      (_deviceType == eDEVICE_TYPE::kVideoRecording && m_VideoDeviceManager.get() == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioRecording && m_AudioDeviceManager.get() == nullptr) ||
      (_deviceType == eDEVICE_TYPE::kAudioPlayback && m_AudioDeviceManager.get() == nullptr))
    {
      return -1;
    }
    int result = 0;

    // get the current device id
    if (_deviceType == eDEVICE_TYPE::kVideoRecording)
    {
      result = m_VideoDeviceManager->setDevice(_deviceID.c_str());
      m_VideoRecording = _deviceID;
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
    {
      result = m_AudioDeviceManager->setRecordingDevice(_deviceID.c_str());
      m_AudioRecording = _deviceID;
    }
    else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
    {
      result = m_AudioDeviceManager->setPlaybackDevice(_deviceID.c_str());
      m_AudioPlayback = _deviceID;
    }
    else if (_deviceType == eDEVICE_TYPE::kScreen)
    {
#if defined(_WIN32)
      // find all available devices
      std::vector<ScreenDevice> screens;
      int index = 0;
      result = -1;
      while (index >= 0)
      {
        DISPLAY_DEVICEA device;
        device.cb = sizeof(device);
        if (EnumDisplayDevicesA(NULL, index++, &device, 0))
        {
          if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
          {
            continue;
          }
          DEVMODEA device_mode;
          device_mode.dmSize = sizeof(DEVMODE);
          device_mode.dmDriverExtra = 0;
          EnumDisplaySettingsExA(device.DeviceName, ENUM_CURRENT_SETTINGS, &device_mode, 0);

          ScreenDevice newScreen;
          newScreen.deviceName = std::string(device.DeviceName);
          newScreen.x = device_mode.dmPosition.x;
          newScreen.y = device_mode.dmPosition.y;
          newScreen.w = device_mode.dmPelsWidth;
          newScreen.h = device_mode.dmPelsHeight;
          screens.push_back(newScreen);
        }
        else
        {
          index = -1;
        }
      }

      // Set the new coordinates for each monitor
      for (size_t i = 0; i < screens.size(); i++)
      {
        ScreenDevice current = screens.at(i);

        if (current.x < 0)
        {
          int offset = abs(current.x);
          for (size_t j = 0; j < screens.size(); j++)
          {
            screens.at(j).x += offset;
          }
        }

        if (current.y < 0)
        {
          int offset = abs(current.y);
          for (size_t j = 0; j < screens.size(); j++)
          {
            screens.at(j).y += offset;
          }
        }
      }

      // find the asked screen
      for (size_t i = 0; i < screens.size(); i++)
      {
        if (_deviceID == screens.at(i).deviceName)
        {
          m_ScreenDevice = screens.at(i);
          agora::rtc::Rectangle local(m_ScreenDevice.x, m_ScreenDevice.y, m_ScreenDevice.w, m_ScreenDevice.h);
          result = m_RtcEngine->updateScreenCaptureRegion(local);
        }
      }
#endif
    }
    else if (_deviceType == eDEVICE_TYPE::kWindow)
    {
#if defined(_WIN32)
      std::vector<HWND> windList;
      ::EnumWindows(&AgoraRTC::WndEnumProc, (LPARAM)&windList);
      for (size_t i = 0; i < windList.size(); i++)
      {
        char winName[255]{};
        ::GetWindowTextA(windList.at(i), winName, 255);
        if (_deviceID == std::string(winName))
        {
          m_Window = windList.at(i);
          if (m_WindowCapture)
          {
            ScreenCaptureParameters captureConfig;
            captureConfig.dimensions.width = m_ScreenCaptureConfig.width;
            captureConfig.dimensions.height = m_ScreenCaptureConfig.height;
            captureConfig.bitrate = m_ScreenCaptureConfig.bitrate;
            captureConfig.captureMouseCursor = m_ScreenCaptureConfig.captureMouseCursor;
            captureConfig.frameRate = m_ScreenCaptureConfig.frameRate;

            m_RtcEngine->stopScreenCapture();
            result = m_RtcEngine->startScreenCaptureByWindowId(m_Window, agora::rtc::Rectangle(), captureConfig);
          }
        }
      }
#endif
    }
    return result;
  }

  int AgoraRTC::SetCurrentDeviceByName(const std::string& _deviceName, const eDEVICE_TYPE _deviceType)
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    return SetCurrentDeviceByID(GetDeviceID(_deviceName, _deviceType), _deviceType);
  }

  int AgoraRTC::SetAudioRecordingVolume(const float _volume) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr || m_AudioDeviceManager.get() == nullptr)
    {
      return -1;
    }

    // set the recording device volume
    float volume = (_volume > 1.0f ? 1.0f : (_volume < 0.0f ? 0.0f : _volume));
    return m_AudioDeviceManager->setRecordingDeviceVolume(int(volume * 255));
  }

  int AgoraRTC::SetAudioPlaybackVolume(const float _volume) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid all if the rtc engine are not ready
    if (m_RtcEngine == nullptr || m_AudioDeviceManager.get() == nullptr)
    {
      return -1;
    }

    // set the recording device volume
    float volume = (_volume > 1.0f ? 1.0f : (_volume < 0.0f ? 0.0f : _volume));
    return m_AudioDeviceManager->setPlaybackDeviceVolume(int(volume * 255));
  }

  int AgoraRTC::SetVideoEncoderConfiguration(const VideoEncoderConfig& _config) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    VideoEncoderConfiguration config;
    config.dimensions.width = _config.width;
    config.dimensions.height = _config.height;
    config.frameRate = static_cast<FRAME_RATE>(_config.frameRate);
    config.orientationMode = static_cast<ORIENTATION_MODE>(_config.orientationMode);
    config.bitrate = _config.bitrate;
    config.degradationPreference = static_cast<DEGRADATION_PREFERENCE>(_config.degradationPreference);
    config.minBitrate = _config.minBitrate;
    config.minFrameRate = _config.minBitrate;
    config.mirrorMode = static_cast<VIDEO_MIRROR_MODE_TYPE>(_config.mirrorMode);

    // set the config in engine
    return m_RtcEngine->setVideoEncoderConfiguration(config);
  }

  int AgoraRTC::SetScreenCaptureConfig(const ScreenCaptureConfig& _config) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    ScreenCaptureParameters captureConfig;
    captureConfig.dimensions.width = _config.width;
    captureConfig.dimensions.height = _config.height;
    captureConfig.bitrate = _config.bitrate;
    captureConfig.captureMouseCursor = _config.captureMouseCursor;
    captureConfig.frameRate = _config.frameRate;

    // set the config in engine
    return m_RtcEngine->updateScreenCaptureParameters(captureConfig);
  }

  int AgoraRTC::SetClientRole(const eCLIENT_ROLE _roleType) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // start/stop the preview local feed visualization
    return m_RtcEngine->setClientRole(static_cast<CLIENT_ROLE_TYPE>(_roleType));
  }

  int AgoraRTC::SetRemoteVideoQuality(const unsigned int _userID, const eREMOTE_VIDEO_QUALITY _qualityType) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // start/stop the preview local feed visualization
    return m_RtcEngine->setRemoteVideoStreamType(_userID, static_cast<REMOTE_VIDEO_STREAM_TYPE>(_qualityType));
  }

  int AgoraRTC::SetRemoteVideoPriority(const unsigned int _userID, const eREMOTE_VIDEO_PRIORITY _priorityType) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // start/stop the preview local feed visualization
    return m_RtcEngine->setRemoteUserPriority(_userID, static_cast<PRIORITY_TYPE>(_priorityType));
  }

  int AgoraRTC::SetAudioEffect(const eAUDIO_EFFECT _effect) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }
    int result = 0;

    // set audio profile
    if (_effect == eAUDIO_EFFECT::AUDIO_EFFECT_OFF)
    {
      result = m_RtcEngine->setAudioProfile(AUDIO_PROFILE_DEFAULT, AUDIO_SCENARIO_DEFAULT);
      if (result < 0)
      {
        return result;
      }
    }
    else
    {
#if (AGORA_SDK > 312)
      result = m_RtcEngine->setAudioProfile(AUDIO_PROFILE_MUSIC_HIGH_QUALITY_STEREO, AUDIO_SCENARIO_MEETING);
#else
      result = m_RtcEngine->setAudioProfile(AUDIO_PROFILE_MUSIC_HIGH_QUALITY_STEREO, AUDIO_SCENARIO_EDUCATION);
#endif
      if (result < 0)
      {
        return result;
      }
    }

    // set effect
#if (AGORA_SDK > 312)
    result = m_RtcEngine->setAudioEffectPreset(static_cast<AUDIO_EFFECT_PRESET>(_effect));
#endif
    if (result < 0)
    {
      return result;
    }
    return result;
  }

  int AgoraRTC::SetCameraCapturerConfiguration(const CameraCapturerConfig& _config) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    CameraCapturerConfiguration config;
    config.preference = static_cast<CAPTURER_OUTPUT_PREFERENCE>(_config.preference);

    // start/stop the preview local feed visualization
    return m_RtcEngine->setCameraCapturerConfiguration(config);
  }

  int AgoraRTC::SetDefaultRemoteVideoQuality(const eREMOTE_VIDEO_QUALITY _qualityType) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif
    // set the default remote stream video type
    return m_RtcEngine->setRemoteDefaultVideoStreamType(static_cast<REMOTE_VIDEO_STREAM_TYPE>(_qualityType));
  }

  int AgoraRTC::EnableRemoteVideoSuperResolution(const unsigned int _userID, const bool _switchFlag) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // start/stop the preview local feed visualization
#if (AGORA_SDK > 312)
    return m_RtcEngine->enableRemoteSuperResolution(_userID, _switchFlag);
#else
    return 0;
#endif
  }

  int AgoraRTC::EnableScreenShare(const bool _switchFlag, const ScreenCaptureConfig& _config)
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }
    int result = 0;

    // avoid start if a device is not selected
    if (m_ScreenDevice.deviceName.empty() ||
      (m_ScreenDevice.deviceName.find(std::string("Not Set")) != std::string::npos))
    {
      return -1;
    }

    // Sets the content hint for screen sharing.
    result = m_RtcEngine->setScreenCaptureContentHint(CONTENT_HINT_DETAILS);
    if (result < 0)
    {
      return result;
    }

#if defined(_WIN32)
    // get main screen settings
    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) - GetSystemMetrics(SM_XVIRTUALSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) - GetSystemMetrics(SM_YVIRTUALSCREEN);

    // Set the desktop share config
    agora::rtc::Rectangle world(rc.left, rc.top, rc.right, rc.bottom);
    agora::rtc::Rectangle local(m_ScreenDevice.x, m_ScreenDevice.y, m_ScreenDevice.w, m_ScreenDevice.h);

    // start/stop the feature
    if (_switchFlag)
    {
      m_WindowCapture = false;
      ScreenCaptureParameters captureConfig;
      captureConfig.dimensions.width = _config.width;
      captureConfig.dimensions.height = _config.height;
      captureConfig.bitrate = _config.bitrate;
      captureConfig.captureMouseCursor = _config.captureMouseCursor;
      captureConfig.frameRate = _config.frameRate;
      result = m_RtcEngine->startScreenCaptureByScreenRect(world, local, captureConfig);
    }
    else
    {
      m_WindowCapture = false;
      result = m_RtcEngine->stopScreenCapture();
      m_VideoDeviceManager->setDevice(m_VideoRecording.c_str());
    }
#elif defined(__APPLE__)
    // start/stop the feature
    if (_switchFlag)
    {
      m_WindowCapture = false;
      result = m_RtcEngine->startScreenCapture(0, 5, nullptr, 0);
    }
    else
    {
      m_WindowCapture = false;
      result = m_RtcEngine->stopScreenCapture();
      m_VideoDeviceManager->setDevice(m_VideoRecording.c_str());
    }
#endif
    return result;
  }

  int AgoraRTC::EnableWindowShare(const bool _switchFlag, const ScreenCaptureConfig& _config)
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }
    int result = 0;

    // avoid start if a device is not selected
    if (m_Window == nullptr)
    {
      return -1;
    }

    // Sets the content hint for screen sharing.
    result = m_RtcEngine->setScreenCaptureContentHint(CONTENT_HINT_DETAILS);
    if (result < 0)
    {
      return result;
    }

#if defined(_WIN32)
    // take the window rect
    RECT rect;
    ::GetWindowRect(m_Window, &rect);
    agora::rtc::Rectangle winRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

    // start/stop the feature
    if (_switchFlag)
    {
      m_WindowCapture = true;
      ScreenCaptureParameters captureConfig;
      captureConfig.dimensions.width = _config.width;
      captureConfig.dimensions.height = _config.height;
      captureConfig.bitrate = _config.bitrate;
      captureConfig.captureMouseCursor = _config.captureMouseCursor;
      captureConfig.frameRate = _config.frameRate;
      result = m_RtcEngine->startScreenCaptureByWindowId(m_Window, winRect, captureConfig);
    }
    else
    {
      m_WindowCapture = false;
      result = m_RtcEngine->stopScreenCapture();
      m_VideoDeviceManager->setDevice(m_VideoRecording.c_str());
    }
    if (result < 0)
    {
      return result;
    }
#endif
    m_ScreenCaptureConfig = _config;
    return result;
  }

  int AgoraRTC::MuteLocalAudioStream(const bool _switchFlag) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // assign the current device here
    if (_switchFlag)
    {
      m_AudioDeviceManager->setRecordingDevice(m_AudioRecording.c_str());
    }

    // start/stop the stream
    return m_RtcEngine->muteLocalAudioStream(_switchFlag);
  }

  int AgoraRTC::MuteLocalVideoStream(const bool _switchFlag) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // assign the current device here
    if (_switchFlag)
    {
      m_VideoDeviceManager->setDevice(m_VideoRecording.c_str());
    }

    // start/stop the preview local feed visualization
    return m_RtcEngine->muteLocalVideoStream(_switchFlag);
  }

  int AgoraRTC::MuteRemoteAudioStream(const unsigned int _userID, const bool _switchFlag) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // start/stop the stream
    return m_RtcEngine->muteRemoteAudioStream(_userID, _switchFlag);
  }

  int AgoraRTC::MuteRemoteVideoStream(const unsigned int _userID, const bool _switchFlag) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // start/stop the stream
    return m_RtcEngine->muteRemoteVideoStream(_userID, _switchFlag);
  }

  int AgoraRTC::EnableVideoRecordingTest(const bool _switchFlag) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }
    int result = 0;

    // assign the current device here
    if (_switchFlag)
    {
      m_VideoDeviceManager->setDevice(m_VideoRecording.c_str());
    }

    // start/stop the preview local feed visualization
    if (_switchFlag)
    {
      result = m_RtcEngine->startPreview();
    }
    else
    {
      result = m_RtcEngine->stopPreview();
    }
    return result;
  }

  int AgoraRTC::EnableAudioRecordingTest(const bool _switchFlag) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return false;
    }
    int result = 0;

    // assign the current device here
    if (_switchFlag)
    {
      m_AudioDeviceManager->setRecordingDevice(m_AudioRecording.c_str());
      m_AudioDeviceManager->setPlaybackDevice(m_AudioPlayback.c_str());
    }

    // start/stop the preview local feed visualization
    if (_switchFlag)
    {
      result = m_RtcEngine->startEchoTest(cEchoTestInterval);
    }
    else
    {
      result = m_RtcEngine->stopEchoTest();
    }
    return result;
  }

  int AgoraRTC::JoinChannel(const ChannelParams& _channelParams) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }
    int result = 0;

    // set the encryption mode
    result = m_RtcEngine->setEncryptionMode(_channelParams.encryptionType.empty() ? cEncryptionType.c_str() : _channelParams.encryptionType.c_str());
    if (result < 0)
    {
      return result;
    }

    // set the encryption key
    result = m_RtcEngine->setEncryptionSecret(_channelParams.encryptionKey.c_str());
    if (result < 0)
    {
      return result;
    }

    // turn on/off the volume indicator
    result = m_RtcEngine->enableAudioVolumeIndication(cTestInterval, cSmothInterval, false);
    if (result < 0)
    {
      return result;
    }

    // enable the dual stream mode
    result = m_RtcEngine->enableDualStreamMode(true);
    if (result < 0)
    {
      return result;
    }

    // set the channel profile (CHANNEL_PROFILE_LIVE_BROADCASTING by default to allow screen share in a normal call)
    result = m_RtcEngine->setChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
    if (result < 0)
    {
      return result;
    }

    // set the channel client role
    result = m_RtcEngine->setClientRole(static_cast<CLIENT_ROLE_TYPE>(_channelParams.clientRole));
    if (result < 0)
    {
      return result;
    }

    // join to the assigned channel
    std::string info("{\"owner\":true,\"width\":640,\"height\":480,\"bitrate\":500}");
    result = m_RtcEngine->joinChannel(nullptr, _channelParams.channelName.c_str(), info.c_str(), _channelParams.clientID);
    if (result < 0)
    {
      return result;
    }
    return result;
  }

  int AgoraRTC::LeaveChannel() const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // Send disconnect request
    return m_RtcEngine->leaveChannel();
  }

  int AgoraRTC::PublishStreamUrl(const std::string& _url, const StreamConfig& _config) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    InjectStreamConfig config;
    config.audioBitrate = _config.audioBitrate;
    config.audioChannels = _config.audioChannels;
    config.audioSampleRate = static_cast<AUDIO_SAMPLE_RATE_TYPE>(_config.audioSampleRate);
    config.height = _config.height;
    config.videoBitrate = _config.videoBitrate;
    config.videoFramerate = _config.videoFramerate;
    config.videoGop = _config.videoGop;
    config.width = _config.width;

    // Push stream
    return m_RtcEngine->addInjectStreamUrl(_url.c_str(), config);
  }

  int AgoraRTC::RemoveStreamUrl(const std::string& _url) const
  {
#if (ENABLE_AGORA_RTC == 0)
    return 0;
#endif

    // avoid bad calls if the engine are not ready
    if (m_RtcEngine == nullptr)
    {
      return -1;
    }

    // pop stream
    return m_RtcEngine->removeInjectStreamUrl(_url.c_str());
  }

#if defined(_WIN32)
  BOOL CALLBACK AgoraRTC::WndEnumProc(HWND hWnd, LPARAM lParam)
  {
    LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
    BOOL isCloaked = FALSE;
    if (!((lStyle & WS_EX_APPWINDOW) == WS_EX_APPWINDOW) ||
      !::IsWindowVisible(hWnd) ||
      ::IsIconic(hWnd) ||
      (SUCCEEDED(DwmGetWindowAttribute(hWnd, DWMWA_CLOAKED, &isCloaked, sizeof(isCloaked))) && isCloaked) ||
      !::GetWindowTextLengthA(hWnd))
    {
      return TRUE;
    }
    std::vector<HWND>& winList = *reinterpret_cast<std::vector<HWND>*>(lParam);
    winList.push_back(hWnd);
    return TRUE;
  };
#endif

  extern AgoraRTC& GetAgoraRTC(const std::string& _appID)
  {
    static AgoraRTC* pAgoraManager = nullptr;

    if (pAgoraManager == nullptr)
    {
      pAgoraManager = new AgoraRTC();
    }
    if (!pAgoraManager->GetStatus())
    {
      pAgoraManager->StartUp(_appID);
    }
    return *pAgoraManager;
  }
}
