#include <string.hpp>

string::string() {
    m_storage.pushBack('\0');
}

string::string(const wchar_t* str) {
    for (size_t i = 0; i < wcslen(str); ++i) {
        m_storage.pushBack(str[i]);
    }

    m_storage.pushBack(L'\0');
}

string::string(int64_t number) {
    if (number == 0) {
        m_storage.pushBack(L'0');
        m_storage.pushBack(L'\0');
        return;
    }

    bool isNegative = false;
    if (number < 0) {
        isNegative = true;
        number = -number;
    }

    while (number != 0) {
        wchar_t digit = (number % 10) + L'0';
        m_storage.pushBack(digit);
        number /= 10;
    }

    if (isNegative) {
        m_storage.pushBack(L'-');
    }

    size_t start = 0;
    size_t end = m_storage.getSize() - 1;
    while (start < end) {
        std::swap(m_storage[start], m_storage[end]);
        start++;
        end--;
    }

    m_storage.pushBack(L'\0');
}

string::~string() {}

size_t string::getSize() const { return m_storage.getSize() - 1; }

const wchar_t* string::getData() const { return m_storage.getStorage(); }

bool string::isEmpty() const { return getSize() == 0; }

void string::Clear() {
    m_storage.Clear();
    m_storage.pushBack(L'\0');
}

void string::Append(const string& str) {
    m_storage.Pop(); // remove null terminator from the current string
    for (size_t i = 0; i < str.getSize(); ++i) {
        m_storage.pushBack(str[i]);
    }
    m_storage.pushBack(L'\0'); // re-add the null terminator
}

string& string::operator=(const string& str) {
    if (this != &str) {
        m_storage = str.m_storage;
    }
    return *this;
}

string string::operator+(const string& str) const {
    string result = *this;
    result.Append(str);
    return result;
}

bool string::operator==(const string& str) const {
    if (getSize() != str.getSize()) return false;

    for (size_t i = 0; i < getSize(); ++i) {
        if ((*this)[i] != str[i]) {
            return false;
        }
    }
    return true;
}

wchar_t string::operator[](size_t index) const {
    if (index < getSize()) {
        return m_storage[index];
    }
}

string string::toString(int64_t number) {
    return string(number);
}

string& string::operator+=(const string& str) {
    Append(str);
    return *this;
}