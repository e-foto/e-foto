/**************************************************************************
PathUtils.cpp
**************************************************************************/
/*Copyright 2002-2025 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PathUtils.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cwctype>
#include <deque>
#include <system_error>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <fstream>
#include <sstream>
#endif

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace PathUtils {

namespace {

// ---------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------

std::string& projectFileStorage() {
  static std::string storage;
  return storage;
}

std::filesystem::path workingDirectory() {
  std::error_code ec;
  std::filesystem::path cwd = std::filesystem::current_path(ec);
  if (ec) return std::filesystem::path();
  return cwd;
}

bool equalText(const std::filesystem::path::string_type& left,
               const std::filesystem::path::string_type& right) {
#ifdef _WIN32
  if (left.size() != right.size()) return false;
  for (size_t i = 0; i < left.size(); i++) {
    if (std::towlower(left[i]) != std::towlower(right[i])) return false;
  }
  return true;
#else
  return left == right;
#endif
}

/**
 * Adds the extended length prefix required by Windows for paths that do not
 * fit in MAX_PATH. The disk is not touched, so this is safe for paths that
 * do not exist yet.
 */
std::filesystem::path apiPath(const std::filesystem::path& path) {
#ifdef _WIN32
  std::wstring native = path.native();
  if (native.size() < 248) return path;
  if (native.compare(0, 4, L"\\\\?\\") == 0) return path;

  std::filesystem::path preferred = path;
  preferred.make_preferred();
  native = preferred.native();

  if (native.compare(0, 2, L"\\\\") == 0) {
    // UNC share: \\server\share -> \\?\UNC\server\share
    return std::filesystem::path(L"\\\\?\\UNC\\" + native.substr(2));
  }
  if (path.is_absolute()) {
    return std::filesystem::path(L"\\\\?\\" + native);
  }
  return path;
#else
  return path;
#endif
}

std::vector<std::filesystem::path> componentsWithoutRoot(
    const std::filesystem::path& path) {
  std::vector<std::filesystem::path> result;
  std::filesystem::path rootName = path.root_name();
  std::filesystem::path rootDir = path.root_directory();
  for (const std::filesystem::path& part : path) {
    if (part.empty()) continue;
    if (!rootName.empty() && part == rootName) continue;
    if (!rootDir.empty() && part == rootDir) continue;
    result.push_back(part);
  }
  return result;
}

}  // namespace

// ---------------------------------------------------------------------
// Encoding conversions
// ---------------------------------------------------------------------
#ifdef _WIN32

std::wstring toWide(const std::string& utf8OrLocal) {
  if (utf8OrLocal.empty()) return std::wstring();

  int size =
      MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8OrLocal.data(),
                          static_cast<int>(utf8OrLocal.size()), nullptr, 0);
  UINT codePage = CP_UTF8;
  if (size <= 0) {
    // Not valid UTF-8: a project written by an older version, which used the
    // local code page. Reading it this way lets the path still be found.
    codePage = CP_ACP;
    size =
        MultiByteToWideChar(codePage, 0, utf8OrLocal.data(),
                            static_cast<int>(utf8OrLocal.size()), nullptr, 0);
    if (size <= 0) {
      // Last resort (a system whose ANSI code page is UTF-8 and a corrupted
      // string): widen byte by byte so the path is not silently lost.
      return std::wstring(utf8OrLocal.begin(), utf8OrLocal.end());
    }
  }

  std::wstring result(static_cast<size_t>(size), L'\0');
  MultiByteToWideChar(codePage, 0, utf8OrLocal.data(),
                      static_cast<int>(utf8OrLocal.size()), &result[0], size);
  return result;
}

std::string toUtf8(const std::wstring& wide) {
  if (wide.empty()) return std::string();
  int size = WideCharToMultiByte(CP_UTF8, 0, wide.data(),
                                 static_cast<int>(wide.size()), nullptr, 0,
                                 nullptr, nullptr);
  if (size <= 0) return std::string();
  std::string result(static_cast<size_t>(size), '\0');
  WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()),
                      &result[0], size, nullptr, nullptr);
  return result;
}

#endif  // _WIN32

std::filesystem::path toPath(const std::string& utf8) {
#ifdef _WIN32
  return std::filesystem::path(toWide(utf8));
#else
  return std::filesystem::path(utf8);
#endif
}

