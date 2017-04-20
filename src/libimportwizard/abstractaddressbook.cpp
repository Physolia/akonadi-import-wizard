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
#include "abstractaddressbook.h"
#include "abstractdisplayinfo.h"

#include <KContacts/Addressee>
#include <kcontacts/contactgroup.h>
#include <KLocalizedString>
#include "libimportwizard_debug.h"
#include <AkonadiCore/ItemCreateJob>
#include <AkonadiCore/Item>
#include <AkonadiWidgets/CollectionDialog>

#include <QPointer>
using namespace LibImportWizard;
AbstractAddressBook::AbstractAddressBook()
    : mCollection(-1)
{
}

AbstractAddressBook::~AbstractAddressBook()
{
}

bool AbstractAddressBook::selectAddressBook()
{
    addAddressBookImportInfo(i18n("Creating new contact..."));
    if (!mCollection.isValid()) {
        const QStringList mimeTypes(KContacts::Addressee::mimeType());
        QPointer<Akonadi::CollectionDialog> dlg = new Akonadi::CollectionDialog(mAbstractDisplayInfo->parentWidget());
        dlg->setMimeTypeFilter(mimeTypes);
        dlg->setAccessRightsFilter(Akonadi::Collection::CanCreateItem);
        dlg->setWindowTitle(i18n("Select Address Book"));
        dlg->setDescription(i18n("Select the address book the new contact shall be saved in:"));

        if (dlg->exec() == QDialog::Accepted && dlg) {
            mCollection = dlg->selectedCollection();
        } else {
            addAddressBookImportError(i18n("Address Book was not selected."));
            delete dlg;
            return false;
        }
        delete dlg;
    }
    return true;
}

void AbstractAddressBook::createGroup(const KContacts::ContactGroup &group)
{
    if (selectAddressBook()) {
        Akonadi::Item item;
        item.setPayload<KContacts::ContactGroup>(group);
        item.setMimeType(KContacts::ContactGroup::mimeType());

        Akonadi::ItemCreateJob *job = new Akonadi::ItemCreateJob(item, mCollection);
        connect(job, &Akonadi::ItemCreateJob::result, this, &AbstractAddressBook::slotStoreDone);
    }
}

void AbstractAddressBook::addImportContactNote(KContacts::Addressee &address, const QString &applicationName)
{
    QString currentNote = address.note();
    if (!currentNote.isEmpty()) {
        currentNote += QLatin1Char('\n');
    }
    currentNote += i18n("Imported from \"%1\"", applicationName);
    address.setNote(currentNote);
}

void AbstractAddressBook::createContact(const KContacts::Addressee &address)
{
    if (selectAddressBook()) {
        Akonadi::Item item;
        item.setPayload<KContacts::Addressee>(address);
        item.setMimeType(KContacts::Addressee::mimeType());
        Akonadi::ItemCreateJob *job = new Akonadi::ItemCreateJob(item, mCollection);
        connect(job, &Akonadi::ItemCreateJob::result, this, &AbstractAddressBook::slotStoreDone);
    }
}

void AbstractAddressBook::slotStoreDone(KJob *job)
{
    if (job->error()) {
        qCDebug(LIBIMPORTWIZARD_LOG) << " job->errorString() : " << job->errorString();
        addAddressBookImportError(i18n("Error during contact creation: %1", job->errorString()));
        return;
    }
    addAddressBookImportInfo(i18n("Contact creation complete"));
}

void AbstractAddressBook::addImportInfo(const QString &log)
{
    addAddressBookImportInfo(log);
}

void AbstractAddressBook::addImportError(const QString &log)
{
    addAddressBookImportError(log);
}

void AbstractAddressBook::addAddressBookImportInfo(const QString &log)
{
    if (mAbstractDisplayInfo) {
        mAbstractDisplayInfo->addressbookImportInfo(log);
    }
}

void AbstractAddressBook::addAddressBookImportError(const QString &log)
{
    if (mAbstractDisplayInfo) {
        mAbstractDisplayInfo->addressbookImportError(log);
    }
}

void AbstractAddressBook::cleanUp()
{
    mCollection = Akonadi::Collection();
}
