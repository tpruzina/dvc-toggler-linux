#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QSettings>
#include <QMap>

#define CONFIG_SLEEP_STR        "watcher_sleep_ms"
#define CONFIG_START_MIN_STR    "start_minimized"
#define CONFIG_ENABLED_STR      "enabled"
#define CONFIG_AUTOHIDE_STR     "autohide"
#define CONFIG_TRAY_INFO_SHOWN  "tray_icon_warning_shown"
#define CONFIG_DEFAULT_PROFILE_STR      "default"

using DVC_map = QMap<int,int>;

class Config : public QSettings
{
public:
        Config() noexcept;

        QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant()) const noexcept;
        void setValue(const QString &key, const QVariant &value) noexcept;

        QStringList queryProfiles() noexcept;

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

        QString queryIconPath(const QString &profile_name) noexcept;
        void setIconPath(const QString &profile_name, const QString &path) noexcept;

        DVC_map queryDVC(const QString &profile_name) noexcept;
        void setDVC(const QString &profile_name, DVC_map &map) noexcept;

        unsigned querySleepTime_ms(void) noexcept;
        void setSleepTime_ms(unsigned ms) noexcept;

        void removeProfile(const QString &key) noexcept;
};
#endif // CONFIG_HPP
