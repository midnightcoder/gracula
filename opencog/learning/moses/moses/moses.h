/*
 * opencog/learning/moses/moses/moses.h
 *
 * Copyright (C) 2002-2008 Novamente LLC
 * All Rights Reserved
 *
 * Written by Moshe Looks
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
#ifndef _MOSES_MOSES_H
#define _MOSES_MOSES_H

#include "metapopulation.h"

namespace moses
{

typedef std::set<combo::vertex> operator_set;
typedef std::set<combo::combo_tree, opencog::size_tree_order<combo::vertex> >
combo_tree_ns_set;

static const operator_set empty_ignore_ops = operator_set();

/**
 * parameters to decide how to run moses
 */
struct moses_parameters {
    moses_parameters(int _max_evals = 10000,
                     int _max_gens = -1,
                     score_t _max_score = 0,
                     const operator_set _ignore_ops = empty_ignore_ops,
                     const combo_tree_ns_set* _perceptions = NULL,
                     const combo_tree_ns_set* _actions = NULL) 
        : max_evals(_max_evals), max_gens(_max_gens), max_score(_max_score),
          ignore_ops(_ignore_ops), perceptions(_perceptions),
          actions(_actions) {}
    // total maximun number of evals
    int max_evals;
    // the max number of demes to create and optimize, if negative,
    // then no limit
    int max_gens;
    // the max score
    score_t max_score;
    // the set of operators to ignore
    const operator_set& ignore_ops;
    // the set of perceptions of an optional interactive agent
    const combo_tree_ns_set* perceptions;
    // the set of actions of an optional interactive agent
    const combo_tree_ns_set* actions;
};

/**
 * the main function of MOSES
 *
 * @param mp the metapopulation 
 * @param pa the parameters to run moses
 */
template<typename Scoring, typename BScoring, typename Optimization>
void moses(metapopulation<Scoring, BScoring, Optimization>& mp,
           const moses_parameters& pa = moses_parameters())
{
    // Logger
    logger().info("MOSES starts");
    // ~Logger
    int gen_idx = 0;

    while ((mp.n_evals() < pa.max_evals) && (pa.max_gens != gen_idx++)) {
        // Logger
        logger().info("Deme expansion: %i", gen_idx);
        // ~Logger

        //run a generation
        if (mp.expand(pa.max_evals - mp.n_evals(), pa.ignore_ops,
                      pa.perceptions, pa.actions)) {
        } else // In iterative hillclimbing it is possible (but not
               // likely) that the metapop gets empty and expand
               // return false
            break;
        if (mp.best_score() >= pa.max_score || mp.empty())
            break;
    }    
    // Logger
    logger().info("MOSES ends");
    // ~Logger
}


/**
 * @brief The sliced version of moses
 *
 * It is only used for testing
 * 
 * @todo should be removed once slice and non-slice MOSES are totally
 * factorized
 *
 * Lists of relevant operators, perceptions, and actions may or may not be
 * provided. The initial design assumed fixed lists, this version
 * has a constructor including these parameters, specific for actions
 * 
 * @param mp the metapopulation
 * @param pa the parameters to run moses
 */
template<typename Scoring, typename Domination, typename Optimization>
void moses_sliced(metapopulation<Scoring, Domination, Optimization>& mp,
                  const moses_parameters& pa)
{
    int o;
    int max_for_slice = 20;

    while (mp.n_evals() < pa.max_evals) {
        o = 0;
        if (mp.create_deme(pa.ignore_ops, pa.perceptions, pa.actions)) {
            while (o >= 0)
                o = mp.optimize_deme(pa.max_evals, max_for_slice, pa.max_score);

            mp.close_deme();

        } else
            break;
    }

    // print the best solution
    std::cout << "sampled " << mp.n_evals()
              << " best " << mp.best_score() << endl
              << mp.best_tree() << std::endl;
}


} //~namespace moses

#endif
