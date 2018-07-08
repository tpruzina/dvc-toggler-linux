#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QSettings>
#include <QMap>

#define CONFIG_SLEEP_STR	"watcher_sleep_ms"
#define CONFIG_START_MIN_STR	"start_minimized"
#define CONFIG_ENABLED_STR	"enabled"
#define CONFIG_AUTOHIDE_STR	"autohide"
#define CONFIG_DEFAULT_PROFILE_STR "default"

class Config : public QSettings
{
public:
	Config();

	QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant()) const;
	void setValue(const QString &key, const QVariant &getValue);

	QStringList queryProfiles();

	bool queryStartMinimized(void);
	void setStartMinimized(bool val);
	void toggleStartMinimzed() {setStartMinimized(!queryStartMinimized());}

	bool queryAutohide(void);
	void setAutohide(bool val);
	void toggleAutohide() {setAutohide(!queryAutohide());}

	bool queryEnabled(void);
	void setEnabled(bool val);
	void toggleEnabled() {setEnabled(!queryEnabled());}

	QString queryIconPath(const QString &profile_name);
	void setIconPath(const QString &profile_name, const QString &path);

	QMap<int,int> queryDVC(const QString &profile_name);
	void setDVC(const QString &profile_name, QMap<int,int> &map);

	unsigned querySleepTime_ms(void);
	void setSleepTime_ms(unsigned ms);

	void removeProfile(const QString &key);
};
#endif // CONFIG_HPP
