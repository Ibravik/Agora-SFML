#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui-SFML.h"
#include "AgoraRTC.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>

const int stringLength = 2048;

// Main window
sf::RenderWindow g_MainWindow;
sf::Clock g_DeltaClock;
sf::Clock g_Time;
bool g_MainMenu = true;

// channel settings
int g_ClientRole = 0;
bool g_ChannelSettingsFlag = false;
int g_MaxChannelTabs = 1;
struct frameBuffer;
struct channelSettings 
{
  bool onCall;
  unsigned int clientID;
  int encryptionType;
  char tabName[stringLength]{};
  char channelName[stringLength]{};
  char encryptionKey[stringLength]{};
  int peopleInChannel;
  unsigned int activeSpeaker;

  bool streaming;
  char rtmpLink[stringLength]{};
  int rtmpWidth;
  int rtmpHeight;
  int rtmpVideoGop;
  int rtmpVideoFramerate;
  int rtmpVideoBitrate;
  int rtmpAudioSampleRate;
  int rtmpAudioBitrate;
  int rtmpAudioChannels;
 
  channelSettings() :
    onCall(false),
    clientID(0),
    encryptionType(0), 
    peopleInChannel(0),
    activeSpeaker(0),
    streaming(false),
    rtmpWidth(320),
    rtmpHeight(180),
    rtmpVideoGop(30),
    rtmpVideoFramerate(15),
    rtmpVideoBitrate(400),
    rtmpAudioSampleRate(2),
    rtmpAudioBitrate(48),
    rtmpAudioChannels(0){}
};
std::vector<std::string> g_EncryptionTypes { 
  std::string("aes-128-xts"), 
  std::string("aes-128-ecb"),
  std::string("aes-256-xts")
};
std::vector<std::string> g_ClientRoles {
  std::string("Broadcaster"),
  std::string("Audience")
};
std::vector<int> g_AudioSampleRate{
  32000, 44100, 48000
};
std::vector<std::string> g_AudioChannels{
  std::string("Mono"),
  std::string("Stereo")
};
ImVector<channelSettings> g_ChannelTabs;

// Agora settings
bool g_EnableVideoTest = false;
bool g_EnableAudioTest = false;
int g_ConnectedChannels = 0;
bool g_SettingsFlag = false;
bool g_LocalVideoStreamMute = false;
bool g_LocalAudioStreamMute = false;
bool g_ScreenShare = false;
int g_VoiceEffect = 0;
std::vector<std::string> g_VideoRecordingDeviceList;
std::vector<std::string> g_AudioRecordingDeviceList;
std::vector<std::string> g_AudioPlaybackDeviceList;
std::vector<std::string> g_ScreenDeviceList;
struct encoderConfig
{
  int width;
  int height;
  int frameRate;
  int minFrameRate;
  int bitrate;
  int minBitrate;
  int orientationMode;
  int degradationPreference;
  int mirrorMode;

  encoderConfig() :
    width(640),
    height(360),
    frameRate(3),
    minFrameRate(-1),
    bitrate(0), 
    minBitrate(-1), 
    degradationPreference(0),
    mirrorMode(0) {}
};
std::vector<int> g_FrameRate{
  1, 7, 10, 15, 24, 30, 60
};
std::map<int, std::string> g_DegradationPreference{
  { 0, "Maintain Quality" }, 
  { 1, "Maintain Framerate" },
  { 2, "Maintain Balanced" }
};
std::map<int, std::string> g_VideoMirror{
  { 0, "Mirror Mode Auto" },
  { 1, "Mirror Mode Enabled" },
  { 2, "Mirror Mode Disabled" }
};
std::map<int, std::string> g_VoiceEffects{
  { 0, "Off" },
  { 1, "Uncle" },
  { 2, "Old Man" },
  { 3, "Boy" },
  { 4, "Sister" },
  { 5, "Girl" },
  { 6, "Pig King " },
  { 7, "Hulk" },
};

// Frame buffer
sf::Vector2f g_LocalFrameMaxSize(320.0f, 180.0f);
sf::Vector2f g_FrameMaxSize(240.0f, 135.0f);
struct frameBuffer
{
  sf::Texture texture;
  sf::Uint8* buffer = nullptr;

  ~frameBuffer()
  {
    if (buffer != nullptr)
    {
      delete[] buffer;
      buffer = nullptr;
    }
  }
};

// local frame
frameBuffer g_LocalFrame;

// Remote user Info
struct remoteUser
{
  frameBuffer frame;
  bool audioStreamMute;
  bool videoStreamMute;
  bool videoStreamQuality;
  bool videoStreamPriority;
  bool videoSuperResolution;
  int fps;
  int width;
  int height;
  int bitrate;

  remoteUser() : 
    audioStreamMute(false),
    videoStreamMute(false),
    videoStreamQuality(false),
    videoStreamPriority(false),
    videoSuperResolution(false),
    fps(0),
    width(0),
    height(0),
    bitrate(0){}
};

// remote frame
std::map<unsigned int, remoteUser> g_RemoteFrame;

// Agora logs callbacks------------------------------------------------------------------------

void onWarning(const WarnInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onWarning()- warn:" << _info.warn << " msg:" << _info.msg << "\n";
    log.close();
  }
}

void onError(const ErrorInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onError()- err:" << _info.err << " msg:" << _info.msg << "\n";
    log.close();
  }
}

