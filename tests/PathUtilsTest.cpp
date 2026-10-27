/**************************************************************************
PathUtilsTest.cpp
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

/*
 * Standalone regression test for PathUtils (no Qt, no GDAL).
 *
 *   g++ -std=c++17 -Wall -Wextra tests/PathUtilsTest.cpp \
 *       c/infrastructure/PathUtils.cpp -o pathutils_test
 *   ./pathutils_test
 *
 * It exercises the cases that used to break the project loading:
 * non ASCII (Chinese) names, names with spaces and XML special characters,
 * paths longer than MAX_PATH, and image folders placed on another drive
 * than the project file.
 */

#include <cstdio>
#include <filesystem>
#include <string>

#include "../c/infrastructure/PathUtils.h"

#ifdef _WIN32
#include <windows.h>
#endif

using br::uerj::eng::efoto::PathUtils::absolute;
using br::uerj::eng::efoto::PathUtils::exists;
using br::uerj::eng::efoto::PathUtils::findFileByName;
using br::uerj::eng::efoto::PathUtils::forFileApi;
using br::uerj::eng::efoto::PathUtils::fromPath;
using br::uerj::eng::efoto::PathUtils::indexFilesByName;
using br::uerj::eng::efoto::PathUtils::join;
using br::uerj::eng::efoto::PathUtils::projectDirectory;
using br::uerj::eng::efoto::PathUtils::readFile;
using br::uerj::eng::efoto::PathUtils::relativeOrAbsolute;
using br::uerj::eng::efoto::PathUtils::relativeTo;
using br::uerj::eng::efoto::PathUtils::resolveImagePath;
using br::uerj::eng::efoto::PathUtils::sameRoot;
using br::uerj::eng::efoto::PathUtils::setProjectFile;
using br::uerj::eng::efoto::PathUtils::toPath;
using br::uerj::eng::efoto::PathUtils::writeFile;
using br::uerj::eng::efoto::PathUtils::xmlEscape;
using br::uerj::eng::efoto::PathUtils::xmlUnescape;

