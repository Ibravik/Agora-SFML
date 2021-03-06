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

using namespace agoraYCE;

// Misc settings
const int stringLength = 2048;

// Main window settings
sf::RenderWindow g_MainWindow;
sf::Clock g_DeltaClock;
sf::Clock g_Time;

// full screen window
bool g_Fullscreen = false;
unsigned int g_FullscreenID = 0;

// Grid window settings
bool g_GridFlag = true;

// channel window settings
bool g_ChannelFlag = false;
int g_MaxChannelTabs = 1;
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
ScreenCaptureConfig g_ScreenCapture;
ImVector<channelSettings> g_ChannelTabs;
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

// device window settings
bool g_SettingsFlag = false;
bool g_EnableVideoTest = false;
bool g_EnableAudioTest = false;
int g_ConnectedChannels = 0;
std::vector<std::string> g_VideoRecordingDeviceList;
std::vector<std::string> g_AudioRecordingDeviceList;
std::vector<std::string> g_AudioPlaybackDeviceList;
std::vector<std::string> g_ScreenDeviceList;
std::vector<std::string> g_WindowList;
std::vector<int> g_FrameRate{
  1, 7, 10, 15, 24, 30, 60
};
std::vector<std::string> g_DegradationPreference{
  "Quality" , "Framerate", "Balanced"
};
std::vector<std::string> g_VideoMirror{
  "Mirror Mode Auto", "Mirror Mode Enabled", "Mirror Mode Disabled"
};
std::vector<std::string> g_VoiceEffects{
  "Off", "Uncle", "Old Man", "Boy", "Sister", "Girl", "Pig King", "Hulk"
};
std::vector<std::string> g_CaptureConfig{
  "Auto", "Performance", "Preview"
};

// Frame settings
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

// user info settings
struct userInfo
{
  frameBuffer frame;
  bool audioStreamMute;
  bool videoStreamMute;
  bool videoStreamQuality;
  bool videoStreamPriority;
  bool videoSuperResolution;
  bool screenShare;
  bool windowShare;
  int fps;
  int width;
  int height;
  int bitrate;
  int clientRole;
  int voiceEffect;

  userInfo() :
    audioStreamMute(false),
    videoStreamMute(false),
    videoStreamQuality(false),
    videoStreamPriority(false),
    videoSuperResolution(false),
    screenShare(false),
    windowShare(false),
    fps(0),
    width(0),
    height(0),
    bitrate(0),
    clientRole(0),
    voiceEffect(0){}
};

// Users
userInfo g_LocalUser;
std::map<unsigned int, userInfo> g_RemoteUser;

// log system
void log(const std::string& _log)
{
  std::ofstream log("log", std::ios::app);
  if (log.is_open())
  {
    log << _log.c_str();
    log.close();
  }
}

// Agora logs callbacks------------------------------------------------------------------------

void onWarning(const WarnInfo& _info)
{
  log(std::string("-onWarning()- warn:") + std::to_string(_info.warn) + 
      std::string(" msg:") + _info.msg + std::string("\n"));
}

void onError(const ErrorInfo& _info)
{
  log(std::string("-onError()- err:") + std::to_string(_info.err) + 
      std::string(" msg:") + _info.msg + std::string("\n"));
}

void onJoinChannelSuccess(const JoinChannelInfo& _info)
{
  log(std::string("-onJoinChannelSuccess()- uid:") + std::to_string(_info.uid) + 
                  std::string(" elapsed:") + std::to_string(_info.elapsed) + 
                  std::string(" channel:") + _info.channel +
                  std::string("\n"));
}

void onLeaveChannel(const CallStatsInfo& _info)
{
  log(std::string("-onLeaveChannel()- users in call:") + std::to_string(_info.userCount) + 
      std::string("\n"));
}

void onClientRoleChanged(const RoleChangedInfo& _info)
{
  log(std::string("-onClientRoleChanged()- new Role:") + std::to_string(_info.newRole) + 
      std::string(" old Role:") + std::to_string(_info.oldRole) + 
      std::string("\n"));
}

void onConnectionLost()
{
  log(std::string("-onConnectionLost()") +
      std::string("\n"));
}

