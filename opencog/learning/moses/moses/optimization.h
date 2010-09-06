/*
 * opencog/learning/moses/moses/optimization.h
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
#ifndef _MOSES_OPTIMIZATION_H
#define _MOSES_OPTIMIZATION_H

#include <opencog/util/selection.h>
#include <opencog/util/dorepeat.h>
#include <opencog/util/exceptions.h>
#include <opencog/util/oc_assert.h>

#include "../eda/termination.h"
#include "../eda/replacement.h"
#include "../eda/logging.h"
#include "../eda/local_structure.h"
#include "../eda/optimize.h"
#include "moses.h"
#include "neighborhood_sampling.h"

#define MAX_DISTANCE_FROM_EXEMPLAR 5
#define FRACTION_OF_REMAINING     10
#define MINIMUM_DEME_SIZE         50
#define MAX_EVALS_PER_SLICE       10
#define TEMP_STEPSIZE             0.5

namespace moses
{

inline double information_theoretic_bits(const eda::field_set& fs)
{
    double res = 0;
    foreach(const eda::field_set::disc_spec& d, fs.disc_and_bits())
        res += log2(double(d.arity));
    foreach(const eda::field_set::contin_spec& c, fs.contin()) {
        // number of possible contins with depth d is 2^(d+1)-1 because
        // after a Stop only Stop is allowed which is why it is not 3^d
        unsigned int contin_count = (1 << (c.depth + 1)) - 1;
        res += log2(double(contin_count));
    }
    foreach(const eda::field_set::onto_spec& o, fs.onto())
        res += log2(double(o.branching)) * double(o.depth);
    return res;
}

struct eda_parameters {
    eda_parameters(double _pop_size_ratio = 200) :
        // optimization is teriminated after term_total*n generations,
        // or term_improv*sqrt(n/w) consecutive generations with no
        // improvement (w=windowsize)
        term_total(1),
        term_improv(1),

//was 20...
        pop_size_ratio(_pop_size_ratio), //populations are sized at
                                         //N = popsize_ratio*n^1.05
                                         //where n is problem size in info-t bits
        
        window_size_pop(0.05), //window size for RTR is
        window_size_len(1),    //min(windowsize_pop*N,windowsize_len*n)
        
        selection(2),          //if <=1, truncation selection ratio,
        //if >1, tournament selection size (should be int)
        selection_ratio(1),    //ratio of population size selected for modeling
        
        replacement_ratio(0.5),//ratio of population size sampled and integrated
        
        
        model_complexity(1),   //model parsimony term log(N)*model_complexity
        
        terminate_if_gte(0)    //optimization is terminated if best score is >= this
    { }

    bool is_tournament_selection() {
        return selection > 1;
    }
    bool is_truncation_selection() {
        return selection <= 1;
    }


    //N=p.popsize_ratio*n^1.05
    inline int pop_size(const eda::field_set& fs) {
        return int(ceil((double(pop_size_ratio)*
                         pow(information_theoretic_bits(fs), 1.05))));
    }

    //min(windowsize_pop*N,windowsize_len*n)
    inline int rtr_window_size(const eda::field_set& fs) {
        return int(ceil(min(window_size_pop*pop_size(fs),
                            window_size_len*information_theoretic_bits(fs))));
    }

    //term_total*n
    inline int max_gens_total(const eda::field_set& fs) {
        return int(ceil(term_total*information_theoretic_bits(fs)));
    }
    //term_improv*sqrt(n/w)
    inline int max_gens_improv(const eda::field_set& fs) {
        return int(ceil(term_improv*
                        sqrt(information_theoretic_bits(fs) /
                             rtr_window_size(fs))));
    }

    double term_total;
    double term_improv;

    double pop_size_ratio;
    double window_size_pop;
    double window_size_len;
    double selection;
    double selection_ratio;
    double replacement_ratio;
    double model_complexity;
    double terminate_if_gte;
};

struct univariate_optimization {
    univariate_optimization(opencog::RandGen& _rng,
                            const eda_parameters& p = eda_parameters())
        : rng(_rng), params(p) {}

    //return # of evaluations actually performed
    template<typename Scoring>
    int operator()(eda::instance_set<composite_score>& deme,
                   const Scoring& score, int max_evals) {
        int pop_size = params.pop_size(deme.fields());
        int max_gens_total = params.max_gens_total(deme.fields());
        int max_gens_improv = params.max_gens_improv(deme.fields());
        int n_select = int(double(pop_size) * params.selection_ratio);
        int n_generate = int(double(pop_size * params.replacement_ratio));

        //adjust parameters based on the maximal # of evaluations allowed
        if (max_evals < pop_size) {
            pop_size = max_evals;
            max_gens_total = 0;
        } else {
            max_gens_total = min(max_gens_total,
                                 (max_evals - pop_size) / n_generate);
        }

        //create the initial sample
        //generate the initial sample to populate the deme
        deme.resize(pop_size);
        generate_initial_sample(deme.fields(), pop_size, deme.begin(),
                                deme.end(), rng);

        if (params.is_tournament_selection()) {
            eda::cout_log_best_and_gen logger;
            return eda::optimize
                   (deme, n_select, n_generate, max_gens_total, score,
                    eda::terminate_if_gte_or_no_improv<composite_score>
                    (composite_score(params.terminate_if_gte,
                                      worst_possible_score.second),
                     max_gens_improv),
                    opencog::tournament_selection(int(params.selection), rng),
                    eda::univariate(), eda::local_structure_probs_learning(),
                    eda::rtr_replacement(deme.fields(),
                                         params.rtr_window_size(deme.fields()),
                                         rng),
                    logger, rng);
        } else { //truncation selection
            OC_ASSERT( false,
                       "Trunction selection not implemented."
                       " Tournament should be used instead.");
            return 42;
            /*
            return optimize(deme,n_select,n_generate,args.max_gens,score,
              terminate_if_gte_or_no_improv(params.terminate_if_gte,
                       max_gens_improv),
              //truncation selection goes here
              univariate(),local_structure_probs_learning(),
              replace_the_worst(),cout_log_best_and_gen());
            */
        }
    }

    opencog::RandGen& rng;
    eda_parameters params;
};

