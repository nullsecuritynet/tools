#pragma once

#include <string>
#include <vector>

class TestData
{
public:
    struct Data
    {
        std::vector<uint8_t> bytes;
    };

private:
    std::vector<Data> _db;

public:
    static TestData& get();

    bool importTextFile(const std::string& filePath);

    size_t size() const;

    const Data& operator[](size_t idx) const;
};
