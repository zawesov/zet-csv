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

#include "zCSV.h"

static std::string replace(const std::string &src, const std::string &str, const std::string &rep)
{
 size_t n=str.size();
 if(src.size() == 0 || n == 0) return src;
 std::string ret; ret.reserve(src.size());
 size_t k=0;
 size_t s;
 for(;;)
 {
  s=src.find(str,k);
  if(s == std::string::npos) { ret.append(src, k, s); return ret; }
  ret.append(src, k, s-k);
  ret+=rep;
  k=(s+n);
 }
};

static std::string replace(const std::string &src, char str, const std::string &rep)
{
 if(src.size() == 0) return src;
 std::string ret; ret.reserve(src.size());
 size_t k=0;
 size_t s;
 for(;;)
 {
  s=src.find(str,k);
  if(s == std::string::npos) { ret.append(src, k, s); return ret; }
  ret.append(src, k, s-k);
  ret+=rep;
  k=(s+1);
 }
};

static size_t parse_cell(std::string &ret, const char* p, size_t len, size_t start_pos, char delim, char quote)
{
 ret.clear();
 if(start_pos >= len) return std::string::npos;
 if(p[start_pos] == quote)
 {
  size_t n= len-start_pos-1;
  p+=(start_pos+1);
  for(size_t i=0; i < n; ++i, ++p)
  {
   if(*p == quote)
   {
    if(p[1] != quote) { return (start_pos+2+i); }
    ret+=*p; ++i; ++p;
   }
   else ret+=*p;
  }
  return std::string::npos;
 }
 else
 {
  size_t n= len-start_pos;
  p+=start_pos;
  for(size_t i=0; i < n; ++i, ++p)
  {
   if(*p == quote) return std::string::npos;
   if(*p == delim || *p == '\r' || *p == '\n') { return (start_pos+i); }
   ret+=*p;
  }
 }
 return len;
};


zCSV::zCSV(const zCSV& src):
 m_value()
{
 for(size_t i=0; i < src.m_value.size(); ++i)
 { m_value.push_back(new std::vector<std::string>(*src.m_value[i])); }
};

zCSV& zCSV::operator=(const zCSV& src)
{
 if(&src == this) return *this;
 clear();
 for(size_t i=0; i < src.m_value.size(); ++i)
 { m_value.push_back(new std::vector<std::string>(*src.m_value[i])); }
 return *this;
};

void zCSV::clear()
{
 for(size_t i=0; i < m_value.size(); ++i) { delete m_value[i]; }
 m_value.clear();
};

size_t zCSV::insert(size_t pos, const std::vector<std::string>& row)
{
 if(pos >= m_value.size()) return push_back(row);
 m_value.insert(m_value.begin()+pos, new std::vector<std::string>(row));
 return pos;
};

size_t zCSV::insert(size_t pos, std::vector<std::string>* row)
{
 if(row == NULL) return std::string::npos;
 if(pos >= m_value.size()) return push_back(row);
 m_value.insert(m_value.begin()+pos, row);
 return pos;
};

size_t zCSV::push_back(const std::vector<std::string>& row)
{
 m_value.push_back(new std::vector<std::string>(row));
 return (m_value.size()-1);
};

size_t zCSV::push_back(std::vector<std::string>* row)
{
 if(row == NULL) return std::string::npos;
 m_value.push_back(row);
 return (m_value.size()-1);
};

bool zCSV::erase(size_t pos)
{
 if(pos >= m_value.size()) return false;
 delete m_value[pos];
 m_value.erase(m_value.begin()+pos);
 return true;
};

bool zCSV::pop_back()
{
 size_t pos=m_value.size();
 if(pos == 0) return false;
 --pos;
 delete m_value[pos];
 m_value.erase(m_value.begin()+pos);
 return true;
};

void zCSV::normalize()
{
 size_t n=0;
 for(size_t i=0; i < m_value.size(); ++i) { if(m_value[i]->size() > n) n=m_value[i]->size(); }
 for(size_t i=0; i < m_value.size(); ++i) { m_value[i]->resize(n); }
};

void zCSV::write(std::string &ret, char delim, char quote) const
{
 std::string dq; dq+=delim; dq+=quote;
 std::string qq(2, quote);
 std::string smb("\r\n"); smb+=delim; smb+=quote;
 std::vector<std::string>* p;
 size_t n;
 for(size_t i=0; i < m_value.size(); ++i)
 {
  p= m_value[i];
  if(p->empty()) { continue; }
  n=p->at(0).find_first_of(smb);
  if(n == std::string::npos) ret+=p->at(0);
  else ret+=quote+replace(p->at(0), quote, qq)+quote;
  for(size_t j=1; j < p->size(); ++j)
  {
   n=p->at(j).find_first_of(smb);
   if(n == std::string::npos) ret+=delim+p->at(j);
   else ret+=dq+replace(p->at(j), quote, qq)+quote;
  }
  ret+="\r\n";
 }
};

bool zCSV::parse(const char* p, size_t len, char delim, char quote)
{
 size_t pos=0;
 std::string s;
 std::vector<std::string>* v= new std::vector<std::string>();
 for(size_t n;;)
 {
  if(pos >= len)
  {
   if(v->size()) m_value.push_back(v);
   else delete v;
   v=NULL;
   break;
  }
  if(p[pos] == delim)
  {
   v->push_back(""); ++pos;
   if(p[pos] == '\n' || p[pos] == '\r') v->push_back("");
   continue;
  }
  if(p[pos] == '\r' && p[pos+1] == '\n')
  {
   pos+=2;
   if(v->size())
   {
    m_value.push_back(v);
    v= new std::vector<std::string>();
   }
   continue;
  }
  if(p[pos] == '\n' || p[pos] == '\r')
  {
   ++pos;
   if(v->size())
   {
    m_value.push_back(v);
    v= new std::vector<std::string>();
   }
   continue;
  }
  n=parse_cell(s, p, len, pos, delim, quote);
  if(n == std::string::npos) { delete v; return false; }
  v->push_back(s);
  pos=n;
  if(pos >= len) continue;
  if(p[pos] == delim) { ++pos; continue; }
  if(p[pos] == '\r' || p[pos] == '\n') continue;
  delete v; return false;
 }
 return true;
};










