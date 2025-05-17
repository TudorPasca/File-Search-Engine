#include "../../../include/Service/SpellingCorrection/SpellingCorrectorService.h"
#include <algorithm>
#include <fstream>
#include <regex>
#include <sstream>

SpellingCorrectorService::SpellingCorrectorService(const std::string &dictionaryPath) {
    parseDictionary(dictionaryPath);
}

std::string SpellingCorrectorService::correct(const std::string &word) {
    std::string lowercaseWord = word;
    std::transform(word.begin(), word.end(), lowercaseWord.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::unordered_set<std::string> candidates = getCandidates(lowercaseWord);
    if (candidates.empty())
        return word;
    return *std::max_element(candidates.begin(), candidates.end(),
                             [this](const std::string &s1, const std::string &s2) {
                                 return getProbability(s1) > getProbability(s2);
                             });
}

double SpellingCorrectorService::getProbability(const std::string &word) {
    if (word_count_.find(word) == word_count_.end() || word_count_[word] == 0)
        return 0.0;
    return static_cast<double>(word_count_[word]) / total_word_count_;
}


std::unordered_set<std::string> SpellingCorrectorService::getCandidates(const std::string &word) const {
    std::unordered_set<std::string> candidates = getKnownWords({word});
    if (!candidates.empty())
        return candidates;
    candidates = getKnownWords(getEdits1(word));
    if (!candidates.empty())
        return candidates;
    candidates = getKnownWords(getEdits2(word));
    if (!candidates.empty())
        return candidates;
    return {word};
}

std::unordered_set<std::string> SpellingCorrectorService::getKnownWords(const std::unordered_set<std::string>& words) const {
    std::unordered_set<std::string> knownWords;
    for (const auto &word: words)
        if (word_count_.contains(word))
            knownWords.insert(word);
    return knownWords;
}

std::unordered_set<std::string> SpellingCorrectorService::getEdits2(const std::string &word) const {
    std::unordered_set<std::string> result;
    std::unordered_set<std::string> edits1 = getEdits1(word);
    for (const auto &it: edits1)
        result.merge(getEdits1(it));
    return result;
}

std::unordered_set<std::string> SpellingCorrectorService::getEdits1(const std::string &word) const {
    std::unordered_set<std::string> result;
    std::vector<std::pair<std::string, std::string>> splits;
    for (int i = 0; i <= word.length(); ++i) {
        splits.emplace_back(word.substr(0, i), word.substr(i));
    }
    //Deletes
    for (const auto& s : splits) {
        if (!s.second.empty()) {
            result.insert(s.first + s.second.substr(1));
        }
    }
    //Transposes
    for (const auto& s : splits) {
        if (s.second.length() > 1) {
            result.insert(s.first + s.second[1] + s.second[0] + s.second.substr(2));
        }
    }
    //Replaces
    for (const auto& s : splits) {
        if (!s.second.empty()) {
            for (char c : alphabet_) {
                result.insert(s.first + c + s.second.substr(1));
            }
        }
    }
    //Insertion
    for (const auto& s : splits) {
        for (char c : alphabet_) {
            result.insert(s.first + c + s.second);
        }
    }
    return result;
}

std::vector<std::string> SpellingCorrectorService::tokenize(const std::string &content) {
    std::string lowercaseContent = content;
    std::transform(content.begin(), content.end(), lowercaseContent.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::regex wordRegex(R"(\w+)");
    auto words_begin = std::sregex_iterator(content.begin(), content.end(), wordRegex);
    auto words_end = std::sregex_iterator();
    std::vector<std::string> tokens;
    for (auto it = words_begin; it != words_end; ++it) {
        tokens.push_back(it->str());
    }
    return tokens;
}


void SpellingCorrectorService::parseDictionary(const std::string &dictionaryPath) {
    std::fstream file(dictionaryPath);
    if (!file.is_open()) {
        throw std::runtime_error("[SpellingCorrectorService] Could not open dictionary at " + dictionaryPath);
    }
    std::ostringstream contentStream;
    contentStream << file.rdbuf();
    file.close();
    std::string content = contentStream.str();
    std::vector<std::string> words = tokenize(content);
    for (const auto &word: words) {
        word_count_[word]++;
    }
    total_word_count_ = 0;
    for (const auto &it: word_count_) {
        total_word_count_ += it.second;
    }
}
