#pragma once 
#include <iostream> 
#include <vector> 
 
// class IteratorRange - implementation

template <typename Iter> 
class IteratorRange { 
	public: 
		IteratorRange(const Iter& range_begin, const Iter& range_end) 
			: begin_(range_begin)
			, end_(range_end) 
		{}
 
		Iter   begin() const { return begin_       ; }
		Iter   end  () const { return end_         ; }
		size_t size () const { return end_ - begin_; }
 
	private: 
		Iter begin_; 
		Iter end_; 
}; 
 
// class Paginator - implementation

template <typename Iterator> 
class Paginator { 
	public: 
		Paginator(const Iterator& range_begin, const Iterator& range_end, const size_t& page_size) 
			: range_begin_(range_begin)
			, range_end_(range_end)
			, page_size_(page_size)
		{ 
			FillPages(); 
		} 
 
		void FillPages() { 
			for (auto it = range_begin_; it < range_end_; it += page_size_) {
				if (it + page_size_ >= range_end_) {
					IteratorRange page(it, range_end_); 
					pages_.push_back(page); 
				} 
				else {
					IteratorRange page_last(it, it + page_size_); 
					pages_.push_back(page_last); 
				} 
			} 
		} 
 
		auto begin() const { return pages_.begin(); }
		auto end() const { return pages_.end(); }
		size_t size() const { return pages_.end() - pages_.begin(); }
 
	private: 
		Iterator range_begin_; 
		Iterator range_end_; 
		size_t page_size_; 
		std::vector <IteratorRange<Iterator>> pages_; 
}; 
 
template <typename Container> 
auto Paginate(const Container& c, const size_t& page_size) { 
	return Paginator(begin(c), end(c), page_size); 
} 
 
template <typename Iterator> 
std::ostream& operator<<(std::ostream& output, const IteratorRange<Iterator>& a) { 
	for (auto it = a.begin(); it < a.end(); ++it) { 
		output << *it; 
	} 
	return output; 
}