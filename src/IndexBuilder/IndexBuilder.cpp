#include "../../include/IndexBuilder/IndexBuilder.h"
#include <iostream>

namespace fs = std::filesystem;

pqxx::connection IndexBuilder::createDBConnection() const {
    try {
        pqxx::connection conn(CONNECTION_STRING);
        if (!conn.is_open()) {
            throw std::runtime_error("Failed to open database connection.");
        }
        return conn;
    } catch (const std::exception &e) {
        std::cerr << "Database connection error: " << e.what() << std::endl;
        throw;
    }
}

void IndexBuilder::indexFiles(const std::filesystem::path &path) const {
    std::vector<FileDTO> files = scraper->getFilesRecursively(path);
    pqxx::connection conn = createDBConnection();
    try {
        pqxx::work txn(conn);
        for (const auto &file : files) {
            std::string query = R"(INSERT INTO public.file ("path", "content", is_folder) VALUES ()"
                                + txn.quote(file.getAbsolutePath()) + ", "
                                + txn.quote(file.getContents()) + ", "
                                + txn.quote(file.isFolder()) + ")";
            txn.exec(query);
        }
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Error during file indexing: " << e.what() << std::endl;
    }
}