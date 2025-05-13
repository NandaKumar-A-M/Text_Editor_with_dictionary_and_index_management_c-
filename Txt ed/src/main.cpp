#include "Dictionary.h"
#include "DictionaryUI.h"
#include "TextEditor.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits> // For std::numeric_limits

using namespace std;

string currentFile = ""; // To store the name of the currently opened file

int main() {
    Dictionary dictionary("data/dictionary.txt", "data/index.txt");

    int choice;
    do {
        // Clear the screen before displaying the menu
        system("cls");

        // Display the main menu
        cout << "Text Editor Menu:\n";
        cout << "1. Open Existing Text File\n";
        cout << "2. Manage Dictionary\n";
        cout << "3. Search in Text File\n";
        cout << "4. Create and Save File\n";
        cout << "5. Edit File\n";
        cout << "6. Spell Check\n";
        cout << "7. Replace Words with Synonyms\n";
        cout << "8. Analyze Word Frequency in Real Time\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                system("cls"); // Clear the screen before opening a file
                cout << "Enter file name to open: ";
                cin >> currentFile;
                TextEditor::openFile(currentFile);
                break;
            }
            case 2:
                system("cls"); // Clear the screen before managing the dictionary
                manageDictionary(dictionary);
                break;
            case 3:
                system("cls"); // Clear the screen before searching
                if (currentFile.empty()) {
                    cout << "No file is currently opened. Please open a file first.\n";
                } else {
                    dictionary.checkAndRebuildIndex();
                    string word;
                    cout << "Enter word to search in the text file: ";
                    cin >> word;
                    TextEditor::searchInFile(currentFile, word);
                }
                break;
            case 4: {
                system("cls"); // Clear the screen before creating a file
                cout << "Enter file name to save: ";
                cin >> currentFile;
                if (currentFile.empty()) {
                    cout << "No file name provided. Please specify a file name.\n";
                    break;
                }
                cout << "Enter content for the file (end with an empty line):\n";
                ostringstream content; //to store stribg buffer content.
                string line;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                while (true) {
                    getline(cin, line);
                    if (line.empty()) break;
                    content << line << "\n";
                }
                TextEditor::saveFile(currentFile, content.str());
                break;
            }
            case 5:
                system("cls"); // Clear the screen before editing a file
                if (currentFile.empty()) {
                    cout << "No file is currently opened. Please open a file first.\n";
                } else {
                    TextEditor::editFile(currentFile);
                }
                break;
            case 6: {
                system("cls"); // Clear the screen before spell checking
                if (currentFile.empty()) {
                    cout << "No file is currently opened. Please open a file first.\n";
                } else {
                    dictionary.checkAndRebuildIndex();
                    TextEditor editor;
                    editor.spellCheck(currentFile, dictionary);
                }
                break;
            }
            case 7: {
                system("cls"); // Clear the screen before replacing synonyms
                if (currentFile.empty()) {
                    cout << "No file is currently opened. Please open a file first.\n";
                } else {
                    TextEditor editor;
                    editor.replaceWithSynonyms(currentFile, dictionary);
                }
                break;
            }
            case 8: {
                system("cls");
                if (currentFile.empty()) {
                    cout << "No file is currently opened. Please open a file first.\n";
                } else {
                    TextEditor editor;
                    editor.analyzeWordFrequencyRealTime(currentFile, dictionary);
                }
                break;
            }
            case 9:
                system("cls"); // Clear the screen before exiting
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
                 // Clear the error state and discard invalid input
                cin.clear(); // Clear the error flag on cin
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                break;
        }

        // Pause before returning to the menu, except when exiting
        if (choice != 9) {
            cout << "\nPress Enter to return to the menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

    } while (choice != 9);

    return 0;
}