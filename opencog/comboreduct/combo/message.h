/*
 * opencog/comboreduct/combo/message.h
 *
 * Copyright (C) 2002-2008 Novamente LLC
 * All Rights Reserved
 *
 * Written by Nil Geisweiller
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
#ifndef _COMBO_MESSAGE_H
#define _COMBO_MESSAGE_H

#include <opencog/util/exceptions.h>

#include "type_tree_def.h"
#include "operator_base.h"

#define COMBO_MESSAGE_PREFIX "message:"

namespace combo {

  //message is essentially a string but is coded as a different type
  //as definite_object because it semantically denotes something else
  class message {
  private:
    std::string _content;
  public:
    message(std::string m) {
      _content = m;
    }

    std::string getContent() const {
      return _content;
    }

    bool operator==(message m) const {
      return _content==m.getContent();
    }
    bool operator!=(message m) const {
      return _content!=m.getContent();
    }
    bool operator<(message m) const {
      return _content<m.getContent();
    }
    bool operator<=(message m) const {
      return _content<=m.getContent();
    }
    bool operator>(message m) const {
      return _content>m.getContent();
    }
    bool operator>=(message m) const {
      return _content>=m.getContent();
    }
    
    static std::string prefix() {
      return COMBO_MESSAGE_PREFIX;
    }
  };

  typedef std::set<message> message_set;
  typedef message_set::iterator message_set_it;
  typedef message_set::const_iterator message_set_const_it;

}//~namespace combo

std::ostream& operator<<(std::ostream&,const combo::message&);

#endif

