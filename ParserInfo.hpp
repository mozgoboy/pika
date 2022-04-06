#pragma once

#include "ASTNode.hpp";
#include "Clause.hpp";
#include "Seq.hpp";
#include "Terminal.hpp";
#include "Grammar.hpp";
#include "Match.hpp";
#include "MemoKey.hpp";
#include "MemoTable.hpp";

#define max(x,y) (((int)((x)<(y)) * (y)) + ((int)((y)<=(x)) * (x)))

class ParserInfo
{
private:
    int findCycleDepth(Match* match,
        unordered_map<int, unordered_map<int, unordered_map<int, Match>>>* cycleDepthToMatches)
    {
        auto cycleDepth = 0;
        for (auto subClauseMatchEnt : *match->getSubClauseMatches()) 
        {
            auto subClauseMatch = subClauseMatchEnt.second;
            auto subClauseIsInDifferentCycle = //
                match->memoKey->clause->clauseIdx <= subClauseMatch->memoKey->clause->clauseIdx;
            auto subClauseMatchDepth = findCycleDepth(subClauseMatch, cycleDepthToMatches);
            cycleDepth = max(cycleDepth, subClauseIsInDifferentCycle ? subClauseMatchDepth + 1 : subClauseMatchDepth);
        }
        /*
        auto matchesForDepth = cycleDepthToMatches.computeIfAbsent(cycleDepth,
            k -> new TreeMap<>(Collections.reverseOrder()));

        var matchesForClauseIdx = matchesForDepth.computeIfAbsent(match.memoKey.clause.clauseIdx,
            k -> new TreeMap<>());

        matchesForClauseIdx.put(match.memoKey.startPos, match);
        */ 
        // ¬ыше че то снова сложно через treemap
        return cycleDepth;
    }

public:
    void printClauses(Grammar grammar)
    {
        for (int i = grammar.allClauses.size() - 1; i >= 0; --i)
        {
            auto clause = grammar.allClauses[i];
            cout << i << " : " << clause->toStringWithRuleNames();
        }
    }

    void printMemoTable(MemoTable memoTable)
    {
        vector<string> buf;
        int marginWidth = 0;
        for (int i = 0; i < memoTable.grammar.allClauses.size(); i++)
        {
            string s = memoTable.grammar.allClauses.size() - 1 - i + " : ";
            buf[i].append(s);
            Clause* clause = memoTable.grammar.allClauses[memoTable.grammar.allClauses.size() - 1 - i];
            if (clause->TypeOfClause == TypesOfClauses::Terminal) 
            {
                buf[i].append("[terminal] ");
            }
            if (clause->canMatchZeroChars) 
            {
                buf[i].append("[canMatchZeroChars] ");
            }
            buf[i].append(clause->toStringWithRuleNames());
            marginWidth = max(marginWidth, buf[i].length() + 2);
        }
        int tableWidth = marginWidth + memoTable.input.length() + 1;
        for (int i = 0; i < memoTable.grammar.allClauses.size(); i++) 
        {
            while (buf[i].length() < marginWidth) {
                buf[i].append(" ");
            }
            while (buf[i].length() < tableWidth) {
                buf[i].append(" - ");
            }
        }
        auto nonOverlappingMatches = memoTable.getAllNonOverlappingMatches();
        for (auto clauseIdx = memoTable.grammar.allClauses.size() - 1; clauseIdx >= 0; --clauseIdx) 
        {
            auto row = memoTable.grammar.allClauses.size() - 1 - clauseIdx;
            auto clause = memoTable.grammar.allClauses[clauseIdx];
            auto matchesForClause = nonOverlappingMatches[clause];
            if (matchesForClause != nullptr) 
            {
                for (auto matchEnt : matchesForClause.entrySet()) 
                {
                    auto match = matchEnt.second;
                    auto matchStartPos = match.memoKey.startPos;
                    auto matchEndPos = matchStartPos + match.len;
                    if (matchStartPos <= memoTable.input.length()) 
                    {
                        buf[row][marginWidth + matchStartPos] = '#';
                        for (int j = matchStartPos + 1; j < matchEndPos; j++) 
                        {
                            if (j <= memoTable.input.length()) {
                                buf[row][marginWidth + j] = '=';
                            }
                        }
                    }
                }
            }
            cout << buf[row];
        }
        for (int j = 0; j < marginWidth; j++) {
            cout << " ";
        }
        for (int i = 0; i < memoTable.input.length(); i++) {
            cout << i % 10;
        }
        cout << endl;

        for (int i = 0; i < marginWidth; i++) {
            cout << " ";
        }
        cout << StringUtils::replaceNonASCII(memoTable.input);
    }

