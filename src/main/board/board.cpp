/*
 * board.cpp
 *
 *  Created on: Sep 13, 2017
 *      Author: bramsden
 */
#include "board.hpp"
#include <algorithm>

constexpr int Board::NOT_SET;

Board::Board() {
  std::fill(_board.begin(), _board.end(), NOT_SET);
}

Board::Board(BoardRawData board) {
  std::copy(board.begin(), board.end(), _board.begin());
}

int Board::get(size_t idx_x, size_t idx_y) const {
  return _board[idx_x * ROWS + idx_y];
}

int& Board::reference(size_t idx_x, size_t idx_y) {
  return _board[idx_x * ROWS + idx_y];
}

void Board::set(size_t idx_x, size_t idx_y, int val) {
  _board[idx_x * ROWS + idx_y] = val;
}

SubBoard Board::get_subboard(size_t idx_x, size_t idx_y) {
  return SubBoard(*this, idx_x, idx_y);
}

BoardSlice Board::get_row(size_t idx) {
  return BoardSlice(*this, true, idx);
}

BoardSlice Board::get_column(size_t idx) {
  return BoardSlice(*this, false, idx);
}

SubBoardGenerator Board::subboards() {
  return SubBoardGenerator(*this);
}

SliceGenerator Board::rows() {
  return SliceGenerator(*this, true);
}

SliceGenerator Board::cols() {
  return SliceGenerator(*this, false);
}

const Board::BoardRawData& Board::raw_data() {
  return _board;
}
SubBoard::SubBoard(Board& board, size_t idx_x, size_t idx_y)
  : _board{board}, _idx_x{idx_x}, _idx_y{idx_y}
{
}

int SubBoard::get(size_t x, size_t y) const
{
  return _board.get( _idx_x * SUB_BOARD_ROWS + x,
                     _idx_y * SUB_BOARD_ROWS + y);
}

int& SubBoard::reference(size_t x, size_t y) const
{
  return _board.reference( _idx_x * SUB_BOARD_ROWS + x,
                     _idx_y * SUB_BOARD_ROWS + y);
}

void SubBoard::set(size_t x, size_t y, int val)
{
  _board.set( _idx_x * SUB_BOARD_ROWS + x,
              _idx_y * SUB_BOARD_ROWS + y,
              val);
}

BoardSlice::BoardSlice(Board& board, bool isRow, size_t idx)
  : _board{board}, _isRow{isRow}, _idx{idx}
{ }

int BoardSlice::get(size_t get_idx) const
{
  return _isRow ? _board.get(_idx, get_idx)
                : _board.get(get_idx, _idx);
}

int& BoardSlice::reference(size_t get_idx) const
{
  return _isRow ? _board.reference(_idx, get_idx)
                : _board.reference(get_idx, _idx);
}

SubBoardGenerator::SubBoardGenerator(Board& board) : _board{board} { }

// board slice iterators
BoardSlice::iterator::iterator(size_t idx, const BoardSlice& slice) : _idx{idx}, _board_slice{slice} { }
const int& BoardSlice::iterator::operator*() const {return _board_slice.reference(_idx);}
BoardSlice::iterator BoardSlice::iterator::operator++() {_idx++; return *this;}
bool BoardSlice::iterator::operator!= (const BoardSlice::iterator& other) {return _idx != other._idx;}


// subboard iterators
constexpr size_t SubBoard::SUB_BOARD_ROWS;

SubBoard::iterator::iterator(size_t idx, const SubBoard& subboard) : _idx{idx}, _subboard{subboard} { }
const int& SubBoard::iterator::operator*() const {return _subboard.reference(_idx / SubBoard::SUB_BOARD_ROWS, _idx % SubBoard::SUB_BOARD_ROWS); }
SubBoard::iterator SubBoard::iterator::operator++() {_idx++; return *this;}
bool SubBoard::iterator::operator!= (const SubBoard::iterator& other) {return _idx != other._idx;}

// subboard generator iterators
constexpr size_t SubBoardGenerator::SUBBOARD_NUMBER;
SubBoardGenerator::iterator::iterator(size_t idx, Board& board) : _idx{idx}, _board{board} { }
const SubBoard SubBoardGenerator::iterator::operator*() const { return _board.get_subboard(_idx / SubBoardGenerator::SUBBOARD_NUMBER, _idx % SubBoardGenerator::SUBBOARD_NUMBER); }
SubBoardGenerator::iterator SubBoardGenerator::iterator::operator++() {_idx++; return *this;}
bool SubBoardGenerator::iterator::operator!= (const SubBoardGenerator::iterator& other) {return _idx != other._idx;}

// SliceGenerator slice iterators
SliceGenerator::SliceGenerator(Board& board, bool isRow) : _board{board}, _isRow{isRow} { }
SliceGenerator::iterator::iterator(size_t idx, Board& board, bool isRow) : _idx{idx}, _board{board}, _isRow{isRow} { }
const BoardSlice SliceGenerator::iterator::operator*() const { return _isRow ? _board.get_row(_idx) : _board.get_column(_idx); }
SliceGenerator::iterator SliceGenerator::iterator::operator++() {_idx++; return *this;}
bool SliceGenerator::iterator::operator!= (const SliceGenerator::iterator& other) {return (_idx != other._idx);}

