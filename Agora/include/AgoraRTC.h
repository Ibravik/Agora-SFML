
#pragma once
#define ENABLE_AGORA_RTC 1
#define AGORA_SDK 312

#include <string>
#include <functional>
#include <map>
#include <vector>

#if defined(UE_BUILD_DEBUG) || defined(UE_BUILD_DEVELOPMENT) || defined(UE_BUILD_TEST) || defined(UE_BUILD_SHIPPING) || defined(UE_GAME) || defined(UE_EDITOR)
#include <Agora/IAgoraRtcEngine.h>
#include <Agora/IAgoraMediaEngine.h>
#include <Agora/IAgoraRtcChannel.h>
#else
#include <IAgoraRtcEngine.h>
#include <IAgoraMediaEngine.h>
#include <IAgoraRtcChannel.h>
#define AGORAYCE_API
#endif

/**
 * @brief   The follow struct only show an interface for the event message info that the SDK
 *          send.
 * @Note    I did not add comments since all the info are in the agora SDK code :).
 */
namespace agoraYCE
{
  using namespace agora;
  using namespace agora::rtc;
  using namespace agora::media;

  struct WarnInfo
  {
    int warn;
    std::string msg;

    WarnInfo() :
      warn(0) {}
  };

  struct ErrorInfo
  {
    int err;
    std::string msg;

    ErrorInfo() :
      err(0) {}
  };

  struct JoinChannelInfo
  {
    std::string channel;
    unsigned int uid;
    int elapsed;

    JoinChannelInfo() :
      uid(0),
      elapsed(0) {}
  };

  struct CallStatsInfo
  {
    unsigned int duration;
    unsigned int txBytes;
    unsigned int rxBytes;
    unsigned int txAudioBytes;
    unsigned int txVideoBytes;
    unsigned int rxAudioBytes;
    unsigned int rxVideoBytes;
    unsigned short txKBitRate;
    unsigned short rxKBitRate;
    unsigned short rxAudioKBitRate;
    unsigned short txAudioKBitRate;
    unsigned short rxVideoKBitRate;
    unsigned short txVideoKBitRate;
    unsigned short lastmileDelay;
    unsigned short txPacketLossRate;
    unsigned short rxPacketLossRate;
    unsigned int userCount;
    double cpuAppUsage;
    double cpuTotalUsage;
    int gatewayRtt;
    double memoryAppUsageRatio;
    double memoryTotalUsageRatio;
    int memoryAppUsageInKbytes;
    CallStatsInfo()
      : duration(0)
      , txBytes(0)
      , rxBytes(0)
      , txAudioBytes(0)
      , txVideoBytes(0)
      , rxAudioBytes(0)
      , rxVideoBytes(0)
      , txKBitRate(0)
      , rxKBitRate(0)
      , rxAudioKBitRate(0)
      , txAudioKBitRate(0)
      , rxVideoKBitRate(0)
      , txVideoKBitRate(0)
      , lastmileDelay(0)
      , txPacketLossRate(0)
      , rxPacketLossRate(0)
      , userCount(0)
      , cpuAppUsage(0)
      , cpuTotalUsage(0)
      , gatewayRtt(0)
      , memoryAppUsageRatio(0)
      , memoryTotalUsageRatio(0)
      , memoryAppUsageInKbytes(0) {}
  };

  struct RoleChangedInfo
  {
    int oldRole;
    int newRole;

    RoleChangedInfo() :
      oldRole(0),
      newRole(0) {}
  };

  struct StreamErrorInfo
  {
    unsigned int uid;
    int streamId;
    int code;
    int missed;
    int cached;

    StreamErrorInfo() :
      uid(0),
      streamId(0),
      code(0),
      missed(0),
      cached(0) {}
  };

  struct StreamStatusInfo
  {
    std::string url;
    unsigned int uid;
    int status;

    StreamStatusInfo() :
      uid(0),
      status(0) {}
  };

  struct UserJoinInfo
  {
    unsigned int uid;
    int elapsed;

    UserJoinInfo() :
      uid(0),
      elapsed(0) {}
  };

