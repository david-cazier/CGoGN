#include "dialogs/pluginsDialog.h"

//#include <libxml2/libxml/tree.h>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomText>
#include <QMessageBox>
//#include <QRegExp>
//#include <QTextBrowser>

#include "system.h"
#include "window.h"
#include "plugin.h"

namespace CGoGN
{

namespace SCHNApps
{

PluginsDialog::PluginsDialog(Window* window) :
	QDialog(window),
	m_window(window),
	init(true)
{
	this->setupUi(this);

	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(addButton, SIGNAL(pressed()), this, SLOT(cb_addPlugins()));
	connect(removeButton, SIGNAL(pressed()), this, SLOT(cb_removePlugins()));
	connect(directoryButton, SIGNAL(pressed()), this, SLOT(cb_addPluginsDirectory()));

	connect(treeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(cb_togglePlugin(QTreeWidgetItem *, int)));
//	connect(treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenu(const QPoint &)));

//	connect(this, SIGNAL(accepted()), this, SLOT(cb_acceptDialog()));

//	restoreState();

	if (System::Error::code != System::Error::SUCCESS)
		System::Error::showError(this);

	init = false;
}

PluginsDialog::~PluginsDialog()
{}

//bool PluginsDialog::restoreState()
//{
//	QFile xmlFile(System::app_path.toStdString().c_str() + QString("/state_save.xml"));
//
//	if (!xmlFile.exists())
//	{
//		System::Error::code = System::Error::NO_PLUGIN_PATH_FILE;
//		return false;
//	}
//
//	if (!xmlFile.open(QIODevice::ReadOnly))
//	{
//		System::Error::code = System::Error::ERROR_OPEN_PLUGIN_FILE;
//		return false;
//	}
//
//	QDomDocument doc;
//
//	if (!doc.setContent(&xmlFile))
//	{
//		System::Error::code = System::Error::BAD_PLUGIN_PATH_FILE;
//		xmlFile.close();
//		return false;
//	}
//
//	xmlFile.close();
//
//	QDomElement root = doc.documentElement();
//	QDomElement plugins_node = root.firstChildElement("PLUGINS");
//
//	const PluginHash& activePlugins = m_window->getPluginsHash();
//
//	if (!plugins_node.isNull())
//	{
//		QDomElement plugins_subNode = plugins_node.firstChildElement();
//
//		while (!plugins_subNode.isNull())
//		{
//			if (plugins_subNode.tagName() == "DIR")
//			{
//				QString pluginDirPath = plugins_subNode.attribute("path", "./plugins");
//				QFileInfo fi(pluginDirPath);
//
//				if (fi.exists() && fi.isDir())
//				{
//					QDir pluginDir(pluginDirPath);
//
//					QTreeWidgetItem *dirItem = new QTreeWidgetItem(treeWidget, DIR);
//					dirItem->setText(1, pluginDir.path());
//
//					QStringList filters, dirFiles;
//					filters << "lib*.so";
//					filters << "lib*.dylib";
//
//					dirFiles = pluginDir.entryList(filters, QDir::Files);
//					foreach(QString fileName, dirFiles)
//					{
//						QFileInfo pfi(fileName);
//						QString pluginName = pfi.baseName().remove(0, 3);
//
//						QTreeWidgetItem *item = new QTreeWidgetItem(dirItem, FILE_DIR);
//						item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
//
//						if (activePlugins.contains(pluginName))
//							item->setCheckState(0, Qt::Checked);
//						else
//							item->setCheckState(0, Qt::Unchecked);
//
//						item->setText(1, pluginDir.absoluteFilePath(fileName));
//					}
//				}
//			}
//			else if (plugins_subNode.tagName() == "FILE")
//			{
//				QString pluginPath = plugins_subNode.attribute("path");
//
//				if (!pluginPath.isEmpty())
//				{
//					QFileInfo fi(pluginPath);
//
//					if (fi.exists() && pluginPath.left(3) == "lib" && (fi.suffix() == "so" || fi.suffix() == "dylib"))
//					{
//						QString pluginName = fi.baseName().remove(0, 3);
//						QTreeWidgetItem *item =  new QTreeWidgetItem(treeWidget, FILE);
//						item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
//
//						if (activePlugins.contains(pluginName))
//							item->setCheckState(0, Qt::Checked);
//						else
//							item->setCheckState(0, Qt::Unchecked);
//
//						item->setText(1, pluginPath);
//					}
//				}
//			}
//
//			plugins_subNode = plugins_subNode.nextSiblingElement();
//		}
//	}
//
//	return true;
//}

void PluginsDialog::cb_addPlugins()
{
	init = true;

	QStringList files = QFileDialog::getOpenFileNames(
		this,
		"Select one or more plugins",
		m_window->getAppPath() + QString("/../Plugins/"),
		"Plugins (lib*.so lib*.dylib)"
	);

	if (!files.empty())
	{
		foreach(QString pluginPath, files)
		{
			QFileInfo pfi(pluginPath);
			QString pluginName = pfi.baseName().remove(0, 3);
			PluginInfo pinfo(pluginPath, pluginName);

			QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget, FILE);
			item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
			item->setCheckState(0, Qt::Unchecked);
			item->setText(1, pluginName);

			m_listedPlugins[item] = pinfo;
		}
	}

