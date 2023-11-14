#include "TestData.h"

#include <fstream>
#include <iostream>

static TestData _testData;

TestData& TestData::get()
{
    return _testData;
}

bool TestData::importTextFile(const std::string& filePath)
{
    std::ifstream ifs(filePath);
    if (ifs.is_open() == false)
    {
        return false;
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        // skip empty lines:
        if (line.empty())
            continue;

        Data data;
        data.bytes.resize(line.size());
        for (size_t i = 0; i < line.size(); i++)
        {
            data.bytes[i] = (uint8_t)line[i];
        }

        _db.emplace_back(std::move(data));
    }

    return true;
}

size_t TestData::size() const
{
    return _db.size();
}

const TestData::Data& TestData::operator[](size_t idx) const
{
    return _db[idx];
}

