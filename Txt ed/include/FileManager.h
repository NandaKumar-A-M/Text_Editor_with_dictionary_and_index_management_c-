// Handles file I/O operations
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

using namespace std; // Simplifies code by removing the need for std::

class FileManager {
public:
    // Reads all lines from a file and returns them as a vector of strings
    static vector<string> readLines(const string& filePath);

    // Writes a vector of strings to a file, each string on a new line
    static void writeLines(const string& filePath, const vector<string>& lines);

    // Reads the entire content of a file and returns it as a single string
    static string readFile(const string& filePath);

    // Writes a string to a file
    static void writeFile(const string& filePath, const string& content);
};

#endif // FILEMANAGER_H