/*
 * opencog/learning/moses/eda/optimize.h
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
#ifndef _EDA_OPTIMIZE_H
#define _EDA_OPTIMIZE_H

#include "instance_set.h"
#include <opencog/util/foreach.h>
#include <opencog/util/Logger.h>

namespace eda {
    
    using namespace std;
    using opencog::logger;
  
    //return # of evaluations actually performed
    template <typename ScoreT,
              typename ScoringPolicy,
              typename TerminationPolicy,
              typename SelectionPolicy,
              typename StructureLearningPolicy,
              typename ProbsLearningPolicy,
              typename ReplacementPolicy,
              typename LoggingPolicy>
    int optimize(instance_set<ScoreT>& current,
                 int n_select,int n_generate,int max_gens,
                 const ScoringPolicy& score,
                 const TerminationPolicy& termination_criterion,
                 const SelectionPolicy& select,
                 const StructureLearningPolicy& learn_structure,
                 const ProbsLearningPolicy& learn_probs,
                 const ReplacementPolicy& replace,
                 LoggingPolicy& write_log, 
                 opencog::RandGen& rng) 
    {
        // Logger
        logger().debug("Probabilistic Learning Optimization");
        // ~Logger

        typedef typename StructureLearningPolicy::model_type model_type;
        
        //compute scores of the initial instance set
        logger().debug("Evaluate the initial population (%u individuals)",
                       current.size());
        transform(current.begin(),current.end(),current.begin_scores(),score);
        
        //main loop
        int gen=0;
        for (;gen<max_gens 
                 && !termination_criterion(current.begin(), current.end());
             ++gen) {
            
            //do logging of instance_set and scores
            write_log(current.begin(),current.end(),current.fields(),gen);
            
            //select promising instances
            logger().debug("Select %d promising instances for model building",
                           n_select);
            std::vector<scored_instance<ScoreT> > promising(n_select);
            select(current.begin(),current.end(),promising.begin(),n_select);
            
            //initialize the model
            logger().debug("Build probabilistic model");
            model_type model(current.fields(),promising.begin(),
                             promising.end(), rng);
            
            //update the model
            learn_structure(current.fields(),promising.begin(),
                            promising.end(),model);
            learn_probs(current.fields(),promising.begin(),
                        promising.end(),model);
            
            //create new instances and integrate them into the current
            //instance set, replacing existing instances
            logger().debug("Sample, evaluate and replace %d new candidates"
                           " according to that model", n_generate);
            replace(begin_generator(bind(score_instance<ScoringPolicy>,
                                         bind(model),score)),
                    end_generator(bind(score_instance<ScoringPolicy>,
                                       bind(model),score),n_generate),
                    current.begin(),current.end());      
        }

        //log the final result
        write_log(current.begin(),current.end(),current.fields(),gen);
        
        //return # of evaluations actually performed
        return current.size()+gen*n_generate;
    }
    
} //~namespace eda

#endif
