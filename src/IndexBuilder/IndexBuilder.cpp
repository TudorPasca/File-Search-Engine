#include "../../include/IndexBuilder/IndexBuilder.h"

#include <ostream>
#include <iomanip>
#include <chrono>

namespace fs = std::filesystem;

void IndexBuilder::indexFiles(const std::filesystem::path &path) {
    std::vector<FileDTO> files = scraper->getFilesRecursively(path);
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        const std::string preparedStatement = "insert_update_file";
        conn.prepare(preparedStatement, R"(
            INSERT INTO public.file (filename, path, content, is_folder, extension, size_bytes)
            VALUES ($1, $2, $3, $4, $5, $6)
            ON CONFLICT (path) DO UPDATE SET
                filename = EXCLUDED.filename,
                content = EXCLUDED.content,
                is_folder = EXCLUDED.is_folder,
                extension = EXCLUDED.extension,
                size_bytes = EXCLUDED.size_bytes
        )");
        for (const auto &file: files) {
            pqxx::subtransaction file_savepoint(txn);
            try {
                file_savepoint.exec_prepared(preparedStatement,
                                             file.getName(),
                                             file.getAbsolutePath(),
                                             file.getContents(),
                                             file.isFolder(),
                                             file.getExtension(),
                                             file.getSizeBytes());
                file_savepoint.commit();
            } catch (const pqxx::sql_error &e) {
                std::cerr << "[IndexBuilder] Error for file: " << file.getAbsolutePath()
                          << ": " << e.sqlstate() << " | " << e.what() << std::endl;
            } catch (const std::exception &e) {
                std::cerr << "[IndexBuilder] Error for file (skipping): " << file.getAbsolutePath()
                          << ": " << e.what() << std::endl;
            }
        }
        txn.commit();
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