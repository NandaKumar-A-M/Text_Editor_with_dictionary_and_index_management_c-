// Defines Interface for Dictionary operations
#ifndef IDICTIONARY_H
#define IDICTIONARY_H

#include <string>
#include <vector>

using namespace std; // Simplifies code by removing the need for std::

class IDictionary {
public:
    // Adds a new word with its synonyms to the dictionary
    virtual void addWord(const string& word, const vector<string>& synonyms) = 0;

    // Searches for a word in the dictionary
    virtual void searchWord(const string& word) const = 0;

    // Updates the synonyms of an existing word in the dictionary
    virtual void updateWord(const string& word, const vector<string>& synonyms) = 0;

    // Deletes a word from the dictionary
    virtual void deleteWord(const string& word) = 0;

    // Virtual destructor
    virtual ~IDictionary() = default;
};

#endif // IDICTIONARY_H