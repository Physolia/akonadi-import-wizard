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

#include "thunderbirdaddressbook.h"
#include "addressbook/MorkParser.h"

#include <KLocalizedString>
#include <KContacts/Addressee>
#include <QUrl>
//#include "importwizard_debug.h"

ThunderBirdAddressBook::ThunderBirdAddressBook(const QDir &dir, ImportWizard *parent)
    : LibImportWizard::AbstractAddressBook(parent),
      mDir(dir)
{
}

ThunderBirdAddressBook::~ThunderBirdAddressBook()
{

}

void ThunderBirdAddressBook::importAddressBook()
{
    const QString path = mDir.path();
    readAddressBook(path + QLatin1String("/impab.mab"));
    const QStringList filesimportab = mDir.entryList(QStringList(QStringLiteral("impab-[0-9]*.map")), QDir::Files, QDir::Name);
    for (const QString &file : filesimportab) {
        readAddressBook(path + QLatin1Char('/') + file);
    }
    readAddressBook(path + QLatin1String("/abook.mab"));

    const QStringList files = mDir.entryList(QStringList(QStringLiteral("abook-[0-9]*.map")), QDir::Files, QDir::Name);
    for (const QString &file : files) {
        readAddressBook(path + QLatin1Char('/') + file);
    }
    readAddressBook(path + QLatin1String("/history.mab"));

    cleanUp();
}

