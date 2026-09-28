#include "Utils.h"
#include <ctime>
#include <format>
#include <fstream>
#include <sstream>

namespace oriongl::utils {
auto readFile(const std::string &path) -> std::string {
    try {
        std::ifstream file(path);

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::stringstream buffer;

        buffer << file.rdbuf();

        return buffer.str();
    } catch (const std::ifstream::failure &e) {

        throw std::runtime_error(e.what());
    }
}

void writeFile(const std::string &path, const std::string &content) {
    try {
        std::ofstream file(path);

        file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        file << content << '\n';

        file.close();
    } catch (const std::ofstream::failure &e) {

        throw std::runtime_error(e.what());
    }
}

void logger(std::string log) {
    time_t timestamp = time(nullptr);
    tm datetime{};

#ifdef _WIN32
    localtime_s(&datetime, &timestamp);
#else
    localtime_r(&timestamp, &datetime);
#endif

    char *ascDateTime = std::asctime(&datetime);

    std::string formattedStr = std::format("Date: {} - {}", ascDateTime, log);

    writeFile("log.txt", formattedStr);
}
} // namespace oriongl::utils