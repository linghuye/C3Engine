 /*
 * 	string.h
 *
 *  Created on: Sep 5, 2009
 *      Author: Cod3 Monk3y, Inc.
 */

#ifndef _STRING_FIX_H_
#define _STRING_FIX_H_

namespace c3fix 
{
	/*
	 * Replacement for std::string
	 */
	class string 
	{
		char* buffer;

	public:
		string(const char* s);
		string();
		string(const string& rhs);
		~string();
		int size() const;
		int length() const;
		const char* c_str() const { return buffer; }
		const char* data() const { return buffer; }
		string& operator=(const char* cstr);
		string& operator=(const string& rhs);
		char operator[](int i) const;
		bool operator<(const string& rhs) const;
		bool operator!=(const string& rhs) const { return (strcmp(buffer, rhs.data()) != 0); }
		string& assign(const char* cstr);
		string& assign(const string& rhs);
		string& operator+=(const char* cstr);
		string operator+(const char* cstr) const;
		bool empty() const { return (buffer==NULL || buffer[0]==0); }
		int compare(const string& rhs) const { return strcmp(buffer, rhs.data()); }
	};
}

#endif /* STRING_H_ */
