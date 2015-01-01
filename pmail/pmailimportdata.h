/*
  Copyright (c) 2012-2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef PMailImportData_H
#define PMailImportData_H

#include "abstractimporter.h"
class ImportWizard;

class PMailImportData : public AbstractImporter
{
public:
    explicit PMailImportData(ImportWizard *parent);
    ~PMailImportData();
    
    TypeSupportedOptions supportedOption();
    bool foundMailer() const;

    bool importMails();
    bool importSettings();

    QString name() const;

};

#endif /* PMail ImportData_H */

