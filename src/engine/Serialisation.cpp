#include "engine/Serialisation.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>

bool Serialisation::begin(const std::filesystem::path& path,
                          std::ios::openmode mode)
{
    m_file.open(path, mode);

    return alive();
}

void Serialisation::beginSection(std::string_view name)
{
    writeLine(name);

    m_currentSections.push(name.data());
    m_depth++;
}

void Serialisation::indent()
{
    std::string output;
    uint32_t indentSpaces{4};

    for(uint32_t i{}; i < m_depth * indentSpaces; i++)
    {
        output.append(" ");
    }
    m_file << output;
}

void Serialisation::writeLineBuffer(std::string_view data,
                                    std::string_view sep)
{
    m_buffer.append(data);
    m_buffer.append(sep);
}

void Serialisation::writeLine(std::string_view data)
{
    m_buffer.append(data);
    endLine();
}

void Serialisation::endLine()
{
    indent();
    m_file << m_buffer << std::endl;
    m_buffer.clear();
}

void Serialisation::endSection()
{
    m_depth--;

    indent();
    m_file << "End" << m_currentSections.top() << std::endl;

    m_currentSections.pop();
    assert(m_depth >= 0 && "Depth is negative!\n");
}

bool Serialisation::end()
{
    m_file.close();
    return !m_file.is_open();
}