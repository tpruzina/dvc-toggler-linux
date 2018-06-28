#include "config.hpp"

#include <QtDebug>
#include <QStandardPaths>

Config::Config() :
	QSettings(
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/settings.ini",
		QSettings::IniFormat)
{
	if(!contains("autohide"))	set_autohide(false);
	if(!contains("enabled"))		set_enabled(true);

	beginGroup("default");
	if(!contains("dvc"))		setValue("dvc", 0);
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

int
Config::query_dvc(const QString &profile_name)
{
	beginGroup(profile_name);
	int res = value("dvc").toInt();
	endGroup();
	return res;
}

void
Config::set_dvc(const QString &profile_name, int level)
{
	beginGroup(profile_name);
	setValue("dvc", level);
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
