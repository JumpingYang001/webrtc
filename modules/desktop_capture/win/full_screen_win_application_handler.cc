/*
 *  Copyright (c) 2019 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "modules/desktop_capture/win/full_screen_win_application_handler.h"

#include <algorithm>
#include <cwctype>
#include <memory>
#include <string>
#include <vector>

#include "absl/strings/ascii.h"
#include "absl/strings/match.h"
#include "modules/desktop_capture/win/screen_capture_utils.h"
#include "modules/desktop_capture/win/window_capture_utils.h"
#include "rtc_base/arraysize.h"
#include "rtc_base/logging.h"  // For RTC_LOG_GLE
#include "rtc_base/string_utils.h"

namespace webrtc {
namespace {

// Utility function to verify that `window` has class name equal to `class_name`
bool CheckWindowClassName(HWND window, const wchar_t* class_name) {
  const size_t classNameLength = wcslen(class_name);

  // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa
  // says lpszClassName field in WNDCLASS is limited by 256 symbols, so we don't
  // need to have a buffer bigger than that.
  constexpr size_t kMaxClassNameLength = 256;
  WCHAR buffer[kMaxClassNameLength];

  const int length = ::GetClassNameW(window, buffer, kMaxClassNameLength);
  if (length <= 0)
    return false;

  if (static_cast<size_t>(length) != classNameLength)
    return false;
  return wcsncmp(buffer, class_name, classNameLength) == 0;
}

std::string WindowText(HWND window) {
  size_t len = ::GetWindowTextLength(window);
  if (len == 0)
    return std::string();

  std::vector<wchar_t> buffer(len + 1, 0);
  size_t copied = ::GetWindowTextW(window, buffer.data(), buffer.size());
  if (copied == 0)
    return std::string();
  return webrtc::ToUtf8(buffer.data(), copied);
}

DWORD WindowProcessId(HWND window) {
  DWORD dwProcessId = 0;
  ::GetWindowThreadProcessId(window, &dwProcessId);
  return dwProcessId;
}

std::wstring FileNameFromPath(const std::wstring& path) {
  auto found = path.rfind(L"\\");
  if (found == std::string::npos)
    return path;
  return path.substr(found + 1);
}

// Returns windows which belong to given process id
// `sources` is a full list of available windows
// `processId` is a process identifier (window owner)
// `window_to_exclude` is a window to be exluded from result
DesktopCapturer::SourceList GetProcessWindows(
    const DesktopCapturer::SourceList& sources,
    DWORD processId,
    HWND window_to_exclude) {
  DesktopCapturer::SourceList result;
  std::copy_if(sources.begin(), sources.end(), std::back_inserter(result),
               [&](DesktopCapturer::Source source) {
                 const HWND source_hwnd = reinterpret_cast<HWND>(source.id);
                 return window_to_exclude != source_hwnd &&
                        WindowProcessId(source_hwnd) == processId;
               });
  return result;
}

class FullScreenPowerPointHandler : public FullScreenApplicationHandler {
 public:
  explicit FullScreenPowerPointHandler(DesktopCapturer::SourceId sourceId)
      : FullScreenApplicationHandler(sourceId) {}

  ~FullScreenPowerPointHandler() override {}

  DesktopCapturer::SourceId FindFullScreenWindow(
      const DesktopCapturer::SourceList& window_list,
      int64_t timestamp) const override {
    if (window_list.empty())
      return 0;

    HWND original_window = reinterpret_cast<HWND>(GetSourceId());
    if (GetWindowType(original_window) != WindowType::kEditor)
      return 0;

    DesktopCapturer::SourceList powerpoint_windows = GetProcessWindows(
        window_list, WindowProcessId(original_window), original_window);

    // No relevant window with the same process id as the `original_window` was
    // found.
    if (powerpoint_windows.empty())
      return 0;

    const std::string original_document_title =
        GetDocumentTitleFromEditor(original_window);
    for (const auto& source : powerpoint_windows) {
      HWND window = reinterpret_cast<HWND>(source.id);

      // Looking for fullscreen slide show window for the corresponding editor
      // document
      if (GetWindowType(window) == WindowType::kSlideShow &&
          GetDocumentTitleFromSlideShow(window) == original_document_title) {
        return source.id;
      }
    }
    return 0;
  }

 private:
  enum class WindowType { kEditor, kSlideShow, kOther };

  WindowType GetWindowType(HWND window) const {
    if (IsEditorWindow(window))
      return WindowType::kEditor;
    else if (IsSlideShowWindow(window))
      return WindowType::kSlideShow;
    else
      return WindowType::kOther;
  }

  constexpr static char kDocumentTitleSeparator = '-';

  // This function extracts the title from the editor. It needs to be
  // updated everytime PowerPoint changes its editor title format. Currently, it
  // supports editor title in the format "Window - Title - PowerPoint".
  std::string GetDocumentTitleFromEditor(HWND window) const {
    std::string title = WindowText(window);
    return std::string(
        absl::StripAsciiWhitespace(absl::string_view(title).substr(
            0, title.rfind(kDocumentTitleSeparator))));
  }

  // This function extracts the title from the slideshow when PowerPoint goes
  // fullscreen. This function needs to be updated whenever PowerPoint changes
  // its title format. Currently, it supports Fullscreen titles of the format
  // "PowerPoint Slide Show - [Window - Title]" or "PowerPoint Slide Show -
  // Window - Title".
  std::string GetDocumentTitleFromSlideShow(HWND window) const {
    std::string title = WindowText(window);
    auto position = title.find(kDocumentTitleSeparator);
    if (position != std::string::npos) {
      title = std::string(absl::StripAsciiWhitespace(
          absl::string_view(title).substr(position + 1, std::wstring::npos)));
    }

    auto left_bracket_pos = title.find("[");
    auto right_bracket_pos = title.rfind("]");
    if (left_bracket_pos == std::string::npos ||
        right_bracket_pos == std::string::npos ||
        right_bracket_pos <= left_bracket_pos) {
      return title;
    }

    return std::string(absl::StripAsciiWhitespace(title.substr(
        left_bracket_pos + 1, right_bracket_pos - left_bracket_pos - 1)));
  }

  bool IsEditorWindow(HWND window) const {
    return CheckWindowClassName(window, L"PPTFrameClass");
  }

  bool IsSlideShowWindow(HWND window) const {
    const LONG style = ::GetWindowLong(window, GWL_STYLE);
    const bool min_box = WS_MINIMIZEBOX & style;
    const bool max_box = WS_MAXIMIZEBOX & style;
    return !min_box && !max_box;
  }
};

class OpenOfficeApplicationHandler : public FullScreenApplicationHandler {
 public:
  explicit OpenOfficeApplicationHandler(DesktopCapturer::SourceId sourceId)
      : FullScreenApplicationHandler(sourceId) {}

  DesktopCapturer::SourceId FindFullScreenWindow(
      const DesktopCapturer::SourceList& window_list,
      int64_t timestamp) const override {
    if (window_list.empty())
      return 0;

    DWORD process_id = WindowProcessId(reinterpret_cast<HWND>(GetSourceId()));

    DesktopCapturer::SourceList app_windows =
        GetProcessWindows(window_list, process_id, nullptr);

    DesktopCapturer::SourceList document_windows;
    std::copy_if(
        app_windows.begin(), app_windows.end(),
        std::back_inserter(document_windows),
        [this](const DesktopCapturer::Source& x) { return IsEditorWindow(x); });

    // Check if we have only one document window, otherwise it's not possible
    // to securely match a document window and a slide show window which has
    // empty title.
    if (document_windows.size() != 1) {
      return 0;
    }

    // Check if document window has been selected as a source
    if (document_windows.front().id != GetSourceId()) {
      return 0;
    }

    // Check if we have a slide show window.
    auto slide_show_window =
        std::find_if(app_windows.begin(), app_windows.end(),
                     [this](const DesktopCapturer::Source& x) {
                       return IsSlideShowWindow(x);
                     });

    if (slide_show_window == app_windows.end())
      return 0;

    return slide_show_window->id;
  }

 private:
  bool IsEditorWindow(const DesktopCapturer::Source& source) const {
    if (source.title.empty()) {
      return false;
    }

    return CheckWindowClassName(reinterpret_cast<HWND>(source.id), L"SALFRAME");
  }

  bool IsSlideShowWindow(const DesktopCapturer::Source& source) const {
    // Check title size to filter out a Presenter Control window which shares
    // window class with Slide Show window but has non empty title.
    if (!source.title.empty()) {
      return false;
    }

    return CheckWindowClassName(reinterpret_cast<HWND>(source.id),
                                L"SALTMPSUBFRAME");
  }
};

std::wstring GetPathByWindowId(HWND window_id) {
  DWORD process_id = WindowProcessId(window_id);
  HANDLE process =
      ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_id);
  if (process == NULL)
    return L"";
  DWORD path_len = MAX_PATH;
  WCHAR path[MAX_PATH];
  std::wstring result;
  if (::QueryFullProcessImageNameW(process, 0, path, &path_len))
    result = std::wstring(path, path_len);
  else
    RTC_LOG_GLE(LS_ERROR) << "QueryFullProcessImageName failed.";

  ::CloseHandle(process);
  return result;
}

}  // namespace

std::unique_ptr<FullScreenApplicationHandler>
CreateFullScreenWinApplicationHandler(DesktopCapturer::SourceId source_id) {
  std::unique_ptr<FullScreenApplicationHandler> result;
  HWND hwnd = reinterpret_cast<HWND>(source_id);
  std::wstring exe_path = GetPathByWindowId(hwnd);
  std::wstring file_name = FileNameFromPath(exe_path);
  std::transform(file_name.begin(), file_name.end(), file_name.begin(),
                 std::towupper);

  if (file_name == L"POWERPNT.EXE") {
    result = std::make_unique<FullScreenPowerPointHandler>(source_id);
  } else if (file_name == L"SOFFICE.BIN" &&
             absl::EndsWith(WindowText(hwnd), "OpenOffice Impress")) {
    result = std::make_unique<OpenOfficeApplicationHandler>(source_id);
  }

  return result;
}

}  // namespace webrtc
