/**************************************************************************
ProjectPathTest.cpp
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
 * Integration test of the project file path handling: the path is escaped,
 * written to disk, read back, parsed by the real EDom parser exactly as
 * Image::xmlSetData() does, unescaped and resolved against the project
 * directory.
 *
 *   g++ -std=c++17 tests/ProjectPathTest.cpp c/infrastructure/PathUtils.cpp \
 *       c/infrastructure/EDom.cpp -Ic/infrastructure -o project_path_test
 *   ./project_path_test
 */

#include <cstdio>
#include <filesystem>
#include <string>

#include "../c/infrastructure/EDom.h"
#include "../c/infrastructure/PathUtils.h"

using br::uerj::eng::efoto::EDomElement;
using namespace br::uerj::eng::efoto::PathUtils;

namespace {

int failures = 0;
int checks = 0;

void check(bool condition, const std::string& what, const std::string& detail = "") {
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

#ifdef _WIN32
std::string wideToUtf8(const std::wstring& wide) {
  return br::uerj::eng::efoto::PathUtils::toUtf8(wide);
}
#else
std::string wideToUtf8(const std::wstring& wide) {
  std::string result;
  for (wchar_t character : wide) {
    unsigned int code = static_cast<unsigned int>(character);
    if (code < 0x80) {
      result.push_back(static_cast<char>(code));
    } else if (code < 0x800) {
      result.push_back(static_cast<char>(0xC0 | (code >> 6)));
      result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    } else if (code < 0x10000) {
      result.push_back(static_cast<char>(0xE0 | (code >> 12)));
      result.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
      result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    } else {
      result.push_back(static_cast<char>(0xF0 | (code >> 18)));
      result.push_back(static_cast<char>(0x80 | ((code >> 12) & 0x3F)));
      result.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
      result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    }
  }
  return result;
}
#endif

std::string makeDirectory(const std::string& parent, const std::string& name) {
  const std::string full = join(parent, name);
  std::error_code ec;
  std::filesystem::create_directories(toPath(forFileApi(full)), ec);
  return full;
}

/**
 * Reproduces what the software does with a project file: build the XML with
 * the paths escaped, save it, load it, parse it and resolve the image.
 */
std::string roundTrip(const std::string& projectFile,
                      const std::string& imageName,
                      const std::string& storedPath) {
  std::string xml;
  xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  xml += "<efotoPhotogrammetricProject version=\"1.0.42\">\n";
  xml += "<images>\n";
  xml += "<image key=\"1\">\n";
  xml += "<fileName>" + xmlEscape(imageName) + "</fileName>\n";
  xml += "<filePath>" + xmlEscape(storedPath) + "</filePath>\n";
  xml += "</image>\n";
  xml += "</images>\n";
  xml += "</efotoPhotogrammetricProject>\n";

  // ProjectManager::saveFile() and ::loadFile()
  check(writeFile(projectFile, xml), "project file written", projectFile);
  std::string stored;
  check(readFile(projectFile, stored), "project file read", projectFile);

  // ProjectManager::loadFile() removes the blank lines before parsing.
  EDomElement cleaned(stored);
  const std::string normalized = cleaned.removeBlankLines(true).getContent();

  EDomElement root(normalized);
  EDomElement images(root.elementByTagName("images"));
  std::deque<EDomElement> imageList = images.elementsByTagName("image");
  if (imageList.size() != 1) {
    check(false, "one image parsed");
    return std::string();
  }

  // Image::xmlSetData()
  const std::string fileNameTag =
      xmlUnescape(trim(imageList.at(0).elementByTagName("fileName").toString()));
  const std::string filePathTag =
      xmlUnescape(trim(imageList.at(0).elementByTagName("filePath").toString()));

  checkEqual(fileNameTag, imageName, "fileName survives the XML round trip");
  checkEqual(filePathTag, storedPath, "filePath survives the XML round trip");

  setProjectFile(projectFile);
  return resolveImagePath(filePathTag, fileNameTag);
}

}  // namespace

int main() {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  std::error_code ec;

  const std::string root =
      join(fromPath(std::filesystem::current_path(ec)), "project_path_test");
  std::filesystem::remove_all(toPath(forFileApi(root)), ec);
  makeDirectory(fromPath(std::filesystem::current_path(ec)), "project_path_test");

  // A folder and file names with Chinese characters, spaces and '&': before
  // the fix the '&' alone made the whole project unreadable.
  const std::string directory =
      makeDirectory(root, wideToUtf8(L"测区 A & B #2 100%"));
  const std::string imageName = wideToUtf8(L"相片 01 & 02.tif");
  const std::string imagePath = join(directory, imageName);
  check(writeFile(imagePath, "IMAGE"), "image written", imagePath);

  const std::string projectFile = join(root, wideToUtf8(L"工程 & 一号.epp"));

  // (1) <filePath> relative to the project directory.
  checkEqual(roundTrip(projectFile, imageName, wideToUtf8(L"测区 A & B #2 100%")),
             imagePath, "relative image path with special characters");

  // (2) <filePath> is ".": the image sits next to the project file.
  const std::string besideProject = join(root, imageName);
  check(writeFile(besideProject, "IMAGE"), "image beside project written");
  checkEqual(roundTrip(projectFile, imageName, "."), besideProject,
             "image next to the project file");

  // (3) <fileName> carrying the whole path, as written by old buggy versions.
  checkEqual(roundTrip(projectFile, imagePath, ""), imagePath,
             "legacy fileName with the whole path");

  std::filesystem::remove_all(toPath(forFileApi(root)), ec);

  std::printf("%d checks, %d failure(s)\n", checks, failures);
  return failures == 0 ? 0 : 1;
}
