#ifndef ERRORNUMBER_H
#define ERRORNUMBER_H

class ErrorNumber{
public:
	ErrorNumber();
	static const char* getErrorString(int errorNumber);
};

#endif