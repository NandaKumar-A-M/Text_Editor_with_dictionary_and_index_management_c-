#include "Dictionary.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

// Declare the function before it is used
std::time_t getLastModifiedTime(const std::string& filePath);

Dictionary::Dictionary(const string& dictionaryFilePath, const string& indexFilePath)
    : dictionaryFilePath(dictionaryFilePath), indexFilePath(indexFilePath) {
    rebuildIndex();
    lastModifiedTime = getLastModifiedTime(dictionaryFilePath); // Initialize last modified time
}

time_t getLastModifiedTime(const string& filePath) {
    struct stat fileInfo; //declaring struct to hold file info //stat is used to store meta data about file such as size, permission and modified_time.
    if (stat(filePath.c_str(), &fileInfo) == 0) { //c_str is used to convert str to c-style string that is required by the stat.
        return fileInfo.st_mtime; // Return the last modified time
    }
    return 0; // Return 0 if the file does not exist
}

void Dictionary::rebuildIndex() {
    ifstream dictionaryFile(dictionaryFilePath);
    ofstream indexFile(indexFilePath, ios::trunc);

    if (!dictionaryFile || !indexFile) {
        cerr << "Error opening dictionary or index file.\n";
        return;
    }

    string line;
    streamoff offset = 0;

    while (getline(dictionaryFile, line)) {
        istringstream iss(line); //iss used for parsing the line to extract the word before :
        string word;
        if (getline(iss, word, ':')) {
            indexFile << word << ":" << offset << "\n";
        }
        offset = dictionaryFile.tellg(); //tellg() is an inline member function that returns the current position of the input stream's read pointer
    }

    dictionaryFile.close();
    indexFile.close();
}

void Dictionary::addWord(const string& word, const vector<string>& synonyms) {
    
    if (wordExists(word)) {
        cerr << "Error: Word already exists in the dictionary.\n";
        return;
    }

    ofstream dictionaryFile(dictionaryFilePath, ios::app);
    if (!dictionaryFile) {
        cerr << "Error opening dictionary file.\n";
        return;
    }

    dictionaryFile << word << ":";
    for (size_t i = 0; i < synonyms.size(); ++i) {
        dictionaryFile << synonyms[i];//append each synonym from vector to dict
        if (i < synonyms.size() - 1) {
            dictionaryFile << ",";//append , after each synonym's append
        }
    }
    dictionaryFile << "\n";//append newline at end so that next time append continues from new line
    dictionaryFile.close();

    rebuildIndex();
}

void Dictionary::searchWord(const string& word) const {
    if (!wordExists(word)) {
        cout << "Word not found.\n";
        return;
    }

    ifstream dictionaryFile(dictionaryFilePath);
    if (!dictionaryFile) {
        cerr << "Error opening dictionary file.\n";
        return;
    }

    ifstream indexFile(indexFilePath);
    if (!indexFile) {
        cerr << "Error opening index file.\n";
        return;
    }

    string line;
    while (getline(indexFile, line)) {
        if (line.find(word + ":") == 0) {//returns 0 if the word finds at very beginning of the line, else the index position will be returned but it wont accepts it
            size_t colonPos = line.find(':');
            streamoff offset = stoll(line.substr(colonPos + 1)); // Extract offset //stoll converts string to a long long integer
            dictionaryFile.seekg(offset); // Seek to the word's location in the dictionary file //seekg is use to move the get pointer to extract info
            string dictionaryLine;
            getline(dictionaryFile, dictionaryLine);
            cout << "Found: " << dictionaryLine << "\n";
            dictionaryFile.close();
            indexFile.close();
            return;
        }
    }

    dictionaryFile.close();
    indexFile.close();
    cout << "Word not found.\n";
}

void Dictionary::updateWord(const string& word, const vector<string>& synonyms) {
    if (word.empty() || synonyms.empty()) {
        cerr << "Error: Word or synonyms cannot be empty.\n";
        return;
    }
    
    if (!wordExists(word)) {
        cerr << "Word not found.\n";
        return;
    }

    ifstream dictionaryFile(dictionaryFilePath);
    ofstream tempFile("temp.txt");

    if (!dictionaryFile || !tempFile) {
        cerr << "Error opening files.\n";
        return;
    }

    string line;
    while (getline(dictionaryFile, line)) {
        if (line.find(word + ":") == 0) {
            tempFile << word << ":";
            for (size_t i = 0; i < synonyms.size(); ++i) {
                tempFile << synonyms[i];
                if (i < synonyms.size() - 1) {
                    tempFile << ",";
                }
            }
            tempFile << "\n";
        } else {
            tempFile << line << "\n";
        }
    }

    dictionaryFile.close();
    tempFile.close();

    remove(dictionaryFilePath.c_str());
    rename("temp.txt", dictionaryFilePath.c_str());
    rebuildIndex();
}

void Dictionary::deleteWord(const string& word) {
    if (!wordExists(word)) {
        cerr << "Word not found.\n";
        return;
    }

    ifstream dictionaryFile(dictionaryFilePath);
    ofstream tempFile("temp.txt");

    if (!dictionaryFile || !tempFile) {
        cerr << "Error opening files.\n";
        return;
    }

    string line;
    while (getline(dictionaryFile, line)) {
        if (line.find(word + ":") != 0) { // Skip the line with the word to delete
            tempFile << line << "\n";
        }
    }

    dictionaryFile.close();
    tempFile.close();

    remove(dictionaryFilePath.c_str());
    rename("temp.txt", dictionaryFilePath.c_str());
    rebuildIndex();
}

bool Dictionary::wordExists(const string& word) const {
    ifstream indexFile(indexFilePath);
    if (!indexFile) {
        cerr << "Error opening index file.\n";
        return false;
    }

    string line;
    while (getline(indexFile, line)) {
        if (line.find(word + ":") == 0) {
            indexFile.close();
            return true;
        }
    }

    indexFile.close();
    return false;
}