struct iterative_hillclimbing {
    iterative_hillclimbing(opencog::RandGen& _rng,
                           const eda_parameters& p = eda_parameters())
        : rng(_rng), params(p) {}

    //return # of evaluations actually performed
    template<typename Scoring>
    int operator()(eda::instance_set<composite_score>& deme,
                   const Scoring& score, int max_evals) {
        const eda::field_set& fields = deme.fields();
        int pop_size = params.pop_size(fields);
        int max_gens_total = params.max_gens_total(fields);

        long long current_number_of_instances = 0;

        //adjust parameters based on the maximal # of evaluations allowed
        int max_number_of_instances = max_gens_total * pop_size;
        if (max_number_of_instances > max_evals)
            max_number_of_instances = max_evals;

        int number_of_fields = fields.n_bits() + fields.n_disc()
            + fields.n_contin() + fields.n_onto();

        // it is assumed that the exemplar instance is null
        eda::instance exemplar(fields.packed_width());

        // score the exemplar, note that this could be avoided as the
        // exemplar has already been scored, but that means the
        // optimization API has to be upgraded, and obviously scoring
        // one candidate doesn't cost much compared to the entire
        // optimization
        eda::scored_instance<composite_score> scored_exemplar = 
            eda::score_instance(exemplar, score);
        score_t exemplar_score = score(scored_exemplar).first;

        int distance = 1;
        bool bImprovement_made = false;
        score_t best_score;
        do {
            // the number of all neighbours at the distance d
            long long total_number_of_neighbours = count_n_changed_knobs(deme.fields(), distance);

            long long number_of_new_instances;

            number_of_new_instances = 
                (max_number_of_instances - current_number_of_instances)
                / FRACTION_OF_REMAINING;

            if (number_of_new_instances  < MINIMUM_DEME_SIZE)
                number_of_new_instances =
                    (max_number_of_instances - current_number_of_instances);

            if (number_of_new_instances < total_number_of_neighbours) {
                //resize the deme so it can take new instances
                deme.resize(current_number_of_instances + number_of_new_instances);
                //sample 'number_of_new_instances' instances on the
                //distance 'distance' from the exemplar
                sample_from_neighborhood(deme.fields(), distance,
                                         number_of_new_instances,
                                         deme.begin() + current_number_of_instances, deme.end(), rng);
            } else {
                number_of_new_instances = total_number_of_neighbours;
                //resize the deme so it can take new instances
                deme.resize(current_number_of_instances + number_of_new_instances);
                //add all instances on the distance 'distance' from
                //the exemplar
                generate_all_in_neighborhood(deme.fields(), distance,
                                             deme.begin() + current_number_of_instances, deme.end());
            }
            
            // score all new instances in the deme
            transform(deme.begin() + current_number_of_instances, deme.end(),
                      deme.begin_scores() + current_number_of_instances, score);

            best_score = exemplar_score;
            // check if there is an instance in the deme better than the exemplar
            //foreach(const eda::scored_instance<composite_score>& inst,deme) {
            for (int i = current_number_of_instances;
                 deme.begin() + i != deme.end(); i++) {

                //score(deme.begin()+i,deme.begin_scores()+i);
                //transform(deme.begin()+current_number_of_instances,deme.end(),deme.begin_scores()+current_number_of_instances,score);

                const eda::scored_instance<composite_score>& inst = deme[i];

                if (get_score(inst.second) > best_score) {
                    best_score = get_score(inst.second);
                    bImprovement_made = true;

                    // cout << endl << "Improvement, new best score:" << inst.second << "(distance: " << distance << ", old score: " << exemplar_score << ")" << endl;
                    // cout << "Found instance:" << deme.fields().stream(inst) << endl;
                    // cout << "Score:" << get_score(inst.second) << endl;
                    // cout << "--------------------------------------" << endl;
                    //  break;
                }
            }

            current_number_of_instances += number_of_new_instances;
            distance++;

        } while (!bImprovement_made &&
                 distance <= number_of_fields &&
                 distance <= MAX_DISTANCE_FROM_EXEMPLAR &&
                 current_number_of_instances < max_number_of_instances &&
                 best_score < 0.0 // @todo: should be using some max_score var
                 );
        
        return current_number_of_instances;
    }

