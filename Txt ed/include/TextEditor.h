// Handles text file operations (open, edit, save, search).
#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include "Dictionary.h" 
#include <string>

using namespace std; // Simplifies code by removing the need for std::

class TextEditor {
public:
    // Opens a file and displays its content
    static void openFile(const string& filePath);

    // Saves content to a file
    static void saveFile(const string& filePath, const string& content);

    // Edits a file by allowing the user to modify or add lines
    static void editFile(const string& filePath);

    // Searches for a word in a file and displays the lines where it is found
    static void searchInFile(const string& filePath, const string& word);

    // Spell-checks a text file and suggests corrections
    // static void spellCheck(const string& filePath, const Dictionary& dictionary);
    void spellCheck(const std::string& filePath, Dictionary& dictionary);

    // Suggests and replaces words with synonyms
void replaceWithSynonyms(const string& filePath, Dictionary& dictionary);
void analyzeWordFrequencyRealTime(const std::string& filePath, Dictionary& dictionary);
};

#endif // TEXTEDITOR_H