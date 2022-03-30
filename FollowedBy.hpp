//
// This file is part of the pika parser reference implementation:
//
//     https://github.com/lukehutch/pikaparser
//
// The pika parsing algorithm is described in the following paper: 
//
//     Pika parsing: reformulating packrat parsing as a dynamic programming algorithm solves the left recursion
//     and error recovery problems. Luke A. D. Hutchison, May 2020.
//     https://arxiv.org/abs/2005.06444
//
// This software is provided under the MIT license:
//
// Copyright 2020 Luke A. D. Hutchison
//  
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#pragma ones

#include "Clause.hpp"
#include "Match.hpp"
#include "MemoKey.hpp";
#include "MemoTable.hpp";

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

/** The FollowedBy (lookahead) PEG operator. */
class FollowedBy : public Clause {
    TypesofClauses TypeOfClause = TypesofClauses::FollowedBy;
    FollowedBy(Clause* subClause) : Clause(vector<Clause*> { subClause }) {

    }
    void determineWhetherCanMatchZeroChars() {
        // Don't set canMatchZeroChars to true, because FollowedBy will only match if it subclause
        // consumes at least one matching character
        if (labeledSubClauses[0]->clause->canMatchZeroChars) {
            cout << "Subclause always matches zero characters, so this clause has no effect: " + this->toString();
            abort();
        }
    }

    Match* match(MemoTable* memoTable, MemoKey* memoKey, string input) {
        LabeledClause* labeledSubClause = labeledSubClauses[0];
        MemoKey subClauseMemoKey(labeledSubClause->clause, memoKey->startPos);
        Match* subClauseMatch = memoTable->lookUpBestMatch(&subClauseMemoKey);
        if (subClauseMatch != nullptr) {
            // If there is any valid subclause match, return a new zero-length match
            Match mast(memoKey);
            return &mast;
        }
        return nullptr;
    }

    string toString() {
        if (toStringCached.empty()) {
            toStringCached = "&" + labeledSubClauses[0]->toStringWithASTNodeLabel(this);
        }
        return toStringCached;
    }
};
