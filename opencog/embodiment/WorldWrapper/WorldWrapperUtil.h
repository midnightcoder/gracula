/*
 * opencog/embodiment/WorldWrapper/WorldWrapperUtil.h
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Nil Geisweiller
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

#ifndef _WORLDWRAPPERUTIL_H
#define _WORLDWRAPPERUTIL_H

#include <opencog/util/exceptions.h>

#include <opencog/comboreduct/combo/vertex.h>
#include <opencog/comboreduct/combo/variable_unifier.h>
#include <opencog/comboreduct/combo/definite_object.h>

#include <opencog/atomspace/AtomSpace.h>

#include <opencog/embodiment/Control/EmbodimentConfig.h>
#include <opencog/embodiment/PetComboVocabulary/PetComboVocabulary.h>
#include <opencog/embodiment/Control/MessagingSystem/NetworkElement.h>

#include "WorldWrapperUtilCache.h"

#include <exception>

namespace WorldWrapper
{

/**
 * Class containing a set of methods to access perceptions or objects
 * into the atomSpace or spaceMap according to vertex representation
 */

using namespace opencog;

class WorldWrapperUtil
{

private:

    // inform if cache data should be used when evaluationg predicates
    static bool useCache;

    // cache predicates information between timestamps.
    static WorldWrapperUtilCache cache;

    // return a list of definite objects for the given vertex. In general,
    // this vertex would be a definite_object, an indefinite_object or a
    // wild_card (_*_). Definite_object and indefinite_objects will produce
    // a one-element vector while wild_card will result in a multi-valued
    // vector.
    static std::vector<combo::definite_object>
    getDefiniteObjects(opencog::RandGen& rng,
                       Handle smh,
                       unsigned long time,
                       AtomSpace& atomSpace,
                       const std::string& self_id,
                       const std::string& owner_id,
                       combo::vertex& v,
                       bool isInThePast = false,
                       combo::variable_unifier& vu = combo::variable_unifier::DEFAULT_VU());

    /**
     * Change precondition predicate name before evaluating it via rec_lookup function.
     * Such step is necessary because the predicate name that needs to be searched
     * isn't the same as the preception. Althought this creates an extra work,
     * the user usage gets simpler. is_aggressive(self) is more meaningful
     * like than aggressiveness(self), for example.
     *
     * @param tr Tree that needs to be changed
     * @param name The new predicate name
     */
    static void changePredicateName(combo::combo_tree& tr, const std::string& name);

    /**
     * Evaluate the logical operation
     *
     * @param operation The operation to be evaluated
     * @param valueA The first value
     * @param valueB The second value
     * @return The operation result (true or false).
     */
    static bool evaluateLogicalOperation(int operation,
                                         double valueA,
                                         double valueB);

    /**
     * Return the value for the given physiological feeling stored within the
     * atomSpace. Feelings not present in atomSpace will return -1.0.
     *
     * @param as The atomSpace where to look for the feeling predicate.
     * @param name The feeling predicate name. Use the ones defined in
     *             PredefinedProcedureNames.h in AtomSpace folder.
     * @param target The object whose feeling is to be retrieved.
     * @param time The current timestamp (used for cache searches).`
     */
    static float getPhysiologicalFeeling(const AtomSpace& as,
                                         const std::string& name,
                                         const std::string& target,
                                         unsigned long time);

    /**
     * Return the value for the a emotional feeling or trait stored within
     * the atomSpace. The feeling or trait whose value is to be retrieved is
     * presented in the combo tree representation.
     *
     * @param rng          random number generator
     * @param smh          a SpaceMap Handle
     * @param time         world timestamp at which the evaluation occurs
     * @param atomSpace    the AtomSpace
     * @param self_id      the agent's id
     * @param owner_id     the agent's owner id
     * @param it           iterator to the combo script tree
     * @param isInThePast  specifies whether the predicate is to be evaluated
     *                     using past state or current state
     * @param vu      variable_unifier object used to resolve wild_card symbol _*_
     */

    static float getEmotionalFeelingOrTrait(opencog::RandGen& rng,
                                            Handle smh,
                                            unsigned long time,
                                            AtomSpace& atomSpace,
                                            const std::string& self_id,
                                            const std::string& owner_id,
                                            const combo::combo_tree::iterator it,
                                            bool isInThePast = false,
                                            combo::variable_unifier& vu = combo::variable_unifier::DEFAULT_VU());

public:
    //currently combo percepts are boolean, and nm percepts may continuous
    //truth values - thresholding is used for conversion to bool
    static const float meanTruthThreshold = 0.5;

    static unsigned long getHasSaidDelay() {
        return opencog::config().get_int("HAS_SAID_DELAY");
    }

    /**
     * returns the definite_object corresponding to an atom name
     * in case of ambiguities between self_id and owner_id,
     * self_id is considered first
     */
    static combo::definite_object atom_name_to_definite_object(const std::string& atom_name, const std::string& self_id, const std::string& owner_id);

    /**
     * returns the atom name corresponding to a definite object
     * in case of ambiguities between self_id and owner_id,
     * self_id is considered first
     */
    static std::string definite_object_to_atom_name(const std::string& atom_name,
            const std::string& self_id,
            const std::string& owner_id);

    /**
     * test whether 2 definite objects are equal
     * id corresponding to owner and self
     * are compare to their keyword as well
     */
    static bool definite_object_equal(const combo::definite_object& d1,
                                      const combo::definite_object& d2,
                                      const std::string& self_id,
                                      const std::string& owner_id);
    /**
     * retrieve the Handle in the AtomSpace of a corresponding definite object
     * food_bowl, water_bowl and pet_home, self and owner are comprehended
     */
    static Handle toHandle(const AtomSpace& as, combo::definite_object obj,
                           const std::string& self_id, const std::string& owner_id);

