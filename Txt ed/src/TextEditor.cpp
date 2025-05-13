#include "TextEditor.h"
#include "FileManager.h"
#include "Dictionary.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

using namespace std;



// Opens a file and displays its content
void TextEditor::openFile(const string& filePath) {
    string content = FileManager::readFile(filePath); // Read the file content
    if (!content.empty()) {
        cout << "File Content:\n" << content << "\n"; // Display the content
    } else {
        cerr << "Error: File is empty or could not be read.\n";
    }
}

// Saves content to a file
void TextEditor::saveFile(const string& filePath, const string& content) {
    FileManager::writeFile(filePath, content); // Write the content to the file
    cout << "File saved successfully.\n";
}

void TextEditor::editFile(const string& filePath) {
    ifstream inputFile(filePath.c_str());
    if (!inputFile) {
        cerr << "Error opening file. Make sure the file exists.\n";
        return;
    }

    vector<string> lines;
    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    cout << "Current content of the file:\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        cout << i + 1 << ": " << lines[i] << "\n";
    }



    int lineNumber;
    cout << "Enter the line number to edit (or 0 to add new lines): ";
    cin >> lineNumber;

    if (lineNumber > 0 && lineNumber <= static_cast<int>(lines.size())) {
        cout << "Enter the new content for line " << lineNumber << ": ";
        cin.ignore();
        getline(cin, line);
        lines[lineNumber - 1] = line;
    } else if (lineNumber == 0) {
        cout << "Enter new lines to add (type 'END' on a new line to finish):\n";
        cin.ignore();
        while (true) {
            getline(cin, line);
            if (line == "END") break;
            lines.push_back(line);
        }
    } else {
        cout << "Invalid line number.\n";
    }

    ofstream outputFile(filePath.c_str());
    if (!outputFile) {
        cerr << "Error opening file for writing.\n";
        
        return;
    }

    for (const auto& updatedLine : lines) {
        outputFile << updatedLine << "\n";
    }
    outputFile.close();

    cout << "File edited and saved successfully.\n";

}


// Searches for a word in a file and displays the lines where it is found
void TextEditor::searchInFile(const string& filePath, const string& word) {
    vector<string> lines = FileManager::readLines(filePath); // Read all lines from the file
    bool found = false;

    // Loop through each line to search for the word
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].find(word) != string::npos) { // Check if the word exists in the line
            cout << "Found \"" << word << "\" on line " << i + 1 << ": " << lines[i] << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "Word \"" << word << "\" not found in the file.\n";
    }
}

// Spell-checks a text file and suggests corrections
void TextEditor::spellCheck(const string& filePath, Dictionary& dictionary) {
    ifstream file(filePath);
    if (!file) {
        cerr << "Error opening file: " << filePath << "\n";
        return;
    }

    vector<string> lines;
    string line;
    int lineNumber = 0;

    // Read the file line by line and store it in memory
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    // Display the file content to the user
    cout << "File Content:\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        cout << i + 1 << ": " << lines[i] << "\n";
    }

    // Prompt the user to select a line for spell-checking
    cout << "Enter the line number to perform spell check (1 to " << lines.size() << "): ";
    int selectedLine;
    cin >> selectedLine;

    if (selectedLine <= 0 || selectedLine > static_cast<int>(lines.size())) {
        cout << "Invalid line number. Exiting spell check.\n";
        return;
    }

    string& targetLine = lines[selectedLine - 1];
    istringstream iss(targetLine);
    string word;
    string updatedLine;
    bool hasErrors = false;
    int processedWords = 0;

    // Process the selected line word by word
    while (iss >> word) {
        string originalWord = word;
        string punctuation = "";

        // Separate punctuation from the word
        if (ispunct(word.back())) {
            punctuation = word.back();
            word.pop_back();
        }

        // Convert the word to lowercase for dictionary lookup
        string lowerWord = word;
        transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);

        // Check if the word exists in the dictionary
        if (!dictionary.wordExists(lowerWord)) {
            hasErrors = true;
            cout << "Misspelled Word: ***" << originalWord << "***\n";

            // Get suggestions for the misspelled word
            vector<string> suggestions = dictionary.suggestCorrections(lowerWord);

            // Display suggestions to the user
            if (!suggestions.empty()) {
                cout << "Suggestions: ";
                for (size_t i = 0; i < min(suggestions.size(), size_t(5)); ++i) {
                    cout << i + 1 << ". " << suggestions[i] << " ";
                }
                cout << "\nEnter the number of the suggestion to replace, or 0 to ignore: ";
                int choice;
                cin >> choice;

                // Replace the word if the user selects a suggestion
                if (choice > 0 && choice <= static_cast<int>(suggestions.size())) {
                    originalWord = suggestions[choice - 1];
                    cout << "Replaced with: " << originalWord << "\n";
                }
            } else {
                cout << "No suggestions available.\n";
            }
        }

        // Reconstruct the updated line
        updatedLine += originalWord + punctuation + " ";
        processedWords++;

        // Prompt the user to continue after every 5 words
        if (processedWords % 5 == 0) {
            char continueChoice;
            cout << "Do you want to continue spell checking? (y/n): ";
            cin >> continueChoice;
            if (continueChoice == 'n' || continueChoice == 'N') {
                cout << "Spell-check stopped by the user.\n";
                break;
            }
        }
    }

    // Update the selected line in memory
    lines[selectedLine - 1] = updatedLine;

    // Save the updated content back to the file
    ofstream outFile(filePath);
    if (!outFile) {
        cerr << "Error opening file for writing: " << filePath << "\n";
        return;
    }
    for (const auto& updatedLine : lines) {
        outFile << updatedLine << "\n";
    }
    outFile.close();

    if (!hasErrors) {
        cout << "No spelling errors found.\n";
    } else {
        cout << "Spell-check completed. Corrections saved to the file.\n";
    }
}

