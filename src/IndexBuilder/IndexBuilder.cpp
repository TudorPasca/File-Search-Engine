#include "../../include/IndexBuilder/IndexBuilder.h"

#include <ostream>

namespace fs = std::filesystem;

void IndexBuilder::indexFiles(const std::filesystem::path &path) {
    logger->logInfo("[IndexBuilder] Starting indexing files from " + path.generic_string());
    std::vector<FileDTO> files = scraper->getFilesRecursively(path);
    if (files.empty())
        logger->logWarning("[IndexBuilder] No files found in path " + path.generic_string());
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
                logger->logError("[IndexBuilder] SQL exception for file: " + file.getAbsolutePath() + ": " + e.sqlstate());
            } catch (const std::exception &e) {
                logger->logError("[IndexBuilder] Exception for file (skipping) " + file.getAbsolutePath() + ": " + e.what());
            }
        }
        txn.commit();
    } catch (const std::exception &e) {
        logger->logError("[IndexBuilder] CRITICAL ERROR - INDEXING FAILED FOR PATH " + path.generic_string() + ": " + e.what());
    }
    logger->logInfo("[IndexBuilder] Indexing finished for path " + path.generic_string());
}