#include <QStandardPaths>

#include "config.hpp"

Config::Config() :
	QSettings(
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/settings.ini",
		QSettings::IniFormat)
{
	if(!contains(CONFIG_AUTOHIDE_STR))	set_bool(CONFIG_AUTOHIDE_STR, false);
	if(!contains(CONFIG_ENABLED_STR))	set_bool(CONFIG_ENABLED_STR, true);
	if(!contains(CONFIG_START_MIN_STR))	set_bool(CONFIG_START_MIN_STR, false);
	if(!contains(CONFIG_TRAY_INFO_SHOWN))	set_bool(CONFIG_TRAY_INFO_SHOWN, false);
	if(!contains(CONFIG_SLEEP_STR))	setSleepTime_ms(100);

	beginGroup(CONFIG_DEFAULT_PROFILE_STR);
	if(!contains("icon_path"))	setValue("icon", ":/resources/desktop.svg");
	endGroup();
}

QString
Config::queryIconPath(const QString &profile_name)
{
	beginGroup(profile_name);
	QString res = getValue("icon").toString();
	endGroup();
	return res;
}

void
Config::setIconPath(const QString &profile_name, const QString &path)
{
	beginGroup(profile_name);
	setValue("icon", path);
	endGroup();
}

QMap<int,int>
Config::queryDVC(const QString &profile_name)
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
			map[dpyId] = getValue(key).toInt();
		}
	}
	endGroup();
	return map;
}

void
Config::setDVC(const QString &profile_name, QMap<int,int> &map)
{
	beginGroup(profile_name);
	QMap<int,int>::const_iterator it = map.constBegin();
	while(it != map.constEnd())
	{
		setValue("DPY-"+QString::number(it.key()), it.value());
		++it;
	}
	endGroup();
}

QStringList
Config::queryProfiles()
{
	return childGroups();
}

QVariant
Config::getValue(const QString &key, const QVariant &defaultValue) const
{
	return QSettings::value(key, defaultValue);
}

void
Config::setValue(const QString &key, const QVariant &value)
{
	return QSettings::setValue(key, value);
}

void
Config::removeProfile(const QString &key)
{
	beginGroup(key);
	 //removes the group, and all it keys
	remove("");
	endGroup();
}

unsigned
Config::querySleepTime_ms(void)
{
	return getValue(CONFIG_SLEEP_STR).toInt();
}

void
Config::setSleepTime_ms(unsigned ms)
{
	setValue(CONFIG_SLEEP_STR, ms);
}