void TextEditor::replaceWithSynonyms(const string& filePath, Dictionary& dictionary) {
    ifstream file(filePath);
    if (!file) {
        cerr << "Error opening file: " << filePath << "\n";
        return;
    }

    vector<string> lines; // Store the file content in memory
    string line;

    // Load the file into memory
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    // Display the lines to the user
    cout << "File Content:\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        cout << i + 1 << ": " << lines[i] << "\n";
    }

    // Prompt the user to select a line to update
    int lineNumber;
    cout << "Enter the line number to update (or 0 to exit): ";
    cin >> lineNumber;

    if (lineNumber <= 0 || lineNumber > static_cast<int>(lines.size())) {
        cout << "Exiting synonym replacement.\n";
        return;
    }

    string& selectedLine = lines[lineNumber - 1];
    istringstream iss(selectedLine);
    string word;
    string updatedLine;
    int replacementCount = 0;
    int processedWords = 0;

    // Process the selected line
    while (iss >> word) {
        string originalWord = word;
        string punctuation = "";

        // Separate punctuation from the word
        if (ispunct(word.back())) {
            punctuation = word.back();
            word.pop_back();
        }

        // Convert the word to lowercase for dictionary lookup
        string lowerWord = word;
        transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);

        // Get synonyms for the word
        vector<string> synonyms = dictionary.getSynonyms(lowerWord);

        if (!synonyms.empty()) {
            cout << "Word: " << originalWord << "\n";
            cout << "Synonyms: ";
            for (size_t i = 0; i < synonyms.size(); ++i) {
                cout << i + 1 << ". " << synonyms[i] << " ";
            }
            cout << "\nEnter the number of the synonym to replace, or 0 to skip: ";
            int choice;
            cin >> choice;

            // Validate the user's choice
            if (choice > 0 && choice <= static_cast<int>(synonyms.size())) {
                string chosenSynonym = synonyms[choice - 1];

                // Preserve the original case of the word
                if (isupper(originalWord[0])) {
                    chosenSynonym[0] = toupper(chosenSynonym[0]);
                }

                originalWord = chosenSynonym + punctuation;
                cout << "Replaced with: " << originalWord << "\n";

                // Increment the replacement counter
                replacementCount++;
            }
        } else {
            // If no synonyms are found, prompt the user to update the word
            cout << "No synonyms found for the word: " << originalWord << "\n";
            cout << "Do you want to update this word manually? (y/n): ";
            char updateChoice;
            cin >> updateChoice;

            if (updateChoice == 'y' || updateChoice == 'Y') {
                cout << "Enter the replacement for the word \"" << originalWord << "\": ";
                string replacementWord;
                cin >> replacementWord;

                // Preserve the original case of the word
                if (isupper(originalWord[0])) {
                    replacementWord[0] = toupper(replacementWord[0]);
                }

                originalWord = replacementWord + punctuation;
                cout << "Replaced with: " << originalWord << "\n";

                // Increment the replacement counter
                replacementCount++;
            }
        }

        updatedLine += originalWord + " ";
        processedWords++;

        // Prompt the user to continue after every 5 words
        if (processedWords % 5 == 0) {
            char continueChoice;
            cout << "Do you want to continue synonym replacement? (y/n): ";
            cin >> continueChoice;
            if (continueChoice == 'n' || continueChoice == 'N') {
                cout << "Synonym replacement stopped by the user.\n";
                break;
            }
        }
    }

    // Update the selected line in memory
    lines[lineNumber - 1] = updatedLine;

    // Display the updated content in memory
    cout << "\nUpdated Content (In Memory):\n";
    for (const string& updatedLine : lines) {
        cout << updatedLine << "\n";
    }

    // Overwrite the original file with the updated content
    ofstream outFile(filePath);
    if (!outFile) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    for (const string& updatedLine : lines) {
        outFile << updatedLine << "\n";
    }

    outFile.close();

    // Print the save message
    cout << "Updated content saved to the same file: " << filePath << "\n";
}

