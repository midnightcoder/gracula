/** moses-exec.cc --- 
 *
 * Copyright (C) 2010 Nil Geisweiller
 *
 * Author: Nil Geisweiller <ngeiswei@gmail.com>
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
#include "moses-exec.h"

/**
 * Display error message about unspecified combo tree and exit
 */
void unspecified_combo_exit() {
    std::cerr << "error: you must specify which combo tree to learn (option -y)"
              << std::endl;
    exit(1);
}

/**
 * Display error message about unsupported type and exit
 */
void unsupported_type_exit(const type_tree& tt) {
    std::cerr << "error: type " << tt << " currently not supported" << std::endl;
    exit(1);
}
void unsupported_type_exit(type_node type) {
    unsupported_type_exit(type_tree(type));
}

/**
 * Display error message about ill formed combo tree and exit
 */
void illformed_exit(const combo_tree& tr) {
    std::cerr << "error: apparently the combo tree " 
              << tr << "is not well formed" << std::endl;
    exit(1);
}

/**
 * Display error message about unsupported problem and exit
 */
void unsupported_problem_exit(const string& problem) {
    std::cerr << "error: problem " << problem 
              << " unsupported for the moment" << std::endl;
    exit(1);
}

ifstream* open_table_file(string file) {
    ifstream* in = new ifstream(file.c_str());
    if(!in->is_open()) {
        if(file.empty()) {
            std::cerr << "the input file is empty" << std::endl;
            std::cerr << "To indicate the file to open use the option"
                      << " -i or --input-file" << std::endl;
        } else {
            std::cerr << "Could not open " << file << std::endl;
        }
        exit(1);
    }
    return in;
}

/**
 * check the first element of the data file, if it is "0" or "1" then
 * it is boolean, otherwise it is contin. It is not 100% reliable of
 * course and should be improved.
 */
type_node infer_type_from_data_file(string file) {
    auto_ptr<ifstream> in(open_table_file(file));
    string str;
    *in >> str;
    if(str == "0" || str == "1")
        return id::boolean_type;
    else {
        try {
            lexical_cast<contin_t>(str);
            return id::contin_type;
        }
        catch(...) {
            return id::ill_formed_type;
        }
    }
}

/**
 * determine the initial exemplar of a given type
 */
combo_tree type_to_exemplar(type_node type) {
    switch(type) {
    case id::boolean_type: return combo_tree(id::logical_and);
    case id::contin_type: return combo_tree(id::plus);
    case id::ill_formed_type:
        std::cerr << "The data type is incorrect, perhaps it has not been"
                  << " possible to infer it from the input table." << std::endl;
        exit(1);
    default:
        unsupported_type_exit(type);
    }
    return combo_tree();
}

combo_tree ann_exemplar(arity_t arity) {
    combo_tree ann_tr(ann_type(0, id::ann));
    // ann root
    combo_tree::iterator root_node = ann_tr.begin();
    // output node
    combo_tree::iterator output_node =
        ann_tr.append_child(root_node, ann_type(1, id::ann_node));
    // input nodes
    for(arity_t i = 0; i <= arity; i++) 
        ann_tr.append_child(output_node, ann_type(i + 2, id::ann_input));
    // input nodes' weights
    ann_tr.append_children(output_node, 0.0, arity + 1);
    
    return ann_tr;
}

/**
 * determine the alphabet size given the type_tree of the problem and
 * the of operator that are ignored
 */
int alphabet_size(const type_tree& tt, const vertex_set ignore_ops) {
    arity_t arity = type_tree_arity(tt);
    type_node output_type = *type_tree_output_type_tree(tt).begin();
    if(output_type == id::boolean_type) {
        return 3 + arity;
    } else if(output_type == id::contin_type) {
        // set alphabet size, 8 is roughly the number of operators
        // in contin formula, it will have to be adapted
        return 8 + arity - ignore_ops.size();
    } else if(output_type == id::ann_type) {
        return 2 + arity*arity; // to account for hidden neurones, very roughly
    } else {
        unsupported_type_exit(tt);
        return 0;
    }
}


