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

	QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
	void setValue(const QString &key, const QVariant &value);

	QStringList query_profiles();

	bool query_start_minimized(void);
	void set_start_minimized(bool val);
	void toggle_start_minimzed() {set_start_minimized(!query_start_minimized());}

	bool query_autohide(void);
	void set_autohide(bool val);
	void toggle_autohide() {set_autohide(!query_autohide());}

	bool query_enabled(void);
	void set_enabled(bool val);
	void toggle_enabled() {set_enabled(!query_enabled());}

	QString query_icon_path(const QString &profile_name);
	void set_icon_path(const QString &profile_name, const QString &path);

	QMap<int,int> query_dvc(const QString &profile_name);
	void set_dvc(const QString &profile_name, QMap<int,int> &map);

	unsigned query_sleep_time_ms(void);
	void set_sleep_time_ms(unsigned ms);

	void remove_profile(const QString &key);
};
#endif // CONFIG_HPP