  struct StreamingStateChangedInfo
  {
    std::string url;
    int state;
    int errCode;

    StreamingStateChangedInfo() :
      state(0),
      errCode(0) {}
  };

  struct StreamingEventInfo
  {
    std::string url;
    int eventCode;

    StreamingEventInfo() :
      eventCode(0) {}
  };

  struct StreamPublishedInfo
  {
    std::string url;
    int error;

    StreamPublishedInfo() :
      error(0) {}
  };

  struct UserOfflineInfo
  {
    unsigned int uid;
    int reason;

    UserOfflineInfo() :
      uid(0),
      reason(0) {}
  };

  struct RemoteVideoStatsInfo
  {
    unsigned int uid;
    int delay;
    int width;
    int height;
    int receivedBitrate;
    int decoderOutputFrameRate;
    int rendererOutputFrameRate;
    int packetLossRate;
    int rxStreamType;
    int totalFrozenTime;
    int frozenRate;
    int totalActiveTime;
    int publishDuration;

    RemoteVideoStatsInfo() :
      uid(0),
      delay(0),
      width(0),
      height(0),
      receivedBitrate(0),
      decoderOutputFrameRate(0),
      rendererOutputFrameRate(0),
      packetLossRate(0),
      rxStreamType(0),
      totalFrozenTime(0),
      frozenRate(0),
      totalActiveTime(0),
      publishDuration(0) {}
  };

  struct LocalVideoStatsInfo
  {
    int sentBitrate;
    int sentFrameRate;
    int encoderOutputFrameRate;
    int rendererOutputFrameRate;
    int targetBitrate;
    int targetFrameRate;
    int qualityAdaptIndication;
    int encodedBitrate;
    int encodedFrameWidth;
    int encodedFrameHeight;
    int encodedFrameCount;
    int codecType;
    unsigned short txPacketLossRate;
    int captureFrameRate;

    LocalVideoStatsInfo() :
      sentBitrate(0),
      sentFrameRate(0),
      encoderOutputFrameRate(0),
      rendererOutputFrameRate(0),
      targetBitrate(0),
      targetFrameRate(0),
      qualityAdaptIndication(0),
      encodedBitrate(0),
      encodedFrameWidth(0),
      encodedFrameHeight(0),
      encodedFrameCount(0),
      codecType(0),
      txPacketLossRate(0),
      captureFrameRate(0) {}
  };

  /**
   * @brief   The AgoraRTC uses the EventHandler interface class to send callbacks to the
   *          application.
   * @Note    You can find the description for all override functions in the parent class. Also,
   *          not all the available events are exposed here, fell free to add another one in
   *          necessary cases.
   */
  class AGORAYCE_API EventHandler : public IRtcEngineEventHandler
  {
    /**
     * Class variables---------------------------------------------------------------------------
     */
  private:
    std::function<void(const WarnInfo& _info)> m_onWarning = nullptr;
    std::function<void(const ErrorInfo& _info)> m_onError = nullptr;
    std::function<void(const JoinChannelInfo& _info)> m_onJoinChannelSuccess = nullptr;
    std::function<void(const CallStatsInfo& _info)> m_onLeaveChannel = nullptr;
    std::function<void(const RoleChangedInfo& _info)> m_onClientRoleChanged = nullptr;
    std::function<void()> m_onConnectionLost = nullptr;
    std::function<void(const CallStatsInfo& _info)> m_onRtcStats = nullptr;
    std::function<void(const StreamErrorInfo& _info)> m_onStreamMessageError = nullptr;
    std::function<void(const StreamStatusInfo& _info)> m_onStreamInjectedStatus = nullptr;
    std::function<void(const UserJoinInfo& _info)> m_onUserJoined = nullptr;
    std::function<void(const unsigned int _info)> m_onActiveSpeaker = nullptr;
    std::function<void(const StreamingStateChangedInfo& _info)> m_onRtmpStreamingStateChanged = nullptr;
    std::function<void(const StreamingEventInfo& _info)> m_onRtmpStreamingEvent = nullptr;
    std::function<void(const UserOfflineInfo& _info)> m_onUserOffline = nullptr;
    std::function<void(const RemoteVideoStatsInfo& _info)> m_onRemoteVideoStats = nullptr;
    std::function<void(const LocalVideoStatsInfo& _info)> m_onLocalVideoStats = nullptr;

