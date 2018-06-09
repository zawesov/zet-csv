/*
Copyright (C) Alexander Zavesov
Copyright (C) ZET-CSV
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __zCSV_h
#define __zCSV_h 1

#include <iostream>
#include <string>
#include <vector>

class zCSV
{
 public:

 zCSV(): m_value() {};
 zCSV(const zCSV& src);
 zCSV& operator=(const zCSV& src);

virtual ~zCSV() { clear(); };

 bool parse(const char* p, size_t len, char delim=',', char quote='\"');
 bool parse(const std::string &src, char delim=',', char quote='\"') { return parse(src.c_str(), src.size(), delim, quote); };
 void write(std::string &ret, char delim=',', char quote='\"') const;

 void normalize();

 bool empty() const { return m_value.empty(); };
 size_t size() const { return m_value.size(); };

 const std::vector<std::string>& operator[](size_t pos) const { return *(m_value[pos]); };
 std::vector<std::string>& operator[](size_t pos) { return *(m_value[pos]); };
 const std::vector<std::string>& at(size_t pos) const { return *(m_value.at(pos)); };
 std::vector<std::string>& at(size_t pos) { return *(m_value.at(pos)); };
 const std::vector<std::string>& front() const { return *(m_value.front()); };
 std::vector<std::string>& front() { return *(m_value.front()); };
 const std::vector<std::string>& back() const { return *(m_value.back()); };
 std::vector<std::string>& back() { return *(m_value.back()); };

 void clear();
 size_t insert(size_t pos, const std::vector<std::string>& row);
 size_t insert(size_t pos, std::vector<std::string>* row);
 size_t push_back(const std::vector<std::string>& row);
 size_t push_back(std::vector<std::string>* row);
 bool erase(size_t pos);
 bool pop_back();

 protected:

 std::vector<std::vector<std::string>* > m_value;

};

#endif




