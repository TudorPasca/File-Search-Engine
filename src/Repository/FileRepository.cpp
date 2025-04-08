#include "../../include/Repository/FileRepository.h"
#include "../../include/utils/FileFilters/ContentFileFilter.h"
#include "../../include/utils/FileFilters/PathKeywordFileFilter.h"
#include "../../include/utils/FileFilters/FileExtensionFilter.h"
#include "../../include/utils/FileFilters/PathLengthFileFilter.h"
#include <pqxx/pqxx>
#include <iostream>

std::vector<FileDTO> FileRepository::getFiles(const std::string &searchQuery) const {
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
                    to_string(row["content"]),
                    to_string(row["extension"]),
                    row["size_bytes"].as<uint64_t>()
            );
            results.push_back(file);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during file indexing: " << e.what() << std::endl;
    }
    return results;
}

std::vector<FileDTO> FileRepository::getFiles(const std::vector<std::unique_ptr<IFileFilter>> &filters) const {
    std::vector<FileDTO> results;
    if (filters.empty()) {
        std::cerr << "[FileRepository] WARNING: No filters were set\n";
        return results;
    }
    try {
        pqxx::connection conn(CONNECTION_STRING);
        pqxx::work txn(conn);
        std::string base_query =
                "SELECT filename, path, content, is_folder, extension, size_bytes "
                "FROM public.file";
        std::vector<std::string> where_clauses;
        for (const auto& filter_ptr : filters) {
            if (const auto* contentFilter = dynamic_cast<const ContentFileFilter*>(filter_ptr.get())) {
                where_clauses.push_back("content_vector @@ plainto_tsquery('english', " + txn.quote(contentFilter->getContent()) + ")");
            }
            else if (const auto* pathFilter = dynamic_cast<const PathKeywordFileFilter*>(filter_ptr.get())) {
                where_clauses.push_back("path LIKE " + txn.quote("%" + pathFilter->getKeyword() + "%"));
            }
            else if (const auto* extFilter = dynamic_cast<const FileExtensionFilter*>(filter_ptr.get())) {
                where_clauses.push_back("extension = " + txn.quote(extFilter->getExtension()));
            }
            else if (const auto* lenFilter = dynamic_cast<const PathLengthFileFilter*>(filter_ptr.get())) {
                where_clauses.push_back("size_bytes = " + std::to_string(lenFilter->getLength()));
            }
            else {
                std::cerr << "[FileRepository] Warning: Encountered unknown filter type during query building." << std::endl;
            }
        }
        std::string final_query = base_query;
        if (!where_clauses.empty()) {
            final_query += " WHERE ";
            for (size_t i = 0; i < where_clauses.size(); ++i) {
                final_query += where_clauses[i];
                if (i < where_clauses.size() - 1) {
                    final_query += " AND ";
                }
            }
        }
        pqxx::result res = txn.exec(final_query);
        txn.commit();

        results.reserve(res.size());
        for (const auto &row : res) {
            FileDTO file(
                    to_string(row["filename"]),
                    to_string(row["path"]),
                    row["is_folder"].as<bool>(),
                    to_string(row["content"]),
                    to_string(row["extension"]),
                    row["size_bytes"].as<uint64_t>()
            );
            results.push_back(std::move(file));
        }
    } catch (const std::exception &e) {
        std::cerr << "[FileRepository] Error: " << e.what() << std::endl;
    }
    return results;
}