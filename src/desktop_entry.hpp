#include <fstream>  //ifstream, ofstream
#include <cstdio>
#include <sys/stat.h>
#include <QStandardPaths>

// Desktop Entry Specification
// https://specifications.freedesktop.org/desktop-entry-spec/latest

// this creates dvc-toggler-linux.desktop file in desired path
class DesktopEntry
{
public:
        static bool create(const QString &dirpath)
        {
                // autostart directory might not exist on some systems
                // if so create it
                mkdir(dirpath.toStdString().c_str(), S_IRWXU);

                std::ofstream file(dirpath.toStdString() + "/dvc-toggler-linux.desktop",
                                   std::ofstream::out | std::ofstream::trunc);

                if(!file.is_open())
                {
                        std::cerr << "Failure writing DesktopEntry to "
                                  << dirpath.toStdString() << " file" << std::endl;
                        return false;
                }
                // INI style header
                file << "[Desktop Entry]" << std::endl;

                // key=value style
                setValue(file, "Type", "Application");
                setValue(file, "Exec", "dvc-toggler-linux"); // fixme
                setValue(file, "Name", "Linux DVC Toggler");
                setValue(file, "GenericName",
                                 "Digital Vibrance controll app"
                                 " for Nvidia desktops.");
                setValue(file, "Icon", "LinuxDVCToggler");
                setValue(file, "Terminal", "false");

                file.close();
                return true;
        }

        static bool exists(const QString &path)
        {
               std::ifstream f(path.toStdString());
               return f.good();
        }

        static bool remove(const QString &path)
        {
                if(std::remove(path.toStdString().c_str()) == 0)
                        return true;
                else
                        return false;
        }

        static void setValue(std::ofstream &file, const QString &key, const QString &value)
        {
                file << key.toStdString() << "=" << value.toStdString() << std::endl;
        }

        // $HOME/.local/share/applications
        static QString getApplicationsPath()
        {
                return QStandardPaths::writableLocation(
                                        QStandardPaths::ApplicationsLocation);
        }

        // $HOME/.config/autostart
        static QString getAutostartPath()
        {
                return QStandardPaths::writableLocation(
                                        QStandardPaths::ConfigLocation)
                                + "/autostart";
        }
};

