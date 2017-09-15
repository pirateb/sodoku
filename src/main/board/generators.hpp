#pragma once
#include <array>
#include <algorithm>

template <typename T>
class forward_iterator : public std::iterator<
    std::forward_iterator_tag,
    T,
    T,
    const T*,
    T>
{ };

template <size_t N>
class Range
{
public:
  Range() { }
  class iterator :  public forward_iterator<int>{
      public:
        iterator(size_t idx) : _idx{idx} { };
        iterator operator++() { _idx ++; return *this;};
        bool operator != (const iterator& other) { return _idx != other._idx;} ;
        int operator*() const { return _idx;}

      private:
        size_t                _idx;
     };

    iterator begin() const { return iterator(0); }
    iterator end() const { return iterator(N); }
};

template <size_t N>
class RandomRange
{
public:
  RandomRange() {
    int i = 0;
    std::generate(_range.begin(), _range.end(), [&i](){return i++;} );
    // shuffle
    std::random_shuffle(_range.begin(), _range.end());
  }

  class iterator :  public forward_iterator<int>{
      public:
        iterator(size_t idx, const std::array<int, N>& range) : _idx{idx}, _range{range} {
        };
        iterator operator++() { _idx ++; return *this;};
        bool operator != (const iterator& other) { return !((_idx == other._idx) && (_range == other._range));} ;
        int operator*() const { return _range[_idx];}

      private:
        size_t                        _idx;
        const std::array<int, N>&     _range;
     };

    iterator begin() const { return iterator(0, _range); }
    iterator end() const { return iterator(N, _range); }
  private:
    std::array<int, N>    _range;
};


