#pragma once
#include <string>
class MyException/* : public std::exception ... ni*/
{
private:
	std::string errorMessage{};
	int lineNumber{};
	const char* filename{};
public:
	MyException(const std::string& errorMessage, const int lineNumber, const char* filename);

	std::string whatWentWrong() const;
};