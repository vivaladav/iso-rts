#pragma once

namespace lib
{
namespace ai
{

class PathNode
{
public:
    PathNode(int row = 0, int col = 0, int g = 0, int h = 0, PathNode * parent = nullptr);

    bool operator<(const PathNode & other) const;

    int GetRow() const;
    int GetCol() const;
    void SetCell(int row, int col);

    int GetF() const;
    int GetG() const;
    int GetH() const;
    void SetCosts(int g, int h);

    PathNode * GetParent() const;
    void SetParent(PathNode * p);

private:
    PathNode * mParent;

    int mRow;
    int mCol;

    int mF;
    int mG;
    int mH;
};

inline PathNode::PathNode(int row, int col, int g, int h, PathNode * parent)
    : mParent(parent)
    , mRow(row)
    , mCol(col)
    , mF(g + h)
    , mG(g)
    , mH(h)
{
}

inline int PathNode::GetRow() const { return mRow; }
inline int PathNode::GetCol() const { return mCol; }
inline void PathNode::SetCell(int row, int col)
{
    mRow = row;
    mCol = col;
}

inline int PathNode::GetF() const { return mF; }
inline int PathNode::GetG() const { return mG; }
inline int PathNode::GetH() const { return mH; }
inline void PathNode::SetCosts(int g, int h)
{
    mF = g + h;
    mG = g;
    mH = h;
}

inline bool PathNode::operator<(const PathNode & other) const
{
    return mF < other.mF;
}

inline PathNode * PathNode::GetParent() const { return mParent; }
inline void PathNode::SetParent(PathNode * p) { mParent = p; }

} // namespace graphic
} // namespace ai
