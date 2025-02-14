#pragma once
#include "Types.hpp"
#include <iostream>
#include <string>
#include <cassert>
#include <utility>

class Move {
   public:

    Move() = default;
    constexpr explicit Move(std::uint16_t d) :
        data(d) {}

    constexpr Move(Square from, Square to) :
        data((from << 6) + to) {}

    constexpr Move(Square from, Square to, MoveType mt)
        : data(from + (to << 6) + (mt << 12))
    {}

    // template<MoveType T>
    // static constexpr Move make(Square from, Square to, PieceType pt = KNIGHT) {
    //     return Move(T + ((pt - KNIGHT) << 12) + (from << 6) + to);
    // }

    constexpr Square from_sq() const {
        assert(is_ok());
        return Square((data >> 6) & 0x3F);
    }

    constexpr Square to_sq() const {
        assert(is_ok());
        return Square(data & 0x3F);
    }

    constexpr int from_to() const { return data & 0xFFF; }

    constexpr MoveType type_of() const { return MoveType(data & (3 << 14)); }

    constexpr PieceType promotion_type() const { return PieceType(((data >> 12) & 3) + KNIGHT); }

    constexpr bool is_ok() const { return none().data != data && null().data != data; }

    // Move(Hash hash)
    //     : data((hash & 0b111111111111) | ((hash & 0b11000000000000) << 3))
    // {}

    static Move from_int(uint16_t number)
    {
        Move move;
        move.data = number;
        return move;
    }

    static constexpr Move null() { return Move(65); }
    static constexpr Move none() { return Move(0); }

    constexpr Square from() const { return data & 0b111111; }
    constexpr Square to() const { return (data & 0b111111000000) >> 6; }
    constexpr bool is_capture() const { return move_type() & CAPTURE; }
    constexpr bool is_ep_capture() const { return move_type() == EP_CAPTURE; }
    constexpr bool is_double_pawn_push() const { return move_type() == DOUBLE_PAWN_PUSH; }
    constexpr bool is_promotion() const { return move_type() & KNIGHT_PROMO; }
    constexpr bool is_castle() const { return (move_type() == KING_CASTLE) || (move_type() == QUEEN_CASTLE); }
    constexpr PieceType promo_piece() const { return static_cast<PieceType>((move_type() & 0b0011) + 1); }
    constexpr MoveType move_type() const { return static_cast<MoveType>(data >> 12); }
    constexpr Hash hash() const { return (data & 0b111111111111) | ((data & 0b11000000000000000) >> 3); }
    constexpr Hash to_int() const { return data; }

    constexpr bool operator==(const Move& other) const { return data == other.data; }
    constexpr bool operator!=(const Move& other) const { return data != other.data; }

    constexpr bool operator==(const Move& m) const { return data == m.data; }
    constexpr bool operator!=(const Move& m) const { return data != m.data; }

    constexpr explicit operator bool() const { return data != 0; }

    constexpr std::uint16_t raw() const { return data; }

    struct MoveHash {
        std::size_t operator()(const Move& m) const { return m.data; }
    };

   protected:
    std::uint16_t data;
};


// class Move
// {
//     std::uint16_t m_move;
//     // uint16_t m_move;

// public:
//     constexpr Move()
//         : m_move(0)
//     {}
//     Move(Square from, Square to)
//         : m_move(from + (to << 6))
//     {}
//     Move(Square from, Square to, MoveType mt)
//         : m_move(from + (to << 6) + (mt << 12))
//     {}
//     Move(Hash hash)
//         : m_move((hash & 0b111111111111) | ((hash & 0b11000000000000) << 3))
//     {}

//     static Move from_int(uint16_t number)
//     {
//         Move move;
//         move.m_move = number;
//         return move;
//     }

//     static constexpr Move null() { return Move(65); }
//     static constexpr Move none() { return Move(0); }
    
//     constexpr Square from() const { return m_move & 0b111111; }
//     constexpr Square to() const { return (m_move & 0b111111000000) >> 6; }
//     constexpr bool is_capture() const { return move_type() & CAPTURE; }
//     constexpr bool is_ep_capture() const { return move_type() == EP_CAPTURE; }
//     constexpr bool is_double_pawn_push() const { return move_type() == DOUBLE_PAWN_PUSH; }
//     constexpr bool is_promotion() const { return move_type() & KNIGHT_PROMO; }
//     constexpr bool is_castle() const { return (move_type() == KING_CASTLE) || (move_type() == QUEEN_CASTLE); }
//     constexpr PieceType promo_piece() const { return static_cast<PieceType>((move_type() & 0b0011) + 1); }
//     constexpr MoveType move_type() const { return static_cast<MoveType>(m_move >> 12); }
//     constexpr Hash hash() const { return (m_move & 0b111111111111) | ((m_move & 0b11000000000000000) >> 3); }
//     constexpr Hash to_int() const { return m_move; }

