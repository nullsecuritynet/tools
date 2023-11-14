#include <iostream>
#include "HashMaker.h"
#include "TestData.h"
#include "Evaluator.h"

#include "HashModelFNV1a.h"
#include "HashModelCRC32.h"
#include "HashModelMurmur32.h"

void reportResults(Evaluator::Result_t& res)
{
    const TestData& testData = TestData::get();

    double collisionRate = (res.totalCollisions / res.totalTests);
    double fitnessCollisions = 1.0 - collisionRate;

    double fitnessStateWrite = res.bitsWrite / res.bitsTotal;
    double fitnessStateRead = res.bitsRead / res.bitsTotal;
    double fitnessState = (fitnessStateWrite + fitnessStateRead) / 2.0;

    double totalFitness = (fitnessCollisions + fitnessState);
    double fitness = std::pow(totalFitness, 4.0);

    printf("-> Test Entries: %zu; Fitness: %.08f ; Collisions: %.08f (%zu) ; State: %.08f\n",
        testData.size(),
        fitness,
        collisionRate,
        (size_t)res.totalCollisions,
        fitnessState);
}

void testHashModels()
{
    Evaluator evaluator;

    // FNV1a
    {
        printf("Running FNV-1a...\n");
        auto mdl = CreateHashModelFNV1a();
        auto res = evaluator.evaluate(mdl);
        reportResults(res);
    }

    // CRC32
    {
        printf("Running CRC32...\n");
        auto mdl = CreateHashModelCRC32();
        auto res = evaluator.evaluate(mdl);
        reportResults(res);
    }

    // Murmur32
    {
        printf("Running Murmur32...\n");
        auto mdl = CreateHashModelMurmur32();
        auto res = evaluator.evaluate(mdl);
        reportResults(res);
    }
}

void testHashMaker()
{
    HashMakerParams params;
    params.hashSize = 4;

    HashMaker maker;
    maker.reset(params);

    printf("Running HashMaker<%zu>...\n", params.hashSize * 8);

    while (true)
    {
        maker.evaluate();
        maker.epoch();
        maker.printStats();
    }
}

int main(int argc, const char *argv[])
{
    const char *testDataTextFile = nullptr;
    for (int i = 1; i < argc; i++)
    {
        if (_stricmp(argv[i], "-t") == 0)
        {
            if (i + 1 >= argc)
            {
                printf("Missing parameter -t\n");
                return -1;
            }
            testDataTextFile = argv[++i];
        }
    }

    TestData& testData = TestData::get();

    if (testDataTextFile != nullptr)
    {
        if (testData.importTextFile(testDataTextFile))
        {
            printf("Loaded test data: %s\n", testDataTextFile);
        }
    }

    testHashModels();
    testHashMaker();

    return 0;
}