    /**
     * Class set functions-----------------------------------------------------------------------
     */
  public:
    void SetOnWarning(std::function<void(const WarnInfo& _info)> _callback);
    void SetOnError(std::function<void(const ErrorInfo& _info)> _callback);
    void SetOnJoinChannelSuccess(std::function<void(const JoinChannelInfo& _info)> _callback);
    void SetOnLeaveChannel(std::function<void(const CallStatsInfo& _info)> _callback);
    void SetOnClientRoleChanged(std::function<void(const RoleChangedInfo& _info)> _callback);
    void SetOnConnectionLost(std::function<void()> _callback);
    void SetOnRtcStats(std::function<void(const CallStatsInfo& _info)> _callback);
    void SetOnStreamMessageError(std::function<void(const StreamErrorInfo& _info)> _callback);
    void SetOnStreamInjectedStatus(std::function<void(const StreamStatusInfo& _info)> _callback);
    void SetOnUserJoined(std::function<void(const UserJoinInfo& _info)> _callback);
    void SetOnActiveSpeaker(std::function<void(const unsigned int uid)> _callback);
    void SetOnRtmpStreamingStateChanged(std::function<void(const StreamingStateChangedInfo& _info)> _callback);
    void SetOnRtmpStreamingEvent(std::function<void(const StreamingEventInfo& _info)> _callback);
    void SetOnUserOffline(std::function<void(const UserOfflineInfo& _info)> _callback);
    void SetOnRemoteVideoStats(std::function<void(const RemoteVideoStatsInfo& _info)> _callback);
    void SetOnLocalVideoStats(std::function<void(const LocalVideoStatsInfo& _info)> _callback);

