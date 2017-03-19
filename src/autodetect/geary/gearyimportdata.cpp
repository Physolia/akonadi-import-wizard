/*
   Copyright (C) 2017 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "geary/gearyimportdata.h"
#include "geary/gearysettings.h"
#include "geary/gearyaddressbook.h"
#include "mailimporter/filterinfo.h"
#include "mailimporter/othermailerutil.h"
#include "importfilterinfogui.h"
#include "importwizard.h"
#include "importwizard_debug.h"

#include <QDir>

GearyImportData::GearyImportData(ImportWizard *parent)
    : AbstractImporter(parent)
{
    mPath = MailImporter::OtherMailerUtil::gearyDefaultPath();
}

GearyImportData::~GearyImportData()
{
}

bool GearyImportData::foundMailer() const
{
    QDir directory(mPath);
    if (directory.exists()) {
        return true;
    }
    return false;
}

QString GearyImportData::name() const
{
    return QStringLiteral("Geary");
}

bool GearyImportData::importMails()
{
    return false;
}

bool GearyImportData::importSettings()
{
    QDir directory(mPath);
    const QStringList lstDir = directory.entryList(QDir::AllDirs);
    qCDebug(IMPORTWIZARD_LOG) << "Number of directory " << lstDir.count();
    const QString configName = QStringLiteral("geary.ini");
    for (int i = 0; i < lstDir.count(); ++i) {
        const QString fullPath = lstDir.at(i) + QDir::separator() + configName;
        QFile f(fullPath);
        if (f.exists()) {
            GearySettings setting(fullPath, mImportWizard);
        }
    }
    return true;
}

bool GearyImportData::importAddressBook()
{
    return false;
}

AbstractImporter::TypeSupportedOptions GearyImportData::supportedOption()
{
    TypeSupportedOptions options;
    //TODO reactivate it
    //options |=AbstractImporter::Mails;
    //options |= AbstractImporter::Settings;
    //options |= AbstractImporter::AddressBooks;
    return options;
}
