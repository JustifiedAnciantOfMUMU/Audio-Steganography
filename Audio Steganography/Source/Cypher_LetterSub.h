#pragma once
#include "Cypher.h"

/**Cypher_LetterSub - takes in string substitudes chars within. inherits from Cypher class
@see Cypher*/
class Cypher_LetterSub : public Cypher
{
public:
	/**encryptString - encrypts string by substituting chars with other ascii values. inherited class from Cypher
	@param inputString - std::string to be encrypted
	@returnb std::string - encrypted string
	@see Cypher*/
	virtual std::string encryptString(std::string inputString) override;

	/**decryptString - encrypts string by substituting chars with origina; ascii values. inherited class from Cypher
	@param inputstring - std::string to be decrypted
	@returnb std::string - decrypted string
	@see Cypher*/
	virtual std::string decryptString(std::string inputString) override;
private:

	char subbedValues[95] = { 'M', '#', '5', 'A', '$', '%', '&', '"', '(', 'P', 'D', '8', ',', '-', 'O', '/', '0', 'K', '2', ':', '4', '\'', 'H', '7', '+', 'J', '3', ';', '<', '=', '>', 'F', '@', '!', 'B', 'G', '*', 'E', '?', 'C', '6', 'I', '9', '1', 'Y', 'T', 'N', '.', ')', 'Q', 'R', 'S', ' ', 'U', 'L', 'W', 'X', 'V', 'd', '[', '/', ']', 'n', 'b', '`', 'c', 'j', 'a', 'Z', '_', 'g', 'h', 'f', 'm', 'e', 'k', 'l','i', '^', 'o', 'p', 'q', 'r', 's', '{', '|', '}', '~', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
};



//array of ascii values { " ", "!", "'", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "/", "]", "^", "_", "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l","m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x" "y", "z", "{", "|", "}", "~"};