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
        std::string query = "SELECT filename, path, content, is_folder, extension, size_bytes, score "
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
            file.setScore(row["score"].as<double>());
            results.push_back(file);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during file indexing: " << e.what() << std::endl;
    }
    return results;
}

std::vector<FileDTO> FileRepository::getFiles(const std::vector<std::unique_ptr<IFileFilter>>& filters) const {
    std::vector<FileDTO> results;
    if (filters.empty()) {
        std::cerr << "[FileRepository] WARNING: No filters were provided.\n";
        return results;
    }
    std::vector<std::string> where_clauses;
    std::vector<std::string> params;
    int param_index = 1;

    for (const auto& filter_ptr : filters) {
        if (!filter_ptr) continue;
        if (const auto* contentFilter = dynamic_cast<const ContentFileFilter*>(filter_ptr.get())) {
            where_clauses.push_back("content_vector @@ plainto_tsquery('english', $" + std::to_string(param_index++) + ")");
            params.push_back(contentFilter->getContent());
        }
        else if (const auto* pathFilter = dynamic_cast<const PathKeywordFileFilter*>(filter_ptr.get())) {
            std::string keyword = pathFilter->getKeyword();
            std::replace(keyword.begin(), keyword.end(), '\\', '/');
            where_clauses.push_back("path LIKE $" + std::to_string(param_index++));
            params.push_back("%" + keyword + "%");
        }
        else if (const auto* extFilter = dynamic_cast<const FileExtensionFilter*>(filter_ptr.get())) {
            where_clauses.push_back("extension = $" + std::to_string(param_index++));
            params.push_back(extFilter->getExtension());
        }
        else if (const auto* lenFilter = dynamic_cast<const PathLengthFileFilter*>(filter_ptr.get())) {
            where_clauses.push_back("size_bytes = $" + std::to_string(param_index++));
            params.push_back(std::to_string(lenFilter->getLength()));
        }
        else {
            std::cerr << "[FileRepository] Warning: Encountered unknown or unsupported filter type during query building." << std::endl;
        }
    }
    if (where_clauses.empty()) {
        std::cerr << "[FileRepository] WARNING: No valid filters could be translated to SQL clauses.\n";
        return results;
    }
    std::string final_query =
            "SELECT filename, path, content, is_folder, extension, size_bytes, score "
            "FROM public.file WHERE ";
    for (size_t i = 0; i < where_clauses.size(); ++i) {
        final_query += where_clauses[i];
        if (i < where_clauses.size() - 1) {
            final_query += " AND ";
        }
    }
    final_query += " ORDER BY score DESC";

    try {
        pqxx::connection conn(CONNECTION_STRING);
        conn.prepare("find_files", final_query);
        pqxx::read_transaction txn(conn);
        pqxx::params prepared_params;
        for (const auto& val : params) {
            prepared_params.append(val);
        }
        pqxx::result res = txn.exec_prepared("find_files", prepared_params);
        results.reserve(res.size());
        for (const auto& row : res) {
            FileDTO file(
                    to_string(row["filename"]),
                    to_string(row["path"]),
                    row["is_folder"].as<bool>(),
                    to_string(row["content"]),
                    to_string(row["extension"]),
                    row["size_bytes"].as<uint64_t>()
            );
            file.setScore(row["score"].as<double>());
            results.push_back(std::move(file));
        }
    } catch (const pqxx::broken_connection& e) {
        std::cerr << "[FileRepository] Database Connection Error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[FileRepository] Database Query Error: " << e.what() << std::endl;
    }

    return results;
}