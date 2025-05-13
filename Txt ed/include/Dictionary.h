#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "IDictionary.h"
#include "FileManager.h"
#include <string>
#include <vector>
#include <ctime> // For std::time_t

using namespace std;

class Dictionary : public IDictionary {
private:
    string dictionaryFilePath; // Path to the dictionary file
    string indexFilePath;      // Path to the index file
    std::time_t lastModifiedTime; // Stores the last modified time of dictionary.txt

    // Helper function to rebuild the index file
    void rebuildIndex();

public:
    // Constructor: Initializes the dictionary with file paths
    Dictionary(const string& dictionaryFilePath, const string& indexFilePath);

    // CRUD operations
    void addWord(const string& word, const vector<string>& synonyms) override;
    void searchWord(const string& word) const override;
    void updateWord(const string& word, const vector<string>& synonyms) override;
    void deleteWord(const string& word) override;

    // Checks if a word exists in the dictionary
    bool wordExists(const string& word) const;

    // Suggests corrections for a misspelled word
    vector<string> suggestCorrections(const string& word) const;

    // Removes duplicate entries from the dictionary file
    void removeDuplicatesFromDictionary();

    // Retrieves synonyms for a given word
    vector<string> getSynonyms(const string& word) const;

    // Checks if the dictionary file has been modified and rebuilds the index
    void checkAndRebuildIndex();

    // Import dictionary from a CSV file
    void importFromCSV(const string& filePath);

    // Export dictionary to a CSV file
    void exportToCSV(const string& csvFilePath) const;
};

#endif // DICTIONARY_H