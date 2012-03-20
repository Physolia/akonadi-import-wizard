/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>

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

#ifndef ABSTRACTSETTINGS_H
#define ABSTRACTSETTINGS_H

#include <QObject>

class ImportWizard;
class KJob;
class AbstractSettings : public QObject
{
  Q_OBJECT
public:
  explicit AbstractSettings(ImportWizard *parent);
  ~AbstractSettings();
private slots:
  void instanceCreateResult( KJob* job );

protected:
  void createResource( const QString& resources );

  ImportWizard *m_parent;
};

#endif // ABSTRACTSETTINGS_H