std::string fromPath(const std::filesystem::path& path) {
#ifdef _WIN32
  // Directory iterators opened through a "\\?\" path report prefixed entries;
  // the prefix must never leak into the project file or into the interface.
  std::wstring native = path.native();
  if (native.compare(0, 8, L"\\\\?\\UNC\\") == 0) {
    return toUtf8(
        std::filesystem::path(L"\\\\" + native.substr(8)).generic_wstring());
  }
  if (native.compare(0, 4, L"\\\\?\\") == 0) {
    return toUtf8(std::filesystem::path(native.substr(4)).generic_wstring());
  }
  return toUtf8(path.generic_wstring());
#else
  return path.generic_string();
#endif
}

std::string fromPathNative(const std::filesystem::path& path) {
#ifdef _WIN32
  return toUtf8(path.native());
#else
  return path.native();
#endif
}

// ---------------------------------------------------------------------
// Queries
// ---------------------------------------------------------------------
bool isAbsolute(const std::string& utf8Path) {
  if (utf8Path.empty()) return false;
  return toPath(utf8Path).is_absolute();
}

bool exists(const std::string& utf8Path) {
  if (utf8Path.empty()) return false;
  std::error_code ec;
  bool result = std::filesystem::exists(apiPath(toPath(utf8Path)), ec);
  return result && !ec;
}

bool isRegularFile(const std::string& utf8Path) {
  if (utf8Path.empty()) return false;
  std::error_code ec;
  bool result = std::filesystem::is_regular_file(apiPath(toPath(utf8Path)), ec);
  return result && !ec;
}

bool isDirectory(const std::string& utf8Path) {
  if (utf8Path.empty()) return false;
  std::error_code ec;
  bool result = std::filesystem::is_directory(apiPath(toPath(utf8Path)), ec);
  return result && !ec;
}

std::string fileName(const std::string& utf8Path) {
  if (utf8Path.empty()) return std::string();
  return fromPath(toPath(utf8Path).filename());
}

std::string stem(const std::string& utf8Path) {
  if (utf8Path.empty()) return std::string();
  return fromPath(toPath(utf8Path).stem());
}

std::string parentDirectory(const std::string& utf8Path) {
  if (utf8Path.empty()) return std::string();
  return fromPath(toPath(utf8Path).parent_path());
}

std::string normalizeSeparators(const std::string& utf8Path) {
#ifdef _WIN32
  std::string result = utf8Path;
  std::replace(result.begin(), result.end(), '\\', '/');
  return result;
#else
  return utf8Path;
#endif
}

// ---------------------------------------------------------------------
// Normalization
// ---------------------------------------------------------------------
std::string absolute(const std::string& utf8Path,
                     const std::string& baseDirectory) {
  std::filesystem::path path = toPath(utf8Path);
  if (path.empty()) return std::string();

  if (path.is_relative()) {
    std::filesystem::path base;
    if (!baseDirectory.empty()) {
      base = toPath(baseDirectory);
    }
    if (base.empty()) base = workingDirectory();
    if (!base.empty()) path = base / path;
  }
  return fromPath(path.lexically_normal());
}

bool sameRoot(const std::string& utf8Left, const std::string& utf8Right) {
  std::filesystem::path left = toPath(utf8Left);
  std::filesystem::path right = toPath(utf8Right);
  return equalText(left.root_name().native(), right.root_name().native());
}

std::string relativeTo(const std::string& utf8Path,
                       const std::string& utf8BaseDirectory) {
  if (utf8Path.empty() || utf8BaseDirectory.empty()) return std::string();

  std::filesystem::path path = toPath(utf8Path).lexically_normal();
  std::filesystem::path base = toPath(utf8BaseDirectory).lexically_normal();

  // A relative path cannot be expressed relative to another root.
  if (path.is_relative() || base.is_relative()) return std::string();
  if (!sameRoot(fromPath(path), fromPath(base))) return std::string();

  std::vector<std::filesystem::path> pathParts = componentsWithoutRoot(path);
  std::vector<std::filesystem::path> baseParts = componentsWithoutRoot(base);

  size_t common = 0;
  while (common < pathParts.size() && common < baseParts.size() &&
         equalText(pathParts[common].native(), baseParts[common].native())) {
    common++;
  }

  std::filesystem::path result;
  for (size_t i = common; i < baseParts.size(); i++) result /= "..";
  for (size_t i = common; i < pathParts.size(); i++) result /= pathParts[i];

  if (result.empty()) return ".";
  return fromPath(result);
}