void onRtcStats(const CallStatsInfo& _info)
{
  g_ChannelTabs[0].peopleInChannel = _info.userCount;
}

void onStreamMessageError(const StreamErrorInfo& _info)
{
  log(std::string("-onStreamMessageError()- code:") + std::to_string(_info.code) +
      std::string(" stream Id:") + std::to_string(_info.streamId) +
      std::string("\n"));
}

void onStreamInjectedStatus(const StreamStatusInfo& _info)
{
  log(std::string("-onStreamInjectedStatus()- uid:") + std::to_string(_info.uid) +
      std::string(" status:") + std::to_string(_info.status) +
      std::string(" url:") + _info.url +
      std::string("\n"));
}

void onUserJoined(const UserJoinInfo& _info)
{
  log(std::string("-onUserJoined()- uid:") + std::to_string(_info.uid) +
      std::string(" elapsed:") + std::to_string(_info.elapsed) +
      std::string("\n"));

  // add the frame buffer for the remote user
  if (g_RemoteUser.find(_info.uid) == g_RemoteUser.end())
  {
    g_RemoteUser.emplace(_info.uid, userInfo());
  }
}

void onActiveSpeaker(const unsigned int _uid)
{
  g_ChannelTabs[0].activeSpeaker = _uid;
}

void onRtmpStreamingStateChanged(const StreamingStateChangedInfo& _info)
{
  log(std::string("-onRtmpStreamingStateChanged()- errorCode:") + std::to_string(_info.errCode) +
      std::string(" state:") + std::to_string(_info.state) +
      std::string(" url:") + _info.url +
      std::string("\n"));
}

void onRtmpStreamingEvent(const StreamingEventInfo& _info)
{
  log(std::string("-onRtmpStreamingEvent()- eventCode:") + std::to_string(_info.eventCode) +
      std::string(" url:") + _info.url +
      std::string("\n"));
}

void onUserOffline(const UserOfflineInfo& _info)
{
  log(std::string("-onUserOffline()- uid:") + std::to_string(_info.uid) +
      std::string(" reason:") + std::to_string(_info.reason) +
      std::string("\n"));

  // erase the remote user info
  if (g_RemoteUser.find(_info.uid) != g_RemoteUser.end())
  {
    g_RemoteUser.erase(_info.uid);
  }
}

void onRemoteVideoStats(const RemoteVideoStatsInfo& _info)
{
  // update the remote user info
  if (g_RemoteUser.find(_info.uid) != g_RemoteUser.end())
  {
    auto &remote = g_RemoteUser.at(_info.uid);
    remote.bitrate = _info.receivedBitrate;
    remote.fps = _info.decoderOutputFrameRate;
    remote.width = _info.width;
    remote.height = _info.height;
  }
}