// Real-time Word Frequency Analysis
void TextEditor::analyzeWordFrequencyRealTime(const string& filePath, Dictionary& dictionary) {
    ifstream file(filePath);
    if (!file) {
        cerr << "Error opening file: " << filePath << "\n";
        return;
    }

    map<string, int> wordFrequency;
    vector<string> lines;
    string line;

    // Read file and calculate word frequencies
    while (getline(file, line)) {
        lines.push_back(line);
        istringstream iss(line);
        string word;
        while (iss >> word) {
            word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            wordFrequency[word]++;
        }
    }
    file.close();

    // Identify commonly used words
    set<string> commonWords;
    for (const auto& pair : wordFrequency) {
        const string& word = pair.first;
        int frequency = pair.second;
        if (frequency > 3) {
            commonWords.insert(word);
        }
    }
    if (commonWords.empty()) {
        cout << "No commonly used words found.\n";
        return;
    }

    int processedWords = 0; // Counter for processed words

    // Suggest replacements for commonly used words
    for (const string& word : commonWords) {
        cout << word << " (used " << wordFrequency[word] << " times)\n";
        vector<string> synonyms = dictionary.getSynonyms(word);

        if (synonyms.empty()) {
            cout << "No synonyms found for \"" << word << "\".\n";
            continue;
        }

        for (string& line : lines) {
            size_t pos = 0;
            while ((pos = line.find(word, pos)) != string::npos) {
                // Display the line containing the word
                cout << "Line: " << line << "\n";
                cout << "Synonyms for \"" << word << "\": ";
                for (size_t i = 0; i < synonyms.size(); ++i) {
                    cout << i + 1 << ". " << synonyms[i] << " ";
                }
                cout << "\nEnter the number of the synonym to replace, or 0 to skip: ";

                int choice;
                cin >> choice;

                // Validate user input
                if (cin.fail() || choice < 0 || choice > static_cast<int>(synonyms.size())) {
                    cin.clear(); // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Invalid choice. Try again.\n";
                    continue;
                }

                if (choice > 0) {
                    string chosenSynonym = synonyms[choice - 1];
                    line.replace(pos, word.length(), chosenSynonym);
                    cout << "Replaced \"" << word << "\" with \"" << chosenSynonym << "\".\n";
                }
                pos += word.length(); // Move past the replaced word

                processedWords++;

                // Prompt the user to continue after every 5 words
                if (processedWords % 5 == 0) {
                    char continueChoice;
                    cout << "Do you want to continue replacing words? (y/n): ";
                    cin >> continueChoice;
                    if (continueChoice == 'n' || continueChoice == 'N') {
                        cout << "Replacement process stopped by the user.\n";
                        goto save_file; // Exit the replacement process
                    }
                }
            }
        }
    }

save_file:
    // Save updated content back to the file
    ofstream outFile(filePath);
    if (!outFile) {
        cerr << "Error opening file for writing: " << filePath << "\n";
        return;
    }

    for (const string& updatedLine : lines) {
        outFile << updatedLine << "\n";
    }
    outFile.close();

    cout << "Word frequency analysis completed. Updates saved to the file.\n";
}