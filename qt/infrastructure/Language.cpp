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

#include "Language.h"

#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>
#include <QSettings>
#include <QStringList>
#include <QTranslator>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

namespace {

QSettings& settings() {
  static QSettings instance("uerj", "efoto");
  return instance;
}

QTranslator* translator() {
  static QTranslator instance;
  return &instance;
}

QStringList searchDirectories() {
  const QString appDir = QCoreApplication::applicationDirPath();
  QStringList dirs;
  dirs << appDir + "/translations" << appDir;
#ifdef Q_OS_MAC
  dirs << appDir + "/../Resources/translations";
#endif
  // Translations shipped with Qt itself (standard dialogs, etc.).
  dirs << QLibraryInfo::path(QLibraryInfo::TranslationsPath);
  dirs << ":/i18n";
  return dirs;
}

bool loadFile(const QString& locale) {
  QTranslator* tr = translator();
  QCoreApplication::removeTranslator(tr);
  const QStringList candidates{locale, locale.section('_', 0, 0)};
  for (const QString& name : candidates) {
    if (name.isEmpty()) continue;
    for (const QString& dir : searchDirectories()) {
      if (tr->load("e-foto_" + name, dir)) {
        QCoreApplication::installTranslator(tr);
        return true;
      }
    }
  }
  return false;
}

} // namespace

void Language::init() {
  const QString stored = settings().value("language", "system").toString();
  if (stored != "system" && !stored.isEmpty()) {
    loadFile(stored);
    return;
  }
  loadFile(resolveSystemLocale());
}

bool Language::setCurrent(const QString& code) {
  settings().setValue("language", code);
  if (code == "system" || code.isEmpty()) {
    return loadFile(resolveSystemLocale());
  }
  if (code == "en") {
    QCoreApplication::removeTranslator(translator());
    return true;
  }
  return loadFile(code);
}

QString Language::current() {
  return settings().value("language", "system").toString();
}

bool Language::isAvailable(const QString& code) {
  if (code == "en") return true;
  const QString locale = (code == "system" || code.isEmpty())
                             ? resolveSystemLocale()
                             : code;
  if (locale.isEmpty()) return false;
  // A locale is available when the .qm can be loaded; probing with a throwaway
  // translator keeps the active one untouched.
  QTranslator probe;
  const QStringList candidates{locale, locale.section('_', 0, 0)};
  for (const QString& name : candidates) {
    for (const QString& dir : searchDirectories()) {
      if (probe.load("e-foto_" + name, dir)) return true;
    }
  }
  return false;
}

QString Language::resolveSystemLocale() {
  const QString name = QLocale::system().name();  // e.g. "zh_CN"
  if (name.startsWith("zh")) return "zh_CN";
  if (name.startsWith("pt")) return "pt_BR";
  return "en";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
