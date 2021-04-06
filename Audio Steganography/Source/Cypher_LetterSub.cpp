#include "Cypher_LetterSub.h"

std::string Cypher_LetterSub::encryptString(std::string inputString)
{
    std::string subbedString;

    for (int i = 0; i < inputString.length(); i++) //cycles through string values
    {
        int8_t asciiValue = inputString[i]; //gets substituted value
        subbedString.operator+=(subbedValues[asciiValue]); //add subbed value to string
    }
    DBG(subbedString);

    return subbedString;
}

std::string Cypher_LetterSub::decryptString(std::string inputString)
{
    std::string outputString;

    for (int i = 0; i < inputString.length(); i++) //cycles through sting 
    {
        int index = 0;
        while (inputString[i] != subbedValues[index]) //cycles through array of subbed value
        {
            index++;
        }
        outputString.operator+=(subbedValues[index]);
    }
    return outputString;
}
