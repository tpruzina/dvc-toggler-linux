#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QSettings>
#include <QMap>

#define CONFIG_SLEEP_STR	"watcher_sleep_ms"
#define CONFIG_START_MIN_STR	"start_minimized"
#define CONFIG_ENABLED_STR	"enabled"
#define CONFIG_AUTOHIDE_STR	"autohide"
#define CONFIG_DEFAULT_PROFILE_STR "default"
#define CONFIG_TRAY_INFO_SHOWN	"tray_icon_warning_shown"

class Config : public QSettings
{
public:
	Config();

	QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant()) const;
	void setValue(const QString &key, const QVariant &getValue);

	QStringList queryProfiles();

	bool get_bool(const QString &attribute)
	{
		return getValue(attribute).toBool();
	}

	bool set_bool(const QString &attribute, bool val)
	{
		setValue(attribute, val);
		return val;
	}

	bool toggle_bool(const QString &attribute)
	{
		return set_bool(attribute, !get_bool(attribute));
	}

	QString queryIconPath(const QString &profile_name);
	void setIconPath(const QString &profile_name, const QString &path);

	QMap<int,int> queryDVC(const QString &profile_name);
	void setDVC(const QString &profile_name, QMap<int,int> &map);

	unsigned querySleepTime_ms(void);
	void setSleepTime_ms(unsigned ms);

	void removeProfile(const QString &key);
};
#endif // CONFIG_HPP