void onLocalVideoStats(const LocalVideoStatsInfo& _info)
{
  // update the remote user info
  g_LocalUser.bitrate = _info.encodedBitrate;
  g_LocalUser.fps = _info.encoderOutputFrameRate;
  g_LocalUser.width = _info.encodedFrameWidth;
  g_LocalUser.height = _info.encodedFrameHeight;
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

void fullscreen()
{
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
  ImGui::SetNextWindowSize(g_MainWindow.getSize(), ImGuiCond_Always);

  // show device settings window
  if (ImGui::Begin((std::string("FullScreen") + std::to_string(g_FullscreenID)).c_str(), &g_Fullscreen, 
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
  {
    if (g_RemoteUser.find(g_FullscreenID) != g_RemoteUser.end())
    {
      AgoraRTC& agoraObj = GetAgoraRTC();
      static sf::Color videoUnmute(0xffffffff);
      static sf::Color videoMute(0x222222ff);
      static sf::Color talk(0x00ff00ff);
      static sf::Color audioUnmute(0x000000ff);
      static sf::Color audioMute(0xff0000ff);
      auto& remoteUser = g_RemoteUser[g_FullscreenID];

      // remote frame
      auto& ruid = g_FullscreenID;
      std::string desc = std::string("ID: ");
      desc += std::to_string(ruid);
      desc += std::string("(");
      desc += std::to_string(remoteUser.width);
      desc += std::string("x");
      desc += std::to_string(remoteUser.height);
      desc += std::string(") ");
      desc += std::to_string(remoteUser.bitrate);
      desc += std::string("kb ");
      desc += std::to_string(remoteUser.fps);
      desc += std::string("fps");
      
      // fullscreen image
      ImGui::Image(remoteUser.frame.texture, sf::Vector2f(g_MainWindow.getSize().x * 0.80f, g_MainWindow.getSize().y * 0.80f),
                   (remoteUser.videoStreamMute ? videoMute : videoUnmute),
                   (remoteUser.audioStreamMute ? audioMute : (g_ChannelTabs[0].activeSpeaker == g_FullscreenID ? talk : audioUnmute)));
      ImGui::Text(desc.c_str());

      // Main frame settings
      ImGui::PushID(g_FullscreenID);
      if (ImGui::Checkbox(remoteUser.audioStreamMute ? "Unmute Audio" : "Mute Audio", &remoteUser.audioStreamMute))
      {
        agoraObj.MuteRemoteAudioStream(g_FullscreenID, remoteUser.audioStreamMute);
      }
      ImGui::SameLine();
      if (ImGui::Checkbox(remoteUser.videoStreamMute ? "Unmute Video" : "Mute Video", &remoteUser.videoStreamMute))
      {
        agoraObj.MuteRemoteVideoStream(g_FullscreenID, remoteUser.videoStreamMute);
      }
      if (ImGui::Button(remoteUser.videoStreamQuality ? "Low Video Quality" : "High Video Quality"))
      {
        remoteUser.videoStreamQuality = !remoteUser.videoStreamQuality;
        agoraObj.SetRemoteVideoQuality(g_FullscreenID, remoteUser.videoStreamQuality ? eREMOTE_VIDEO_QUALITY::kHIGH : eREMOTE_VIDEO_QUALITY::kLOW);
      }
      ImGui::SameLine();
      if (ImGui::Button(remoteUser.videoStreamPriority ? "Low Video Priority" : "High Video Priority"))
      {
        remoteUser.videoStreamPriority = !remoteUser.videoStreamPriority;
        agoraObj.SetRemoteVideoPriority(g_FullscreenID, remoteUser.videoStreamPriority ? eREMOTE_VIDEO_PRIORITY::kHIGH : eREMOTE_VIDEO_PRIORITY::kNORMAL);
      }
      if (ImGui::Button(remoteUser.videoSuperResolution ? "Turn OFF Super Resolution" : "Turn ON Super Resolution"))
      {
        remoteUser.videoSuperResolution = !remoteUser.videoSuperResolution;
        agoraObj.EnableRemoteVideoSuperResolution(g_FullscreenID, remoteUser.videoSuperResolution);
      }
      ImGui::PopID();
    }
    else
    {
      g_Fullscreen = false;
      g_FullscreenID = 0;
    }
  }
  else
  {
    g_Fullscreen = false;
    g_FullscreenID = 0;
  }
  ImGui::End();
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
      log.open("log", std::ofstream::out | std::ofstream::trunc);
      log.close();
    }

    // Read the log file
    std::string line;
    std::ifstream log("log");
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
    if (g_LocalUser.clientRole == 0)
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
            config.audioSampleRate = g_AudioSampleRate.at(_channel.rtmpAudioSampleRate);
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

          agoraObj.MuteLocalVideoStream(g_LocalUser.videoStreamMute);
          agoraObj.MuteLocalAudioStream(g_LocalUser.audioStreamMute);
          ChannelParams channelParams;
          channelParams.channelName = _channel.channelName;
          channelParams.clientID = _channel.clientID;
          channelParams.encryptionKey = _channel.encryptionKey;
          channelParams.encryptionType = _channel.encryptionType;
          channelParams.clientRole = g_LocalUser.clientRole == 0 ? eCLIENT_ROLE::kBROADCASTER : eCLIENT_ROLE::kAUDIENCE;
          agoraObj.SetDefaultRemoteVideoQuality(eREMOTE_VIDEO_QUALITY::kLOW);
          agoraObj.JoinChannel(channelParams);
                    
          g_ConnectedChannels++;
        }
        else
        {
          _channel.streaming = false;
          agoraObj.LeaveChannel();
          g_RemoteUser.clear();
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
  if (ImGui::Begin("Channel Settings", &g_ChannelFlag, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                                               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | 
                                                               ImGuiWindowFlags_NoSavedSettings))
  {
    // description
    ImGui::Text("The config for each channel.");

    // client role
    if (ImGui::BeginCombo("Client Role", g_ClientRoles.at(g_LocalUser.clientRole).c_str()))
    {
      for (int i = 0; i < g_ClientRoles.size(); i++)
      {
        const bool isFocus = (g_LocalUser.clientRole == i);
        if (ImGui::Selectable(g_ClientRoles.at(i).c_str(), isFocus))
        {
          g_LocalUser.clientRole = i;
          GetAgoraRTC().SetClientRole(g_LocalUser.clientRole == 0 ? eCLIENT_ROLE::kBROADCASTER : eCLIENT_ROLE::kAUDIENCE);
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
  else if (_deviceType == eDEVICE_TYPE::kWindow)
  {
    tittle = "Window Share";
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

void videoConfig()
{
  AgoraRTC& agoraObj = GetAgoraRTC();
  static VideoEncoderConfig highEncoder;
  static VideoEncoderConfig lowEncoder(320, 180, 7, 0, 140, 0, 0, 0, 0);

  // enable screen share
  if (ImGui::Checkbox(g_LocalUser.screenShare ? "Disable Screen Share" : "Enable Screen Share", &g_LocalUser.screenShare))
  {
    if (g_LocalUser.screenShare)
    {
      if (g_LocalUser.windowShare)
      {
        g_LocalUser.windowShare = false;
        agoraObj.EnableWindowShare(g_LocalUser.windowShare, g_ScreenCapture);
      }
    }
    agoraObj.EnableScreenShare(g_LocalUser.screenShare, g_ScreenCapture);
  }

  // enable window share
  if (ImGui::Checkbox(g_LocalUser.windowShare ? "Disable window Share" : "Enable window Share", &g_LocalUser.windowShare))
  {
    if (g_LocalUser.windowShare)
    {
      if (g_LocalUser.screenShare)
      {
        g_LocalUser.screenShare = false;
        agoraObj.EnableScreenShare(g_LocalUser.screenShare, g_ScreenCapture);
      }
    }
    agoraObj.EnableWindowShare(g_LocalUser.windowShare, g_ScreenCapture);
  }

  // enable local video stream
  if (ImGui::Checkbox(g_LocalUser.videoStreamMute ? "Unmute local video stream" : "Mute local video stream", &g_LocalUser.videoStreamMute))
  {
    agoraObj.MuteLocalVideoStream(g_LocalUser.videoStreamMute);
  }
  ImGui::Separator();

  if (g_LocalUser.screenShare || g_LocalUser.windowShare)
  {
    static int screenSize[2] = { g_ScreenCapture.width, g_ScreenCapture.height };
    if (g_LocalUser.windowShare)
    {
      static int windowIndex = 0;

      // screen settings
      showDeviceComboBox(eDEVICE_TYPE::kWindow, windowIndex, g_WindowList);
    }
    else
    {
      static int screenIndex = 0;
      

      // screen capture settings
      showDeviceComboBox(eDEVICE_TYPE::kScreen, screenIndex, g_ScreenDeviceList);
    }

    if (ImGui::InputInt2("Video frame dimensions(w/h)", screenSize))
    {
      screenSize[0] = (screenSize[0] < 0 ? 0 : screenSize[0]);
      screenSize[1] = (screenSize[1] < 0 ? 0 : screenSize[1]);
    }
    if (ImGui::InputInt("FPS", &g_ScreenCapture.frameRate))
    {
      g_ScreenCapture.frameRate = (g_ScreenCapture.frameRate < -1 ? -1 : g_ScreenCapture.frameRate);
    }
    if (ImGui::InputInt("Video encoding bitrate", &g_ScreenCapture.bitrate))
    {
      g_ScreenCapture.bitrate = (g_ScreenCapture.bitrate < -1 ? -1 : g_ScreenCapture.bitrate);
    }
    ImGui::Checkbox(g_ScreenCapture.captureMouseCursor ? "Disable Mouse Capture" : "Enable Mouse Capture", &g_ScreenCapture.captureMouseCursor);
    ImGui::Separator();
    if (ImGui::Button("Apply Capture Settings"))
    {
      g_ScreenCapture.width = screenSize[0];
      g_ScreenCapture.height = screenSize[1];
      agoraObj.SetScreenEncoderConfiguration(g_ScreenCapture);
    }
  }
  else
  {
    static int videoRecordingIndex = 0;
    static int frHighIndex = 5;
    static int frLowIndex = 1;
    static int degIndex = 0;
    static int mirrIndex = 0;
    static int capIndex = 0;

    // video settings device
    showDeviceComboBox(eDEVICE_TYPE::kVideoRecording, videoRecordingIndex, g_VideoRecordingDeviceList);

    // High video encoder config
    if (ImGui::CollapsingHeader("High video encoder config"))
    {
      static int videoSize[2] = { highEncoder.width, highEncoder.height };
      if (ImGui::InputInt2("Video frame dimensions(w/h)", videoSize))
      {
        videoSize[0] = (videoSize[0] < 0 ? 0 : videoSize[0]);
        videoSize[1] = (videoSize[1] < 0 ? 0 : videoSize[1]);
        highEncoder.width = videoSize[0];
        highEncoder.height = videoSize[1];
      }
      if (ImGui::BeginCombo("FPS", std::to_string(g_FrameRate.at(frHighIndex)).c_str()))
      {
        for (int i = 0; i < g_FrameRate.size(); i++)
        {
          const bool isFocus = (frHighIndex == i);
          if (ImGui::Selectable(std::to_string(g_FrameRate.at(i)).c_str(), isFocus))
          {
            frHighIndex = i;
          }
          if (isFocus)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::InputInt("Minimum frame rate", &highEncoder.minFrameRate))
      {
        highEncoder.minFrameRate = (highEncoder.minFrameRate < -1 ? -1 : highEncoder.minFrameRate);
      }
      if (ImGui::InputInt("Video encoding bitrate", &highEncoder.bitrate))
      {
        highEncoder.bitrate = (highEncoder.bitrate < -1 ? -1 : highEncoder.bitrate);
      }
      if (ImGui::InputInt("Minimum encoding bitrate ", &highEncoder.minBitrate))
      {
        highEncoder.minBitrate = (highEncoder.minBitrate < -1 ? -1 : highEncoder.minBitrate);
      }
      if (ImGui::BeginCombo("Degradation Preference", g_DegradationPreference.at(degIndex).c_str()))
      {
        for (int i = 0; i < g_DegradationPreference.size(); i++)
        {
          const bool isFocus = (degIndex == i);
          if (ImGui::Selectable(g_DegradationPreference.at(i).c_str(), isFocus))
          {
            degIndex = i;
          }
          if (isFocus)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }
    if (ImGui::CollapsingHeader("Low video encoder config"))
    {
      static int videoSize[2] = { lowEncoder.width, lowEncoder.height };
      if (ImGui::InputInt2("Video frame dimensions(w/h)", videoSize))
      {
        videoSize[0] = (videoSize[0] < 0 ? 0 : videoSize[0]);
        videoSize[1] = (videoSize[1] < 0 ? 0 : videoSize[1]);
        lowEncoder.width = videoSize[0];
        lowEncoder.height = videoSize[1];
      }
      if (ImGui::BeginCombo("FPS", std::to_string(g_FrameRate.at(frLowIndex)).c_str()))
      {
        for (int i = 0; i < g_FrameRate.size(); i++)
        {
          const bool isFocus = (frLowIndex == i);
          if (ImGui::Selectable(std::to_string(g_FrameRate.at(i)).c_str(), isFocus))
          {
            frLowIndex = i;
          }
          if (isFocus)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::InputInt("Video encoding bitrate", &lowEncoder.bitrate))
      {
        lowEncoder.bitrate = (lowEncoder.bitrate < -1 ? -1 : lowEncoder.bitrate);
      }
    }
    if (ImGui::CollapsingHeader("Local video config"))
    {
      if (ImGui::BeginCombo("Mirror Mode", g_VideoMirror.at(mirrIndex).c_str()))
      {
        for (int i = 0; i < g_VideoMirror.size(); i++)
        {
          const bool isFocus = (mirrIndex == i);
          if (ImGui::Selectable(g_VideoMirror.at(i).c_str(), isFocus))
          {
            mirrIndex = i;
          }
          if (isFocus)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      if (ImGui::BeginCombo("Camera capture config", g_CaptureConfig.at(capIndex).c_str()))
      {
        for (int i = 0; i < g_CaptureConfig.size(); i++)
        {
          const bool isFocus = (capIndex == i);
          if (ImGui::Selectable(g_CaptureConfig.at(i).c_str(), isFocus))
          {
            capIndex = i;

            CameraCapturerConfig config;
            config.preference = capIndex;
            agoraObj.SetCameraCapturerConfiguration(config);
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
    if (ImGui::Button("Apply Encoding Settings"))
    {
      highEncoder.degradationPreference = degIndex;
      highEncoder.frameRate = g_FrameRate.at(frHighIndex);
      highEncoder.mirrorMode = mirrIndex;
      agoraObj.SetHighVideoEncoderConfiguration(highEncoder);
      lowEncoder.frameRate = g_FrameRate.at(frLowIndex);
      agoraObj.SetLowVideoEncoderConfiguration(lowEncoder);
    }
  }
  ImGui::Separator();
  
  if (g_ConnectedChannels <= 0)
  {
    // Enable video device test
    if (ImGui::Checkbox(g_EnableVideoTest ? "Disable device test" : "Enable device test", &g_EnableVideoTest))
    {
      agoraObj.EnableVideoRecordingTest(g_EnableVideoTest);
      if (g_LocalUser.screenShare)
      {
        agoraObj.EnableScreenShare(g_LocalUser.screenShare, g_ScreenCapture);
      }
      if (g_LocalUser.windowShare)
      {
        agoraObj.EnableWindowShare(g_LocalUser.windowShare, g_ScreenCapture);
      }
    }

    // video test image
    if (g_EnableVideoTest)
    {
      static sf::Color videoUnmute(0xffffffff);
      static sf::Color videoMute(0x222222ff);

      ImGui::Image(g_LocalUser.frame.texture, g_LocalFrameMaxSize,
                   (g_LocalUser.videoStreamMute ? videoMute : videoUnmute));
    }
  }
}

void audioConfig()
{
  AgoraRTC& agoraObj = GetAgoraRTC();

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
  if (ImGui::BeginCombo("Voice effect", g_VoiceEffects.at(g_LocalUser.voiceEffect).c_str()))
  {
    for (int i = 0; i < g_VoiceEffects.size(); i++)
    {
      const bool isFocus = (g_LocalUser.voiceEffect == i);
      if (ImGui::Selectable(g_VoiceEffects.at(i).c_str(), isFocus))
      {
        g_LocalUser.voiceEffect = i;
        eAUDIO_EFFECT effect = eAUDIO_EFFECT::AUDIO_EFFECT_OFF;
        switch (g_LocalUser.voiceEffect)
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
  std::string state = (g_LocalUser.audioStreamMute ? std::string("Unmute") : std::string("Mute"));
  if (ImGui::Checkbox((state + std::string(" local audio stream")).c_str(), &g_LocalUser.audioStreamMute))
  {
    agoraObj.MuteLocalAudioStream(g_LocalUser.audioStreamMute);
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
      // video settings tab
      if (ImGui::BeginTabItem("Video settings"))
      {
        videoConfig();
        ImGui::EndTabItem();
      }

      // audio settings tab
      if (ImGui::BeginTabItem("Audio settings"))
      {
        audioConfig();
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
  if (ImGui::Begin("grid", &g_GridFlag, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                        ImGuiWindowFlags_NoMove | 
                                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus))
  {
    // ignore if is not in a call
    if (g_ConnectedChannels > 0)
    {
      // Main frame
      auto& uid = g_ChannelTabs[0].clientID;
      sf::Vector2u size = g_LocalUser.frame.texture.getSize();
      std::string desc;
      desc = std::string("ID: ");
      desc += std::to_string(g_ChannelTabs[0].clientID);
      desc += std::string("(");
      desc += std::to_string(g_LocalUser.width);
      desc += std::string("x");
      desc += std::to_string(g_LocalUser.height);
      desc += std::string(") ");
      desc += std::to_string(g_LocalUser.bitrate);
      desc += std::string("kb ");
      desc += std::to_string(g_LocalUser.fps);
      desc += std::string("fps");
      ImGui::Image(g_LocalUser.frame.texture, g_LocalFrameMaxSize,
                   (g_LocalUser.videoStreamMute ? videoMute : videoUnmute),
                   (g_LocalUser.audioStreamMute ? audioMute : (g_ChannelTabs[0].activeSpeaker == uid ? talk : audioUnmute)));
      ImGui::Text(desc.c_str());

      // Main frame settings
      if (ImGui::Checkbox(g_LocalUser.audioStreamMute ? "Unmute Audio" : "Mute Audio", &g_LocalUser.audioStreamMute))
      {
        agoraObj.MuteLocalAudioStream(g_LocalUser.audioStreamMute);
      }
      ImGui::SameLine();
      if (ImGui::Checkbox(g_LocalUser.videoStreamMute ? "Unmute Video" : "Mute Video", &g_LocalUser.videoStreamMute))
      {
        agoraObj.MuteLocalVideoStream(g_LocalUser.videoStreamMute);
      }
    
      // Grid
      if (ImGui::BeginTable("Remote Frame", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame))
      {
        int row = 0, column = 0;
        for (auto& remoteUser : g_RemoteUser)
        {
          if (column == 0)
          {
            ImGui::TableNextRow();
          }
          ImGui::TableSetColumnIndex(column);

          // remote frame
          auto& ruid = remoteUser.first;
          desc = std::string("ID: ");
          desc += std::to_string(remoteUser.first);
          desc += std::string("(");
          desc += std::to_string(remoteUser.second.width);
          desc += std::string("x");
          desc += std::to_string(remoteUser.second.height);
          desc += std::string(") ");
          desc += std::to_string(remoteUser.second.bitrate);
          desc += std::string("kb ");
          desc += std::to_string(remoteUser.second.fps);
          desc += std::string("fps");
          ImGui::Image(remoteUser.second.frame.texture, g_FrameMaxSize,
                       (remoteUser.second.videoStreamMute ? videoMute : videoUnmute),
                       (remoteUser.second.audioStreamMute ? audioMute : (g_ChannelTabs[0].activeSpeaker == ruid ? talk : audioUnmute)));
          ImGui::Text(desc.c_str());

          // Main frame settings
          ImGui::PushID(remoteUser.first);
          if (ImGui::Checkbox(remoteUser.second.audioStreamMute ? "Unmute Audio" : "Mute Audio", &remoteUser.second.audioStreamMute))
          {
            agoraObj.MuteRemoteAudioStream(remoteUser.first, remoteUser.second.audioStreamMute);
          }
          ImGui::SameLine();
          if (ImGui::Checkbox(remoteUser.second.videoStreamMute ? "Unmute Video" : "Mute Video", &remoteUser.second.videoStreamMute))
          {
            agoraObj.MuteRemoteVideoStream(remoteUser.first, remoteUser.second.videoStreamMute);
          }
          if (ImGui::Button(remoteUser.second.videoStreamQuality ? "Low Video Quality" : "High Video Quality"))
          {
            remoteUser.second.videoStreamQuality = !remoteUser.second.videoStreamQuality;
            agoraObj.SetRemoteVideoQuality(remoteUser.first, remoteUser.second.videoStreamQuality ? eREMOTE_VIDEO_QUALITY::kHIGH : eREMOTE_VIDEO_QUALITY::kLOW);
          }
          ImGui::SameLine();
          if (ImGui::Button(remoteUser.second.videoStreamPriority ? "Low Video Priority" : "High Video Priority"))
          {
            remoteUser.second.videoStreamPriority = !remoteUser.second.videoStreamPriority;
            agoraObj.SetRemoteVideoPriority(remoteUser.first, remoteUser.second.videoStreamPriority ? eREMOTE_VIDEO_PRIORITY::kHIGH : eREMOTE_VIDEO_PRIORITY::kNORMAL);
          }
          if (ImGui::Button(remoteUser.second.videoSuperResolution ? "Turn OFF Super Resolution" : "Turn ON Super Resolution"))
          {
            remoteUser.second.videoSuperResolution = !remoteUser.second.videoSuperResolution;
            agoraObj.EnableRemoteVideoSuperResolution(remoteUser.first, remoteUser.second.videoSuperResolution);
          }
          ImGui::SameLine();
          if (ImGui::Button("Fullscreen"))
          {
            g_FullscreenID = remoteUser.first;
            g_Fullscreen = true;
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
  if (ImGui::Begin("mainMenu", &g_GridFlag, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | 
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
        g_ChannelFlag = !g_SettingsFlag;
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Channel Settings"))
      {
        g_ChannelFlag = true;
        g_SettingsFlag = !g_ChannelFlag;
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
  if (g_ChannelFlag)
  {
    channelSettingWindow();
  }
  consoleWindow();
  if (g_Fullscreen)
  {
    fullscreen();
  }
}

void updateLocalFrame(const VideoBuffer& _bufferInfo)
{
  // check if its necessary reset the texture
  sf::Vector2u textureSize = g_LocalUser.frame.texture.getSize();
  sf::Vector2u bufferSize(static_cast<unsigned int>(_bufferInfo.width), static_cast<unsigned int>(_bufferInfo.height));
  int bufferLenght = int(bufferSize.x) * int(bufferSize.y) * 4;
  bool resetTexture = (textureSize.x != bufferSize.x || textureSize.y != bufferSize.y);

  // reset the local frame texture
  if (resetTexture)
  {
    log(std::string("-updateLocalFrame()-") + 
        std::string(" size:") + std::to_string(_bufferInfo.width) + std::string("x") + std::to_string(_bufferInfo.height));
    if (g_LocalUser.frame.buffer != nullptr)
    {
      delete[] g_LocalUser.frame.buffer;
      g_LocalUser.frame.buffer = nullptr;
    }

    g_LocalUser.frame.texture.create(bufferSize.x, bufferSize.y);
    g_LocalUser.frame.buffer = new sf::Uint8[bufferLenght];
    return;
  }

  // feed the local frame texture
  memcpy(g_LocalUser.frame.buffer, _bufferInfo.yBuffer, bufferLenght);
  g_LocalUser.frame.texture.update(g_LocalUser.frame.buffer);
}

void updateRemoteFrame(const VideoBuffer& _bufferInfo, const unsigned int _clientID)
{
  // add the frame buffer for the remote user
  if (g_RemoteUser.find(_clientID) == g_RemoteUser.end())
  {
    return;
  }
  auto& remoteFrame = g_RemoteUser.at(_clientID);

  // check if its necessary reset the texture
  sf::Vector2u textureSize = remoteFrame.frame.texture.getSize();
  sf::Vector2u bufferSize(static_cast<unsigned int>(_bufferInfo.width), static_cast<unsigned int>(_bufferInfo.height));
  int bufferLenght = int(bufferSize.x) * int(bufferSize.y) * 4;
  bool resetTexture = (textureSize.x != bufferSize.x || textureSize.y != bufferSize.y);
  
  // reset the local frame texture
  if (resetTexture)
  {
    log(std::string("-updateRemoteFrame()- uid:") + std::to_string(_clientID) + 
        std::string(" size:") + std::to_string(_bufferInfo.width) + std::string("x") + std::to_string(_bufferInfo.height));
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
  std::ifstream appID("appID");
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
  log.open("log", std::ofstream::out | std::ofstream::trunc);
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
  agoraObj.m_EventHandler.SetOnLocalVideoStats(onLocalVideoStats);

  g_VideoRecordingDeviceList = agoraObj.GetDeviceList(eDEVICE_TYPE::kVideoRecording);
  g_AudioRecordingDeviceList = agoraObj.GetDeviceList(eDEVICE_TYPE::kAudioRecording);
  g_AudioPlaybackDeviceList = agoraObj.GetDeviceList(eDEVICE_TYPE::kAudioPlayback);
  g_ScreenDeviceList = agoraObj.GetDeviceList(eDEVICE_TYPE::kScreen);
  g_WindowList = agoraObj.GetDeviceList(eDEVICE_TYPE::kWindow);
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