    opencog::RandGen& rng;
    eda_parameters params;
};

struct sliced_iterative_hillclimbing {

    typedef enum {
        M_INIT,
        M_BUILD_CANDIDATES,
        M_EVALUATE_CANDIDATES
    } MState;


    sliced_iterative_hillclimbing(opencog::RandGen& _rng,
                                  const eda_parameters& p = eda_parameters())
        : rng(_rng), params(p), m_state(M_INIT),
          _evals_per_slice(MAX_EVALS_PER_SLICE) {}

    void set_evals_per_slice(int evals_per_slice) {
        _evals_per_slice = evals_per_slice;
    }

    //return # of evaluations actually performed
    //WARNING: if an improvement has been made it returns
    // -number_of_eval
    //it is rather ugly and it is a temporary hack till the API of MOSES
    //is better
    template<typename Scoring>
    int operator()(eda::instance_set<composite_score>& deme,
                   const Scoring& score, int max_evals) {
        switch (m_state) {

        case M_INIT:  {
            current_number_of_instances = 0;

            cout << "bits size: " << deme.fields().n_bits() << endl;
            cout << "disc size: " << deme.fields().n_disc() << endl;
            cout << "max evals : " << max_evals << endl << endl;

            max_number_of_instances = max_evals;

            number_of_fields = deme.fields().n_bits() + deme.fields().n_disc();
            exemplar = eda::instance(deme.fields().packed_width());

            // set to 0 all fields (contin and onto fields are ignored) to represent the exemplar
            for (eda::field_set::bit_iterator it = deme.fields().begin_bits(exemplar);
                    it != deme.fields().end_bits(exemplar); ++it)
                *it = false;
            for (eda::field_set::disc_iterator it = deme.fields().begin_disc(exemplar);
                    it != deme.fields().end_disc(exemplar); ++it)
                *it = 0;

            eda::scored_instance<composite_score> scored_exemplar = exemplar;
            exemplar_score = score(scored_exemplar).first;
            // cout << "Exemplar:" <<deme.fields().stream(scored_exemplar) << " Score:" << exemplar_score << endl;

            distance = 1;

            // more precisely, instead of 0 there should be max_score, but this value is not passed as an argument
            if (exemplar_score == 0) {
                // found a perfect solution
                deme.resize(1);
                *(deme.begin()++) = exemplar;
                m_state = M_INIT;
            } else
                m_state = M_BUILD_CANDIDATES;

            break;
        }
        // --------------------------------------------------------------------------

        case M_BUILD_CANDIDATES:  {

            if (current_number_of_instances >= max_number_of_instances) {
                m_state = M_INIT;
                return EVALUATED_ALL_AVAILABLE; // This is ugly, see the note in moses.h
            }

            if (distance > MAX_DISTANCE_FROM_EXEMPLAR || distance > number_of_fields) {
                m_state = M_INIT;
                return EVALUATED_ALL_AVAILABLE; // This is ugly, see the note in moses.h
            }

            cout << "Distance in this iteration:" << distance << endl;

            // the number of all neighbours at the distance d
            long long total_number_of_neighbours = count_n_changed_knobs(deme.fields(), distance);
            cout << "Number of possible instances:" << total_number_of_neighbours << endl;

            if (total_number_of_neighbours == 0) {
                m_state = M_INIT;
                return EVALUATED_ALL_AVAILABLE; // This is ugly, see the note in moses.h
            }

            number_of_new_instances = (max_number_of_instances - current_number_of_instances) / FRACTION_OF_REMAINING;
            if (number_of_new_instances  < MINIMUM_DEME_SIZE)
                number_of_new_instances = (max_number_of_instances - current_number_of_instances);

            if (number_of_new_instances < total_number_of_neighbours) {
                //resize the deme so it can take new instances
                deme.resize(current_number_of_instances + number_of_new_instances);
                //sample 'number_of_new_instances' instances on the distance 'distance' from the exemplar
                sample_from_neighborhood(deme.fields(), distance, number_of_new_instances, deme.begin() + current_number_of_instances, deme.end(), rng);
            } else {
                number_of_new_instances = total_number_of_neighbours;
                //resize the deme so it can take new instances
                deme.resize(current_number_of_instances + number_of_new_instances);
                //add all instances on the distance 'distance' from the exemplar
                generate_all_in_neighborhood(deme.fields(), distance, deme.begin() + current_number_of_instances, deme.end());
            }

            target_size = current_number_of_instances + number_of_new_instances;
            cout << "New target size:" << target_size << endl;

            m_state = M_EVALUATE_CANDIDATES;

            break;
        }
        // --------------------------------------------------------------------------

        case M_EVALUATE_CANDIDATES: {

            bool bImprovement_made = false;
            int n = 0; // number of evaluations in this chunk

            score_t best_score = exemplar_score;

            // cout << " _evals_per_slice " << _evals_per_slice << endl;

            // check if there is an instance in the deme better than the exemplar
            for (int i = 0; deme.begin() + current_number_of_instances != deme.end() &&
                    i < _evals_per_slice && !bImprovement_made; i++) {

                *(deme.begin_scores() + current_number_of_instances) = score(*(deme.begin() + current_number_of_instances));

                const eda::scored_instance<composite_score>& inst = deme[current_number_of_instances];

                if (get_score(inst.second) > best_score) {

                    best_score = get_score(inst.second);
                    bImprovement_made = true;

                    cout << endl << "Improvement, new best score:" << endl;
                    cout << inst.second << "(distance: " << distance << ", old score: " << exemplar_score << ")" << endl;
                    cout << "Found instance:" << deme.fields().stream(inst) << endl;
                    cout << "Score:" << get_score(inst.second) << endl;
                    cout << "--------------------------------------" << endl;
                }

                current_number_of_instances++;
                n++;
            }

            if (bImprovement_made) {
                m_state = M_INIT;
                return -n;
            } else if (deme.begin() + current_number_of_instances < deme.end())  {
                m_state = M_EVALUATE_CANDIDATES;
            } else {
                m_state = M_BUILD_CANDIDATES;
                distance++;
            }

            return n;
            break;
        }
        // --------------------------------------------------------------------------

        default:
            break;
        }

        return 0;
    }

