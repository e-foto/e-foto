/**************************************************************************
PathUtils.h
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

#ifndef EFOTO_PATHUTILS_H
#define EFOTO_PATHUTILS_H

#include <cstdio>
#include <filesystem>
#include <istream>
#include <map>
#include <ostream>
#include <streambuf>
#include <string>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
 * \file PathUtils.h
 * \class PathUtils
 * \brief Centralized, encoding safe file path handling.
 *
 * All paths exchanged through the e-foto API (project XML, form fields,
 * manager interfaces) are UTF-8 encoded std::string. This namespace is the
 * single place where such strings are converted to/from the operating system
 * representation, so that the rest of the software never has to deal with:
 *
 *  - the local 8 bit code page (Chinese, Japanese, accented file names);
 *  - paths longer than MAX_PATH on Windows (the "\\\\?\\" form is applied
 *    automatically right before the file is touched);
 *  - paths that live on another drive/volume than the project file (a
 *    relative path cannot express that, so an absolute path is kept
 *    instead of silently producing an empty/wrong path).
 *
 * The XML encoding of paths is handled here too: a path containing '&',
 * '<' or '>' would otherwise corrupt the project file and make it
 * unreadable.
 */
namespace PathUtils {

// ---------------------------------------------------------------------
// Encoding conversions (Windows only; on POSIX paths are UTF-8 bytes)
// ---------------------------------------------------------------------
#ifdef _WIN32
/**
 * \brief Converts an UTF-8 string to a wide string.
 *
 * If the input is not valid UTF-8 it is interpreted as a string in the local
 * code page. This keeps projects created by older e-foto versions (which
 * wrote paths with the local code page) readable.
 */
std::wstring toWide(const std::string& utf8OrLocal);
/// \brief Converts a wide string to UTF-8.
std::string toUtf8(const std::wstring& wide);
#endif

// ---------------------------------------------------------------------
// Path <-> UTF-8 string
// ---------------------------------------------------------------------
/// \brief Builds a path from an UTF-8 string ('/' and '\' both accepted).
std::filesystem::path toPath(const std::string& utf8);
/// \brief Converts a path to an UTF-8 string using '/' as separator.
std::string fromPath(const std::filesystem::path& path);
/// \brief Converts a path to an UTF-8 string using the native separator.
std::string fromPathNative(const std::filesystem::path& path);

// ---------------------------------------------------------------------
// Queries (never throw)
// ---------------------------------------------------------------------
bool isAbsolute(const std::string& utf8Path);
bool exists(const std::string& utf8Path);
bool isRegularFile(const std::string& utf8Path);
bool isDirectory(const std::string& utf8Path);
/// \brief Final component of a path ("image.tif").
std::string fileName(const std::string& utf8Path);
/// \brief Final component without extension ("image").
std::string stem(const std::string& utf8Path);
/// \brief Parent directory ("D:/data"), empty for a bare file name.
std::string parentDirectory(const std::string& utf8Path);
/// \brief Converts both separators to '/', collapses duplicate separators.
std::string normalizeSeparators(const std::string& utf8Path);

// ---------------------------------------------------------------------
// Normalization
// ---------------------------------------------------------------------
/**
 * \brief Returns an absolute, lexically normalized path.
 * \param utf8Path     Path to resolve (may be relative).
 * \param baseDirectory Directory used for relative paths; the current
 *        working directory when empty. The disk is never accessed, so a
 *        non existing path is still expanded consistently.
 */
std::string absolute(const std::string& utf8Path,
                     const std::string& baseDirectory = std::string());
/**
 * \brief Relative path from baseDirectory to path, supporting "..".
 * \return Empty string when the two paths have different root names
 *         (different drives on Windows), because no relative path exists.
 */
std::string relativeTo(const std::string& utf8Path,
                       const std::string& utf8BaseDirectory);
/**
 * \brief Relative path when possible, absolute path otherwise.
 *
 * This is the function to use when writing a path into the project file:
 * a portable relative path is stored whenever both locations share a root,
 * otherwise the absolute path is stored instead of an unusable relative one.
 */
std::string relativeOrAbsolute(const std::string& utf8Path,
                               const std::string& utf8BaseDirectory);
/// \brief Joins two paths and normalizes the result.
std::string join(const std::string& utf8Left, const std::string& utf8Right);
/// \brief True when both paths start with the same root ("C:/", "//srv/", "/").
bool sameRoot(const std::string& utf8Left, const std::string& utf8Right);

// ---------------------------------------------------------------------
// Current project anchor
// ---------------------------------------------------------------------
/**
 * \brief Remembers the project file being handled.
 *
 * Relative image paths are resolved against the directory of the project
 * file, never against the process working directory: the software can then
 * be started from anywhere and a project can be opened by double clicking
 * it.
 */
void setProjectFile(const std::string& utf8ProjectFile);
std::string projectFile();
std::string projectDirectory();
void clearProjectFile();

// ---------------------------------------------------------------------
// Image path resolution
// ---------------------------------------------------------------------
/**
 * \brief Resolves the <filePath>/<fileName> pair of an image.
 *
 * Resolution order (the first existing candidate wins):
 *  1. an absolute <filePath>, or <fileName> that already carries a path;
 *  2. <project directory>/<filePath>/<fileName>;
 *  3. <project directory>/<fileName>   (filePath is stale or lost);
 *  4. <working directory>/<filePath>/<fileName>, <working directory>/<fileName>
 *     (projects created by older versions, which relied on the CWD).
 *
 * When nothing exists the first candidate is returned, so the caller can
 * report a meaningful path to the user.
 */
std::string resolveImagePath(const std::string& filePathTag,
                             const std::string& fileNameTag);
std::string resolveImagePath(const std::string& filePathTag,
                             const std::string& fileNameTag,
                             const std::string& baseDirectory);

/**
 * \brief Searches for a file name below a directory (bounded).
 * \param maxDepth       Maximum number of directory levels to walk.
 * \param maxEntries     Safety limit on the number of visited entries.
 * \return The first match, or an empty string when nothing was found.
 */
std::string findFileByName(const std::string& utf8RootDirectory,
                           const std::string& utf8FileName, int maxDepth = 6,
                           int maxEntries = 20000);

/**
 * \brief Indexes every file below a directory by its (lower case) name.
 *
 * Used once when a project is opened, so that many images that were moved
 * with the project are all found with a single directory walk.
 */
std::map<std::string, std::string> indexFilesByName(
    const std::string& utf8RootDirectory, int maxDepth = 6,
    int maxEntries = 20000);

/// \brief Lower case form of an ASCII string, used as index key.
std::string asciiLower(const std::string& text);

// ---------------------------------------------------------------------
// XML helpers
// ---------------------------------------------------------------------
/// \brief Escapes '&', '<', '>', '"' and '\'' so the value can be a text node.
std::string xmlEscape(const std::string& text);
/// \brief Reverts xmlEscape(); unknown entities are left untouched.
std::string xmlUnescape(const std::string& text);
/// \brief Trims spaces, tabs and line breaks (XML text nodes are indented).
std::string trim(const std::string& text);

// ---------------------------------------------------------------------
// File input/output
// ---------------------------------------------------------------------
/**
 * \brief Reads a whole file.
 *
 * The path is UTF-8 and may exceed MAX_PATH on Windows: the platform call is
 * done with the appropriate API, so no local code page conversion happens.
 */
bool readFile(const std::string& utf8Path, std::string& contents);
/// \brief Writes (truncates) a whole file, UTF-8 safe and long path safe.
bool writeFile(const std::string& utf8Path, const std::string& contents);
/**
 * \brief Prepares a path for an operating system call.
 *
 * On Windows an absolute path longer than the legacy MAX_PATH limit is
 * prefixed with "\\\\?\\" (or "\\\\?\\UNC\\" for network shares) and its
 * separators are converted to '\'. Elsewhere the path is returned as is.
 * Use it for libraries that do not handle long paths themselves (GDAL, for
 * instance).
 */
std::string forFileApi(const std::string& utf8Path);

/**
 * \brief Opens a file with a UTF-8 path, using the wide API on Windows.
 *
 * This is the replacement for fopen()/std::fopen() on any path that may come
 * from the user: those interpret the bytes with the local code page, so a
 * directory named "中文目录" cannot be opened from a UTF-8 string.
 *
 * \param mode Standard fopen() mode ("wb", "r", ...).
 * \return The stream, or nullptr on failure.
 */
std::FILE* openFile(const std::string& utf8Path, const char* mode);

/**
 * \brief std::ofstream replacement that opens files through openFile().
 *
 * It behaves like std::ofstream for the operations e-foto uses (the <<
 * operators, is_open(), fail(), precision(), close()), writing to a UTF-8
 * path of any length. On failure is_open() and fail() report the error
 * instead of silently dropping the output.
 */
class OutputFileStream : public std::ostream {
 public:
  explicit OutputFileStream(const std::string& utf8Path, bool binary = false);
  ~OutputFileStream() override;

  bool isOpen() const;
  /// Alias with the std::ofstream name, for drop in replacements.
  bool is_open() const { return isOpen(); }
  /// Flushes and closes the file (safe to call twice).
  void close();

 private:
  class Buffer;
  Buffer* buffer_;
};

/**
 * \brief std::ifstream replacement that opens files through openFile().
 *
 * A file saved under a path that std::ofstream cannot open is not very
 * useful, so the readers use the same conversions as the writers.
 */
class InputFileStream : public std::istream {
 public:
  explicit InputFileStream(const std::string& utf8Path, bool binary = false);
  ~InputFileStream() override;

  bool isOpen() const;
  /// Alias with the std::ifstream name, for drop in replacements.
  bool is_open() const { return isOpen(); }
  /// Closes the file (safe to call twice).
  void close();

 private:
  class Buffer;
  Buffer* buffer_;
};

}  // namespace PathUtils
}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br

#endif  // EFOTO_PATHUTILS_H