std::string relativeOrAbsolute(const std::string& utf8Path,
                               const std::string& utf8BaseDirectory) {
  if (utf8Path.empty()) return std::string();

  const std::string absolutePath = absolute(utf8Path, utf8BaseDirectory);
  if (utf8BaseDirectory.empty()) return absolutePath;

  const std::string relativePath = relativeTo(absolutePath, utf8BaseDirectory);
  if (relativePath.empty()) return absolutePath;
  return relativePath;
}

std::string join(const std::string& utf8Left, const std::string& utf8Right) {
  std::filesystem::path left = toPath(utf8Left);
  std::filesystem::path right = toPath(utf8Right);

  if (left.empty()) return fromPath(right.lexically_normal());
  if (right.empty()) return fromPath(left.lexically_normal());
  if (right.is_absolute()) return fromPath(right.lexically_normal());
  return fromPath((left / right).lexically_normal());
}

// ---------------------------------------------------------------------
// Project anchor
// ---------------------------------------------------------------------
void setProjectFile(const std::string& utf8ProjectFile) {
  if (utf8ProjectFile.empty()) {
    projectFileStorage().clear();
    return;
  }
  projectFileStorage() = absolute(trim(utf8ProjectFile));
}

std::string projectFile() { return projectFileStorage(); }

std::string projectDirectory() {
  const std::string file = projectFileStorage();
  if (file.empty()) return std::string();
  return fromPath(toPath(file).parent_path());
}

void clearProjectFile() { projectFileStorage().clear(); }

// ---------------------------------------------------------------------
// Image path resolution
// ---------------------------------------------------------------------
std::string resolveImagePath(const std::string& filePathTag,
                             const std::string& fileNameTag) {
  return resolveImagePath(filePathTag, fileNameTag, projectDirectory());
}

std::string resolveImagePath(const std::string& filePathTag,
                             const std::string& fileNameTag,
                             const std::string& baseDirectory) {
  std::filesystem::path fileNamePath = toPath(trim(fileNameTag));
  if (fileNamePath.empty()) return std::string();

  // A <fileName> that already carries its own absolute path wins: some old
  // projects stored the whole path there.
  if (fileNamePath.is_absolute()) {
    return fromPath(fileNamePath.lexically_normal());
  }

  std::filesystem::path filePathPath =
      toPath(trim(filePathTag)).lexically_normal();
  const bool filePathUsable = !filePathPath.empty() &&
                              filePathPath != std::filesystem::path(".") &&
                              filePathPath != std::filesystem::path("./");

  std::filesystem::path base = baseDirectory.empty()
                                   ? toPath(projectDirectory())
                                   : toPath(baseDirectory);
  if (base.empty()) base = workingDirectory();

  std::vector<std::filesystem::path> candidates;
  if (filePathUsable) {
    std::filesystem::path directory;
    if (filePathPath.is_absolute()) {
      directory = filePathPath;
    } else if (!base.empty()) {
      directory = base / filePathPath;
    }
    if (!directory.empty()) candidates.push_back(directory / fileNamePath);
  } else if (!base.empty()) {
    candidates.push_back(base / fileNamePath);
  }

  // The image may sit right next to the project even when <filePath> is stale.
  if (!base.empty()) candidates.push_back(base / fileNamePath);

  // Compatibility with projects written by older versions, which were resolved
  // against the process working directory.
  std::filesystem::path workingDir = workingDirectory();
  if (!workingDir.empty()) {
    if (filePathUsable && filePathPath.is_relative()) {
      candidates.push_back(workingDir / filePathPath / fileNamePath);
    }
    candidates.push_back(workingDir / fileNamePath);
  }

  if (candidates.empty()) candidates.push_back(fileNamePath);

  std::vector<std::filesystem::path> checked;
  for (const std::filesystem::path& candidate : candidates) {
    std::filesystem::path normalized = candidate.lexically_normal();
    const std::string key = fromPath(normalized);
    if (std::find(checked.begin(), checked.end(), normalized) != checked.end())
      continue;
    checked.push_back(normalized);
    if (exists(key)) return absolute(key);
  }

  return absolute(fromPath(candidates.front()));
}

