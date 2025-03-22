#include "../../include/Repository/FileRepository.h"
#include <pqxx/pqxx>
#include <iostream>

std::vector<FileDTO> FileRepository::getFiles(const std::string &searchQuery) {
    std::vector<FileDTO> results;
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        std::string query = "SELECT filename, path, content, is_folder "
                            "FROM public.file "
                            "WHERE content_vector @@ phraseto_tsquery('english', " + txn.quote(searchQuery) + ")";
        pqxx::result res = txn.exec(query);
        txn.commit();
        for (const auto &row : res) {
            FileDTO file(
                    to_string(row["filename"]),
                    to_string(row["path"]),
                    row["is_folder"].as<bool>(),
                    to_string(row["content"])
            );
            results.push_back(file);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during file indexing: " << e.what() << std::endl;
    }
    return results;
}