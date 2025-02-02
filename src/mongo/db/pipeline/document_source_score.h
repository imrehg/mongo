/**
 *    Copyright (C) 2024-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include <list>

#include <boost/smart_ptr/intrusive_ptr.hpp>

#include "mongo/bson/bsonelement.h"
#include "mongo/db/pipeline/document_source.h"
#include "mongo/db/pipeline/expression_context.h"

namespace mongo {

/**
 * A projection-like stage, $score will output documents which are the same as the input
 * documents, now with extra metadata.
 *
 * This stage's goal is twofold:
 * - Help satisfy the constraint of $scoreFusion to participate in hybrid search. A valid input
 *   to $scoreFusion (known as a scored selection pipeline) is forbidden from modifying the
 *   actual documents, so this stage aims to allow computing a new thing as metadata without
 *   being considered a modification.
 * - Provide a way to normalize input scores to the same domain (usually between 0 and 1).
 */
class DocumentSourceScore final {
public:
    static constexpr StringData kStageName = "$score"_sd;

    /**
     * Allows computation of score metadata for non-search pipelines, and also allows weighting or
     * normalizing scores.
     */
    static std::list<boost::intrusive_ptr<DocumentSource>> createFromBson(
        BSONElement elem, const boost::intrusive_ptr<ExpressionContext>& pExpCtx);

private:
    // It is illegal to construct a DocumentSourceScore directly, use createFromBson()
    // instead.
    DocumentSourceScore() = delete;
};

}  // namespace mongo