std::string findFileByName(const std::string& utf8RootDirectory,
                           const std::string& utf8FileName, int maxDepth,
                           int maxEntries) {
  std::filesystem::path root = toPath(trim(utf8RootDirectory));
  std::filesystem::path wanted = toPath(trim(utf8FileName)).filename();
  if (root.empty() || wanted.empty() || maxDepth < 0 || maxEntries <= 0)
    return std::string();
  if (!isDirectory(fromPath(root))) return std::string();

  std::deque<std::pair<std::filesystem::path, int>> pending;
  pending.emplace_back(root, 0);
  int visited = 0;

  while (!pending.empty() && visited < maxEntries) {
    std::filesystem::path directory = pending.front().first;
    const int depth = pending.front().second;
    pending.pop_front();

    std::error_code ec;
    std::filesystem::directory_iterator it(
        apiPath(directory),
        std::filesystem::directory_options::skip_permission_denied, ec);
    if (ec) continue;
    for (const std::filesystem::directory_entry& entry : it) {
      if (++visited >= maxEntries) break;
      std::error_code entryError;
      if (entry.is_directory(entryError)) {
        if (depth < maxDepth) pending.emplace_back(entry.path(), depth + 1);
      } else if (entry.is_regular_file(entryError)) {
        if (equalText(entry.path().filename().native(), wanted.native())) {
          return fromPath(entry.path());
        }
      }
    }
  }
  return std::string();
}

std::map<std::string, std::string> indexFilesByName(
    const std::string& utf8RootDirectory, int maxDepth, int maxEntries) {
  std::map<std::string, std::string> index;
  std::filesystem::path root = toPath(trim(utf8RootDirectory));
  if (root.empty() || maxDepth < 0 || maxEntries <= 0) return index;
  if (!isDirectory(fromPath(root))) return index;

  std::deque<std::pair<std::filesystem::path, int>> pending;
  pending.emplace_back(root, 0);
  int visited = 0;

  while (!pending.empty() && visited < maxEntries) {
    std::filesystem::path directory = pending.front().first;
    const int depth = pending.front().second;
    pending.pop_front();

    std::error_code ec;
    std::filesystem::directory_iterator it(
        apiPath(directory),
        std::filesystem::directory_options::skip_permission_denied, ec);
    if (ec) continue;
    for (const std::filesystem::directory_entry& entry : it) {
      if (++visited >= maxEntries) break;
      std::error_code entryError;
      if (entry.is_directory(entryError)) {
        if (depth < maxDepth) pending.emplace_back(entry.path(), depth + 1);
      } else if (entry.is_regular_file(entryError)) {
        const std::string key = asciiLower(fromPath(entry.path().filename()));
        // The first occurrence wins (breadth first, shallow paths first).
        if (index.find(key) == index.end()) index[key] = fromPath(entry.path());
      }
    }
  }
  return index;
}

std::string asciiLower(const std::string& text) {
  std::string result = text;
  for (char& c : result) {
    if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
  }
  return result;
}

// ---------------------------------------------------------------------
// XML helpers
// ---------------------------------------------------------------------
std::string xmlEscape(const std::string& text) {
  std::string result;
  result.reserve(text.size() + 16);
  for (char c : text) {
    switch (c) {
      case '&':
        result += "&amp;";
        break;
      case '<':
        result += "&lt;";
        break;
      case '>':
        result += "&gt;";
        break;
      case '"':
        result += "&quot;";
        break;
      case '\'':
        result += "&apos;";
        break;
      default:
        result.push_back(c);
        break;
    }
  }
  return result;
}

std::string xmlUnescape(const std::string& text) {
  std::string result;
  result.reserve(text.size());
  for (size_t i = 0; i < text.size(); i++) {
    if (text[i] != '&') {
      result.push_back(text[i]);
      continue;
    }
    const size_t limit = std::min(text.size(), i + 8);
    const size_t semicolon = text.find(';', i + 1);
    if (semicolon == std::string::npos || semicolon >= limit) {
      result.push_back(text[i]);
      continue;
    }
    const std::string entity = text.substr(i + 1, semicolon - i - 1);
    if (entity == "amp") {
      result.push_back('&');
    } else if (entity == "lt") {
      result.push_back('<');
    } else if (entity == "gt") {
      result.push_back('>');
    } else if (entity == "quot") {
      result.push_back('"');
    } else if (entity == "apos") {
      result.push_back('\'');
    } else if (!entity.empty() && entity[0] == '#') {
      // Numeric reference: &#NN; or &#xHH;
      unsigned long code = 0;
      const bool hex =
          entity.size() > 1 && (entity[1] == 'x' || entity[1] == 'X');
      const std::string digits = entity.substr(hex ? 2 : 1);
      if (!digits.empty() &&
          std::all_of(digits.begin(), digits.end(), [hex](char c) {
            return hex ? std::isxdigit(static_cast<unsigned char>(c)) != 0
                       : std::isdigit(static_cast<unsigned char>(c)) != 0;
          })) {
        code = std::strtoul(digits.c_str(), nullptr, hex ? 16 : 10);
      }
      if (code > 0 && code < 0x80) {
        result.push_back(static_cast<char>(code));
      } else {
        // Leave any non ASCII reference untouched: it is already valid XML.
        result.append(text, i, semicolon - i + 1);
      }
    } else {
      result.append(text, i, semicolon - i + 1);
    }
    i = semicolon;
  }
  return result;
}

