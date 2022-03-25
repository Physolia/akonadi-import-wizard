/*
   SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "abstractimporter.h"

class ThunderbirdImportData : public LibImportWizard::AbstractImporter
{
public:
    explicit ThunderbirdImportData(QObject *parent, const QList<QVariant> & = QList<QVariant>());
    ~ThunderbirdImportData() override;

    TypeSupportedOptions supportedOption() override;
    bool foundMailer() const override;

    bool importSettings() override;
    bool importMails() override;
    bool importFilters() override;
    bool importAddressBook() override;
    QString name() const override;

private:
    QString defaultProfile();

    QString mDefaultProfile;
};