	if (System::Error::code != System::Error::SUCCESS)
		System::Error::showError(this);

	init = false;
}

void PluginsDialog::cb_addPluginsDirectory()
{
	init = true;

	QString dir = QFileDialog::getExistingDirectory(
		this,
		tr("Select a directory"),
		m_window->getAppPath() + QString("/../Plugins/"),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
	);

	if (!dir.isEmpty())
	{
		QDir directory(dir);

		if (!directory.exists())
			System::Error::code = System::Error::BAD_PLUGIN_PATH_IN_FILE_f(directory.absolutePath());

		QTreeWidgetItem *dirItem = new QTreeWidgetItem(treeWidget, DIR);
		dirItem->setText(1, directory.path());

		QStringList filters;
		filters << "lib*.so";
		filters << "lib*.dylib";

		QStringList dirFiles;
		dirFiles = directory.entryList(filters, QDir::Files);

		const PluginHash& activePlugins = m_window->getPluginsHash();

		foreach(QString pluginPath, dirFiles)
		{
			QFileInfo pfi(pluginPath);
			QString pluginName = pfi.baseName().remove(0, 3);
			PluginInfo pinfo(directory.absoluteFilePath(pluginPath), pluginName);

			QTreeWidgetItem *item = new QTreeWidgetItem(dirItem, FILE_DIR);
			item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

			if (activePlugins.contains(pluginName))
				item->setCheckState(0, Qt::Checked);
			else
				item->setCheckState(0, Qt::Unchecked);

			item->setText(1, pluginName);

			m_listedPlugins[item] = pinfo;
		}

		if (dirFiles.isEmpty())
			System::Error::code = System::Error::NO_PLUGIN_IN_DIR_f(directory.absolutePath());
	}

	if (System::Error::code != System::Error::SUCCESS)
		System::Error::showError(this);

	init = false;
}

void PluginsDialog::cb_removePlugins()
{
	QList<QTreeWidgetItem*> itemList = treeWidget->selectedItems();

	if (!itemList.isEmpty())
	{
		foreach(QTreeWidgetItem* item, itemList)
		{
			if (item->type() == FILE)
			{
				if (item->checkState(0) == Qt::Checked)
				{
					QMessageBox msgBox;
					msgBox.setText("Le plugin\n"
					               "\t" + item->text(1) +
					               " doit être désactivé avant de pouvoir être\n"
					               "supprimé.");
					msgBox.exec();
				}
				else
				{
					item = treeWidget->takeTopLevelItem(treeWidget->indexOfTopLevelItem(item));
					delete item;

					m_listedPlugins.remove(item);
				}
			}
			else if (item->type() == FILE_DIR)
			{
				QMessageBox msgBox;
				msgBox.setText("Le plugin\n"
				               "\t" + item->text(1) +
				               " fait partit d'un paquet/directory.\n"
				               "Il ne peut être supprimé séparément.");
				msgBox.exec();
			}
			else if (item->type() == DIR)
			{
				bool isAnyPluginActive = false;
				QTreeWidgetItem *fileItem;

				for (int i = 0; (i < item->childCount() && !isAnyPluginActive); ++i)
				{
					fileItem = item->child(i);
					if (fileItem->checkState(0) == Qt::Checked)
						isAnyPluginActive = true;
				}

				if (isAnyPluginActive)
				{
					QMessageBox msgBox;
					msgBox.setText("Un ou plusieurs plugins du dossier sont actifs\n"
					               "Veuillez désactiver tous les plugins du paquet avant de supprimer celui-ci.");
					msgBox.exec();
				}
				else
				{
					for (int i = 0; i < item->childCount(); ++i)
					{
						fileItem = item->child(i);
						m_listedPlugins.remove(fileItem);
					}

					item = treeWidget->takeTopLevelItem(treeWidget->indexOfTopLevelItem(item));
					delete item;
				}
			}
		}
	}

	if (System::Error::code != System::Error::SUCCESS)
		System::Error::showError(this);
}

void PluginsDialog::cb_togglePlugin(QTreeWidgetItem *item, int column)
{
	if (!init && column == 0)
	{
		PluginInfo pinfo = m_listedPlugins[item];

		QString pluginPath = pinfo.pluginPath;
		QString pluginName = pinfo.pluginName;

		if (item->checkState(0) == Qt::Checked)
		{
			const PluginHash& activePlugins = m_window->getPluginsHash();

			if (activePlugins.contains(pluginName))
			{
				System::Error::code = System::Error::PLUGIN_EXISTS_f(pluginName);
				System::Error::showError(this);
				init = true;
				item->setCheckState(0, Qt::Unchecked);
				init = false;
				return;
			}

			Plugin* p = m_window->loadPlugin(pluginPath);

			if (p == NULL)
			{
				init = true;
				item->setCheckState(0, Qt::Unchecked);
				init = false;
			}
		}
		else if (item->checkState(0) == Qt::Unchecked)
		{
			Plugin* p = m_window->getPlugin(pluginName);

			if(p->isUsed())
			{
				QMessageBox::warning(this, tr("Warning"), "Plugin is currently used");
				init = true;
				item->setCheckState(0, Qt::Checked);
				init = false;
				return;
			}
			else
			{
				m_window->unloadPlugin(pluginName);
			}
		}
	}
}

//void PluginsDialog::customContextMenu(const QPoint &pos)
//{
//	QPoint globalPos = treeWidget->mapToGlobal(pos);
//
//	QTreeWidgetItem *item = treeWidget->itemAt(pos);
//
//	if (item && (item->type() == FILE || item->type() == FILE_DIR))
//	{
//		item->setSelected(true);
//
//		QMenu myMenu("Plus...", this);
//		QAction pluginInfo("Informations sur le plugin", this);
//		myMenu.addAction(&pluginInfo);
//		connect(&pluginInfo, SIGNAL(triggered()), this, SLOT(showPluginInfo()));
//
//		myMenu.exec(globalPos);
//	}
//}

//void PluginsDialog::cb_acceptDialog()
//{
//	QStringList paths;
//
//	int t = treeWidget->topLevelItemCount();
//
//	for (int i = 0; i < t; ++i)
//	{
//		QTreeWidgetItem *item = treeWidget->topLevelItem(i);
//
//		if (item->type() == FILE || item->type() == DIR)
//		{
//			QString path = item->text(1);
//			paths.push_back(path);
//		}
//	}
//
//	if (!System::StateHandler::savePluginsInfo(m_window, activePlugins, paths))
//		System::Error::showError();
//}

//void PluginsDialog::showPluginInfo()
//{
//	QTreeWidgetItem *item = treeWidget->currentItem();
//	QString strUrl = item->text(1);
//
//	System::Info::showPluginInfo(strUrl);
//}

} // namespace SCHNApps

} // namespace CGoGN