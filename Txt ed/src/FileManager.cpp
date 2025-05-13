#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std; // Simplifies code by removing the need for std::

/*
 * Reads all lines from a file and returns them as a vector of strings.
 */
vector<string> FileManager::readLines(const string& filePath) {
    ifstream file(filePath); // Open the file for reading
    vector<string> lines;    // Vector to store the lines

    if (!file) { // Check if the file was opened successfully
        cerr << "Error opening file: " << filePath << "\n";
        return lines; // Return an empty vector if the file couldn't be opened
    }

    string line;
    while (getline(file, line)) { // Read each line from the file
        lines.push_back(line);    // Add the line to the vector
    }

    file.close(); // Close the file
    return lines; // Return the vector of lines
}

/*
 * Writes a vector of strings to a file, each string on a new line.
 */
void FileManager::writeLines(const string& filePath, const vector<string>& lines) {
    ofstream file(filePath); // Open the file for writing

    if (!file) { // Check if the file was opened successfully
        cerr << "Error opening file: " << filePath << "\n";
        return;
    }

    for (const auto& line : lines) { // Write each line to the file
        file << line << "\n";
    }

    file.close(); // Close the file
}

/*
 * Reads the entire content of a file and returns it as a single string.
 */
string FileManager::readFile(const string& filePath) {
    ifstream file(filePath); // Open the file for reading

    if (!file) { // Check if the file was opened successfully
        cerr << "Error opening file: " << filePath << "\n";
        return ""; // Return an empty string if the file couldn't be opened
    }

    ostringstream content;
    content << file.rdbuf(); // Read the entire file content into a string stream

    file.close(); // Close the file
    return content.str(); // Return the file content as a string
}

/*
 * Writes a string to a file.
 */
void FileManager::writeFile(const string& filePath, const string& content) {
    ofstream file(filePath); // Open the file for writing

    if (!file) { // Check if the file was opened successfully
        cerr << "Error opening file: " << filePath << "\n";
        return;
    }

    file << content; // Write the content to the file
    file.close();    // Close the file
}