// Function to calculate Levenshtein Distance

int levenshteinDistance(const string& a, const string& b, size_t i, size_t j) {
    // Base cases
    if (i == 0) return j; // If string `a` is empty, all characters of `b` need to be inserted
    if (j == 0) return i; // If string `b` is empty, all characters of `a` need to be deleted

    // If characters are the same, no operation is needed
    if (a[i - 1] == b[j - 1]) {
        return levenshteinDistance(a, b, i - 1, j - 1);
    }

    // Otherwise, consider all operations: insertion, deletion, substitution
    return 1 + min({
        levenshteinDistance(a, b, i - 1, j),    // Deletion
        levenshteinDistance(a, b, i, j - 1),    // Insertion
        levenshteinDistance(a, b, i - 1, j - 1) // Substitution
    });
}

// Wrapper function
int levenshteinDistance(const string& a, const string& b) {
    return levenshteinDistance(a, b, a.size(), b.size());
}

// Suggest corrections for a misspelled word
vector<string> Dictionary::suggestCorrections(const string& word) const {
    ifstream dictionaryFile(dictionaryFilePath);
    if (!dictionaryFile) {
        cerr << "Error opening dictionary file.\n";
        return {};
    }

    vector<string> suggestions;
    string line;

    // Read each word from the dictionary
    while (getline(dictionaryFile, line)) {
        string dictionaryWord = line.substr(0, line.find(':')); // Extract the word before the colon

        // Calculate the Levenshtein Distance
        int distance = levenshteinDistance(word, dictionaryWord);

        // Add the word to suggestions if the distance is <= 2
        if (distance <= 2) {
            suggestions.push_back(dictionaryWord);
        }
    }

    dictionaryFile.close();



    // Sort suggestions by relevance (shorter distance first)
    sort(suggestions.begin(), suggestions.end(), [&](const string& a, const string& b) {
        return levenshteinDistance(word, a) < levenshteinDistance(word, b);
    });

    // Limit to top 5 suggestions
    if (suggestions.size() > 5) {
        suggestions.resize(5);
    }

    return suggestions;
}

void Dictionary::checkAndRebuildIndex() {
    std::time_t currentModifiedTime = getLastModifiedTime(dictionaryFilePath);
    if (currentModifiedTime != lastModifiedTime) {
        std::cout << "Detected changes in dictionary.txt. Rebuilding index...\n";
        rebuildIndex();
        lastModifiedTime = currentModifiedTime; // Update the last modified time
    }
}

vector<string> Dictionary::getSynonyms(const string& word) const {
    ifstream dictionaryFile(dictionaryFilePath);
    if (!dictionaryFile) {
        cerr << "Error opening dictionary file.\n";
        return {};
    }

    string line;
    while (getline(dictionaryFile, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != string::npos) {
            string dictionaryWord = line.substr(0, colonPos);
            if (dictionaryWord == word) {
                string synonymsStr = line.substr(colonPos + 1);
                vector<string> synonyms;
                stringstream ss(synonymsStr);
                string synonym;
                while (getline(ss, synonym, ',')) {
                    synonyms.push_back(synonym);
                }
                return synonyms;
            }
        }
    }

    return {}; // Return an empty vector if no synonyms are found
}

void Dictionary::importFromCSV(const string& filePath) {
    ifstream csvFile(filePath);
    if (!csvFile) {
        cerr << "Error opening CSV file for import: " << filePath << "\n";
        return;
    }

    string dictionaryFileName;
    cout << "Enter the dictionary file name to save the imported data (e.g., dictionary.txt): ";
    cin >> dictionaryFileName;

    ofstream dictionaryFile(dictionaryFileName, ios::app);
    if (!dictionaryFile) {
        cerr << "Error opening dictionary file for writing: " << dictionaryFileName << "\n";
        return;
    }

    string line;
    while (getline(csvFile, line)) {
        // Each line in the CSV should be in the format: word,synonym1,synonym2,...
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string word = line.substr(0, commaPos);
            string synonyms = line.substr(commaPos + 1);
            dictionaryFile << word << ":" << synonyms << "\n";
        }
    }

    csvFile.close();
    dictionaryFile.close();

    rebuildIndex(); // Rebuild the index after importing
    cout << "Import completed successfully from " << filePath << " to " << dictionaryFileName << ".\n";
}
void Dictionary::exportToCSV(const string& csvFilePath) const {
    cout << "Enter the source dictionary file names to export (separated by spaces): ";
    cin.ignore();
    string dictionaryFileNames;
    getline(cin, dictionaryFileNames);

    vector<string> fileNames;
    stringstream ss(dictionaryFileNames);
    string fileName;
    while (ss >> fileName) fileNames.push_back(fileName);

    ofstream csvFile(csvFilePath);
    if (!csvFile) {
        cerr << "Error opening CSV file: " << csvFilePath << "\n";
        return;
    }

    bool dataWritten = false;
    for (const string& dictionaryFileName : fileNames) {
        ifstream dictionaryFile(dictionaryFileName);
        if (!dictionaryFile) {
            cerr << "Error opening file: " << dictionaryFileName << "\n";
            continue;
        }

        string line;
        while (getline(dictionaryFile, line)) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                csvFile << line.substr(0, colonPos) << "," << line.substr(colonPos + 1) << "\n";
                dataWritten = true;
            }
        }
    }

    csvFile.close();
    if (dataWritten)
        cout << "Export completed to " << csvFilePath << ".\n";
    else {
        remove(csvFilePath.c_str());
        cerr << "No data exported. Check input files.\n";
    }
}