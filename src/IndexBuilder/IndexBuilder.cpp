#include "../../include/IndexBuilder/IndexBuilder.h"

#include <ostream>
#include <iomanip>
#include <chrono>

namespace fs = std::filesystem;

void IndexBuilder::indexFiles(const std::filesystem::path &path) {
    std::vector<FileDTO> files = scraper->getFilesRecursively(path);
    try {
        pqxx::connection conn(CONNECTION_STRING);
        for (const auto &file: files) {
            pqxx::work txn(conn);
            std::string query = R"(INSERT INTO public.file (filename, path, content, is_folder)
                       VALUES ( )"
                                + txn.quote(file.getName()) + ", "
                                + txn.quote(file.getAbsolutePath()) + ", "
                                + txn.quote(file.getContents()) + ", "
                                + txn.quote(file.isFolder()) + R"()
                       ON CONFLICT (path) DO UPDATE SET
                       filename = EXCLUDED.filename,
                       content = EXCLUDED.content,
                       is_folder = EXCLUDED.is_folder)";
            txn.exec(query);
            txn.commit();
        }
        writeToLog(path, files);
    } catch (const std::exception &e) {
        std::cerr << "Error during file indexing: " << e.what() << std::endl;
    }
}

void IndexBuilder::writeToLog(const std::filesystem::path &path, std::vector<FileDTO> files) {
    if (!logFile.is_open()) {
        std::cerr << "Log file is not open!" << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream time_ss;
    time_ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

    logFile << "[" << time_ss.str() << "] Indexing path: " << path.string() << std::endl;
    for (const auto &file: files) {
        logFile << "  - " << file.getAbsolutePath() << (file.isFolder() ? " (Folder)" : " (File)") << std::endl;
    }
    logFile << "----------------------------------------" << std::endl;
    logFile.flush();
}