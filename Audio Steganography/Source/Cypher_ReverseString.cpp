#include "Cypher_ReverseString.h"

Cypher_ReverseString::Cypher_ReverseString()
{
}

std::string Cypher_ReverseString::encryptString(std::string inputString)
{
    return Reverse(inputString);
}

std::string Cypher_ReverseString::decryptString(std::string inputString)
{
    return Reverse(inputString);
}

std::string Cypher_ReverseString::Reverse(std::string inputString)
{
    std::string outputString;

    for (size_t i = 1; i <= inputString.length(); i++)
    {
        size_t readPosition = inputString.length() - i;
        outputString.operator+=(inputString[readPosition]);
    }

    return outputString;
}
