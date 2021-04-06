#pragma once
#include <JuceHeader.h>
/**Cypher - Base Class For Encrypting and Decrypting Strings*/
class Cypher 
{
public:
	/** encryptString - abstract Fuction takes in Juce::String encrypts it then returns string.
	@param inputString - std::string to be encrypted 
	@return std::string - encrypted string*/
	virtual std::string encryptString(std::string inputString) =0;

	/**decryptString - abstract Fuction takes in previously encrypted std::string then decrypts it and returns.
	@param inputString - std::string to be decrypted
	@return std::string - decrypted string*/
	virtual std::string decryptString(std::string inputString) =0;

	/**Destructor*/
	virtual ~Cypher() {};
private:

};







//array of all ascii values { " ", "!", "'", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "/", "]", "^", "_", "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l","m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x" "y", "z", "{", "|", "}", "~"};