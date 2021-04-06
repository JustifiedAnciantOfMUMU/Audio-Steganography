#pragma once
#include "Cypher.h"

/**Cypher_ReverseString - inherits from Cypher class, encrypts string by reversing the order of chars in string.
@see Cypher*/
class Cypher_ReverseString : public Cypher
{
public:
	/**Constructor*/
	Cypher_ReverseString();

	/**encryptString - inherited method from Cypher class takes in string encrypts it then returns it.
	@param inputString - std::string to encrypt
	@return std::string - encrypted std::string
	@see Cypher*/
	virtual std::string encryptString(std::string inputString) override;

	/**decryptString - inherited method from Cypher class takes in  encrypted string it then returns decrypted string.
	@param inputString - std::string to decrypt
	@return std::string - decrypted std::string
	@see Cypher*/
	virtual std::string decryptString(std::string inputString) override;
private:

	/**Reverse - reverses string order.
	@param inputString - std::string to encrypt
	@return std::string - encrypted std::string*/
	std::string Reverse(std::string inputString);
};