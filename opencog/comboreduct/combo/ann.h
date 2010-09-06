/** ann.h --- 
 *
 * Copyright (C) 2010 Joel Lehman
 *
 * Author: Joel Lehman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef _OPENCOG_ANN_H
#define _OPENCOG_ANN_H

namespace combo
{
namespace id {
enum ann_id {
    ann, ann_node, ann_input //ann additions
};
}
typedef id::ann_id ann_id;

class ann_type {
public:
    ann_type(unsigned int i, ann_id _id) : idx(i), id(_id) {}
    unsigned int idx;
    ann_id id;

    bool operator<(ann_type rhs) const {
        return idx < rhs.idx;
    } 
    bool operator==(ann_type rhs) const {
        return idx == rhs.idx;
    }
    bool operator!=(ann_type rhs) const {
        return idx != rhs.idx;
    }
};

} // ~namespace combo

#endif // _OPENCOG_ANN_H