namespace {

int failures = 0;
int checks = 0;

void check(bool condition, const std::string& what,
           const std::string& detail = "") {
  checks++;
  if (condition) return;
  failures++;
  std::printf("FAIL: %s", what.c_str());
  if (!detail.empty()) std::printf("  [%s]", detail.c_str());
  std::printf("\n");
}

void checkEqual(const std::string& actual, const std::string& expected,
                const std::string& what) {
  checks++;
  if (actual == expected) return;
  failures++;
  std::printf("FAIL: %s\n  expected: %s\n  actual  : %s\n", what.c_str(),
              expected.c_str(), actual.c_str());
}

std::string utf8(const std::wstring& wide) {
#ifdef _WIN32
  return br::uerj::eng::efoto::PathUtils::toUtf8(wide);
#else
  return std::string(wide.begin(), wide.end());
#endif
}

std::string makeDirectory(const std::string& parent, const std::string& name) {
  const std::string full = join(parent, name);
  std::error_code ec;
  std::filesystem::create_directories(
      br::uerj::eng::efoto::PathUtils::toPath(
          br::uerj::eng::efoto::PathUtils::forFileApi(full)),
      ec);
  return full;
}

#ifdef _WIN32
// std::filesystem::remove_all() loops forever on trees that go beyond
// MAX_PATH with this runtime, so the test deletes its own tree with the
// Win32 calls that accept the extended length prefix.
void removeTree(const std::wstring& directory) {
  WIN32_FIND_DATAW entry;
  HANDLE search = FindFirstFileW((directory + L"\\*").c_str(), &entry);
  if (search != INVALID_HANDLE_VALUE) {
    do {
      if (wcscmp(entry.cFileName, L".") == 0 ||
          wcscmp(entry.cFileName, L"..") == 0)
        continue;
      const std::wstring child = directory + L"\\" + entry.cFileName;
      if ((entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        removeTree(child);
      } else {
        DeleteFileW(child.c_str());
      }
    } while (FindNextFileW(search, &entry));
    FindClose(search);
  }
  RemoveDirectoryW(directory.c_str());
}
#endif

void removeTree(const std::string& path) {
#ifdef _WIN32
  std::filesystem::path preferred =
      br::uerj::eng::efoto::PathUtils::toPath(path);
  preferred.make_preferred();
  std::wstring native = preferred.native();
  // The prefix is added to the whole tree, so the recursion below keeps
  // working once the nested paths exceed MAX_PATH.
  if (native.rfind(L"\\\\?\\", 0) != 0 && preferred.is_absolute()) {
    native = L"\\\\?\\" + native;
  }
  removeTree(native);
#else
  std::error_code ec;
  std::filesystem::remove_all(path, ec);
#endif
}

void writeText(const std::string& path, const std::string& text) {
  check(writeFile(path, text), "writeFile " + path);
  check(exists(path), "exists after writeFile " + path);
}

// The exporters, the reports and the DEM/ortho writers used std::ofstream and
// fopen() with a UTF-8 std::string. Those do not accept non ASCII paths on
// Windows, so the output silently disappeared; these are the replacements.
void testFileStreams(const std::string& root) {
  using br::uerj::eng::efoto::PathUtils::InputFileStream;
  using br::uerj::eng::efoto::PathUtils::openFile;
  using br::uerj::eng::efoto::PathUtils::OutputFileStream;

  const std::string directory = makeDirectory(root, utf8(L"导出报告"));

  // Text output, exactly what the report/feature/point exporters do.
  const std::string report = join(directory, utf8(L"摄影测量报告.txt"));
  {
    OutputFileStream out(report);
    check(out.isOpen(), "OutputFileStream opens a non ASCII path");
    out << "line one\n" << 42 << " " << 3.5 << "\n";
    out.close();
  }
  std::string contents;
  check(readFile(report, contents), "OutputFileStream creates the file");
  check(contents.find("line one") != std::string::npos,
        "OutputFileStream writes the text", contents);
  check(contents.find("42 3.5") != std::string::npos,
        "OutputFileStream writes formatted values", contents);

  // Binary output: the bytes must be written verbatim.
  {
    OutputFileStream out(report, true);
    out << "raw\n";
  }
  check(readFile(report, contents), "OutputFileStream binary reopens");
  checkEqual(contents, "raw\n", "binary stream writes exact bytes");

  {
    InputFileStream in(report, true);
    check(in.isOpen(), "InputFileStream opens a non ASCII path");
    std::string firstLine;
    std::getline(in, firstLine);
    checkEqual(firstLine, "raw", "InputFileStream reads the text");
  }

  // Binary round trip, the way the DSM and the ortho-image are stored.
  const std::string grid = join(directory, "dem.dsm");
  {
    OutputFileStream out(grid, true);
    const double header[3] = {1.5, -2.25, 0.001};
    out.write(reinterpret_cast<const char*>(header), sizeof(header));
  }
  {
    InputFileStream in(grid, true);
    double back[3] = {0.0, 0.0, 0.0};
    in.read(reinterpret_cast<char*>(back), sizeof(back));
    check(back[0] == 1.5 && back[1] == -2.25 && back[2] == 0.001,
          "binary round trip through a non ASCII directory");
  }

  // A failure has to be visible: the old code checked nothing on the fopen()
  // paths and crashed on the binary writers.
  const std::string missing = join(directory, "no_such_folder/report.txt");
  {
    OutputFileStream out(missing);
    check(!out.isOpen(), "OutputFileStream reports a missing directory");
    check(out.fail(), "OutputFileStream sets failbit when it cannot open");
  }
  {
    InputFileStream in(missing);
    check(!in.isOpen(), "InputFileStream reports a missing directory");
    check(in.fail(), "InputFileStream sets failbit when it cannot open");
  }
  check(openFile(missing, "wb") == nullptr,
        "openFile returns NULL for a missing directory");
  std::FILE* readable = openFile(report, "rb");
  check(readable != nullptr, "openFile opens an existing file");
  if (readable != nullptr) std::fclose(readable);
}

void checkExistsThroughApi(const std::string& path);

void testChineseAndSpecialCharacters(const std::string& root) {
  // A directory and a file name with Chinese characters, spaces and the
  // characters that used to corrupt the project XML.
  const std::string directory = makeDirectory(root, utf8(L"项目 资料 & #1 %"));
  const std::string imageName = utf8(L"相片 A&B '01' #1 %x.tif");
  const std::string imagePath = join(directory, imageName);
  writeText(imagePath, "IMAGE");

  check(exists(imagePath), "Chinese/special image exists");
  checkEqual(relativeTo(imagePath, directory), imageName,
             "relativeTo keeps non ASCII file name");

  // XML round trip. '<', '>' and '"' are illegal in Windows file names but a
  // project may come from Linux, so escaping must handle them as well.
  const std::string tricky = "/data/" + utf8(L"相片 A&B") + "<01>\"x\".tif";
  const std::string escaped = xmlEscape(tricky);
  check(escaped.find('<') == std::string::npos, "xmlEscape removes '<'");
  check(escaped.find("&amp;") != std::string::npos, "xmlEscape escapes '&'");
  checkEqual(xmlUnescape(escaped), tricky, "xmlEscape/xmlUnescape round trip");
  checkEqual(xmlUnescape(xmlEscape(imagePath)), imagePath,
             "xmlEscape/xmlUnescape round trip on a real path");

  // Resolving <filePath>/<fileName> as stored in the project: the project sits
  // in root, so the image folder is stored relative to it.
  const std::string projectFile = join(root, utf8(L"工程 &.epp"));
  writeText(projectFile, "<project/>");
  setProjectFile(projectFile);
  checkEqual(projectDirectory(), root, "projectDirectory");

  const std::string resolved =
      resolveImagePath(utf8(L"项目 资料 & #1 %"), imageName);
  checkEqual(resolved, imagePath, "resolveImagePath with non ASCII names");

  // Legacy project: <filePath> is "." and the image sits next to the project
  checkEqual(resolveImagePath(".", imageName, directory), imagePath,
             "resolveImagePath with filePath='.'");
}

void testAbsoluteAndRelative(const std::string& root) {
  const std::string base = makeDirectory(root, "project");
  const std::string sibling = makeDirectory(root, "images");
  const std::string image = join(sibling, "photo.tif");
  writeText(image, "X");

  // Relative path that needs ".." (image folder next to the project folder).
  checkEqual(relativeTo(image, base), "../images/photo.tif",
             "relativeTo handles '..'");
  checkEqual(relativeOrAbsolute(image, base), "../images/photo.tif",
             "relativeOrAbsolute keeps a portable relative path");
  checkEqual(absolute(relativeTo(image, base), base), image,
             "relative path resolves back to the absolute path");

  // Same directory.
  checkEqual(relativeTo(join(base, "photo.tif"), base), "photo.tif",
             "relativeTo same directory");
  checkEqual(relativeOrAbsolute(base, base), ".",
             "relativeOrAbsolute same dir");

  // Absolute <filePath> is honoured.
  checkEqual(resolveImagePath(sibling, "photo.tif"), image,
             "resolveImagePath with absolute filePath");

  // Stale <filePath> but the file is next to the project (project moved).
  const std::string besideProject = join(base, "photo.tif");
  writeText(besideProject, "X");
  checkEqual(resolveImagePath("../moved_away", "photo.tif", base),
             besideProject,
             "resolveImagePath falls back to the project directory");
}

void testCrossDrive(const std::string& root) {
  std::string otherRoot;
#ifdef _WIN32
  for (wchar_t letter = L'C'; letter <= L'Z'; letter++) {
    std::wstring candidate;
    candidate.push_back(letter);
    candidate += L":\\";
    const UINT type = GetDriveTypeW(candidate.c_str());
    if (type != DRIVE_FIXED) continue;
    const std::string asUtf8 = utf8(candidate);
    if (!sameRoot(asUtf8, root)) {
      otherRoot = asUtf8;
      break;
    }
  }
#endif
  if (otherRoot.empty()) {
    std::printf("SKIP: cross drive test (no second fixed drive)\n");
    return;
  }

  const std::string target = join(otherRoot, "efoto_cross_drive/image.tif");
  // A relative path cannot express another drive: an absolute path must be
  // returned instead of an empty string.
  checkEqual(relativeTo(target, root), std::string(),
             "relativeTo returns empty for another drive");
  checkEqual(relativeOrAbsolute(target, root), target,
             "relativeOrAbsolute keeps absolute path for another drive");
  checkEqual(absolute(target), target, "absolute keeps path");
}

void testLongPath(const std::string& root) {
  // Build a path deeper than the legacy 260 characters limit.
  std::string directory = makeDirectory(root, "long");
  std::string deepest;
  const std::string levelName = "level_0123456789_0123456789_0123456789_";
  for (int i = 0; i < 5; i++) {
    directory = makeDirectory(directory, levelName + std::to_string(i));
    deepest = directory;
  }
  const std::string image = join(deepest, "photo_0123456789_0123456789.tif");

  if (image.size() < 275) {
    std::printf("SKIP: long path test (path has only %zu characters)\n",
                image.size());
    return;
  }

  writeText(image, "LONG");
  std::string contents;
  check(readFile(image, contents), "readFile long path");
  checkEqual(contents, "LONG", "long path content");
  const std::string relativeDirectory = relativeTo(deepest, root);
  check(!relativeDirectory.empty(), "relativeTo deep path");
  checkEqual(resolveImagePath(relativeDirectory,
                              "photo_0123456789_0123456789.tif", root),
             image, "resolveImagePath long path");
  checkExistsThroughApi(image);

  // Directory scans open the folder through its extended path; the values they
  // return must not carry the "\\?\" prefix.
  checkEqual(indexFilesByName(deepest)["photo_0123456789_0123456789.tif"],
             image, "indexFilesByName long path");
  checkEqual(findFileByName(deepest, "photo_0123456789_0123456789.tif"), image,
             "findFileByName long path");

  // The exporters reach the same path through the stream wrappers.
  {
    br::uerj::eng::efoto::PathUtils::OutputFileStream out(image, true);
    check(out.isOpen(), "OutputFileStream long path");
    out << "LONG2";
  }
  std::string streamed;
  check(readFile(image, streamed), "readFile after OutputFileStream");
  checkEqual(streamed, "LONG2", "OutputFileStream long path content");
}

void checkExistsThroughApi(const std::string& path) {
  // GDAL / Qt receive the value of forFileApi(): on Windows the extended
  // length prefix must be present for paths longer than MAX_PATH.
  const std::string api = forFileApi(path);
#ifdef _WIN32
  if (path.size() >= 248) {
    check(api.rfind("\\\\?\\", 0) == 0,
          "forFileApi prefixes long absolute paths", api);
  }
  HANDLE handle =
      CreateFileW(toPath(api).native().c_str(), GENERIC_READ, FILE_SHARE_READ,
                  nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  check(handle != INVALID_HANDLE_VALUE, "CreateFileW on long path", api);
  if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle);
#else
  check(exists(api), "forFileApi path exists");
#endif
}

void testFindFileByName(const std::string& root) {
  const std::string base = makeDirectory(root, "search");
  const std::string deep = makeDirectory(makeDirectory(base, "a"), "b");
  const std::string image = join(deep, utf8(L"深层照片.tif"));
  writeText(image, "F");

  checkEqual(findFileByName(base, utf8(L"深层照片.tif")), image,
             "findFileByName finds a nested non ASCII file");
  checkEqual(findFileByName(base, "missing.tif"), std::string(),
             "findFileByName reports a missing file");
}

void testLegacyCodePage(const std::string& root) {
#ifdef _WIN32
  if (GetACP() != 936) {
    std::printf("SKIP: legacy code page test (ACP is not 936)\n");
    return;
  }
  const std::string directory = makeDirectory(root, utf8(L"旧工程"));
  const std::string image = join(directory, utf8(L"老照片.tif"));
  writeText(image, "OLD");

  // Older e-foto versions wrote the local code page (GBK) into the project.
  const std::wstring wide = toPath(utf8(L"老照片.tif")).native();
  int size = WideCharToMultiByte(936, 0, wide.c_str(), -1, nullptr, 0, nullptr,
                                 nullptr);
  std::string gbk(static_cast<size_t>(size > 0 ? size - 1 : 0), '\0');
  if (size > 1) {
    WideCharToMultiByte(936, 0, wide.c_str(), -1, &gbk[0], size, nullptr,
                        nullptr);
  }
  const std::string gbkDirectory = [] {
    const std::wstring wideDirectory = toPath(utf8(L"旧工程")).native();
    int length = WideCharToMultiByte(936, 0, wideDirectory.c_str(), -1, nullptr,
                                     0, nullptr, nullptr);
    std::string result(static_cast<size_t>(length > 0 ? length - 1 : 0), '\0');
    if (length > 1) {
      WideCharToMultiByte(936, 0, wideDirectory.c_str(), -1, &result[0], length,
                          nullptr, nullptr);
    }
    return result;
  }();
  setProjectFile(join(root, "legacy.epp"));
  checkEqual(resolveImagePath(gbkDirectory, gbk), image,
             "resolveImagePath reads legacy GBK paths");
#else
  (void)root;
#endif
}

}  // namespace

int main() {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  std::error_code ec;
  const std::string root =
      join(fromPath(std::filesystem::current_path(ec)), "pathutils_test_data");
  removeTree(root);
  std::filesystem::create_directories(
      br::uerj::eng::efoto::PathUtils::toPath(
          br::uerj::eng::efoto::PathUtils::forFileApi(root)),
      ec);
  if (!exists(root)) {
    std::printf("FAIL: cannot create test root %s\n", root.c_str());
    return 2;
  }

  std::printf("-- chinese/special characters\n");
  testChineseAndSpecialCharacters(root);
  std::printf("-- absolute/relative\n");
  testAbsoluteAndRelative(root);
  std::printf("-- cross drive\n");
  testCrossDrive(root);
  std::printf("-- long path\n");
  testLongPath(root);
  std::printf("-- find by name\n");
  testFindFileByName(root);
  std::printf("-- legacy code page\n");
  testLegacyCodePage(root);
  std::printf("-- file streams\n");
  testFileStreams(root);

  removeTree(root);

  std::printf("%d checks, %d failure(s)\n", checks, failures);
  return failures == 0 ? 0 : 1;
}
