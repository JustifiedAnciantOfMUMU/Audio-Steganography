#pragma once
#include "Cypher.h"

/**Cypher_None - inherits from Cypher class, passes string through without any encryption.
@see Cypher*/
class Cypher_None: public Cypher
{
public:
	/**encryptString - inherited method from Cypher class takes in string encrypts it then returns it.
	@param inputSting - std::string to encrypt
	@return std::string - encrypted std::string
	@see Cypher*/
	virtual std::string encryptString(std::string inputString) override;

	/**decryptString - inherited method from Cypher class takes in  encrypted string it then returns decrypted string.
	@param inputString - std::string to decrypt
	@return std::string - decrypted std::string
	@see Cypher*/
	virtual std::string decryptString(std::string inputString) override;
private:

};