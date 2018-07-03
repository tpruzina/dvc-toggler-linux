#include "config.hpp"

#include <QtDebug>
#include <QStandardPaths>

Config::Config() :
	QSettings(
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/settings.ini",
		QSettings::IniFormat)
{
	if(!contains("autohide"))	set_autohide(false);
	if(!contains("enabled"))	set_enabled(true);
	if(!contains("start_minimized"))	set_start_minimized(false);
	if(!contains("polling_rate"))	set_polling_rate(100);

	beginGroup("default");
	if(!contains("icon_path"))	setValue("icon", ":/resources/desktop.svg");
	endGroup();

	qDebug() << QSettings::allKeys();
}

QString
Config::query_icon_path(const QString &profile_name)
{
	beginGroup(profile_name);
	QString res = value("icon").toString();
	endGroup();
	return res;
}

void
Config::set_icon_path(const QString &profile_name, const QString &path)
{
	beginGroup(profile_name);
	setValue("icon", path);
	endGroup();
}

QMap<int,int>
Config::query_dvc(const QString &profile_name)
{
	QMap<int,int> map;
	beginGroup(profile_name);
	QStringList keys = childKeys();
	foreach(QString key, keys)
	{
		if(key.startsWith("DPY-"))
		{
			QString dpyId_s = key;
			dpyId_s.remove(0,4); // remove "DPY-"
			int dpyId = dpyId_s.toInt(); // str -> int
			map[dpyId] = value(key).toInt();
		}
	}
	endGroup();
	return map;
}

void
Config::set_dvc(const QString &profile_name, QMap<int,int> &map)
{
	beginGroup(profile_name);
	QMap<int,int>::const_iterator it = map.constBegin();
	while(it != map.constEnd())
	{
		setValue("DPY-"+QString::number(it.key()), it.value());
		it++;
	}
	endGroup();
}

QStringList
Config::query_profiles()
{
	return childGroups();
}

bool
Config::query_enabled(void)
{
	return value("enabled").toBool();
}

void
Config::set_enabled(bool val)
{
	setValue("enabled", val);
}

bool
Config::query_autohide(void)
{
	return value("autohide").toBool();
}

void
Config::set_start_minimized(bool val)
{
	setValue("start_minimized", val);
}

bool
Config::query_start_minimized(void)
{
	return value("start_minimized").toBool();
}

void
Config::set_autohide(bool val)
{
	setValue("autohide", val);
}

QVariant
Config::value(const QString &key, const QVariant &defaultValue) const
{
	qDebug() <<  "value(" << key << ") = " << QSettings::value(key, defaultValue);
	return QSettings::value(key, defaultValue);
}

void
Config::setValue(const QString &key, const QVariant &value)
{
	qDebug() << "setValue(" << key << ") = " << value;
	return QSettings::setValue(key, value);
}

void
Config::remove_profile(const QString &key)
{
	beginGroup(key);
	 //removes the group, and all it keys
	remove("");
	endGroup();
}

unsigned
Config::query_polling_rate(void)
{
	return QSettings::value("polling_rate").toUInt();
}

void
Config::set_polling_rate(unsigned ms)
{
	setValue("polling_rate", ms);
}
