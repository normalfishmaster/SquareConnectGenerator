/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef POS_HPP
#define POS_HPP

class Pos
{
public:

    Pos() = default;
    Pos(int row, int col);
    Pos(const Pos& pos) = default;
    Pos(Pos&& pos) noexcept = default;

    ~Pos() = default;

    Pos& operator=(const Pos& pos) = default;
    Pos& operator=(Pos&& pos) noexcept = default;

    Pos operator+(const Pos& pos) const;
    Pos operator-(const Pos& pos) const;
    Pos& operator+=(const Pos& pos);
    Pos& operator-=(const Pos& pos);
    bool operator==(const Pos& pos) const;
    bool operator!=(const Pos& pos) const;

    int GetRow() const;
    int GetCol() const;

    Pos& SetRow(int row);
    Pos& SetCol(int col);

    void IncRow(int row);
    void IncCol(int col);
    void IncRowAndCol(int row, int col);

    static bool IsAdjacent(const Pos& pos1, const Pos& pos2);

private:

    int _row{ 0 };
    int _col{ 0 };
};

#endif // POS_HPP
