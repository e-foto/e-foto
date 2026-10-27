#ifndef LANGUAGE_H
#define LANGUAGE_H
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

#include <QString>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
 * Loads and switches the interface translation.
 *
 * The translations are compiled from the .ts files in translations/ into
 * e-foto_<locale>.qm and are looked up next to the executable (in a
 * "translations" folder) or in the Qt translations directory.
 */
class Language {
 public:
  // Reads the stored preference (or the system locale) and installs the
  // matching translator. Call it once, after QApplication was created.
  static void init();

  // Installs the given language at runtime. "system" follows the OS locale,
  // "en" forces English, any other value is a locale name such as "zh_CN".
  static bool setCurrent(const QString& code);

  // The value stored in the settings: "system", "en", "zh_CN", ...
  static QString current();

  // Whether a compiled translation exists for this locale.
  static bool isAvailable(const QString& code);

 private:
  static QString resolveSystemLocale();
  static bool install(const QString& locale);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // LANGUAGE_H
