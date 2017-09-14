#pragma once

#include <array>
#include "generators.hpp"

class SubBoard;
class BoardSlice;
class SubBoardGenerator;
class SliceGenerator;

// illustrate a board for sodoku
class Board
{
public:
  static constexpr size_t ROWS = 9;
  static constexpr int NOT_SET = 0;
  using BoardRawData = std::array<int, ROWS*ROWS>;

  Board();
  Board(BoardRawData board);
  int  get(size_t idx_x, size_t idx_y) const;
  int& reference(size_t idx_x, size_t idx_y);
  void set(size_t idx_x, size_t idx_y, int val);
  SubBoard get_subboard(size_t idx_x, size_t idx_y);
  BoardSlice get_row(size_t idx);
  BoardSlice get_column(size_t idx);
  SubBoardGenerator subboards();
  const BoardRawData& raw_data();
  SliceGenerator rows();
  SliceGenerator cols();

private:
  BoardRawData       _board;
};

// illustrate a subboard
class SubBoard
{
public:
  static constexpr size_t SUB_BOARD_ROWS = 3;

  // to enable range for loops
    class iterator :  public forward_iterator<int>{
    public:
      iterator(size_t idx, const SubBoard& slice);
      iterator operator++();
      bool operator != (const iterator& other);
      const int& operator*() const; // this can not be declared here

    private:
      size_t                _idx;
      const SubBoard&       _subboard;
   };

  SubBoard(Board& board, size_t idx_x, size_t idx_y);
  int get(size_t x, size_t y) const;
  int& reference(size_t x, size_t y) const;
  void set(size_t x, size_t y, int val);

  iterator begin() const { return iterator(0, *this); }
  iterator end() const { return iterator(SUB_BOARD_ROWS * SUB_BOARD_ROWS, *this); }

private:
  Board&                                 _board;
  size_t                                 _idx_x;
  size_t                                 _idx_y;
};

// used for range based for loops across all subboards
class SubBoardGenerator
{
public:
  static constexpr size_t SUBBOARD_NUMBER = 3;
  class iterator : public forward_iterator<SubBoard> {
  public:
    iterator(size_t idx, Board& board);
    iterator operator++();
    bool operator != (const iterator& other);
    const SubBoard operator*() const; // this can not be declared here
  private:
    size_t _idx;
    Board& _board;
  };

  SubBoardGenerator(Board& board);
  iterator begin() const { return iterator(0, _board); }
  iterator end() const { return iterator(SUBBOARD_NUMBER * SUBBOARD_NUMBER, _board); }

private:
  Board& _board;
};

// represent a row / column
class BoardSlice
{
public:
  // to enable range for loops
  class iterator : public forward_iterator<int>
  {
  public:
    iterator(size_t idx, const BoardSlice& slice);
    iterator operator++();
    bool operator != (const iterator& other);
    const value_type& operator*() const; // this can not be declared here

  private:
    size_t                _idx;
    const BoardSlice&     _board_slice;
  };

  BoardSlice(Board& board, bool isRow, size_t idx);
  int  get(size_t idx) const;
  int& reference(size_t idx) const;


  iterator begin() const { return iterator(0, *this); }
  iterator end() const { return _isRow ? iterator(Board::ROWS, *this) : iterator(Board::ROWS, *this); }

private:
  Board&            _board;
  bool              _isRow;
  size_t            _idx;
};

class SliceGenerator
{
public:
  class iterator : public forward_iterator<BoardSlice> {
  public:
    iterator(size_t idx, Board& board, bool isRow);
    iterator operator++();
    bool operator != (const iterator& other);
    const BoardSlice operator*() const; // this can not be declared here
  private:
    size_t  _idx;
    Board&  _board;
    bool    _isRow;
  };

  SliceGenerator(Board& board, bool isRow);
  iterator begin() const { return iterator(0, _board, _isRow); }
  iterator end() const { return iterator(Board::ROWS, _board, _isRow);}

private:
  Board&        _board;
  bool          _isRow;
};