int main(int argc,char** argv) { 

    // program options, see options_description below for their meaning
    unsigned long rand_seed;
    string input_table_file;
    string problem;
    string combo_str;
    unsigned int problem_size;
    int nsamples;
    float min_rand_input;
    float max_rand_input;
    unsigned long max_evals;
    long result_count;
    bool output_bscore;
    bool output_complexity;
    bool output_eval_number;
    int max_gens;
    string log_level;
    string log_file;
    bool log_file_dep_opt;
    float variance;
    float prob;
    vector<string> ignore_ops_str;
    string opt_algo; //optimization algorithm
    vector<string> exemplars_str;
    int reduct_candidate_effort;
    int reduct_knob_building_effort;
    unsigned long cache_size;
    vector<string> jobs_str;
    // metapop_param
    int max_candidates;
    bool reduce_all;
    bool count_base; // true if the scorer is count based, otherwise
                     // complexity based
    bool revisit;
    bool ignore_bscore;
    bool ignore_bscore_visited;
    // eda_param
    double pop_size_ratio;

    // Declare the supported options.
    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message.")
        (opt_desc_str(rand_seed_opt).c_str(),
         value<unsigned long>(&rand_seed)->default_value(1),
         "random seed.")
        (opt_desc_str(max_evals_opt).c_str(),
         value<unsigned long>(&max_evals)->default_value(10000),
         "maximum number of fitness function evaluations.")
        (opt_desc_str(result_count_opt).c_str(),
         value<long>(&result_count)->default_value(10),
         "the number of non-dominated best results to return ordered according to their score, if negative then returns all of them.")
        (opt_desc_str(output_complexity_opt).c_str(),
         value<bool>(&output_complexity)->default_value(false),
         "if 1, outputs the complexity before each candidate (at the right of the score).")
        (opt_desc_str(output_bscore_opt).c_str(),
         value<bool>(&output_bscore)->default_value(false),
         "if 1, outputs the bscore below each candidate.")
        (opt_desc_str(output_eval_number_opt).c_str(),
         value<bool>(&output_eval_number)->default_value(false),
         "if 1, outputs the actual number of evaluations.")
        (opt_desc_str(max_gens_opt).c_str(),
         value<int>(&max_gens)->default_value(-1),
         "maximum number of demes to generate and optimize, negative means no generation limit.")
        (opt_desc_str(input_table_file_opt).c_str(),
         value<string>(&input_table_file),
         "input table file, the maximum number of samples is the number of rows in the file.")
        (opt_desc_str(problem_opt).c_str(),
         value<string>(&problem)->default_value("it"),
         string("problem to solve, supported problems are"
                " regression based on input table (").append(it).
         append("), regression based on input table using ann (").append(ann_it).
         append("), regression based on combo program (").append(cp).
         append("), even parity (").append(pa).
         append("), disjunction (").append(dj).
         append("), regression of f(x)_o = sum_{i={1,o}} x^i (").append("sr").
         append(").").c_str())
        (opt_desc_str(combo_str_opt).c_str(),
         value<string>(&combo_str),
         string("combo program to learn, use when the problem ").append(cp).append(" is selected (option -h).").c_str())
        (opt_desc_str(problem_size_opt).c_str(),
         value<unsigned int>(&problem_size)->default_value(5),
         string("for even parity (").append(pa).
         append(") and disjunction (").append(dj).
         append(") the problem size corresponds to the arity,"
                " for regression of f(x)_o = sum_{i={1,o}} x^i (").append(sr).
         append(") the problem size corresponds to the order o.").c_str())
        (opt_desc_str(nsamples_opt).c_str(),
         value<int>(&nsamples)->default_value(-1),
         "number of samples to describ the problem. If nsample is negative, null or larger than the maximum number of samples allowed it is ignored.")
        (opt_desc_str(min_rand_input_opt).c_str(),
         value<float>(&min_rand_input)->default_value(0),
         "min of an input value chosen randomly, only used when the problem takes continuous inputs")
        (opt_desc_str(max_rand_input_opt).c_str(),
         value<float>(&max_rand_input)->default_value(1),
         "max of an input value chosen randomly, only used when the problem takes continuous inputs")
        (opt_desc_str(log_level_opt).c_str(),
         value<string>(&log_level)->default_value("DEBUG"),
         "log level, possible levels are NONE, ERROR, WARN, INFO, DEBUG, FINE. Case does not matter.")
        (opt_desc_str(log_file_dep_opt_opt).c_str(),
         "the name of the log is determined by the options, for instance if moses-exec is called with -r 123 -H pa the log name is moses_random-seed_123_problem_pa.log. Note that the name will be truncated in order not to be longer than 255 characters.")
        (opt_desc_str(log_file_opt).c_str(),
         value<string>(&log_file)->default_value(default_log_file),
         string("file name where to write the log. This option overwrite ").append(log_file_dep_opt_opt.first).append(".").c_str())
        (opt_desc_str(variance_opt).c_str(),
         value<float>(&variance)->default_value(0),
         "in the case of contin regression. variance of an assumed Gaussian around each candidate's output, useful if the data are noisy or to control an Occam's razor bias, 0 or negative means no Occam's razor, otherwise the higher v the stronger the Occam's razor.")
        (opt_desc_str(prob_opt).c_str(),
         value<float>(&prob)->default_value(0),
         "in the case of boolean regression, probability that an output datum is wrong (returns false while it should return true or the other way around), useful if the data are noisy or to control an Occam's razor bias, only values 0 < p < 0.5 are meaningful, out of this range it means no Occam's razor, otherwise the greater p the greater the Occam's razor.")
        (opt_desc_str(ignore_ops_str_opt).c_str(),
         value<vector<string> >(&ignore_ops_str),
         "ignore the following operator in the program solution, can be used several times, for moment only div, sin, exp and log can be ignored.")
        (opt_desc_str(opt_algo_opt).c_str(),
         value<string>(&opt_algo)->default_value(un),
         string("optimization algorithm, supported algorithms are"
                " univariate (").append(un).
         append("), simulation annealing (").append(sa).
         append("), hillclimbing (").append(hc).append(").").c_str())
        (opt_desc_str(exemplars_str_opt).c_str(),
         value<vector<string> >(&exemplars_str),
         "start the search with a given exemplar, can be used several times.")
        (opt_desc_str(max_candidates_opt).c_str(),
         value<int>(&max_candidates)->default_value(-1),
         "maximum number of considered candidates to be added to the metapopulation after optimizing deme")
        (opt_desc_str(reduce_all_opt).c_str(),
         value<bool>(&reduce_all)->default_value(true),
         "reduce all candidates before being evaluated, otherwise there are only reduced before being added to the metapopulation. This option can be valuable if the cache is enabled to not evaluate multiple time equivalent candidates.")
        (opt_desc_str(reduct_candidate_effort_opt).c_str(),         
         value<int>(&reduct_candidate_effort)->default_value(2),
         "effort allocated for reduction of candidates, 0-3, 0 means minimum effort, 3 means maximum effort.")
        (opt_desc_str(reduct_knob_building_effort_opt).c_str(),
         value<int>(&reduct_knob_building_effort)->default_value(2),
         "effort allocated for reduction during knob building, 0-3, 0 means minimum effort, 3 means maximum effort. The bigger the effort the lower the dimension of the deme.")
        (opt_desc_str(count_base_opt).c_str(),
         value<bool>(&count_base)->default_value(false),
         "if 1 then a count based scorer is used, otherwise, if 0, a complexity based scorer is used.")
        (opt_desc_str(cache_size_opt).c_str(),
         value<unsigned long>(&cache_size)->default_value(1000000),
         "cache size, so that identical candidates are not re-evaluated, 0 means no cache.")
        (opt_desc_str(revisit_opt).c_str(),
         "revisit visited examplars when all have been visited")
        (opt_desc_str(jobs_opt).c_str(),
         value<vector<string> >(&jobs_str),
         string("number of jobs allocated for deme optimization. Jobs can be executed on a remote machine as well, in such case the notation -j N:REMOTE_HOST is used. For instance one can enter the options -j 4 -j 16").append(job_seperator).append("my_server.org (or -j 16").append(job_seperator).append("user@my_server.org if wishes to run the remote jobs under a different user name), meaning that 4 jobs are allocated on the local machine and 16 jobs are allocated on my_server.org. The assumption is that moses-exec must be on the remote machine and is located in a directory included in the PATH environment variable. Beware that a lot of log files are gonna be generated when using this option.").c_str())
        (opt_desc_str(pop_size_ratio_opt).c_str(),
         value<double>(&pop_size_ratio)->default_value(200),
         "the higher the more effort is spent on a deme.")
        (opt_desc_str(ignore_bscore_opt).c_str(),
         value<bool>(&ignore_bscore)->default_value(false),
         "ignore the behavioral score when merging candidates in the population. This option is useful either when the problem has no obvious behavioral score, or it happens that dominated candidates would lead to the solution faster.")
        (opt_desc_str(ignore_bscore_visited_opt).c_str(),
         value<bool>(&ignore_bscore_visited)->default_value(true),
         "when testing dominance for merging a candidate to the metapopulation, visited candidates are ignored, that if a candidate is only dominated by visited candidates it is still added to the metapopulation. Of course if ignore-bscore is enabled this option has no effect.")
        ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    // set flags
    revisit = vm.count(revisit_opt.first) > 0;
    log_file_dep_opt = 
        vm.count(log_file_dep_opt_opt.first) > 0 && log_file == default_log_file;
    
    if (vm.count("help") || argc == 1) {
        cout << desc << "\n";
        return 1;
    }

    // set log
    if(log_file_dep_opt) {
        // determine the name of the log depending on the program options
        log_file = default_log_file_prefix;
        for(variables_map::const_iterator it = vm.begin(); it != vm.end(); it++)
            // we ignore the option log_file_dep_opt and any default one
            if(it->first != log_file_dep_opt_opt.first && !it->second.defaulted()) {
                string str = string("_") + it->first + "_" + to_string(it->second);
                // this is because OSs usually do not handle file name
                // above 255 chars
                unsigned int expected_max_size =
                    log_file.size()+str.size()+default_log_file_suffix.size()+1;
                if(expected_max_size < 255) {
                    log_file += str;
                }
            }
        log_file += string(".") + default_log_file_suffix;
    }
    // remove log_file
    remove(log_file.c_str());
    logger().setFilename(log_file);
    logger().setLevel(logger().getLevelFromString(log_level));
    logger().setBackTraceLevel(opencog::Logger::ERROR);
    // init random generator
    opencog::MT19937RandGen rng(rand_seed);

    // convert ignore_ops_str to the set of actual operators to ignore
    vertex_set ignore_ops;
    foreach(const string& s, ignore_ops_str) {
        vertex v;
        if(builtin_str_to_vertex(s, v))
            ignore_ops.insert(v);
        else {
            std::cerr << "error: " << s 
                      << " is not recognized as combo operator" << std::endl;
            return 1;
        }
    }

    // set the initial exemplars
    vector<combo_tree> exemplars;
    foreach(const string& exemplar_str, exemplars_str) {
        stringstream ss;
        combo_tree exemplar;
        ss << exemplar_str;
        ss >> exemplar;
        exemplars.push_back(exemplar);
    }

    // fill jobs
    jobs_t jobs;
    foreach(const string& js, jobs_str) {
        size_t pos = js.find(job_seperator);
        if(pos != string::npos) {
            unsigned int nj = boost::lexical_cast<unsigned int>(js.substr(0, pos));
            string host_name = js.substr(pos + 1);
            jobs.insert(make_pair(host_name, nj));
        } else {
            jobs.insert(make_pair(localhost, boost::lexical_cast<unsigned int>(js)));
        }
    }

    // set metapopulation parameters
    metapop_parameters meta_param(max_candidates, reduce_all,
                                  count_base, revisit, ignore_bscore,
                                  ignore_bscore_visited);

    // set eda_parameters
    eda_parameters eda_param(pop_size_ratio);

    // set moses_parameters
    moses_parameters moses_param(max_evals, max_gens,
                                 0 /*@todo: max_score*/,
                                 ignore_ops);

    // set metapop_moses_results_parameters
    metapop_moses_results_parameters mmr_pa(result_count, output_complexity,
                                            output_bscore, output_eval_number,
                                            jobs);

    if(problem == it) { // regression based on input table
        
        // if no exemplar has been provided in option try to infer it
        if(exemplars.empty()) {            
            type_node inferred_type = infer_type_from_data_file(input_table_file);
            exemplars.push_back(type_to_exemplar(inferred_type));
        }

        type_node output_type = 
            *(get_output_type_tree(*exemplars.begin()->begin()).begin());

        auto_ptr<ifstream> in(open_table_file(input_table_file));

        if(output_type == id::boolean_type) {
            // read input_table_file file
            truth_table_inputs inputtable;
            partial_truth_table booltable;
            istreamTable<truth_table_inputs,
                         partial_truth_table, bool>(*in, inputtable, booltable);
            if(nsamples>0)
                subsampleTable(inputtable, booltable, nsamples, rng);
            unsigned int arity = inputtable[0].size();
        
            type_tree tt = declare_function(output_type, arity);

            int as = alphabet_size(tt, ignore_ops);

            occam_truth_table_bscore bscore(booltable, inputtable,
                                            prob, as, rng);
            metapop_moses_results(rng, exemplars, tt,
                                  logical_reduction(reduct_candidate_effort),
                                  logical_reduction(reduct_knob_building_effort),
                                  bscore, cache_size, opt_algo,
                                  eda_param, meta_param, moses_param,
                                  vm, mmr_pa);
        }
        else if(output_type == id::contin_type) {
            // read input_table_file file
            contin_table_inputs inputtable;
            contin_table contintable;
            istreamTable<contin_table_inputs,
                         contin_table, contin_t>(*in, inputtable, contintable);
            if(nsamples>0)
                subsampleTable(inputtable, contintable, nsamples, rng);

            unsigned int arity = inputtable[0].size();
            type_tree tt = declare_function(output_type, arity);
            int as = alphabet_size(tt, ignore_ops);

            // if no exemplar has been provided in option use the default
            // contin_type exemplar (+)
            if(exemplars.empty()) {            
                exemplars.push_back(type_to_exemplar(id::contin_type));
            }

            occam_contin_bscore bscore(contintable, inputtable,
                                       variance, as, rng);
            metapop_moses_results(rng, exemplars, tt,
                                  contin_reduction(ignore_ops, rng),
                                  contin_reduction(ignore_ops, rng),
                                  bscore, cache_size, opt_algo,
                                  eda_param, meta_param, moses_param,
                                  vm, mmr_pa);
        } else {
            unsupported_type_exit(output_type);
        }
    } else if(problem == cp) { // regression based on combo program
        if(combo_str.empty())
            unspecified_combo_exit();
        // get the combo_tree and infer its type
        stringstream ss;
        combo_tree tr;
        ss << combo_str;
        ss >> tr;
        type_tree tt = infer_type_tree(tr);

        if(is_well_formed(tt)) {
            type_node output_type = *type_tree_output_type_tree(tt).begin();
            arity_t arity = type_tree_arity(tt);
            // if no exemplar has been provided in option use the default one
            if(exemplars.empty()) {
                exemplars.push_back(type_to_exemplar(output_type));
            }
            if(output_type == id::boolean_type) {
                // @todo: Occam's razor and nsamples is not taken into account
                logical_bscore bscore(tr, arity);
                metapop_moses_results(rng, exemplars, tt,
                                      logical_reduction(reduct_candidate_effort),
                                      logical_reduction(reduct_knob_building_effort),
                                      bscore, cache_size, opt_algo,
                                      eda_param, meta_param, moses_param,
                                      vm, mmr_pa);                
            }
            else if (output_type == id::contin_type) {
                // @todo: introduce some noise optionally
                if(nsamples<=0)
                    nsamples = default_nsamples;
                
                contin_table_inputs inputtable(nsamples, arity, rng,
                                               max_rand_input, min_rand_input);
                contin_table table_outputs(tr, inputtable, rng);

                int as = alphabet_size(tt, ignore_ops);

                occam_contin_bscore bscore(table_outputs, inputtable,
                                           variance, as, rng);
                metapop_moses_results(rng, exemplars, tt,
                                      contin_reduction(ignore_ops, rng),
                                      contin_reduction(ignore_ops, rng),
                                      bscore, cache_size, opt_algo,
                                      eda_param, meta_param, moses_param,
                                      vm, mmr_pa);
            } else {
                unsupported_type_exit(tt);
            }
        }
        else {
            illformed_exit(tr);
        }
    } else if(problem == pa) { // even parity
        // @todo: for the moment occam's razor and partial truth table are ignored
        unsigned int arity = problem_size;
        even_parity func;

        // if no exemplar has been provided in option use the default
        // contin_type exemplar (and)
        if(exemplars.empty()) {
            exemplars.push_back(type_to_exemplar(id::boolean_type));
        }

        type_tree tt = declare_function(id::boolean_type, arity);
        logical_bscore bscore(func, arity);
        metapop_moses_results(rng, exemplars, tt,
                              logical_reduction(reduct_candidate_effort),
                              logical_reduction(reduct_knob_building_effort),
                              bscore, cache_size, opt_algo,
                              eda_param, meta_param, moses_param,
                              vm, mmr_pa);
    } else if(problem == dj) { // disjunction
        // @todo: for the moment occam's razor and partial truth table are ignored
        unsigned int arity = problem_size;
        disjunction func;

        // if no exemplar has been provided in option use the default
        // contin_type exemplar (and)
        if(exemplars.empty()) {            
            exemplars.push_back(type_to_exemplar(id::boolean_type));
        }

        type_tree tt = declare_function(id::boolean_type, arity);
        logical_bscore bscore(func, arity);
        metapop_moses_results(rng, exemplars, tt,
                              logical_reduction(reduct_candidate_effort),
                              logical_reduction(reduct_knob_building_effort),
                              bscore, cache_size, opt_algo,
                              eda_param, meta_param, moses_param,
                              vm, mmr_pa);
    } else if(problem == sr) { // simple regression of f(x)_o = sum_{i={1,o}} x^i
        unsigned int arity = 1;

        // if no exemplar has been provided in option use the default
        // contin_type exemplar (+)
        if(exemplars.empty()) {            
            exemplars.push_back(type_to_exemplar(id::contin_type));
        }
        
        type_tree tt = declare_function(id::contin_type, arity);

        contin_table_inputs rands((nsamples>0? nsamples : default_nsamples),
                                  arity, rng);

        int as = alphabet_size(tt, ignore_ops);

        occam_contin_bscore bscore(simple_symbolic_regression(problem_size),
                                   rands, variance, as, rng);
        metapop_moses_results(rng, exemplars, tt,
                              contin_reduction(ignore_ops, rng),
                              contin_reduction(ignore_ops, rng),
                              bscore, cache_size, opt_algo,
                              eda_param, meta_param, moses_param,
                              vm, mmr_pa);
    //////////////////
    // ANN problems //
    //////////////////
    } else if(problem == ann_it) { // regression based on input table using ann
        auto_ptr<ifstream> in(open_table_file(input_table_file));
        contin_table_inputs inputtable;
        contin_table contintable;
        // read input_table_file file
        istreamTable<contin_table_inputs,
                     contin_table, contin_t>(*in, inputtable, contintable);

        unsigned int arity = inputtable[0].size();

        // if no exemplar has been provided in option insert the default one
        if(exemplars.empty()) {
            exemplars.push_back(ann_exemplar(arity));
        }

        // subsample the table
        if(nsamples>0)
            subsampleTable(inputtable, contintable, nsamples, rng);

        type_tree tt = declare_function(id::ann_type, 0);
        
        int as = alphabet_size(tt, ignore_ops);

        occam_contin_bscore bscore(contintable, inputtable,
                                   variance, as, rng);
        metapop_moses_results(rng, exemplars, tt,
                              ann_reduction(),
                              ann_reduction(),
                              bscore, cache_size, opt_algo,
                              eda_param, meta_param, moses_param,
                              vm, mmr_pa);
    } else if(problem == ann_cp) { // regression based on combo program using ann
        if(combo_str.empty())
            unspecified_combo_exit();
        // get the combo_tree and infer its type
        stringstream ss;
        combo_tree tr;
        ss << combo_str;
        ss >> tr;
        type_tree problem_tt = infer_type_tree(tr);

        if(is_well_formed(problem_tt)) {
            arity_t arity = type_tree_arity(problem_tt);
            // if no exemplar has been provided in option use the default one
            if(exemplars.empty()) {
                exemplars.push_back(ann_exemplar(arity));
            }

            // @todo: introduce some noise optionally
            if(nsamples<=0)
                nsamples = default_nsamples;

            contin_table_inputs inputtable(nsamples, arity, rng,
                                               max_rand_input, min_rand_input);
            contin_table table_outputs(tr, inputtable, rng);
            
            type_tree tt = declare_function(id::ann_type, 0);

            int as = alphabet_size(tt, ignore_ops);
            
            occam_contin_bscore bscore(table_outputs, inputtable,
                                       variance, as, rng);
            metapop_moses_results(rng, exemplars, tt,
                                  contin_reduction(ignore_ops, rng),
                                  contin_reduction(ignore_ops, rng),
                                  bscore, cache_size, opt_algo,
                                  eda_param, meta_param, moses_param,
                                  vm, mmr_pa);
        } else illformed_exit(tr);
    }
    else unsupported_problem_exit(problem);
}
