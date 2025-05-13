// Handles user interaction for dictionary management.
#include "DictionaryUI.h"
#include <iostream>
#include <vector>
#include <sstream>

using namespace std; // Simplifies code by removing the need for std::

/*
 * Manages dictionary operations based on user input.
 */
void manageDictionary(Dictionary& dictionary) {
    int choice;
    cout << "Dictionary Management:\n";
    cout << "1. Add Word\n";
    cout << "2. Search Word\n";
    cout << "3. Update Word\n";
    cout << "4. Delete Word\n";
    cout << "5. Import Dictionary from CSV\n";
    cout << "6. Export Dictionary to CSV\n";
    cout << "Enter your choice: ";
    cin >> choice;
    string filePath;
    string word;
    vector<string> synonyms; //for storing synonyms and adding it to dict.

    switch (choice) {
        case 1: // Add a new word
            cout << "Enter word: ";
            cin >> word;
            cout << "Enter synonyms (comma-separated): ";
            cin.ignore(); // Clear the input buffer
            {
                string synonymsInput;
                getline(cin, synonymsInput); // Read the entire line of synonyms
                stringstream ss(synonymsInput);
                string synonym;
                while (getline(ss, synonym, ',')) {
                    synonyms.push_back(synonym); // Add each synonym to the vector
                }
            }
            dictionary.addWord(word, synonyms); // Add the word to the dictionary
            break;

        case 2: // Search for a word
            cout << "Enter word to search: ";
            cin >> word;
            dictionary.searchWord(word); // Search for the word in the dictionary
            break;

        case 3: // Update an existing word
            cout << "Enter word to update: ";
            cin >> word;
            cout << "Enter new synonyms (comma-separated): ";
            cin.ignore(); // Clear the input buffer
            {
                string synonymsInput;
                getline(cin, synonymsInput); // Read the entire line of new synonyms
                stringstream ss(synonymsInput);//parsing
                string synonym;
                while (getline(ss, synonym, ',')) {
                    synonyms.push_back(synonym); // Add each synonym to the vector
                }
            }
            dictionary.updateWord(word, synonyms); // Update the word in the dictionary
            break;

        case 4: // Delete a word
            cout << "Enter word to delete: ";
            cin >> word;
            dictionary.deleteWord(word); // Delete the word from the dictionary
            break;

            case 5: // Import dictionary
            cout << "Enter the path to the CSV file: ";
            cin >> filePath;
            dictionary.importFromCSV(filePath);
            break;

        case 6: // Export dictionary
            cout << "Enter the destination path or name to save the CSV file: ";
            cin >> filePath;
            dictionary.exportToCSV(filePath);
            break;

        default: // Invalid choice
            cout << "Invalid choice.\n";
    }
}