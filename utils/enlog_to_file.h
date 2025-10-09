#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace RammusLib {

    namespace Log
    {
        class CAppFileLog
        {
            std::ofstream m_f;
        public:
            CAppFileLog()
            {
                namespace fs = std::filesystem;
                // Получаем путь к exe и формируем logs/<имя_программы>.log
                fs::path exePath = get_module_path();
                fs::path logsDir = exePath.parent_path() / "logs";
                if (!fs::exists(logsDir)) {
                    fs::create_directories(logsDir);
                }
                std::string log_filename = (logsDir / (exePath.stem().string() + ".log")).string();

                // Получаем текущее время для имени файла
                auto now = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(now);
                std::tm tm;
#ifdef _WIN32
                localtime_s(&tm, &t);
#else
                localtime_r(&t, &tm);
#endif
                char timebuf[32];
                std::strftime(timebuf, sizeof(timebuf), "%y%m%d-%H%M%S", &tm);

                std::string bak_filename = log_filename + ".bak." + timebuf;

                if (fs::exists(log_filename)) {
                    if (fs::exists(bak_filename)) fs::remove(bak_filename);
                    fs::rename(log_filename, bak_filename);
                }

                m_f.open(log_filename, std::ios_base::out | std::ios_base::trunc);
                SendMessageToLog(0, "**** START LOG FILE");
            }
            ~CAppFileLog()
            {
                SendMessageToLog(0, "FINISH LOG FILE ****");
            }
            void SendMessageToLog(int nMessageType, const std::string& Message)
            {
                auto now = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(now);
                std::tm tm;
#ifdef _WIN32
                localtime_s(&tm, &t);
#else
                localtime_r(&t, &tm);
#endif
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

                m_f << std::setfill('0') << std::setw(2) << tm.tm_mday << ".";
                m_f << std::setfill('0') << std::setw(2) << (tm.tm_mon + 1) << ".";
                m_f << std::setfill('0') << std::setw(2) << (tm.tm_year + 1900) << " ";
                m_f << std::setfill('0') << std::setw(2) << tm.tm_hour << ":";
                m_f << std::setfill('0') << std::setw(2) << tm.tm_min << ":";
                m_f << std::setfill('0') << std::setw(2) << tm.tm_sec << ".";
                m_f << std::setfill('0') << std::setw(3) << ms.count() << " ";
                m_f << Message << std::endl;
                m_f.flush();
            }
        private:
            std::filesystem::path get_module_path() const
            {
#ifdef _WIN32
                wchar_t exePath[MAX_PATH];
                GetModuleFileNameW(nullptr, exePath, MAX_PATH);
                return std::filesystem::path(exePath);
#else
                char exePath[1024];
                ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
                exePath[len] = '\0';
                return std::filesystem::path(exePath);
#endif
            }
        };

    }; //Log

};//RammusLib