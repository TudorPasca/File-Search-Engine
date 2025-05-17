#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class SpellingCorrectorService {
public:
    explicit SpellingCorrectorService(const std::string& dictionaryPath);

    std::string correct(const std::string &word);

private:
    std::unordered_map<std::string, int> word_count_;
    long long total_word_count_ = 0;
    const std::string alphabet_ = "abcdefghijklmnopqrstuvwxyz";

    static std::vector<std::string> tokenize(const std::string &content);

    void parseDictionary(const std::string& dictionaryPath);

    double getProbability(const std::string& word);

    std::unordered_set<std::string> getCandidates(const std::string &word) const;

    std::unordered_set<std::string> getKnownWords(const std::unordered_set<std::string>& words) const;

    std::unordered_set<std::string> getEdits1(const std::string& word) const;

    std::unordered_set<std::string> getEdits2(const std::string& word) const;
};