void ThunderBirdAddressBook::readAddressBook(const QString &filename)
{
    MorkParser mork;
    if (!mork.open(filename)) {
        if (mork.error() == FailedToOpen) {
            addAddressBookImportError(i18n("Contacts file '%1' not found", filename));
        }
        //FIXME qCDebug(IMPORTWIZARD_LOG) << " error during read file " << filename << " Error type " << mork.error();
        return;
    }
    MorkTableMap *tables = mork.getTables(0x80);
    if (tables) {
        MorkTableMap::iterator tableIterEnd(tables->end());
        MorkRowMap *rows = nullptr;
        for (MorkTableMap::iterator tableIter = tables->begin(); tableIter != tableIterEnd; ++tableIter) {
            if (tableIter.key() != 0) {
                rows = mork.getRows(0x80, &tableIter.value());
                if (rows) {
                    MorkRowMap::iterator endRow(rows->end());
                    for (MorkRowMap::iterator rowIter = rows->begin(); rowIter != endRow; ++rowIter) {
                        if (rowIter.key() != 0) {
                            KContacts::Addressee contact;
                            MorkCells cells = rowIter.value();
                            MorkCells::iterator endCellIter = cells.end();
                            KContacts::Address homeAddr = KContacts::Address(KContacts::Address::Home);
                            KContacts::Address workAddr = KContacts::Address(KContacts::Address::Work);
                            int birthday = -1;
                            int birthmonth = -1;
                            int birthyear = -1;

                            for (MorkCells::iterator cellsIter = cells.begin(); cellsIter != endCellIter; ++cellsIter) {
                                const QString value = mork.getValue(cellsIter.value());
                                const QString column = mork.getColumn(cellsIter.key());
                                //FIXME qCDebug(IMPORTWIZARD_LOG) << "column :" << column << " value :" << value;
                                if (column == QLatin1String("LastModifiedDate")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("RecordKey")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("AddrCharSet")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("LastRecordKey")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ns:addrbk:db:table:kind:pab")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ListName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ListNickName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ListDescription")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ListTotalAddresses")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("LowercaseListName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ns:addrbk:db:table:kind:deleted")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("PhotoType")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("PreferDisplayName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("PhotoURI")) {
                                    KContacts::Picture photo;
                                    photo.setUrl(value);
                                    contact.setLogo(photo);
                                } else if (column == QLatin1String("PhotoName")) {
                                    //TODO: verify it
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("DbRowID")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ns:addrbk:db:row:scope:card:all")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ns:addrbk:db:row:scope:list:all")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("ns:addrbk:db:row:scope:data:all")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("FirstName")) {
                                    contact.setName(value);
                                } else if (column == QLatin1String("LastName")) {
                                    contact.setFamilyName(value);
                                } else if (column == QLatin1String("PhoneticFirstName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("PhoneticLastName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("DisplayName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("NickName")) {
                                    contact.setNickName(value);
                                } else if (column == QLatin1String("PrimaryEmail")) {
                                    contact.setEmails(QStringList() << value);
                                } else if (column == QLatin1String("LowercasePrimaryEmail")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("SecondEmail")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("PreferMailFormat")) {
                                    contact.insertCustom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("MailPreferedFormatting"), value);
                                } else if (column == QLatin1String("PopularityIndex")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("AllowRemoteContent")) {
                                    contact.insertCustom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("MailAllowToRemoteContent"), value);
                                } else if (column == QLatin1String("WorkPhone")) {
                                    contact.insertPhoneNumber(KContacts::PhoneNumber(value, KContacts::PhoneNumber::Work));
                                } else if (column == QLatin1String("HomePhone")) {
                                    contact.insertPhoneNumber(KContacts::PhoneNumber(value, KContacts::PhoneNumber::Home));
                                } else if (column == QLatin1String("FaxNumber")) {
                                    contact.insertPhoneNumber(KContacts::PhoneNumber(value, KContacts::PhoneNumber::Fax));
                                } else if (column == QLatin1String("PagerNumber")) {
                                    contact.insertPhoneNumber(KContacts::PhoneNumber(value, KContacts::PhoneNumber::Pager));
                                } else if (column == QLatin1String("CellularNumber")) {
                                    contact.insertPhoneNumber(KContacts::PhoneNumber(value, KContacts::PhoneNumber::Cell));
                                } else if (column == QLatin1String("WorkPhoneType")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("HomePhoneType")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("FaxNumberType")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("PagerNumberType")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("CellularNumberType")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("HomeAddress")) {
                                    homeAddr.setStreet(value);
                                } else if (column == QLatin1String("HomeAddress2")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("HomeCity")) {
                                    homeAddr.setLocality(value);
                                } else if (column == QLatin1String("HomeState")) {
                                    homeAddr.setRegion(value);
                                } else if (column == QLatin1String("HomeZipCode")) {
                                    homeAddr.setPostalCode(value);
                                } else if (column == QLatin1String("HomeCountry")) {
                                    homeAddr.setCountry(value);
                                } else if (column == QLatin1String("WorkAddress")) {
                                    workAddr.setStreet(value);
                                } else if (column == QLatin1String("WorkAddress2")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("WorkCity")) {
                                    workAddr.setLocality(value);
                                } else if (column == QLatin1String("WorkState")) {
                                    workAddr.setRegion(value);
                                } else if (column == QLatin1String("WorkZipCode")) {
                                    workAddr.setPostalCode(value);
                                } else if (column == QLatin1String("WorkCountry")) {
                                    workAddr.setCountry(value);
                                } else if (column == QLatin1String("JobTitle")) {
                                    contact.setTitle(value);
                                } else if (column == QLatin1String("Department")) {
                                    contact.setDepartment(value);
                                } else if (column == QLatin1String("Company")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("_AimScreenName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("AnniversaryYear")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("AnniversaryMonth")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("AnniversaryDay")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("SpouseName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("FamilyName")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("WebPage1")) {
                                    KContacts::ResourceLocatorUrl url;
                                    url.setUrl(QUrl(value));
                                    contact.setUrl(url);
                                } else if (column == QLatin1String("WebPage2")) {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " column " << column << " found but not imported. Need to look at how to import it";
                                } else if (column == QLatin1String("BirthYear")) {
                                    birthyear = value.toInt();
                                } else if (column == QLatin1String("BirthMonth")) {
                                    birthmonth = value.toInt();
                                } else if (column == QLatin1String("BirthDay")) {
                                    birthday = value.toInt();
                                } else if (column == QLatin1String("Custom1")) {
                                    contact.insertCustom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("Custom1"), value);
                                } else if (column == QLatin1String("Custom2")) {
                                    contact.insertCustom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("Custom2"), value);
                                } else if (column == QLatin1String("Custom3")) {
                                    contact.insertCustom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("Custom3"), value);
                                } else if (column == QLatin1String("Custom4")) {
                                    contact.insertCustom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("Custom4"), value);
                                } else if (column == QLatin1String("Notes")) {
                                    contact.setNote(value);
                                } else {
                                    //FIXME qCDebug(IMPORTWIZARD_LOG) << " Columnn not implemented " << column;
                                }
                                ////FIXME qCDebug(IMPORTWIZARD_LOG)<<" value :"<<value<<" column"<<column;
                            }

                            if (!homeAddr.isEmpty()) {
                                contact.insertAddress(homeAddr);
                            }
                            if (!workAddr.isEmpty()) {
                                contact.insertAddress(workAddr);
                            }

                            const QDate birthDate(birthyear, birthmonth, birthday);
                            if (birthDate.isValid()) {
                                contact.setBirthday(birthDate);
                            }
                            addImportContactNote(contact, QStringLiteral("Thunderbird"));
                            createContact(contact);
                            //FIXME qCDebug(IMPORTWIZARD_LOG) << "-----------------------";
                        }
                    }
                }
            }
        }
    }
}