    void printParseTreeInMemoTableForm(MemoTable memoTable)
    {
        if (memoTable.grammar.allClauses.size() == 0) 
        {
            //throw new IllegalArgumentException("Grammar is empty");
            // пробрасываем исключение
        }

        auto cycleDepthToMatches = new unordered_map<int, unordered_map<int, unordered_map<int, Match>>>(Collections.reverseOrder()); // € не понимаю что делает reverseOrder
        auto inputSpanned = new IntervalUnion();

        auto nonOverlappingMatches = memoTable.getAllNonOverlappingMatches();
        auto maxCycleDepth = 0;
        for (auto clauseIdx = memoTable.grammar.allClauses.size() - 1; clauseIdx >= 0; --clauseIdx) 
        {
            auto clause = memoTable.grammar.allClauses[clauseIdx];
            auto matchesForClause = nonOverlappingMatches[clause];
            if (matchesForClause != nullptr) 
            {
                for (auto matchEnt : matchesForClause.entrySet()) 
                {
                    auto match = matchEnt.second;
                    auto matchStartPos = match.memoKey.startPos;
                    auto matchEndPos = matchStartPos + match.len;
                    // Only add parse tree to chart if it doesn't overlap with input spanned by a higher-level match
                    if (!inputSpanned.rangeOverlaps(matchStartPos, matchEndPos)) {
                        // Pack matches into the lowest cycle they will fit into
                        auto cycleDepth = findCycleDepth(match, cycleDepthToMatches);
                        maxCycleDepth = max(maxCycleDepth, cycleDepth);
                        // Add the range spanned by this match
                        inputSpanned.addRange(matchStartPos, matchEndPos);
                    }
                }
            }
        }
        auto matchesForRow = new vector<map<int, Match>>();
        auto clauseForRow = new vector<Clause>();
        for (auto matchesForDepth : cycleDepthToMatches.values()) 
        {
            for (auto matchesForClauseIdxEnt : matchesForDepth.entrySet()) 
            {
                clauseForRow->push_back(memoTable.grammar.allClauses[matchesForClauseIdxEnt.first]);
                matchesForRow->push_back(matchesForClauseIdxEnt.second);
            }
        }

        vector<string> rowLabel;
        auto rowLabelMaxWidth = 0;
        for (auto i = 0; i < clauseForRow->size(); i++) 
        {
            Clause clause = clauseForRow[i];
            if (clause->TypeOfClause == TypesOfClauses::Terminal) 
            {
                rowLabel[i].append("[terminal] ");
            }
            if (clause.canMatchZeroChars) 
            {
                rowLabel[i].append("[canMatchZeroChars] ");
            }
            rowLabel[i].append(clause.toStringWithRuleNames());
            rowLabel[i].append("  ");
            rowLabelMaxWidth = Math.max(rowLabelMaxWidth, rowLabel[i].length());
        }
        for (var i = 0; i < clauseForRow.size(); i++) {
            var clause = clauseForRow.get(i);
            var clauseIdx = clause.clauseIdx;
            // Right-justify the row label
            String label = rowLabel[i].toString();
            rowLabel[i].setLength(0);
            for (int j = 0, jj = rowLabelMaxWidth - label.length(); j < jj; j++) {
                rowLabel[i].append(' ');
            }
            rowLabel[i].append(String.format("%3d", clauseIdx) + " : ");
            rowLabel[i].append(label);
        }
    }

    
};