    /**
     * Class functions override------------------------------------------------------------------
     */
  private:
    void onWarning(int warn, const char* msg) override;
    void onError(int err, const char* msg) override;
    void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed) override;
    void onLeaveChannel(const RtcStats& stats) override;
    void onClientRoleChanged(CLIENT_ROLE_TYPE oldRole, CLIENT_ROLE_TYPE newRole) override;
    void onConnectionLost() override;
    void onRtcStats(const RtcStats& stats) override;
    void onStreamMessageError(uid_t uid, int streamId, int code, int missed, int cached) override;
    void onStreamInjectedStatus(const char* url, uid_t uid, int status) override;
    void onUserJoined(uid_t uid, int elapsed) override;
    void onActiveSpeaker(uid_t uid) override;
    void onRtmpStreamingStateChanged(const char* url, RTMP_STREAM_PUBLISH_STATE state, RTMP_STREAM_PUBLISH_ERROR errCode) override;
    void onRtmpStreamingEvent(const char* url, RTMP_STREAMING_EVENT eventCode) override;
    void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) override;
    void onRemoteVideoStats(const RemoteVideoStats& stats) override;
    void onLocalVideoStats(const LocalVideoStats& stats) override;
  };

  /**
   * @brief   The follow struct only show an interface for the info and settings send by agora
   *          every frame for the frame capture.
   * @Note    I did not add comments since all the info are in the agora SDK code.
   */
  struct VideoBuffer
  {
    int width;
    int height;
    int yStride;
    void* yBuffer;

    VideoBuffer() :
      width(0),
      height(0),
      yStride(0),
      yBuffer(nullptr) {}
  };

  /**
   * @brief   The porpoise of the following class is to allow to take the agora video local and
   *          remote buffers, putting in a unreal texture to use it.
   * @Note    You can find the description for all override functions in the parent class. Also,
   *          not all the available events are exposed here, fell free to add another one in
   *          necessary cases.
   */
  class AGORAYCE_API VideoFrameObserver : public IVideoFrameObserver
  {
public:
    virtual ~VideoFrameObserver() = default;

    /**
     * Class variables---------------------------------------------------------------------------
     */
  private:
    std::function<void(const VideoBuffer& _bufferInfo)> m_OnCaptureLocalFrame = nullptr;
    std::function<void(const VideoBuffer& _bufferInfo, const unsigned int _clientID)> m_OnCaptureRemoteFrame = nullptr;

    /**
     * Class set functions-----------------------------------------------------------------------
     */
  public:
    void SetOnCaptureVideoFrameCallback(std::function<void(const VideoBuffer& _bufferInfo)> _callback);
    void SetOnRenderVideoFrameCallback(std::function<void(const VideoBuffer& _bufferInfo, const unsigned int _clientID)> _callback);

    /**
     * Class functions override------------------------------------------------------------------
     */
  private:
    bool onCaptureVideoFrame(VideoFrame& _videoFrame) override;
    bool onRenderVideoFrame(unsigned int _clientID, VideoFrame& _videoFrame) override;
    VIDEO_FRAME_TYPE getVideoFormatPreference() override { return FRAME_TYPE_RGBA; }
  };

  /**
   * @brief   The different hardware devices that agora recognize.
   */
  enum class eDEVICE_TYPE
  {
    kVideoRecording = 1,
    kAudioRecording,
    kAudioPlayback,
    kScreen,
    kWindow,
  };

  /**
   * @brief   The roles that the client can have in the channel
   */
  enum class eCLIENT_ROLE
  {
    kBROADCASTER = 1,
    kAUDIENCE = 2
  };

  /**
   * @brief   Remote video stream types.
   */
  enum class eREMOTE_VIDEO_QUALITY
  {
    kHIGH = 0,
    kLOW = 1,
  };

  /**
   * @brief   The priority of the remote user.
   */
  enum class eREMOTE_VIDEO_PRIORITY
  {
    kHIGH = 50,
    kNORMAL = 100,
  };

  /**
   * @brief   The options for SDK preset audio effects.
   */
  enum class eAUDIO_EFFECT
  {
    AUDIO_EFFECT_OFF = 0x00000000,
    ROOM_ACOUSTICS_KTV = 0x02010100,
    ROOM_ACOUSTICS_VOCAL_CONCERT = 0x02010200,
    ROOM_ACOUSTICS_STUDIO = 0x02010300,
    ROOM_ACOUSTICS_PHONOGRAPH = 0x02010400,
    ROOM_ACOUSTICS_VIRTUAL_STEREO = 0x02010500,
    ROOM_ACOUSTICS_SPACIAL = 0x02010600,
    ROOM_ACOUSTICS_ETHEREAL = 0x02010700,
    ROOM_ACOUSTICS_3D_VOICE = 0x02010800,
    VOICE_CHANGER_EFFECT_UNCLE = 0x02020100,
    VOICE_CHANGER_EFFECT_OLDMAN = 0x02020200,
    VOICE_CHANGER_EFFECT_BOY = 0x02020300,
    VOICE_CHANGER_EFFECT_SISTER = 0x02020400,
    VOICE_CHANGER_EFFECT_GIRL = 0x02020500,
    VOICE_CHANGER_EFFECT_PIGKING = 0x02020600,
    VOICE_CHANGER_EFFECT_HULK = 0x02020700,
    STYLE_TRANSFORMATION_RNB = 0x02030100,
    STYLE_TRANSFORMATION_POPULAR = 0x02030200,
    PITCH_CORRECTION = 0x02040100
  };

  /**
   * @brief   The params needed to start or join to a stream channel.
   */
  struct ChannelParams
  {
    std::string channelName;
    std::string encryptionKey;
    std::string encryptionType;
    eCLIENT_ROLE clientRole;
    unsigned int clientID;

    ChannelParams() :
      clientRole(eCLIENT_ROLE::kBROADCASTER),
      clientID(0) {}
  };

  /**
   * @brief   The video encoder config for the used in channel connection.
   */
  struct VideoEncoderConfig
  {
    /**
     * @brief   The following table lists the recommended video encoder configurations, where the
     *          base bitrate applies to the `COMMUNICATION` profile. Set your bitrate based on
     *          this table. If you set a bitrate beyond the proper range, the SDK automatically
     *          sets it to within the range.
     */

     /*
     | Resolution  | Frame Rate (fps) | Live Bitrate (Kbps) |
     |-------------|------------------|---------------------|
     | 160 * 120   | 15               | 130                 |
     | 120 * 120   | 15               | 100                 |
     | 320 * 180   | 15               | 280                 |
     | 180 * 180   | 15               | 200                 |
     | 240 * 180   | 15               | 240                 |
     | 320 * 240   | 15               | 400                 |
     | 240 * 240   | 15               | 280                 |
     | 424 * 240   | 15               | 440                 |
     | 640 * 360   | 15               | 800                 |
     | 360 * 360   | 15               | 520                 |
     | 640 * 360   | 30               | 1200                |
     | 360 * 360   | 30               | 800                 |
     | 480 * 360   | 15               | 640                 |
     | 480 * 360   | 30               | 980                 |
     | 640 * 480   | 15               | 1000                |
     | 480 * 480   | 15               | 800                 |
     | 640 * 480   | 30               | 1500                |
     | 480 * 480   | 30               | 1200                |
     | 848 * 480   | 15               | 1220                |
     | 848 * 480   | 30               | 1860                |
     | 640 * 480   | 10               | 800                 |
     | 1280 * 720  | 15               | 2260                |
     | 1280 * 720  | 30               | 3420                |
     | 960 * 720   | 15               | 1820                |
     | 960 * 720   | 30               | 2760                |
     | 1920 * 1080 | 15               | 4160                |
     | 1920 * 1080 | 30               | 6300                |
     | 1920 * 1080 | 60               | 6500                |
     | 2560 * 1440 | 30               | 6500                |
     | 2560 * 1440 | 60               | 6500                |
     | 3840 * 2160 | 30               | 6500                |
     | 3840 * 2160 | 60               | 6500                |
     */
    int width;
    int height;
    int frameRate;
    int minFrameRate;
    int bitrate;
    int minBitrate;
    int orientationMode;
    int degradationPreference;
    int mirrorMode;

    VideoEncoderConfig()
      : width(640)
      , height(480)
      , frameRate(30)
      , minFrameRate(-1)
      , bitrate(0)
      , minBitrate(-1)
      , orientationMode(0)
      , degradationPreference(0)
      , mirrorMode(0)
    {}
  };

  /**
   * @brief   The stream config to inject it in the channel.
   */
  struct StreamConfig
  {
    int width;
    int height;
    int videoGop;
    int videoFramerate;
    int videoBitrate;
    int audioSampleRate;
    int audioBitrate;
    int audioChannels;

    StreamConfig()
      : width(0)
      , height(0)
      , videoGop(30)
      , videoFramerate(15)
      , videoBitrate(400)
      , audioSampleRate(48000)
      , audioBitrate(48)
      , audioChannels(1)
    {}
  };

  /**
   * @brief   Represent the current screen to share settings.
   */
  struct ScreenDevice
  {
    std::string deviceName;
    int x;
    int y;
    int w;
    int h;
  };

  /**
   * @brief   Camera capture configuration.
   */
  struct CameraCapturerConfig
  {
    int preference;
  };

  /**
   * @brief   Screen capture configuration.
   */
  struct ScreenCaptureConfig
  {
    int width;
    int height;
    int frameRate;
    int bitrate;
    bool captureMouseCursor;

    ScreenCaptureConfig() :
      width(1920),
      height(1080),
      frameRate(5),
      bitrate(0),
      captureMouseCursor(true) {}
  };

  /**
   * @brief   This class handle the communication with Agora SDK. Also allows to disconnect the
   *          SDK without any consequences.
   */
  class AGORAYCE_API AgoraRTC
  {
  public:
    AgoraRTC();
    ~AgoraRTC();

    /**
     * Class variables---------------------------------------------------------------------------
     */
  private:
    /**
     * @brief   Default encryption mode used if none one is stetted.
     */
    const std::string cEncryptionType = "aes-128-xts";

    /**
     * @brief   Interval in seconds between when you speak and when the recording plays back
     *          for audio testing.
     */
    const int cEchoTestInterval = 2;

    /**
     * @brief   Interval report for the loudest speaker in the call.
     */
    const int cTestInterval = 250;

    /**
     * @brief   Sets the sensitivity of the audio volume indicator. The value ranges between 0
                and 10. The greater the value, the more sensitive the indicator.
     */
    const int cSmothInterval = 2;

  public:
    /**
     * @brief   Pointer to the MediaEngine agora obj.
     */
    EventHandler m_EventHandler;

    /**
     * @brief   Current video observer. This object set the functionality to use the buffers
     *          send by agora(local/remote).
     */
    VideoFrameObserver m_VideoObserver;

  private:
    /**
     * @brief   Pointer to the IRtcEngine agora obj.
     */
    IRtcEngine* m_RtcEngine = nullptr;

    /**
     * @brief   Pointer to the MediaEngine agora obj.
     */
    agora::util::AutoPtr<IMediaEngine> m_MediaEngine;

    /**
     * @brief   Pointer to the Video device manager.
     */
    agora::util::AutoPtr<IVideoDeviceManager> m_VideoDeviceManager;

    /**
     * @brief   Pointer to the Audio device manager.
     */
    agora::util::AutoPtr<IAudioDeviceManager> m_AudioDeviceManager;

    /**
     * @brief   Pointer to the Video device collection.
     */
    IVideoDeviceCollection* m_VideoRecordingCollection = nullptr;

    /**
     * @brief   Pointer to the Audio device recording collection.
     */
    IAudioDeviceCollection* m_AudioRecordingCollection = nullptr;

    /**
     * @brief   Pointer to the Audio device playback collection.
     */
    IAudioDeviceCollection* m_AudioPlaybackCollection = nullptr;

    /**
     * @brief   The current screen device assigned for desktop share.
     */
    ScreenDevice m_ScreenDevice;

    /**
     * @brief   The current window assigned for window share
     */
#if defined(_WIN32)
    HWND m_Window = nullptr;
#else
    void* m_Window = nullptr;
#endif

    /**
     * @brief   Current Screen assigned for capture share.
     */
    ScreenCaptureConfig m_ScreenCaptureConfig;

    /**
     * @brief   Current video recording device assigned.
     */
    std::string m_VideoRecording;

    /**
     * @brief   Current audio recording device assigned.
     */
    std::string m_AudioRecording;

    /**
     * @brief   Current audio playback device assigned.
     */
    std::string m_AudioPlayback;

    /**
     * @brief   Current status of window share.
     */
    bool m_WindowCapture = false;

    /**
     * Class functions---------------------------------------------------------------------------
     */
  private:
    /**
     * @brief   Update the available devices to use by agora.
     * @bug     No know Bugs.
     */
    void UpdateAvailableDevices();

  public:
    /**
     * @brief   Initialize the agora engine and all the needed components too.
     * @bug     No know Bugs.
     * @return  #int: Return the code of the startup status.
     */
    int StartUp(const std::string& _appID);

    /**
     * @brief   Return the current used sdk ver.
     * @bug     No know Bugs.
     * @return  #std::string: The current version of the sdk.
     */
    std::string GetSDKVersion() const;

    /**
     * @brief   Return true if the agora system is ready to rumble.
     * @bug     No know Bugs.
     * @return  #bool: true if the m_RtcEngine are ready.
     */
    bool GetStatus() const;

    /**
     * @brief   Create an array with the available devices according to the device type param
     * @param   #eDEVICE_TYPE: the desire device type to use in the search.
     * @bug     No know Bugs.
     * @return  #std::vector<std::string>: The device name list.
     */
    std::vector<std::string> GetDeviceList(const eDEVICE_TYPE _deviceType);

    /**
     * @brief   Return the device ID from the device name according to the device type param.
     * @param   #FString: The device name.
     * @param   #eDEVICE_TYPE: the desire device type to use in the search.
     * @bug     No know Bugs.
     * @return  #std::string: The device ID.
     */
    std::string GetDeviceID(const std::string& _deviceName, const eDEVICE_TYPE _deviceType);

    /**
     * @brief   Return the device NAME from the device id according to the device type param.
     * @param   #FString: The device ID.
     * @param   #eDEVICE_TYPE: the desire device type to use in the search.
     * @bug     No know Bugs.
     * @return  #std::string: The device NAME.
     */
    std::string GetDeviceName(const std::string& _deviceID, const eDEVICE_TYPE _deviceType);

    /**
     * @brief   Return the current used device name.
     * @param   #eDEVICE_TYPE: the desire device type to use in the search.
     * @bug     No know Bugs.
     * @return  #std::string: The current setted device Name.
     */
    std::string GetCurrentDevice(const eDEVICE_TYPE _deviceType);

    /**
     * @brief   Return the current audio recording volume.
     * @bug     No know Bugs.
     * @return  #float: The audio recording  device Volume(0.0f - 1.0f).
     */
    float GetAudioRecordingVolume() const;

    /**
     * @brief   Return the current audio playback volume.
     * @bug     No know Bugs.
     * @return  #float: The audio Playback device Volume(0.0 - 1.0).
     */
    float GetAudioPlaybackVolume() const;

    /**
     * @brief   Set a different device to be used by agora by id.
     * @param   #FString: The device ID.
     * @param   #eDEVICE_TYPE: the desire device type to use in the search.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetCurrentDeviceByID(const std::string& _deviceID, const eDEVICE_TYPE _deviceType);

    /**
     * @brief   Set a different device to be used by agora by name.
     * @param   #FString: The device NAME.
     * @param   #eDEVICE_TYPE: the desire device type to use in the search.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetCurrentDeviceByName(const std::string& _deviceName, const eDEVICE_TYPE _deviceType);

    /**
     * @brief   Set the current audio recording value.
     * @param   #float: The current audio recording volume(0.0 - 1.0).
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetAudioRecordingVolume(const float _volume) const;

    /**
     * @brief   Set the current audio Playback value.
     * @param   #float: The current audio Playback volume(0.0 - 1.0).
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetAudioPlaybackVolume(const float _volume) const;

    /**
     * @brief   The parameters specified in this method are the maximum values under ideal
     *          network conditions. If the video engine cannot render the video using the
     *          specified parameters due to poor network conditions, the parameters further
     *          down the list are considered until a successful configuration is found.
     * @param   #VideoEncoderConfig: Video encoding settings
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetVideoEncoderConfiguration(const VideoEncoderConfig& _config) const;

    /**
     * @brief   Updates the screen sharing parameters.
     * @param   #ScreenCaptureConfig: Video encoding settings
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetScreenCaptureConfig(const ScreenCaptureConfig& _config) const;

    /**
     * @brief   Sets the role of the user, such as a host or an audience (default), before/
     *          after joining a channel in a live broadcast.
     * @param   #eCLIENT_ROLE: New client role.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetClientRole(const eCLIENT_ROLE _roleType) const;

    /**
     * @brief   Change the remote video stream quality(low/high).
     * @param   #unsigned int: The user ID.
     * @param   #eREMOTE_VIDEO_QUALITY: The video quality for the remote user.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetRemoteVideoQuality(const unsigned int _userID, const eREMOTE_VIDEO_QUALITY _qualityType) const;

    /**
     * @brief   Change the remote video stream priority(low/high).
     * @param   #unsigned int: The user ID.
     * @param   #eREMOTE_VIDEO_PRIORITY: The video priority for the remote user.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetRemoteVideoPriority(const unsigned int _userID, const eREMOTE_VIDEO_PRIORITY _priorityType) const;

    /**
     * @brief   Set an audio effect to the audio stream.
     * @param   #eAUDIO_EFFECT: The Auto effect to apply to.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetAudioEffect(const eAUDIO_EFFECT _effect) const;

    /**
     * @brief   Sets the camera capture configuration.
     * @param   #CameraCapturerConfig: Capture config for the camera.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetCameraCapturerConfiguration(const CameraCapturerConfig& _config) const;

    /**
     * @brief   Set default remote video quality for all the user that enter to the channel.
     *          The default quality is Hight
     * @param   #eREMOTE_VIDEO_QUALITY: video quality types.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int SetDefaultRemoteVideoQuality(const eREMOTE_VIDEO_QUALITY _qualityType) const;

    /**
     * @brief   Enable the super resolution video algorithm to specific user
     * @param   #unsigned int: The user ID.
     * @param   #bool: Enable flag.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int EnableRemoteVideoSuperResolution(const unsigned int _userID, const bool _switchFlag) const;

    /**
     * @brief   Stop/Resume sharing the screen in the stream.
     * @param   #bool: The switch flag.
     * @param   #ScreenCaptureConfig: Config to capture the screen.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int EnableScreenShare(const bool _switchFlag, const ScreenCaptureConfig& _config);

    /**
     * @brief   Stop/Resume sharing a window in the stream.
     * @param   #bool: The switch flag.
     * @param   #ScreenCaptureConfig: Config to capture the screen.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int EnableWindowShare(const bool _switchFlag, const ScreenCaptureConfig& _config);

    /**
     * @brief   Stop/Resume local audio stream.
     * @param   #bool: The switch flag.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int MuteLocalAudioStream(const bool _switchFlag) const;

    /**
     * @brief   Stop/Resume local video stream.
     * @param   #bool: The switch flag.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int MuteLocalVideoStream(const bool _switchFlag) const;

    /**
     * @brief   Stop/Resume remote audio stream from specific userID.
     * @param   #unsigned int: The user ID.
     * @param   #bool: The switch flag.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int MuteRemoteAudioStream(const unsigned int _userID, const bool _switchFlag) const;

    /**
     * @brief   Stop/Resume remote video stream from specific userID.
     * @param   #unsigned int: The user ID.
     * @param   #bool: The switch flag.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int MuteRemoteVideoStream(const unsigned int _userID, const bool _switchFlag) const;

    /**
     * @brief   Enable video recording feed test for the selected device.
     * @param   #bool: The switch flag.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int EnableVideoRecordingTest(const bool _switchFlag) const;

    /**
     * @brief   Enable audio recording feed test for the selected device.
     * @param   #bool: The switch flag.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int EnableAudioRecordingTest(const bool _switchFlag) const;

    /**
     * @brief   Join a channel with the specific params.
     * @param   #ChannelParams: The params to that will be used to join.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int JoinChannel(const ChannelParams& _channelParams) const;

    /**
     * @brief   Leave the channel.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int LeaveChannel() const;

    /**
     * @brief   Publishes the local stream to a specified CDN live RTMP address.  (CDN live only.)
     * @param   #std::string: The CDN streaming URL in the RTMP format. The maximum length of
     *          this parameter is 1024 bytes. The RTMP URL address must not contain special
     *          characters, such as Chinese language characters.
     * @param   #StreamConfig: The config to inject stream.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int PublishStreamUrl(const std::string& _url, const StreamConfig& _config) const;

    /**
     * @brief   Removes an RTMP stream from the CDN. (CDN live only.
     * @param   #std::string: The RTMP URL address to be removed. The maximum length of this
     *          parameter is 1024 bytes.
     * @bug     No know Bugs.
     * @return  #int: Agora status code.
     */
    int RemoveStreamUrl(const std::string& _url) const;

#if defined(_WIN32)
    /**
     * @brief   Callback to see the current open windows.
     * @bug     No know Bugs.
     * @return  #BOOL: successfully taken the window settings
     */
    static BOOL CALLBACK WndEnumProc(HWND hWnd, LPARAM lParam);
#endif
  };

  /**
   * @brief   Return the unique instance for the manager. If the reference is not ready, then
   *          create a new one, start up and return it.
   * @param   #std::string: The app key to use in the Agora sdk
   * @bug     No know Bugs.
   * @return  #AgoraRTC: A reference to the unique agora manager object.
   */
  extern AGORAYCE_API AgoraRTC& GetAgoraRTC(const std::string& _appID = std::string());
}
