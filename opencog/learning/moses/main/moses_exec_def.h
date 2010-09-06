/** moses_exec_def.h --- 
 *
 * Copyright (C) 2010 Novamente LLC
 *
 * Author: Nil Geisweiller
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


#ifndef _OPENCOG_MOSES_EXEC_DEF_H
#define _OPENCOG_MOSES_EXEC_DEF_H

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>

#include <opencog/util/iostreamContainer.h>

// a map btw host name and number of jobs allocated
typedef std::map<string, unsigned int> jobs_t;

// number of evals string
static const string number_of_evals_str = "#evals";

// program option names and abbreviations
// for their meanings see options_description in moses-exec.cc
static const pair<string, string> rand_seed_opt("random-seed", "r");
static const pair<string, string> input_table_file_opt("input-file", "i");
static const pair<string, string> problem_opt("problem", "H");
static const pair<string, string> combo_str_opt("combo-program", "y");
static const pair<string, string> problem_size_opt("problem-size", "k");
static const pair<string, string> nsamples_opt("nsamples", "b");
static const pair<string, string> min_rand_input_opt("min-rand-input", "q");
static const pair<string, string> max_rand_input_opt("max-rand-input", "w");
static const pair<string, string> max_evals_opt("max-evals", "m");
static const pair<string, string> result_count_opt("result-count", "c");
static const pair<string, string> output_bscore_opt("output-bscore", "t");
static const pair<string, string> output_complexity_opt("output-complexity", "x");
static const pair<string, string> output_eval_number_opt("output-eval-number", "V");
static const pair<string, string> max_gens_opt("max-gens", "g");
static const pair<string, string> log_level_opt("log-level", "l");
static const pair<string, string> log_file_opt("log-file", "f");
static const string default_log_file_prefix = "moses";
static const string default_log_file_suffix = "log";
static const string default_log_file = default_log_file_prefix + "." + default_log_file_suffix;
static const pair<string, string> log_file_dep_opt_opt("log-file-dep-opt", "L");
static const pair<string, string> variance_opt("variance", "v");
static const pair<string, string> prob_opt("probability", "p");
static const pair<string, string> ignore_ops_str_opt("ignore-operator", "n");
static const pair<string, string> opt_algo_opt("opt-algo", "a");
static const pair<string, string> exemplars_str_opt("exemplar", "e");
static const pair<string, string> reduct_candidate_effort_opt("reduct-candidate-effort", "E");
static const pair<string, string> reduct_knob_building_effort_opt("reduct-knob-building-effort", "B");
static const pair<string, string> reduce_all_opt("reduce-all", "d");
static const pair<string, string> count_base_opt("count-based-scorer", "u");
static const pair<string, string> cache_size_opt("cache-size", "s");
static const pair<string, string> revisit_opt("revisit", "R");
static const pair<string, string> jobs_opt("jobs", "j");
static const string job_seperator(":");
static const string localhost("localhost");
static const pair<string, string> pop_size_ratio_opt("pop-size-ratio", "P");
static const pair<string, string> max_candidates_opt("max-candidates", "M");
static const pair<string, string> ignore_bscore_opt("ignore-bscore", "I");
static const pair<string, string> ignore_bscore_visited_opt("ignore-bscore-visited", "S");

string opt_desc_str(const pair<string, string>& opt) {
    return string(opt.first).append(",").append(opt.second);
}

// used to convert program option argument to string.
// @todo: ugly, it is likely something better can be done using
// boost::program_options API
template<typename T>
bool to_string(const boost::program_options::variable_value& vv, string& str)
{
    if(vv.value().type() == typeid(T)) {
        str = boost::lexical_cast<string>(vv.as<T>());
        return true;
    } else if(vv.value().type() == typeid(std::vector<T>)) {
        str = opencog::containerToStr(vv.as<std::vector<T> >(), "_");
        return true;
    }
    return false;
}
string to_string(const boost::program_options::variable_value& vv)
{
    string res;
    if(!(to_string<int>(vv, res)
         || to_string<unsigned int>(vv, res)
         || to_string<long>(vv, res)
         || to_string<unsigned long>(vv, res)
         || to_string<float>(vv, res)
         || to_string<double>(vv, res)
         || to_string<bool>(vv, res)
         || to_string<string>(vv, res)))
        std::cerr << "type not handled yet" << std::endl;
    return res;
}



#endif // _OPENCOG_MOSES_OPTIONS_NAMES_H
