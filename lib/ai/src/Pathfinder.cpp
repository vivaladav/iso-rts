#include "ai/Pathfinder.h"

#include "PathNode.h"
#include "ai/IPathMap.h"

#include <algorithm>

namespace lib
{
namespace ai
{

bool CompareNodes(const PathNode * a, const PathNode * b)
{
    return a->GetF() > b->GetF();
}

Pathfinder::~Pathfinder()
{
    delete[] mNodes;
    mNodes = nullptr;
}

void Pathfinder::SetMap(const IPathMap * map, unsigned int mapRows, unsigned int mapCols)
{
    mMap = map;
    mMapRows = mapRows;
    mMapCols = mapCols;
    mMapSize = mMapRows * mMapCols;

    InitNodes();

    ClearLists();

    // reserve memory
    mOpenList.reserve(mMapSize);
    mOpenMap.reserve(mMapSize);
    mClosedMap.reserve(mMapSize);
}

std::vector<unsigned int> Pathfinder::MakePath(unsigned int r0, unsigned int c0,
                                               unsigned int r1, unsigned int c1,
                                               bool includeStart)
{
    std::vector<unsigned int> path;

    // out of bounds
    if(!(r0 < mMapRows && c0 < mMapCols && r1 < mMapRows && c1 < mMapCols))
        return path;

    // no pathfinding needed
    if(r0 == r1 && c0 == c1)
    {
        if(includeStart)
            path.emplace_back(GetCellIndex(r0, c0));

        return path;
    };

    // unwalkable goal
    if(!mMap->IsCellWalkable(r1, c1))
        return path;

    ClearLists();

    mR1 = r1;
    mC1 = c1;

    // add start node to the open list
    PathNode * n = GetNewNode(r0, c0, 0, CostToGoal(r0, c0));
    AddToOpen(n, GetCellIndex(r0, c0));

    while(!mOpenList.empty())
    {
        std::pop_heap(mOpenList.begin(), mOpenList.end(), CompareNodes);
        PathNode * curr = mOpenList.back();
        mOpenList.pop_back();
        const unsigned int currRow = curr->GetRow();
        const unsigned int currCol = curr->GetCol();

        const unsigned int currIdx = GetCellIndex(currRow, currCol);
        auto itCurr = mOpenMap.find(currIdx);
        mOpenMap.erase(itCurr);

        mClosedMap.insert({currIdx, curr});

        // goal found -> generate path and return
        if(currRow == r1 && currCol == c1)
        {
            while(curr->GetParent() != nullptr)
            {
                path.emplace_back(GetCellIndex(curr->GetRow(), curr->GetCol()));
                curr = curr->GetParent();
            }

            // add start node
            if(includeStart)
                path.emplace_back(GetCellIndex(curr->GetRow(), curr->GetCol()));

            std::reverse(path.begin(), path.end());

            return path;
        }

        // process neighbor nodes (orthagonals)
        HandleNode(curr, -1, 0);
        HandleNode(curr, 0, -1);
        HandleNode(curr, 0, 1);
        HandleNode(curr, 1, 0);

        // process diagonals nodes if allowed
        if(mAllowDiag)
        {
            HandleNode(curr, -1, -1);
            HandleNode(curr, -1, 1);
            HandleNode(curr, 1, -1);
            HandleNode(curr, 1, 1);
        }
    }

    return path;
}

void Pathfinder::AddToOpen(PathNode * node, unsigned int index)
{
    mOpenList.emplace_back(node);
    std::push_heap(mOpenList.begin(), mOpenList.end(), CompareNodes);

    mOpenMap.insert({index, node});
}

void Pathfinder::HandleNode(PathNode * prev, int dr, int dc)
{
    const unsigned int r = prev->GetRow() + dr;
    const unsigned int c = prev->GetCol() + dc;

    // out of bounds
    if(!(r < mMapRows && c < mMapCols))
        return;

    // not walkable
    if(!mMap->IsCellWalkable(r, c))
        return;

    const unsigned int adjIdx = GetCellIndex(r, c);

    // in closed list
    if(mClosedMap.find(adjIdx) != mClosedMap.end())
        return;

    const int adjG = CostToAdj(dr, dc) + prev->GetG();
    const int adjH = CostToGoal(r, c);

    auto oldIt = mOpenMap.find(adjIdx);

    if(oldIt != mOpenMap.end())
    {
        PathNode * old = oldIt->second;

        if(old->GetG() > adjG)
        {
            old->SetCosts(adjG, adjH);
            old->SetParent(prev);
            std::make_heap(mOpenList.begin(), mOpenList.end(), CompareNodes);
        }
    }
    // not in open list -> new node
    else
    {
        PathNode * adj = GetNewNode(r, c, adjG, adjH, prev);
        AddToOpen(adj, adjIdx);
    }
}

void Pathfinder::ClearLists()
{
    // clear all lists
    mOpenList.clear();
    mOpenMap.clear();
    mClosedMap.clear();

    mNextAvailableNode = 0;
}

void Pathfinder::InitNodes()
{
    delete[] mNodes;

    mNodes = new PathNode[mMapSize];

    mNextAvailableNode = 0;
}

PathNode * Pathfinder::GetNewNode(int row, int col, int g, int h, PathNode * parent)
{
    PathNode * n = &mNodes[mNextAvailableNode++];

    n->SetCell(row, col);
    n->SetCosts(g, h);
    n->SetParent(parent);

    return n;
}

} // namespace graphic
} // namespace ai