std::string trim(const std::string& text) {
  size_t begin = 0;
  size_t end = text.size();
  while (begin < end && static_cast<unsigned char>(text[begin]) <= ' ') begin++;
  while (end > begin && static_cast<unsigned char>(text[end - 1]) <= ' ') end--;
  return text.substr(begin, end - begin);
}

// ---------------------------------------------------------------------
// File input/output
// ---------------------------------------------------------------------
bool readFile(const std::string& utf8Path, std::string& contents) {
  contents.clear();
  if (utf8Path.empty()) return false;

#ifdef _WIN32
  const std::filesystem::path path = apiPath(toPath(utf8Path));
  HANDLE handle =
      CreateFileW(path.native().c_str(), GENERIC_READ,
                  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                  nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (handle == INVALID_HANDLE_VALUE) return false;

  LARGE_INTEGER size;
  if (GetFileSizeEx(handle, &size) && size.QuadPart > 0) {
    contents.reserve(static_cast<size_t>(size.QuadPart));
  }

  char buffer[65536];
  bool ok = true;
  for (;;) {
    DWORD read = 0;
    if (!ReadFile(handle, buffer, sizeof(buffer), &read, nullptr)) {
      ok = false;
      break;
    }
    if (read == 0) break;
    contents.append(buffer, read);
  }
  CloseHandle(handle);
  return ok;
#else
  std::ifstream input(utf8Path.c_str(), std::ios::binary);
  if (!input.is_open()) return false;
  std::ostringstream buffer;
  buffer << input.rdbuf();
  contents = buffer.str();
  return true;
#endif
}

bool writeFile(const std::string& utf8Path, const std::string& contents) {
  if (utf8Path.empty()) return false;

#ifdef _WIN32
  const std::filesystem::path path = apiPath(toPath(utf8Path));
  HANDLE handle = CreateFileW(path.native().c_str(), GENERIC_WRITE, 0, nullptr,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (handle == INVALID_HANDLE_VALUE) return false;

  const char* data = contents.data();
  size_t remaining = contents.size();
  bool ok = true;
  while (remaining > 0) {
    const DWORD chunk =
        static_cast<DWORD>(std::min<size_t>(remaining, 1u << 20));
    DWORD written = 0;
    if (!WriteFile(handle, data, chunk, &written, nullptr) || written == 0) {
      ok = false;
      break;
    }
    data += written;
    remaining -= written;
  }
  CloseHandle(handle);
  return ok;
#else
  std::ofstream output(utf8Path.c_str(), std::ios::binary | std::ios::trunc);
  if (!output.is_open()) return false;
  output.write(contents.data(), static_cast<std::streamsize>(contents.size()));
  return output.good();
#endif
}

std::string forFileApi(const std::string& utf8Path) {
  if (utf8Path.empty()) return std::string();
  return fromPathNative(apiPath(toPath(utf8Path)));
}

// ---------------------------------------------------------------------
// UTF-8 safe file streams
// ---------------------------------------------------------------------

std::FILE* openFile(const std::string& utf8Path, const char* mode) {
  if (utf8Path.empty() || mode == nullptr || mode[0] == '\0') return nullptr;

#ifdef _WIN32
  // fopen()/_wfopen() are the only two ways to ask the CRT for a handle; the
  // wide one is the only one that does not go through the local code page.
  const std::filesystem::path path = apiPath(toPath(utf8Path));
  std::wstring wideMode;
  for (const char* c = mode; *c != '\0'; c++) {
    wideMode.push_back(static_cast<wchar_t>(*c));
  }
  return ::_wfopen(path.native().c_str(), wideMode.c_str());
#else
  return std::fopen(utf8Path.c_str(), mode);
#endif
}

/**
 * Shared streambuf: writes go straight to the FILE, reads are buffered.
 * Keeping it on the heap lets the public classes stay copy-free and keeps
 * stdio.h out of PathUtils.h.
 */
class OutputFileStream::Buffer : public std::streambuf {
 public:
  Buffer() = default;
  ~Buffer() override { close(); }

  void attach(std::FILE* file) { file_ = file; }
  bool isOpen() const { return file_ != nullptr; }

  void close() {
    if (file_ == nullptr) return;
    std::fflush(file_);
    std::fclose(file_);
    file_ = nullptr;
  }

 protected:
  int_type overflow(int_type ch) override {
    if (file_ == nullptr) return traits_type::eof();
    if (!traits_type::eq_int_type(ch, traits_type::eof())) {
      if (std::fputc(traits_type::to_char_type(ch), file_) == EOF) {
        return traits_type::eof();
      }
    }
    return traits_type::not_eof(ch);
  }

  std::streamsize xsputn(const char* data, std::streamsize count) override {
    if (file_ == nullptr || count <= 0) return 0;
    return static_cast<std::streamsize>(
        std::fwrite(data, 1, static_cast<size_t>(count), file_));
  }

  int sync() override {
    if (file_ == nullptr) return -1;
    return std::fflush(file_) == 0 ? 0 : -1;
  }

 private:
  std::FILE* file_ = nullptr;
};

OutputFileStream::OutputFileStream(const std::string& utf8Path, bool binary)
    : std::ostream(nullptr), buffer_(new Buffer()) {
  std::FILE* file = openFile(utf8Path, binary ? "wb" : "w");
  buffer_->attach(file);
  init(buffer_);
  // Same contract as std::ofstream: the caller checks is_open() or fail().
  if (file == nullptr) setstate(std::ios::failbit);
}

OutputFileStream::~OutputFileStream() { delete buffer_; }

bool OutputFileStream::isOpen() const {
  return buffer_ != nullptr && buffer_->isOpen();
}

void OutputFileStream::close() {
  if (buffer_ == nullptr || !buffer_->isOpen()) return;
  if (good()) flush();
  buffer_->close();
}

class InputFileStream::Buffer : public std::streambuf {
 public:
  Buffer() = default;
  ~Buffer() override { close(); }

  void attach(std::FILE* file) {
    file_ = file;
    setg(data_, data_, data_);
  }
  bool isOpen() const { return file_ != nullptr; }

  void close() {
    if (file_ == nullptr) return;
    std::fclose(file_);
    file_ = nullptr;
  }

 protected:
  int_type underflow() override {
    if (file_ == nullptr) return traits_type::eof();
    if (gptr() < egptr()) return traits_type::to_int_type(*gptr());

    const size_t read = std::fread(data_, 1, sizeof(data_), file_);
    if (read == 0) return traits_type::eof();
    setg(data_, data_, data_ + read);
    return traits_type::to_int_type(*gptr());
  }

  std::streamsize xsgetn(char* data, std::streamsize count) override {
    if (file_ == nullptr || count <= 0) return 0;

    std::streamsize remaining = count;
    while (remaining > 0 && gptr() < egptr()) {
      *data++ = *gptr();
      gbump(1);
      remaining--;
    }
    if (remaining > 0) {
      const size_t read =
          std::fread(data, 1, static_cast<size_t>(remaining), file_);
      remaining -= static_cast<std::streamsize>(read);
    }
    return count - remaining;
  }

 private:
  std::FILE* file_ = nullptr;
  char data_[8192];
};

InputFileStream::InputFileStream(const std::string& utf8Path, bool binary)
    : std::istream(nullptr), buffer_(new Buffer()) {
  std::FILE* file = openFile(utf8Path, binary ? "rb" : "r");
  buffer_->attach(file);
  init(buffer_);
  if (file == nullptr) setstate(std::ios::failbit);
}

InputFileStream::~InputFileStream() { delete buffer_; }

bool InputFileStream::isOpen() const {
  return buffer_ != nullptr && buffer_->isOpen();
}

void InputFileStream::close() {
  if (buffer_ != nullptr) buffer_->close();
}

}  // namespace PathUtils
}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