    int distance;
    long long number_of_new_instances;
    int number_of_fields;
    eda::instance exemplar;
    int max_number_of_instances;
    score_t exemplar_score;
    int current_number_of_instances;
    int target_size;

    opencog::RandGen& rng;
    eda_parameters params;
    MState m_state;
    int _evals_per_slice;
};

struct simulated_annealing {
     
    typedef score_t energy_t;
 
    simulated_annealing(opencog::RandGen& _rng,
                        const eda_parameters& p = eda_parameters(),
                        double _init_temp = 30,
                        double _min_temp = 0, double _temp_step_size = 0.5,
                        double _accept_prob_temp_intensity = 0.5,
                        double _dist_temp_intensity = 0.5,
                        double _fraction_of_remaining = 10.0,
                        double _min_energy = 0.0)
        : rng(_rng), 
          init_temp(_init_temp),
          min_temp(_min_temp),
          temp_step_size(_temp_step_size),
          accept_prob_temp_intensity(_accept_prob_temp_intensity),
          dist_temp_intensity(_dist_temp_intensity),
          fraction_of_remaining(_fraction_of_remaining),
          min_energy(_min_energy),
          params(p) {}
      
    double accept_probability(energy_t energy_new, energy_t energy_old,
                              double temperature)
    {
        if (energy_new < energy_old)
            return 1.0;
        else
            return std::exp((energy_old - energy_new) / temperature);
    }
      
