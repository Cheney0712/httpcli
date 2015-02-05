// range_queue.h: interface for the range_queue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANGE_QUEUE_H__61DFEF04_76EA_4DE4_8168_BDDAB5B33C5C__INCLUDED_)
#define AFX_RANGE_QUEUE_H__61DFEF04_76EA_4DE4_8168_BDDAB5B33C5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include "range.h"

class range_queue
{
	std::vector<range> _range_q;

public:
	range_queue();
	range_queue( const range&);
	range_queue(const std::vector<range>&);
	virtual ~range_queue();

	std::vector<range>& ranges();
	const std::vector<range>& ranges() const;
	void clear();

	const std::vector<range> &operator = (const range_queue&);
	const std::vector<range> &operator = (const std::vector<range>&);
	const std::vector<range> &operator = ( const range&);
	const std::vector<range> &operator+= ( const range&);
	const std::vector<range> &operator-= ( const range&);
	
	const std::vector<range> &operator+= ( const range_queue&);
	const std::vector<range> &add_to( const range_queue& r_q, range_queue& result ) const;
	const std::vector<range> &add_to( const range_queue& r_q, std::vector<range>& result ) const;

	const std::vector<range> &operator-= ( const range_queue&);
	const std::vector<range> &sub_to( const range_queue& r_q, range_queue& result ) const;
	const std::vector<range> &sub_to( const range_queue& r_q, std::vector<range>& result ) const;

	void swap( range_queue& other );


	const std::vector<range> &operator+= ( const std::vector<range>&);
	const std::vector<range> &operator-= (const std::vector<range>&);
	range bound(void) const; 
	range max_size(void) const;
	range min_size(void) const;
	
	const std::vector<range> &same(const range_queue &q);
	const std::vector<range> &same_to(const range_queue &q , range_queue &result ) const ;
	const std::vector<range> &same_to(const range_queue &q , std::vector<range> &result ) const ;

	//����q�д�СС��min_len��range������same����
	const std::vector<range> &same_to(const range_queue &q , unsigned min_len, range_queue &result ) const;
	//����q�д�СС��min_len��range������same����
	const std::vector<range> &same_to(const range_queue &q , unsigned min_len, std::vector<range> &result ) const;

	const std::vector<range> &same(const range &r );
	const std::vector<range> &same_to( const range &r, range_queue& result ) const;
	const std::vector<range> &same_to( const range &r, std::vector<range>& result ) const;

	//result�����ཻ���ֵ�һ��range
	bool first_same_to( const range &r, range &result ) const;
	void sub_same_to(const range_queue &r_q, range_queue &sub_q, range_queue &same_q);
	void add_new_to(const range_queue &r_q, range_queue &add_q, range_queue &new_q);

	uint64_t all_range_length(void) const;
	bool is_range_relevant(const range&) const;
	bool is_inner_range(const range&) const;

	bool is_complete_range(const range& r, uint64_t file_size) const;

	bool is_contain(const range_queue&) const;
	//������trueʱ��r�������queue���������飬���������ȵ���is_contain���ٵ�max_size
	//��Ϊ��Ч�ʣ������������ܷ���һ��������
	bool is_contain(const range_queue&, range& r ) const;
	bool is_relevant(const range_queue&) const;
	bool is_equal( const range_queue& q ) const;

	void to_string(std::string &result) const;
	std::string to_string() const;
	std::string to_string_line( ) const;

	// range_queue ������λ��inc_offset������λ�����ӣ�dec_offset������λ�ü���
	void inc_offset( uint64_t offset_value );
	void dec_offset( uint64_t offset_value );

protected:
	void init( const std::vector<range>& );
};

void range_sub_range_queue( const range& r, const range_queue&, range_queue& result );

#endif // !defined(AFX_RANGE_QUEUE_H__61DFEF04_76EA_4DE4_8168_BDDAB5B33C5C__INCLUDED_)
