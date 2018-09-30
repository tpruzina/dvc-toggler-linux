#include <QStandardPaths>

#include "config.hpp"

using QSP = QStandardPaths;

Config::Config() noexcept:
        QSettings(
                QSP::writableLocation(QSP::AppConfigLocation) + "/settings.ini",
                IniFormat)
{
        if(!contains(CONFIG_AUTOHIDE_STR))      set_bool(CONFIG_AUTOHIDE_STR, false);
        if(!contains(CONFIG_ENABLED_STR))       set_bool(CONFIG_ENABLED_STR, true);
        if(!contains(CONFIG_START_MIN_STR))     set_bool(CONFIG_START_MIN_STR, false);
        if(!contains(CONFIG_TRAY_INFO_SHOWN))   set_bool(CONFIG_TRAY_INFO_SHOWN, false);
        if(!contains(CONFIG_SLEEP_STR))         setSleepTime_ms(100);

        beginGroup(CONFIG_DEFAULT_PROFILE_STR);
        if(!contains("icon_path"))      setValue("icon", ":/resources/desktop.svg");
        endGroup();
}

auto Config::queryIconPath(const QString &profile_name) noexcept -> QString
{
        beginGroup(profile_name);
        QString res = getValue("icon").toString();
        endGroup();
        return res;
}

auto Config::setIconPath(const QString &profile_name, const QString &path) noexcept -> void
{
        beginGroup(profile_name);
        setValue("icon", path);
        endGroup();
}

auto Config::queryDVC(const QString &profile_name) noexcept -> QMap<int,int>
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
                        int dpyId = dpyId_s.toInt(); // str noexcept -> int
                        map[dpyId] = getValue(key).toInt();
                }
        }
        endGroup();
        return map;
}

auto Config::setDVC(const QString &profile_name, QMap<int,int> &map) noexcept -> void
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

auto Config::queryProfiles() noexcept -> QStringList
{
        return childGroups();
}

auto Config::getValue(const QString &key, const QVariant &defaultValue) const noexcept -> QVariant
{
        return QSettings::value(key, defaultValue);
}

auto Config::setValue(const QString &key, const QVariant &value) noexcept -> void
{
        return QSettings::setValue(key, value);
}

auto Config::removeProfile(const QString &key) noexcept -> void
{
        beginGroup(key);
        remove("");	//removes the group, and all it keys
        endGroup();
}

auto Config::querySleepTime_ms(void) noexcept -> unsigned
{
        return getValue(CONFIG_SLEEP_STR).toInt();
}

auto Config::setSleepTime_ms(unsigned ms) noexcept -> void
{
        setValue(CONFIG_SLEEP_STR, ms);
}