    double cooling_schedule(double t)
    { 
        OC_ASSERT(t > 0, "t should greater than 0");
        //return (double) init_temp / std::log(1.0 + t); 
        return (double) init_temp / (1.0 + t);
    }
      
    energy_t energy(const eda::scored_instance<composite_score>& inst)
    {
        // here let the energy be the reverse of the score, that
        // because the better instance has the lower energy but higher
        // score.
        //
        // NOTICE: it may use some other methods to present the
        // energy.
        return -get_score(inst.second);
    }
    
    /**
     * This method calculate the distance of the jump according to the
     * temperature. The higher the temperature the higher the
     * distance.  @todo: it may be better to have the distance
     * decreasing exponentially instead of linearly
     */
    unsigned int dist_temp(double current_temp)
    {
        return (unsigned int)( ((current_temp - min_temp)/(init_temp - min_temp))
                               *
                               (max_distance - 1) + 1 );
    }

    template<typename Scoring>
    int operator()(eda::instance_set<composite_score>& deme,
                   const Scoring& score, int max_evals) {

        const eda::field_set& fields = deme.fields();
        max_distance = fields.raw_size();

        // @todo this should be adapted for SA
        int pop_size = params.pop_size(fields);
        // int max_gens_total = params.max_gens_total(deme.fields());
        // int max_number_of_instances = max_gens_total * pop_size;
        int max_number_of_instances = pop_size;
        if (max_number_of_instances > max_evals)
            max_number_of_instances = max_evals;

        long long current_number_of_instances = 0;

        int step = 1;

        // it is assumed that the exemplar instance is null
        eda::instance center_instance(fields.packed_width());

        // score the exemplar, note that this could be avoided as the
        // exemplar has already been scored, but that means the
        // optimization API has to be upgraded, and obviously scoring
        // one candidate doesn't cost much compared to the entire
        // optimization
        eda::scored_instance<composite_score> scored_exemplar = 
            eda::score_instance(center_instance, score);
        energy_t center_instance_energy = energy(scored_exemplar);
        double current_temp = init_temp;
        do {
            unsigned int current_distance = 
                max(1, (int)(dist_temp_intensity * dist_temp(current_temp)));

            // score all new instances in the deme
            long long number_of_new_instances = 1; //@todo: possibly change that

            // sample one neighbour (for now) of the center_instance
            // from distance.
            // @todo: one may experiment with changing the distance of
            // the contin as well.
            deme.resize(current_number_of_instances + number_of_new_instances);
            sample_from_neighborhood(fields, current_distance,
                                     number_of_new_instances,
                                     deme.begin() + current_number_of_instances,
                                     deme.end(),
                                     rng, center_instance);
                    
            // score all new instances in the deme (1 for now)
            transform(deme.begin() + current_number_of_instances, deme.end(),
                      deme.begin_scores() + current_number_of_instances,
                      score);
                      
            eda::scored_instance<composite_score>& current_scored_instance =
                deme[current_number_of_instances];
            eda::instance& current_instance = current_scored_instance.first;
            energy_t current_instance_energy = energy(current_scored_instance);
                                          
            // check if the current instance in the deme is better than
            // the center_instance                                        
            double actual_accept_prob = accept_prob_temp_intensity *
                accept_probability(current_instance_energy,
                                   center_instance_energy, current_temp);
                      
            if (actual_accept_prob >= rng.randdouble()) {
                center_instance_energy = current_instance_energy;
                center_instance = current_instance;
            }

            current_number_of_instances += number_of_new_instances;

            // DEBUG
            // cout <<"Instance: " << fields.stream(center_instance) <<endl;
            // cout <<"Energy: " << center_instance_energy << endl;
            // cout <<"-----------------------------------------------" <<endl;
            // ~DEBUG

            current_temp = cooling_schedule((double)step * temp_step_size);
            step++;
        } while(current_number_of_instances < max_number_of_instances &&
                current_temp >= min_temp &&
                center_instance_energy > min_energy);
        
        return current_number_of_instances;
    }
      
    opencog::RandGen& rng;
    double init_temp;
    double min_temp;
    unsigned int max_distance;
    double temp_step_size;
    double accept_prob_temp_intensity;
    double dist_temp_intensity;
    double fraction_of_remaining;
    double min_energy;
    eda_parameters params;
};

} //~namespace moses
  
#endif
