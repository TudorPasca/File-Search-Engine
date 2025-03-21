#include "../../include/IndexBuilder/IndexBuilder.h"
#include <iostream>

namespace fs = std::filesystem;

void IndexBuilder::indexFiles(const std::filesystem::path &path) const {
    std::vector<FileDTO> files = scraper->getFilesRecursively(path);
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        for (const auto &file: files) {
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
        }
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Error during file indexing: " << e.what() << std::endl;
    }
}