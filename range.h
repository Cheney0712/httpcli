// range.h: interface for the range class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANGE_H__4736E282_CE0C_4091_BE84_2C4CB914BD63__INCLUDED_)
#define AFX_RANGE_H__4736E282_CE0C_4091_BE84_2C4CB914BD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using std::ostringstream;
using std::left;

class range  
{
public:
	static uint64_t nlength;
	static range full_range()
	{
		return range( 0, nlength );
	}
	static range zero_range()
	{
		return range( 0, 0 );
	}
	range( const range& r )
		: _pos(r.pos()), _length(r.length())
	{
	}
	explicit range( uint64_t pos = 0, uint64_t length = range::nlength)
		: _pos(pos), _length(length)
	{
	}

	inline uint64_t pos() const
	{
		return _pos;
	}
	inline void set_pos( const uint64_t& pos )
	{
		_pos = pos;
	}
	inline uint64_t length() const
	{
		return _length;
	};
	inline void set_length( const uint64_t& length )
	{
		_length = length;
	}

	inline void set( const uint64_t& pos, const uint64_t& length )
	{
		_pos = pos;
		_length = length;
	}
	//ֻ�ı�pos��end����
	void dec_pos( uint64_t offset )
	{
		_pos -= offset;
		_length += offset;
	}
	//ֻ�ı�pos��end����
	void inc_pos( uint64_t offset )
	{
		_pos += offset;
		_length -= offset;
	}
	inline uint64_t end() const
	{
		if( _length == nlength)
			return _length;
		return _pos + _length;
	}
	inline bool is_in( uint64_t pos ) const
	{
		return ( (pos>=_pos) && (pos<end()) );
	}
	inline bool is_contain( const range& r ) const
	{
		return ( (r.pos()>=_pos) && (r.end()<=this->end()) );
	}
	inline bool is_full_range() const
	{
		return ( ( _pos==0 ) && ( _length==nlength ) );
	};

	bool friend operator < (const range& r1, const range& r2)
	{
		if(r1.pos() < r2.pos())
		{
			return true;
		}
		else if(r1.pos() == r2.pos())
		{
			if(r1.length() < r2.length())
				return true;
			else
				return false;
		}
		return false;
	}

	range& operator -=(const range& r)
	{
		if(_pos >= r.pos())
		{
			if(end() <= r.end())
			{
				this->set_length(0);
			}
			else
			{
				this->set_length(end() - r.end());
				this->set_pos(r.end());
			}
		}
		else
		{
			if(end() > r.pos())
			{
				if(end() > r.end())
				{
					//split two range, return half
					this->set_length(r.pos() - _pos);
				}
				else
				{
					this->set_length(r.pos() - _pos);
				}
			}
		}
		return *this;
	}

	// �������� range �Ľ���
	static range intersection( const range & r1, const range & r2 );
	static bool  intersection( const range & r1, const range & r2,range & result);

	inline range & operator=( const range & r )
	{
		if (this != &r)
		{
			_pos = r.pos();
			_length = r.length();
		}
		return *this;
	}

	inline bool operator==( const range & r ) const
	{
		return (_pos==r.pos())&&(_length==r.length());
	}

	inline bool operator!= ( const range& r ) const
	{
		return !(*this == r);
	}

	bool is_relevant( const range& r ) const
	{
		if( (r.end() <= _pos) || (this->end() <= r.pos() ) )
			return false;
		else
			return true;
	}
	std::string to_string() const
	{
		ostringstream ostr;
		ostr << "["
			 << left << std::setw(10) << _pos << ","
			 << left << std::setw(10) << _length << ","
			 << left << std::setw(10) << this->end()
			 << ")";

		return ostr.str();
	}

private:
	uint64_t _pos;
	uint64_t _length;
};

void range_minus_range(const range& l, const range& r, std::vector<range>& result);
void range_vector_minus_range(const std::vector<range>& l, const range& r, std::vector<range>& result);

#endif // !defined(AFX_RANGE_H__4736E282_CE0C_4091_BE84_2C4CB914BD63__INCLUDED_)