void onJoinChannelSuccess(const JoinChannelInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onJoinChannelSuccess()- uid:" << _info.uid << " elapsed:" << _info.elapsed << " channel:" << _info.channel << "\n";
    log.close();
  }
}

void onLeaveChannel(const CallStatsInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onLeaveChannel()- users in call:" << _info.userCount << "\n";
    log.close();
  }
}

void onClientRoleChanged(const RoleChangedInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onClientRoleChanged()- new Role:" << _info.newRole << " old Role:" << _info.oldRole << "\n";
    log.close();
  }
}

void onConnectionLost()
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onConnectionLost()-" << "\n";
    log.close();
  }
}

void onRtcStats(const CallStatsInfo& _info)
{
  g_ChannelTabs[0].peopleInChannel = _info.userCount;
}

void onStreamMessageError(const StreamErrorInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onStreamMessageError()- code:" << _info.code << " stream Id:" << _info.streamId << "\n";
    log.close();
  }
}

void onStreamInjectedStatus(const StreamStatusInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onStreamInjectedStatus()- uid:" << _info.uid << " status:" << _info.status << " url:" << _info.url << "\n";
    log.close();
  }
}

void onUserJoined(const UserJoinInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onUserJoined()- uid:" << _info.uid << " elapsed:" << _info.elapsed << "\n";
    log.close();
  }
}

void onActiveSpeaker(const unsigned int _uid)
{
  g_ChannelTabs[0].activeSpeaker = _uid;
}

void onRtmpStreamingStateChanged(const StreamingStateChangedInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onRtmpStreamingStateChanged()- errorCode:" << _info.errCode << " state:" << _info.state << " url:" << _info.url << "\n";
    log.close();
  }
}

void onRtmpStreamingEvent(const StreamingEventInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onRtmpStreamingEvent()- eventCode:" << _info.eventCode << " url:" << _info.url << "\n";
    log.close();
  }
}

void onUserOffline(const UserOfflineInfo& _info)
{
  std::ofstream log("log.txt", std::ios::app);
  if (log.is_open())
  {
    log << "-onUserOffline()- uid:" << _info.uid << " reason:" << _info.reason << "\n";
    log.close();
  }

  // erase the remote user info
  if (g_RemoteFrame.find(_info.uid) != g_RemoteFrame.end())
  {
    g_RemoteFrame.erase(_info.uid);
  }
}

void onRemoteVideoStats(const RemoteVideoStats& _info)
{
  // update the remote user info
  if (g_RemoteFrame.find(_info.uid) != g_RemoteFrame.end())
  {
    auto &remote = g_RemoteFrame.at(_info.uid);
    remote.bitrate = _info.receivedBitrate;
    remote.fps = _info.decoderOutputFrameRate;
    remote.width = _info.width;
    remote.height = _info.height;
  }
}

// --------------------------------------------------------------------------------------------

void dispatchEvent()
{
  sf::Event event;
  while (g_MainWindow.pollEvent(event))
  {
    ImGui::SFML::ProcessEvent(event);
    if (event.type == sf::Event::Closed)
    {
      // finish agora SDK
      AgoraRTC& agoraObj = GetAgoraRTC();
      agoraObj.LeaveChannel();
      agoraObj.m_VideoObserver.SetOnCaptureVideoFrameCallback(nullptr);
      agoraObj.m_VideoObserver.SetOnRenderVideoFrameCallback(nullptr);

      g_MainWindow.close();
    }
  }
}

void consoleWindow()
{
  float pos = (float)g_MainWindow.getSize().x * 0.7f;
  ImGui::SetNextWindowPos(ImVec2(g_LocalFrameMaxSize.x + 30, 28.0f), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(pos, g_LocalFrameMaxSize.y), ImGuiCond_Always);

  // show device settings window
  if (ImGui::Begin("Console Output", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                              ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize |
                                              ImGuiWindowFlags_NoSavedSettings))
  {
    // erase console
    if (ImGui::Button("Clear Console"))
    {
      // clean log
      std::ofstream log;
      log.open("log.txt", std::ofstream::out | std::ofstream::trunc);
      log.close();
    }

    // Read the log file
    std::string line;
    std::ifstream log("log.txt");
    if (log.is_open())
    {
      while (getline(log, line))
      {
        ImGui::Text(line.c_str());
      }
      log.close();
    }
  }
  ImGui::End();
}

