/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importmailpluginmanager.h"
#include "abstractimporter.h"
#include "importwizard_debug.h"
#include "kcoreaddons_version.h"
#include <KPluginFactory>
#include <KPluginLoader>
#include <KPluginMetaData>

#include <QFileInfo>

namespace
{
QString pluginVersion()
{
    return QStringLiteral("1.0");
}
}

ImportMailPluginManager::ImportMailPluginManager(QObject *parent)
    : QObject(parent)
{
    initializePluginList();
}

ImportMailPluginManager::~ImportMailPluginManager()
{
}

ImportMailPluginManager *ImportMailPluginManager::self()
{
    static ImportMailPluginManager s_self;
    return &s_self;
}

bool ImportMailPluginManager::initializePluginList()
{
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(5, 85, 0)
    const QVector<KPluginMetaData> plugins = KPluginLoader::findPlugins(QStringLiteral("importwizard"));
#else
    const QVector<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("importwizard"));
#endif

    QVectorIterator<KPluginMetaData> i(plugins);
    i.toBack();
    while (i.hasPrevious()) {
        ImportMailPluginManagerInfo info;
        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
        info.pluginData = createPluginMetaData(data);
        // 2) look at if plugin is activated
        info.metaDataFileNameBaseName = QFileInfo(data.fileName()).baseName();
        info.metaDataFileName = data.fileName();
        if (pluginVersion() == data.version()) {
            info.plugin = nullptr;
            mPluginList.push_back(info);
        } else {
            qCWarning(IMPORTWIZARD_LOG) << "Plugin " << data.name() << " doesn't have correction plugin version. It will not be loaded.";
        }
    }
    QVector<ImportMailPluginManagerInfo>::iterator end(mPluginList.end());
    for (QVector<ImportMailPluginManagerInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
    return true;
}

void ImportMailPluginManager::loadPlugin(ImportMailPluginManagerInfo *item)
{
#if KCOREADDONS_VERSION > QT_VERSION_CHECK(5, 85, 0)
    const auto loadResult = KPluginFactory::instantiatePlugin<LibImportWizard::AbstractImporter>(KPluginMetaData(item->metaDataFileName), this);
    if (loadResult) {
        mPluginDataList.append(item->pluginData);
    }
#else
    KPluginLoader pluginLoader(item->metaDataFileName);
    if (pluginLoader.factory()) {
        item->plugin = pluginLoader.factory()->create<LibImportWizard::AbstractImporter>(this, QVariantList() << item->metaDataFileNameBaseName);
        mPluginDataList.append(item->pluginData);
    }
#endif
}

QVector<LibImportWizard::AbstractImporter *> ImportMailPluginManager::pluginsList() const
{
    QVector<LibImportWizard::AbstractImporter *> lst;
    QVector<ImportMailPluginManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<ImportMailPluginManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

PluginUtilData ImportMailPluginManager::createPluginMetaData(const KPluginMetaData &metaData)
{
    PluginUtilData pluginData;
    pluginData.mName = metaData.name();
    pluginData.mIdentifier = metaData.pluginId();
    return pluginData;
}
