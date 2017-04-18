/*
   Copyright (C) 2012-2017 Montel Laurent <montel@kde.org>

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

#ifndef EVOLUTIONSETTINGS_H
#define EVOLUTIONSETTINGS_H

#include "abstract/abstractsettings.h"
#include <KIdentityManagement/kidentitymanagement/signature.h>
#include <QString>

class ImportWizard;
class QDomElement;

class EvolutionSettings : public AbstractSettings
{
public:
    explicit EvolutionSettings(ImportWizard *parent);
    ~EvolutionSettings();
    void loadAccount(const QString &filename);
    void loadLdap(const QString &filename);
private:
    void readAccount(const QDomElement &account);
    void readLdap(const QString &account);
    void extractAccountInfo(const QString &info);
    void readSignatures(const QDomElement &account);
    void extractSignatureInfo(const QString &info);
    QString getSecurityMethod(const QStringList &listArgument, bool &found);
    QString getAuthMethod(const QString &path, bool &found);
    void addAuth(QMap<QString, QVariant> &settings, const QString &argument, const QString &userName);

    QMap<QString, KIdentityManagement::Signature> mMapSignature;
};

#endif /* EVOLUTIONSETTINGS_H */