void showChannelTab(bool& _isOpen, channelSettings& _channel)
{
  // local agora ref to work on
  AgoraRTC& agoraObj = GetAgoraRTC();

  // show channel Tab
  if (ImGui::BeginTabItem(_channel.tabName, &_isOpen))
  {
    // client ID
    ImGui::LabelText("Client ID", std::to_string(_channel.clientID).c_str());
    ImGui::Separator();

    // name
    if (_channel.onCall)
    {
      ImGui::LabelText("Channel ID", _channel.channelName);
    }
    else
    {
      ImGui::InputText("Channel ID", _channel.channelName, IM_ARRAYSIZE(_channel.channelName));
    }

    // password
    if (_channel.onCall)
    {
      ImGui::LabelText("Channel Password", _channel.encryptionKey);
    }
    else
    {
      ImGui::InputText("Channel Password", _channel.encryptionKey, IM_ARRAYSIZE(_channel.encryptionKey));
    }
    
    // encryption Type
    if (_channel.onCall)
    {
      ImGui::LabelText("Encryption Type", g_EncryptionTypes.at(_channel.encryptionType).c_str());
    }
    else
    {
      if (ImGui::BeginCombo("Encryption Type", g_EncryptionTypes.at(_channel.encryptionType).c_str()))
      {
        for (int i = 0; i < g_EncryptionTypes.size(); i++)
        {
          const bool isFocus = (_channel.encryptionType == i);
          if (ImGui::Selectable(g_EncryptionTypes.at(i).c_str(), isFocus))
          {
            _channel.encryptionType = i;
          }
          if (isFocus)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }
    ImGui::Separator();

    // the stream settings only are possible if the client are a broadcaster
    if (g_ClientRole == 0)
    {
      // Dimension
      static int vec2[2] = { 640, 480 };
      if (ImGui::InputInt2("Video dimensions(w/h)", vec2))
      {
        if (vec2[0] < 0)
        {
          vec2[0] = 0;
        }
        if (vec2[1] < 0)
        {
          vec2[1] = 0;
        }
        _channel.rtmpWidth = vec2[0];
        _channel.rtmpHeight = vec2[1];
      }

      // GOP
      if (ImGui::InputInt("Video GOP (in frames)", &_channel.rtmpVideoGop))
      {
        if (_channel.rtmpVideoGop < 0)
        {
          _channel.rtmpVideoGop = 0;
        }
      }

      // video FPS
      if (ImGui::InputInt("Video frame rate", &_channel.rtmpVideoFramerate))
      {
        if (_channel.rtmpVideoFramerate < 0)
        {
          _channel.rtmpVideoFramerate = 0;
        }
      }

      // video bitrate
      if (ImGui::InputInt("Video bitrate", &_channel.rtmpVideoBitrate))
      {
        if (_channel.rtmpVideoBitrate < 0)
        {
          _channel.rtmpVideoBitrate = 0;
        }
      }

      // audio sample rate
      if (ImGui::BeginCombo("Audio-sample rate", std::to_string(g_AudioSampleRate.at(_channel.rtmpAudioSampleRate)).c_str()))
      {
        for (int i = 0; i < g_AudioSampleRate.size(); i++)
        {
          const bool isFocus = (_channel.rtmpAudioSampleRate == i);
          if (ImGui::Selectable(std::to_string(g_AudioSampleRate.at(i)).c_str(), isFocus))
          {
            _channel.rtmpAudioSampleRate = i;
          }
          if (isFocus)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }

      // audio bitrate
      if (ImGui::InputInt("Audio bitrate", &_channel.rtmpAudioBitrate))
      {
        if (_channel.rtmpAudioBitrate < 0)
        {
          _channel.rtmpAudioBitrate = 0;
        }
      }

      // audio channels
      if (ImGui::BeginCombo("Audio channels", g_AudioChannels.at(_channel.rtmpAudioChannels).c_str()))
      {
        for (int i = 0; i < g_AudioChannels.size(); i++)
        {
          const bool isFocus = (_channel.rtmpAudioChannels == i);
          if (ImGui::Selectable(g_AudioChannels.at(i).c_str(), isFocus))
          {
            _channel.rtmpAudioChannels = i;
          }
          if (isFocus)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      
      // rtmp input
      if (_channel.streaming)
      {
        ImGui::LabelText("rtmp Link", _channel.rtmpLink);
      }
      else
      {
        ImGui::InputText("rtmp Link", _channel.rtmpLink, IM_ARRAYSIZE(_channel.rtmpLink));
      }
      
      // start/end stream
      if (_channel.onCall && _channel.rtmpLink[0] != '\0')
      {
        if (ImGui::Button(_channel.streaming ? "Stop Streaming" : "Start Streaming"))
        {
          _channel.streaming = !_channel.streaming;

          if (_channel.streaming)
          {
            StreamConfig config;
            config.audioBitrate = _channel.rtmpAudioBitrate;
            config.audioChannels = _channel.rtmpAudioChannels + 1;
            config.audioSampleRate = static_cast<AUDIO_SAMPLE_RATE_TYPE>(g_AudioSampleRate.at(_channel.rtmpAudioSampleRate));
            config.height = _channel.rtmpHeight;
            config.videoBitrate = _channel.rtmpVideoBitrate;
            config.videoFramerate = _channel.rtmpVideoFramerate;
            config.videoGop = _channel.rtmpVideoGop;
            config.width = _channel.rtmpWidth;
            agoraObj.PublishStreamUrl(std::string(_channel.rtmpLink), config);
          }
          else
          {
            agoraObj.RemoveStreamUrl(_channel.rtmpLink);
          }
        }
      }
      else
      {
        ImGui::Text(_channel.streaming ? "Stop Streaming" : "Start Streaming");
      }

      ImGui::Separator();
    }

    // join to the channel
    if (_channel.clientID == 0 || _channel.channelName[0] == '\0')
    {
      ImGui::Text(_channel.onCall ? "Leave Channel" : "Join Channel");
    }
    else
    {
      if (ImGui::Button(_channel.onCall ? "Leave Channel" : "Join Channel"))
      {
        _channel.onCall = !_channel.onCall;

        if (_channel.onCall)
        {
          if (g_EnableVideoTest)
          {
            g_EnableVideoTest = false;
            agoraObj.EnableVideoRecordingTest(g_EnableVideoTest);
          }
          if (g_EnableAudioTest)
          {
            g_EnableAudioTest = false;
            agoraObj.EnableAudioRecordingTest(g_EnableAudioTest);
          }
          
          agoraObj.MuteLocalVideoStream(g_LocalVideoStreamMute);
          agoraObj.MuteLocalAudioStream(g_LocalAudioStreamMute);
          ChannelParams channelParams;
          channelParams.channelName = _channel.channelName;
          channelParams.clientID = _channel.clientID;
          channelParams.encryptionKey = _channel.encryptionKey;
          channelParams.encryptionType = _channel.encryptionType;
          channelParams.clientRole = g_ClientRole == 0 ? eCLIENT_ROLE::kBROADCASTER : eCLIENT_ROLE::kAUDIENCE;
          agoraObj.JoinChannel(channelParams);
          
          g_ConnectedChannels++;
        }
        else
        {
          _channel.streaming = false;
          agoraObj.LeaveChannel();
          g_RemoteFrame.clear();
          g_ConnectedChannels--;
        }
      }
    }
    ImGui::Separator();

    // channel stats
    if (_channel.onCall)
    {
      ImGui::LabelText("Users in channel", std::to_string(_channel.peopleInChannel).c_str());
    }
    ImGui::EndTabItem();
  }
}

void channelSettingWindow()
{
  static int tabID = 0;

  AgoraRTC& agoraObj = GetAgoraRTC();
  ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f), ImGuiCond_Always);

  // show settings window
  if (ImGui::Begin("Channel Settings", &g_ChannelSettingsFlag, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                                               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | 
                                                               ImGuiWindowFlags_NoSavedSettings))
  {
    // description
    ImGui::Text("The config for each channel.");

    // client role
    if (ImGui::BeginCombo("Client Role", g_ClientRoles.at(g_ClientRole).c_str()))
    {
      for (int i = 0; i < g_ClientRoles.size(); i++)
      {
        const bool isFocus = (g_ClientRole == i);
        if (ImGui::Selectable(g_ClientRoles.at(i).c_str(), isFocus))
        {
          g_ClientRole = i;
          GetAgoraRTC().SetClientRole(g_ClientRole == 0 ? eCLIENT_ROLE::kBROADCASTER : eCLIENT_ROLE::kAUDIENCE);
        }
        if (isFocus)
        {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    // channel tabs
    if (ImGui::BeginTabBar("Channels tab", ImGuiTabBarFlags_AutoSelectNewTabs))
    {
      // show Help button
      if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
      {
        ImGui::OpenPopup("Channel Help");
      }
      if (ImGui::BeginPopup("Channel Help"))
      {
        ImGui::Selectable("Add a new tab to\njoin to a new channel");
        ImGui::EndPopup();
      }

      // show add tab button
      if (g_ChannelTabs.Size < g_MaxChannelTabs)
      {
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
        {
          // add new channel tab
          channelSettings newChannel;
          std::string tabName("channel ");
          tabName += std::to_string(tabID++);
          memcpy(&newChannel.tabName[0], &tabName[0], tabName.size());
          newChannel.clientID = g_Time.getElapsedTime().asMilliseconds();
          g_ChannelTabs.push_back(newChannel);
        }
      }

      // Show the channel tabs
      for (int i = 0; i < g_ChannelTabs.Size; )
      {
        bool isOpen = true;
        showChannelTab(isOpen, g_ChannelTabs[i]);

        // erase unused channel tabs
        if (!isOpen)
        {
          if (g_ChannelTabs[i].onCall)
          {
            GetAgoraRTC().LeaveChannel();
            g_ConnectedChannels--;
          }
          g_ChannelTabs.erase(g_ChannelTabs.Data + i);
        }
        else
        {
          i++;
        }
      }
      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}

void showDeviceComboBox(eDEVICE_TYPE _deviceType, int& _index, std::vector<std::string>& _list)
{
  // local agora ref to work on
  AgoraRTC& agoraObj = GetAgoraRTC();

  // Tittle selection
  std::string tittle;
  if (_deviceType == eDEVICE_TYPE::kVideoRecording)
  {
    tittle = "Video Recording";
  }
  else if (_deviceType == eDEVICE_TYPE::kAudioRecording)
  {
    tittle = "Audio Recording";
  }
  else if (_deviceType == eDEVICE_TYPE::kAudioPlayback)
  {
    tittle = "Audio Playback";
  }
  else if (_deviceType == eDEVICE_TYPE::kScreen)
  {
    tittle = "Screen Share";
  }

  // current selected label
  static std::string label = "No Device Connected";
  if (!_list.empty())
  {
    label = _list.at(_index);
  }

  // Show combo box
  if (ImGui::BeginCombo(tittle.c_str(), label.c_str()))
  {
    for (int i = 0; i < _list.size(); i++)
    {
      const bool isFocus = (_index == i);

      // selection
      if (ImGui::Selectable(_list.at(i).c_str(), isFocus))
      {
        _index = i;
        agoraObj.SetCurrentDeviceByName(_list.at(_index), _deviceType);
      }

      // focus to the combo
      if (isFocus)
      {
        ImGui::SetItemDefaultFocus();
        size_t length = _list.size();
        _list = agoraObj.GetDeviceList(_deviceType);

        // index reset check
        _index = (length != _list.size() ? 0 : _index);
      }
    }
    ImGui::EndCombo();
  }
}

void settingsWindow()
{
  AgoraRTC& agoraObj = GetAgoraRTC();
  ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f), ImGuiCond_Always);

  // show settings window
  if (ImGui::Begin("Settings", &g_SettingsFlag, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize |
                                                ImGuiWindowFlags_NoSavedSettings))
  {
    // show the tab menu
    if (ImGui::BeginTabBar("Settings Tab", ImGuiTabBarFlags_None))
    {
      // encoder settings tab
      if (ImGui::BeginTabItem("Encoder settings"))
      {
        static encoderConfig encoder;

        // description
        ImGui::Text("Video encoder configurations.");

        // Dimension
        static int vec2[2] = { 640, 480 };
        if (ImGui::InputInt2("Video frame dimensions(w/h)", vec2))
        {
          if (vec2[0] < 0)
          {
            vec2[0] = 0;
          }
          if (vec2[1] < 0)
          {
            vec2[1] = 0;
          }
          encoder.width = vec2[0];
          encoder.height = vec2[1];
        }

        // Frame rate
        if (ImGui::BeginCombo("FPS", std::to_string(g_FrameRate.at(encoder.frameRate)).c_str()))
        {
          for (int i = 0; i < g_FrameRate.size(); i++)
          {
            const bool isFocus = (encoder.frameRate == i);
            if (ImGui::Selectable(std::to_string(g_FrameRate.at(i)).c_str(), isFocus))
            {
              encoder.frameRate = i;
            }
            if (isFocus)
            {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }

        // min frame
        if (ImGui::InputInt("Minimum frame rate", &encoder.minFrameRate))
        {
          if (encoder.minFrameRate < -1)
          {
            encoder.minFrameRate = -1;
          }
        }

        // bitrate
        if (ImGui::InputInt("Video encoding bitrate", &encoder.bitrate))
        {
          if (encoder.bitrate < -1)
          {
            encoder.bitrate = -1;
          }
        }

        // min bitrate
        if (ImGui::InputInt("Minimum encoding bitrate ", &encoder.minBitrate))
        {
          if (encoder.minBitrate < -1)
          {
            encoder.minBitrate = -1;
          }
        }

        // degradation preference
        if (ImGui::BeginCombo("Degradation Preference", g_DegradationPreference.at(encoder.degradationPreference).c_str()))
        {
          for (int i = 0; i < g_DegradationPreference.size(); i++)
          {
            const bool isFocus = (encoder.degradationPreference == i);
            if (ImGui::Selectable(g_DegradationPreference.at(i).c_str(), isFocus))
            {
              encoder.degradationPreference = i;
            }
            if (isFocus)
            {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }

        // degradation preference
        if (ImGui::BeginCombo("Mirror Mode", g_VideoMirror.at(encoder.mirrorMode).c_str()))
        {
          for (int i = 0; i < g_VideoMirror.size(); i++)
          {
            const bool isFocus = (encoder.mirrorMode == i);
            if (ImGui::Selectable(g_VideoMirror.at(i).c_str(), isFocus))
            {
              encoder.mirrorMode = i;
            }
            if (isFocus)
            {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
        ImGui::Separator();

        // apply the encoding settings
        if (ImGui::Button("Apply Encoding Settings"))
        {
          VideoEncoderConfig config;
          config.bitrate = encoder.bitrate;
          config.degradationPreference = static_cast<DEGRADATION_PREFERENCE>(encoder.degradationPreference);
          config.dimensions = VideoDimensions(encoder.width, encoder.height);
          config.frameRate = static_cast<FRAME_RATE>(g_FrameRate.at(encoder.frameRate));
          config.minBitrate = encoder.minBitrate;
          config.minFrameRate = encoder.minFrameRate;
          config.mirrorMode = static_cast<VIDEO_MIRROR_MODE_TYPE>(encoder.mirrorMode);
          agoraObj.SetVideoEncoderConfiguration(config);
        }
        ImGui::EndTabItem();
      }

      // video settings tab
      if (ImGui::BeginTabItem("Video settings"))
      {
        // The combo box for video recording/screen share
        static int videoRecordingIndex = 0;
        static int screenIndex = 0;
        if (g_ScreenShare)
        {
          ImGui::LabelText("Video Recording", g_VideoRecordingDeviceList.empty() ? "No Device Connected" :
                                              g_VideoRecordingDeviceList.at(videoRecordingIndex).c_str());
          ImGui::Separator();
          showDeviceComboBox(eDEVICE_TYPE::kScreen, screenIndex, g_ScreenDeviceList);
        }
        else
        {
          showDeviceComboBox(eDEVICE_TYPE::kVideoRecording, videoRecordingIndex, g_VideoRecordingDeviceList);
          ImGui::Separator();
          ImGui::LabelText("Screen Share", g_ScreenDeviceList.empty() ? "No Device Connected" : 
                                           g_ScreenDeviceList.at(screenIndex).c_str());
        }

        // enable screen share
        std::string state = (g_ScreenShare ? std::string("Disable") : std::string("Enable"));
        if (ImGui::Checkbox((state + std::string(" Screen Share")).c_str(), &g_ScreenShare))
        {
          agoraObj.EnableScreenShare(g_ScreenShare);
        }
        ImGui::Separator();
        
        // enable local video stream
        state = (g_LocalVideoStreamMute ? std::string("Unmute") : std::string("Mute"));
        if (ImGui::Checkbox((state + std::string(" local video stream")).c_str(), &g_LocalVideoStreamMute))
        {
          agoraObj.MuteLocalVideoStream(g_LocalVideoStreamMute);
        }

        if (g_ConnectedChannels <= 0)
        {
          // Enable video device test
          state = (g_EnableVideoTest ? std::string("Disable") : std::string("Enable"));
          if (ImGui::Checkbox((state + std::string(" device test")).c_str(), &g_EnableVideoTest))
          {
            agoraObj.EnableVideoRecordingTest(g_EnableVideoTest);
          }

          // video test image
          if (g_EnableVideoTest)
          {
            ImGui::Image(g_LocalFrame.texture, g_LocalFrameMaxSize);
          }
        }
        
        ImGui::EndTabItem();
      }

      // audio settings tab
      if (ImGui::BeginTabItem("Audio settings"))
      {
        // The combo box for audio recording
        static int audioRecordingIndex = 0;
        showDeviceComboBox(eDEVICE_TYPE::kAudioRecording, audioRecordingIndex, g_AudioRecordingDeviceList);

        // The audio recording volume slider
        float recordingVolume = agoraObj.GetAudioRecordingVolume();
        if (ImGui::SliderFloat("Recording Volume", &recordingVolume, 0.0f, 1.0f))
        {
          agoraObj.SetAudioRecordingVolume(recordingVolume);
        }       
        ImGui::Separator();

        // The combo box for audio playback
        static int audioPlaybackIndex = 0;
        showDeviceComboBox(eDEVICE_TYPE::kAudioPlayback, audioPlaybackIndex, g_AudioPlaybackDeviceList);

        // The audio playback volume slider
        float playbackVolume = agoraObj.GetAudioPlaybackVolume();
        if (ImGui::SliderFloat("Playback Volume", &playbackVolume, 0.0f, 1.0f))
        {
          agoraObj.SetAudioPlaybackVolume(playbackVolume);
        }
        ImGui::Separator();

        // audio effect
        if (ImGui::BeginCombo("Voice effect", g_VoiceEffects.at(g_VoiceEffect).c_str()))
        {
          for (int i = 0; i < g_VoiceEffects.size(); i++)
          {
            const bool isFocus = (g_VoiceEffect == i);
            if (ImGui::Selectable(g_VoiceEffects.at(i).c_str(), isFocus))
            {
              g_VoiceEffect = i;
              eAUDIO_EFFECT effect = eAUDIO_EFFECT::AUDIO_EFFECT_OFF;
              switch (g_VoiceEffect)
              {
              case 1:
                effect = eAUDIO_EFFECT::VOICE_CHANGER_EFFECT_UNCLE;
                break;
              case 2:
                effect = eAUDIO_EFFECT::VOICE_CHANGER_EFFECT_OLDMAN;
                break;
              case 3:
                effect = eAUDIO_EFFECT::VOICE_CHANGER_EFFECT_BOY;
                break;
              case 4:
                effect = eAUDIO_EFFECT::VOICE_CHANGER_EFFECT_SISTER;
                break;
              case 5:
                effect = eAUDIO_EFFECT::VOICE_CHANGER_EFFECT_GIRL;
                break;
              case 6:
                effect = eAUDIO_EFFECT::VOICE_CHANGER_EFFECT_PIGKING;
                break;
              case 7:
                effect = eAUDIO_EFFECT::VOICE_CHANGER_EFFECT_HULK;
                break;
              default:
                effect = eAUDIO_EFFECT::AUDIO_EFFECT_OFF;
                break;
              }
              agoraObj.SetAudioEffect(effect);
            }
            if (isFocus)
            {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }

        // enable local audio stream
        std::string state = (g_LocalAudioStreamMute ? std::string("Unmute") : std::string("Mute"));
        if (ImGui::Checkbox((state + std::string(" local audio stream")).c_str(), &g_LocalAudioStreamMute))
        {
          agoraObj.MuteLocalAudioStream(g_LocalAudioStreamMute);
        }

        // enable audio device test
        if (g_ConnectedChannels <= 0)
        {
          state = (g_EnableAudioTest ? std::string("Disable") : std::string("Enable"));
          if (ImGui::Checkbox((state + std::string(" device test")).c_str(), &g_EnableAudioTest))
          {
            agoraObj.EnableAudioRecordingTest(g_EnableAudioTest);
          }
        }
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
  }
  ImGui::End();
}

void gridWindow()
{
  AgoraRTC& agoraObj = GetAgoraRTC();
  float pos = (float)g_MainWindow.getSize().x * 0.7f;
  ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2((float)g_MainWindow.getSize().x, g_MainWindow.getSize().y - 20.0f), ImGuiCond_Always);
  static sf::Color videoUnmute(0xffffffff);
  static sf::Color videoMute(0x222222ff);
  static sf::Color talk(0x00ff00ff);
  static sf::Color audioUnmute(0x000000ff);
  static sf::Color audioMute(0xff0000ff);

  // show Main Menu
  if (ImGui::Begin("grid", &g_MainMenu, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                        ImGuiWindowFlags_NoMove | 
                                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus))
  {
    // ignore if is not in a call
    if (g_ConnectedChannels > 0)
    {
      // Main frame
      auto& uid = g_ChannelTabs[0].clientID;
      sf::Vector2u size = g_LocalFrame.texture.getSize();
      std::string desc;
      desc = std::string("ID: ");
      desc += std::to_string(g_ChannelTabs[0].clientID);
      desc += std::string("(");
      desc += std::to_string(size.x);
      desc += std::string("x");
      desc += std::to_string(size.y);
      desc += std::string(")");
      ImGui::Image(g_LocalFrame.texture, g_LocalFrameMaxSize,
                   (g_LocalVideoStreamMute ? videoMute : videoUnmute),
                   (g_LocalAudioStreamMute ? audioMute : (g_ChannelTabs[0].activeSpeaker == uid ? talk : audioUnmute)));
      ImGui::Text(desc.c_str());

      // Main frame settings
      if (ImGui::Checkbox(g_LocalAudioStreamMute ? "Unmute Audio" : "Mute Audio", &g_LocalAudioStreamMute))
      {
        agoraObj.MuteLocalAudioStream(g_LocalAudioStreamMute);
      }
      ImGui::SameLine();
      if (ImGui::Checkbox(g_LocalVideoStreamMute ? "Unmute Video" : "Mute Video", &g_LocalVideoStreamMute))
      {
        agoraObj.MuteLocalVideoStream(g_LocalVideoStreamMute);
      }

      // Grid
      if (ImGui::BeginTable("Remote Frame", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame))
      {
        int row = 0, column = 0;
        for (auto& remoteFrame : g_RemoteFrame)
        {
          if (column == 0)
          {
            ImGui::TableNextRow();
          }
          ImGui::TableSetColumnIndex(column);

          // remote frame
          auto& ruid = remoteFrame.first;
          desc = std::string("ID: ");
          desc += std::to_string(remoteFrame.first);
          desc += std::string("(");
          desc += std::to_string(remoteFrame.second.width);
          desc += std::string("x");
          desc += std::to_string(remoteFrame.second.height);
          desc += std::string(") ");
          desc += std::to_string(remoteFrame.second.bitrate);
          desc += std::string("kb");
          ImGui::Image(remoteFrame.second.frame.texture, g_FrameMaxSize,
                       (remoteFrame.second.videoStreamMute ? videoMute : videoUnmute),
                       (remoteFrame.second.audioStreamMute ? audioMute : (g_ChannelTabs[0].activeSpeaker == ruid ? talk : audioUnmute)));
          ImGui::Text(desc.c_str());

          // Main frame settings
          ImGui::PushID(remoteFrame.first);
          if (ImGui::Checkbox(remoteFrame.second.audioStreamMute ? "Unmute Audio" : "Mute Audio", &remoteFrame.second.audioStreamMute))
          {
            agoraObj.MuteRemoteAudioStream(remoteFrame.first, remoteFrame.second.audioStreamMute);
          }
          ImGui::SameLine();
          if (ImGui::Checkbox(remoteFrame.second.videoStreamMute ? "Unmute Video" : "Mute Video", &remoteFrame.second.videoStreamMute))
          {
            agoraObj.MuteRemoteVideoStream(remoteFrame.first, remoteFrame.second.videoStreamMute);
          }
          if (ImGui::Button(remoteFrame.second.videoStreamQuality ? "Low Video Quality" : "High Video Quality"))
          {
            remoteFrame.second.videoStreamQuality = !remoteFrame.second.videoStreamQuality;
            agoraObj.SetRemoteVideoQuality(remoteFrame.first, remoteFrame.second.videoStreamQuality ? eREMOTE_VIDEO_QUALITY::kHIGH : eREMOTE_VIDEO_QUALITY::kLOW);
          }
          ImGui::SameLine();
          if (ImGui::Button(remoteFrame.second.videoStreamPriority ? "Low Video Priority" : "High Video Priority"))
          {
            remoteFrame.second.videoStreamPriority = !remoteFrame.second.videoStreamPriority;
            agoraObj.SetRemoteVideoPriority(remoteFrame.first, remoteFrame.second.videoStreamPriority ? eREMOTE_VIDEO_PRIORITY::kHIGH : eREMOTE_VIDEO_PRIORITY::kNORMAL);
          }
          if (ImGui::Button(remoteFrame.second.videoSuperResolution ? "Turn OFF Super Resolution" : "Turn ON Super Resolution"))
          {
            remoteFrame.second.videoSuperResolution = !remoteFrame.second.videoSuperResolution;
            agoraObj.EnableRemoteVideoSuperResolution(remoteFrame.first, remoteFrame.second.videoSuperResolution);
          }
          ImGui::PopID();

          column++;
          row = (column >= 4 ? row + 1 : row);
          column = (column >= 4 ? 0 : column);
        }
        ImGui::EndTable();
      }
    }
  }
  ImGui::End();
}

void menuWindow()
{
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2((float)g_MainWindow.getSize().x, 20.0f), ImGuiCond_Always);
 
  // show Main Menu
  if (ImGui::Begin("mainMenu", &g_MainMenu, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | 
                                            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | 
                                            ImGuiWindowFlags_NoBringToFrontOnFocus))
  {
    // show menu
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("Device Setting"))
      {
        g_SettingsFlag = true;
        g_ChannelSettingsFlag = !g_SettingsFlag;
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Channel Settings"))
      {
        g_ChannelSettingsFlag = true;
        g_SettingsFlag = !g_ChannelSettingsFlag;
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }
  ImGui::End();
}

void setUISettings()
{
  menuWindow();
  gridWindow();
  if (g_SettingsFlag)
  {
    settingsWindow();
  }
  if (g_ChannelSettingsFlag)
  {
    channelSettingWindow();
  }
  consoleWindow();
}

void updateLocalFrame(const VideoBuffer& _bufferInfo)
{
  // check if its necessary reset the texture
  sf::Vector2u textureSize = g_LocalFrame.texture.getSize();
  sf::Vector2u bufferSize(static_cast<unsigned int>(_bufferInfo.width), static_cast<unsigned int>(_bufferInfo.height));
  int bufferLenght = int(bufferSize.x) * int(bufferSize.y) * 4;
  bool resetTexture = (textureSize.x != bufferSize.x || textureSize.y != bufferSize.y);

  // reset the local frame texture
  if (resetTexture)
  {
    if (g_LocalFrame.buffer != nullptr)
    {
      delete[] g_LocalFrame.buffer;
      g_LocalFrame.buffer = nullptr;
    }

    g_LocalFrame.texture.create(bufferSize.x, bufferSize.y);
    g_LocalFrame.buffer = new sf::Uint8[bufferLenght];
    return;
  }

  // feed the local frame texture
  memcpy(g_LocalFrame.buffer, _bufferInfo.yBuffer, bufferLenght);
  g_LocalFrame.texture.update(g_LocalFrame.buffer);
}

void updateRemoteFrame(const VideoBuffer& _bufferInfo, const unsigned int _clientID)
{
  // add the frame buffer for the remote user
  if (g_RemoteFrame.find(_clientID) == g_RemoteFrame.end())
  {
    g_RemoteFrame.emplace(_clientID, remoteUser());
  }
  auto& remoteFrame = g_RemoteFrame.at(_clientID);

  // check if its necessary reset the texture
  sf::Vector2u textureSize = remoteFrame.frame.texture.getSize();
  sf::Vector2u bufferSize(static_cast<unsigned int>(_bufferInfo.width), static_cast<unsigned int>(_bufferInfo.height));
  int bufferLenght = int(bufferSize.x) * int(bufferSize.y) * 4;
  bool resetTexture = (textureSize.x != bufferSize.x || textureSize.y != bufferSize.y);

  // reset the local frame texture
  if (resetTexture)
  {
    if (remoteFrame.frame.buffer != nullptr)
    {
      delete[] remoteFrame.frame.buffer;
      remoteFrame.frame.buffer = nullptr;
    }

    remoteFrame.frame.texture.create(bufferSize.x, bufferSize.y);
    remoteFrame.frame.buffer = new sf::Uint8[bufferLenght];
    return;
  }

  // feed the local frame texture
  memcpy(remoteFrame.frame.buffer, _bufferInfo.yBuffer, bufferLenght);
  remoteFrame.frame.texture.update(remoteFrame.frame.buffer);
}

void startUp()
{
  std::ifstream appID("appID.txt");
  if (appID.is_open())
  {
    std::string line;
    getline(appID, line);
    GetAgoraRTC(line);
    appID.close();
  }
  else
  {
    return;
  }

  std::ofstream log;
  log.open("log.txt", std::ofstream::out | std::ofstream::trunc);
  log.close();

  AgoraRTC& agoraObj = GetAgoraRTC();
  g_MainWindow.create(sf::VideoMode(1280, 720), agoraObj.GetSDKVersion().c_str());
  ImGui::SFML::Init(g_MainWindow);

  agoraObj.m_VideoObserver.SetOnCaptureVideoFrameCallback(updateLocalFrame);
  agoraObj.m_VideoObserver.SetOnRenderVideoFrameCallback(updateRemoteFrame);
  agoraObj.m_EventHandler.SetOnWarning(onWarning);
  agoraObj.m_EventHandler.SetOnError(onError);
  agoraObj.m_EventHandler.SetOnJoinChannelSuccess(onJoinChannelSuccess);
  agoraObj.m_EventHandler.SetOnLeaveChannel(onLeaveChannel);
  agoraObj.m_EventHandler.SetOnClientRoleChanged(onClientRoleChanged);
  agoraObj.m_EventHandler.SetOnConnectionLost(onConnectionLost);
  agoraObj.m_EventHandler.SetOnRtcStats(onRtcStats);
  agoraObj.m_EventHandler.SetOnStreamMessageError(onStreamMessageError);
  agoraObj.m_EventHandler.SetOnStreamInjectedStatus(onStreamInjectedStatus);
  agoraObj.m_EventHandler.SetOnUserJoined(onUserJoined);
  agoraObj.m_EventHandler.SetOnActiveSpeaker(onActiveSpeaker);
  agoraObj.m_EventHandler.SetOnRtmpStreamingStateChanged(onRtmpStreamingStateChanged);
  agoraObj.m_EventHandler.SetOnRtmpStreamingEvent(onRtmpStreamingEvent);
  agoraObj.m_EventHandler.SetOnUserOffline(onUserOffline);
  agoraObj.m_EventHandler.SetOnRemoteVideoStats(onRemoteVideoStats);

  g_VideoRecordingDeviceList = agoraObj.GetDeviceList(eDEVICE_TYPE::kVideoRecording);
  g_AudioRecordingDeviceList = agoraObj.GetDeviceList(eDEVICE_TYPE::kAudioRecording);
  g_AudioPlaybackDeviceList = agoraObj.GetDeviceList(eDEVICE_TYPE::kAudioPlayback);
  g_ScreenDeviceList = agoraObj.GetDeviceList(eDEVICE_TYPE::kScreen);
}

void update()
{
  dispatchEvent();
  ImGui::SFML::Update(g_MainWindow, g_DeltaClock.restart());
  setUISettings();
}

void render()
{
  g_MainWindow.clear();
  ImGui::SFML::Render(g_MainWindow);
  g_MainWindow.display();
}

void shutDown()
{
  ImGui::SFML::Shutdown();
}

int main()
{
  startUp();
  while (g_MainWindow.isOpen())
  {
    update();
    render();
  }
  shutDown();

  return 0;
}
