/*
 * opencog/atomspace/Node.h
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Thiago Maia <thiago@vettatech.com>
 *            Andre Senna <senna@vettalabs.com>
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

#ifndef _OPENCOG_NODE_H
#define _OPENCOG_NODE_H

#include <opencog/atomspace/Atom.h>

namespace opencog
{

/**
 * This is a subclass of Atom. It represents the most basic kind of
 * pattern known to the OpenCog system.
 */
class Node : public Atom
{
    friend class SavingLoading;

private:

    // properties
    std::string name;

    void init(Type, const std::string&,
         const TruthValue& = TruthValue::NULL_TV())
        throw (InvalidParamException, AssertionException);

public:

    /**
     * Constructor for this class.
     *
     * @param Node type
     * @param Node name A reference to a std::string with the name of
     *                  the node.  Use empty string for unamed node.
     * @param Node truthvalue A reference to a TruthValue object.
     */
    Node(Type t, const std::string& s,
         const TruthValue&tv = TruthValue::NULL_TV())
        : Atom(t,tv) {
        init(t,s,tv);
    }

    /** Copy constructor, does not copy atom table membership! */
    Node(const Node &n) 
        : Atom(n.getType(),n.getTruthValue()) {
        init(n.getType(), n.name, n.getTruthValue());
    }

    /**
     * Destructor for this class.
     */
    virtual ~Node() throw ();

    /**
     * Gets the name of the node.
     *
     * @return The name of the node.
     */
    const std::string& getName() const;

    /*
     * @param Node name A reference to a std::string with the name
     *             of the node.  Use empty string for unamed node.
     * @exception RuntimeException is thrown if this method is
     *             called for an Node already inserted into
     *             AtomSpace. Otherwise, internal index structures
     *              would become inconsistent.
     */
    void  setName(const std::string&) throw (RuntimeException);

    /**
     * Returns a string representation of the node.
     *
     * @return A string representation of the node.
     */
    std::string toString() const;
    std::string toShortString() const;

    /**
     * Returns whether a given atom is equal to the current node.
     * @param Atom to be tested.
     * @return true if they are equal, false otherwise.
     */
    virtual bool operator==(const Atom&) const;

    /**
     * Returns whether a given atom is different from the current node.
     * @param Atom to be tested.
     * @return true if they are different, false otherwise.
     */
    virtual bool operator!=(const Atom&) const;

    /**
     * Returns the hashCode of the node.
     * @return a integer value as the hashCode of the node.
     */
    virtual size_t hashCode(void) const;
};

} // namespace opencog

#endif // _OPENCOG_NODE_H