    /**
     * atom-table querying for inheritance links of nullary predicates
     * (e.g. food_bowl)
     */
    static std::string lookupInheritanceLink(opencog::RandGen& rng,
            const AtomSpace& as, Handle);

    /**
     * atom-table querying for execution links of nullary predicates
     * (e.g. pet_bowl)
     */
    static std::string lookupExecLink(opencog::RandGen& rng,
                                      const AtomSpace& as, Handle);

    /**
     * convenience function - used to create predicates for filtering
     * (cf findNearestFiltered in LocalSpaceMap2D)
     */
    static combo::combo_tree::iterator maketree(std::string str, std::string h);

    /**
     * like above but uses the vertex directly instead of its string
     * representation
     */
    static combo::combo_tree::iterator maketree_vertex(const combo::vertex& v,
            std::string h);

    /**
     * like above but with perception instead of vertex
     */
    static combo::combo_tree::iterator maketree_percept(combo::perception p,
            std::string h);

    /**
     * atom table lookup of a nested link-and-node struct represented
     * as a subtree
     */
    static Handle rec_lookup(const AtomSpace& as, combo::combo_tree::iterator it,
                             const std::string& self_id, const std::string& owner_id);

    /**
     * returns the handle representing pet or avatar of self_id
     * WARNING : if there exists a pet and an avatar with the same id
     * then the handle of the pet is returned
     */
    static Handle selfHandle(const AtomSpace& as, const std::string& self_id);

    /**
     * returns the handle representing owner of owner_id
     */
    static Handle ownerHandle(const AtomSpace& as, const std::string& owner_id);

    /**
     * returns the handle representing the avatar with of avatar_id
     */
    static Handle getAvatarHandle(const AtomSpace& as, const std::string& avatar_id);

    /**
     * check localspacemap for a Handle
     */
    static bool inSpaceMap(const SpaceServer::SpaceMap& sm,
                           const AtomSpace& as,
                           const std::string& self_id,
                           const std::string& owner_id,
                           combo::vertex v);

    /**
     * evalIndefiniteObject
     * eval indefinite object with a given spaceMap and a given AtomSpace
     * WARNING :It does not return the special definite_object self and owner
     * instead it returns their corresponding atom name
     *
     * @param smh          A spaceMap Handle
     * @param time         world timestamp at the evaluation
     * @param isInThePast  specifies whether the predicate is to be evaluated
     *                     using past state or current state
     * @param vu      variable_unifier object used to resolve wild_card symbol _*_
     */
    static combo::vertex evalIndefiniteObject(opencog::RandGen& rng,
            Handle smh,
            unsigned long time,
            AtomSpace& atomSpace,
            const std::string& self_id,
            const std::string& owner_id,
            combo::indefinite_object io,
            bool isInThePast = false,
            combo::variable_unifier& vu = combo::variable_unifier::DEFAULT_VU())
    throw (opencog::ComboException, opencog::AssertionException, std::bad_exception);

    //like above but uses directly pet_indefinite_object_enum
    static combo::vertex evalIndefiniteObject(opencog::RandGen& rng,
            Handle smh,
            unsigned long time,
            AtomSpace& atomSpace,
            const std::string& self_id,
            const std::string& owner_id,
            combo::pet_indefinite_object_enum ioe,
            bool isInThePast = false,
            combo::variable_unifier& vu = combo::variable_unifier::DEFAULT_VU())
    throw (opencog::ComboException, opencog::AssertionException, std::bad_exception);

    /**
     * evalPerception
     * eval perception with a given spaceMap and a given AtomSpace
     * @param smh          a SpaceMap Handle
     * @param time         world timestamp at which the evaluation occurs
     * @param isInThePast  specifies whether the predicate is to be evaluated
     *                     using past state or current state
     * @param vu      variable_unifier object used to resolve wild_card symbol _*_
     */
    static combo::vertex evalPerception(opencog::RandGen& rng,
                                        Handle smh,
                                        unsigned long time,
                                        AtomSpace& atomSpace,
                                        const std::string& self_id,
                                        const std::string& owner_id,
                                        const combo::combo_tree::iterator it,
                                        bool isInThePast = false,
                                        combo::variable_unifier& vu = combo::variable_unifier::DEFAULT_VU());

    /**
     * Inform if the cache usage is enabled or not.
     */
    static bool cacheEnabled();

    /**
     * Set the use or not of cache when evaluating predicates
     */
    static void setCacheUse(bool state);

    /**
     * lookup location in the current localspacemap
     */
    static SpaceServer::SpaceMapPoint getLocation(const SpaceServer::SpaceMap& sm,
            const AtomSpace& as,
            Handle h);

    static SpaceServer::SpaceMapPoint getLocation(const SpaceServer::SpaceMap& sm,
            const AtomSpace& as,
            const std::string& handleName)
    throw (opencog::InvalidParamException, opencog::AssertionException, std::bad_exception);

    static double getOrientation(const SpaceServer::SpaceMap& sm,
                                 const AtomSpace& as,
                                 Handle h)
    throw (opencog::InvalidParamException,
           opencog::AssertionException,
           std::bad_exception);

    //get a indefinite object of type nearest_X or random_X and return
    //the associated perception is_X
    //for instance nearest_moving returns is_moving
    static combo::perception nearest_random_X_to_is_X(combo::pet_indefinite_object_enum ioe);

    //like above but uses indefinite_object instead
    static combo::perception nearest_random_X_to_is_X(combo::indefinite_object io);

    static bool is_builtin_compound_action(const combo::vertex& v);
    static bool is_builtin_atomic_action(const combo::vertex& v);
};
}

#endif
