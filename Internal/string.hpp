#pragma once

#include <cstdint>
#include <vector.hpp>

class string {
private:
	vector<wchar_t> m_storage;

public:
	string();
	string(const wchar_t* str);
	string(int64_t num);
	~string();

	size_t getSize() const;
	const wchar_t* getData() const;
	bool isEmpty() const;

	void Clear();
	void Append(const string& str);

	static string toString(int64_t number);

	string& operator=(const string& str);
	string operator+(const string& str) const;
	bool operator==(const string& str) const;
	wchar_t operator[](size_t index) const;
	string& operator+=(const string& str);
};
