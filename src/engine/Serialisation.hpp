#pragma once

#include <fstream>
#include <filesystem>
#include <stack>

class Serialisation
{
public:
    bool begin(const std::filesystem::path& path, std::ios::openmode mode);
    
    void beginSection(std::string_view name);
    void indent();
    // write to the line buffer. Not written to disk until endLine()
    void writeLineBuffer(std::string_view data, std::string_view sep = " ");
    void writeLine(std::string_view data);
    void endLine();
    void endSection();
    
    void readSection(std::string_view name, std::string_view nameEnd, std::string& buf);
    
    bool alive() { return m_file.good(); }
    bool end();
    
    ~Serialisation()
    {
        end();
    }
private:
    std::fstream m_file;
    std::string m_buffer;
    std::stack<std::string> m_currentSections;
    int m_depth {};
};