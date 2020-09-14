#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>

#include <queue>
#include <unordered_map>
#include <algorithm>

#include <Utility/File.hpp>
#include <Utility/VigenereCommon.hpp>

#define ERROR_INCORRECT_ARGUMENTS -1
#define ERROR_NO_INPUT -2
#define ERROR_NO_OUTPUT -3
#define UNABLE_TO_PREDICT_KEY_LENGTH -4

template <typename Function> void ForEachDivisor (std::size_t number, Function &callback)
{
    for (std::size_t divisor = 2; divisor <= sqrt (number); ++divisor)
    {
        if (number % divisor == 0)
        {
            std::size_t result = number / divisor;
            callback (divisor);

            if (number != result)
            {
                callback (result);
            }
        }
    }
}

std::unordered_map <size_t, size_t> FindDistances (IterableInputFile &inputFile, size_t subsequenceLength)
{
    std::unordered_map <std::string, std::size_t> occurrences {};
    std::unordered_map <std::size_t, std::size_t> distances {};
    std::string currentString {};
    std::size_t encodedPosition = 0;

    for (char symbol : inputFile)
    {
        int offset;
        if (IsVigenereSupportedSymbol (symbol, offset))
        {
            currentString.push_back (symbol - offset);
            ++encodedPosition;

            if (currentString.size () == subsequenceLength)
            {
                if (occurrences.count (currentString) > 0)
                {
                    distances[encodedPosition - occurrences[currentString]]++;
                }

                occurrences[currentString] = encodedPosition;
                currentString.erase (currentString.begin ());
            }
        }
        else
        {
            currentString.clear ();
        }
    }

    return distances;
}

std::unordered_map <std::size_t, std::size_t> CalculateDivisors (
    const std::unordered_map <std::size_t, std::size_t> &distances, std::size_t &total)
{
    struct
    {
        std::unordered_map <std::size_t, std::size_t> counts {};

        void operator() (std::size_t divisor)
        {
            counts[divisor]++;
        }
    } divisorCounter;

    total = 0;
    for (auto valueCountPair : distances)
    {
        ForEachDivisor (valueCountPair.first, divisorCounter);
        total += valueCountPair.second;
    }

    return divisorCounter.counts;
}

std::size_t PredictKeyLength (IterableInputFile &inputFile, std::size_t subsequenceLength)
{
    std::size_t total = 0;
    std::unordered_map <std::size_t, std::size_t> counts = CalculateDivisors (
        FindDistances (inputFile, subsequenceLength), total);

#define PURE_MAXIMIZATION_BARRIER 0.4f
    struct
    {
        std::size_t divisor;
        std::size_t count;
    } bestDivisor = { 0, 0 };

    for (auto divisor : counts)
    {
        if (divisor.first > bestDivisor.divisor && (float) divisor.second / total >= PURE_MAXIMIZATION_BARRIER)
        {
            bestDivisor = { divisor.first, divisor.second };
        }
    }

    return bestDivisor.divisor;
}

int main (int argCount, char **argValue)
{
    if (argCount != 3)
    {
        printf ("Expected arguments:\n"
                "    - text file to crack.\n"
                "    - key output file name.\n");
        return ERROR_INCORRECT_ARGUMENTS;
    }

    IterableInputFile inputFile (argValue[1], false);
    if (inputFile.begin () == inputFile.end ())
    {
        printf ("Input file is empty or does not exists!");
        return ERROR_NO_INPUT;
    }

    FILE *outputFile = fopen (argValue[2], "w");
    if (outputFile == nullptr)
    {
        printf ("Unable to create output file!");
        return ERROR_NO_OUTPUT;
    }

    std::size_t keyLength = PredictKeyLength (inputFile, 10);
    if (keyLength == 0)
    {
        printf ("Unable to predict key length for given input file!");
        return UNABLE_TO_PREDICT_KEY_LENGTH;
    }

    printf ("Predicted key length: %d.\n", keyLength);
    std::vector <std::vector <std::size_t>> counters (
        keyLength, std::vector <std::size_t> (ALPHABET_SIZE, 0));

    std::size_t partIndex = 0;
    for (char input : inputFile)
    {
        int offset = -1;
        if (IsVigenereSupportedSymbol (input, offset))
        {
            counters[partIndex][input - offset]++;
            partIndex = (partIndex + 1) % keyLength;
        }
    }

    for (partIndex = 0; partIndex < keyLength; ++partIndex)
    {
#define MOST_FREQUENT_SYMBOL_CODE ('e' - 'a')
        int maxElementCode = std::max_element (
            counters[partIndex].begin (), counters[partIndex].end ()) - counters[partIndex].begin ();

        int offset = maxElementCode - MOST_FREQUENT_SYMBOL_CODE;
        if (offset < 0)
        {
            offset += ALPHABET_SIZE;
        }

        fputc ('a' + offset, outputFile);
    }

    fputc ('\n', outputFile);
    fclose (outputFile);
    return 0;
}