//     constexpr bool operator==(const Move& other) const { return m_move == other.m_move; }
//     constexpr bool operator!=(const Move& other) const { return m_move != other.m_move; }
// };


constexpr Move MOVE_NULL = Move();


class MoveList
{
    Move* m_moves;
    Move* m_end;

public:
    MoveList()
        : m_moves(nullptr), m_end(nullptr)
    {
    }

    MoveList(Move* start)
        : m_moves(start), m_end(start)
    {
    }

    MoveList(Move* start, Move* end)
        : m_moves(start), m_end(end)
    {
    }

    void push(Move move)
    {
        *(m_end++) = move;
    }

    void push(Square from, Square to)
    {
        *(m_end++) = Move(from, to);
    }

    void push(Square from, Square to, MoveType mt)
    {
        *(m_end++) = Move(from, to, mt);
    }

    template<bool IS_CAPTURE>
    void push_promotions(Square from, Square to)
    {
        if (IS_CAPTURE)
        {
            *(m_end++) = Move(from, to, QUEEN_PROMO_CAPTURE);
            *(m_end++) = Move(from, to, KNIGHT_PROMO_CAPTURE);
            *(m_end++) = Move(from, to, ROOK_PROMO_CAPTURE);
            *(m_end++) = Move(from, to, BISHOP_PROMO_CAPTURE);
        }
        else
        {
            *(m_end++) = Move(from, to, QUEEN_PROMO);
            *(m_end++) = Move(from, to, KNIGHT_PROMO);
            *(m_end++) = Move(from, to, ROOK_PROMO);
            *(m_end++) = Move(from, to, BISHOP_PROMO);
        }
    }

    void pop()
    {
        m_end--;
    }

    void pop(Move* move)
    {
        *move = *(--m_end);
    }

    int length() const
    {
        return m_end - m_moves;
    }

    void clear()
    {
        m_end = m_moves;
    }

    bool contains(Move move) const
    {
        for (Move* i = m_moves; i < m_end; i++)
            if (*i == move)
                return true;

        return false;
    }

    void pop_first()
    {
        m_moves++;
    }

    // Iterators
    Move* begin() { return m_moves; }
    Move* end()   { return m_end; }
    const Move* begin() const { return m_moves; }
    const Move* end()   const { return m_end; }
    const Move* cbegin() const { return m_moves; }
    const Move* cend()   const { return m_end; }
};


class MoveStack
{
    Move* m_moves;
    int m_depth;
    int m_current;

public:
    MoveStack(int depth = 1)
        : m_moves(new Move[NUM_MAX_MOVES * depth]), m_depth(depth), m_current(0)
    {
    }

    virtual ~MoveStack()
    {
        delete[] m_moves;
    }

    MoveStack(const MoveStack& other)
        : m_moves(new Move[NUM_MAX_MOVES * other.m_depth]), m_depth(other.m_depth), m_current(other.m_current)
    {
        for (int i = 0; i < NUM_MAX_MOVES * other.m_depth; i++)
            m_moves[i] = other.m_moves[i];
    }

    MoveStack(MoveStack&& other) noexcept
        : m_moves(other.m_moves), m_depth(other.m_depth), m_current(other.m_current)
    {
        other.m_moves = nullptr;
    }

    MoveStack& operator=(const MoveStack& other)
    {
        if (&other != this)
        {
            if (m_depth != other.m_depth)
            {
                m_depth = other.m_depth;
                Move* tmp = new Move[NUM_MAX_MOVES * m_depth];
                delete[] m_moves;
                m_moves = tmp;
            }

            m_current = other.m_current;
            for (int i = 0; i < NUM_MAX_MOVES * m_depth; i++)
                m_moves[i] = other.m_moves[i];
        }
        return *this;
    }

    MoveStack& operator=(MoveStack&& other) noexcept
    {
        std::swap(m_moves, other.m_moves);
        std::swap(m_depth, other.m_depth);
        std::swap(m_current, other.m_current);
        return *this;
    }

    void reset_pos()
    {
        m_current = 0;
    }

    MoveList list() const
    {
        return MoveList(m_moves + m_current * NUM_MAX_MOVES);
    }

    MoveList list(int pos) const
    {
        return MoveList(m_moves + pos * NUM_MAX_MOVES);
    }

    MoveStack& operator++()
    {
        m_current++;
        return *this;
    }

    MoveStack& operator--()
    {
        m_current--;
        return *this;
    }
};
