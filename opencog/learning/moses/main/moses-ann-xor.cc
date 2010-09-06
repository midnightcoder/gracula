#include <opencog/comboreduct/combo/eval.h>

#include <iostream>
#include <opencog/learning/moses/moses/moses.h>
#include <opencog/learning/moses/moses/optimization.h>
#include <opencog/learning/moses/moses/scoring_functions.h>
#include <opencog/learning/moses/moses/scoring.h>
#include <opencog/learning/moses/moses/ann_scoring.h>

#include <opencog/util/mt19937ar.h>

#include <opencog/util/Logger.h>
#include <opencog/learning/moses/moses/representation.h>

using namespace moses;
using namespace reduct;
using namespace boost;
using namespace std;
using namespace opencog;


int main(int argc, char** argv)
{

    //set flag to print only cassert and other ERROR level logs on stdout
    opencog::logger().setPrintErrorLevelStdout();

    //read in maximum evaluations and RNG seed from command line
    int max_evals;
    int seed;
    bool reduce=true;
    try {
        //if (argc!=3)
        //    throw "foo";
        max_evals=lexical_cast<int>(argv[1]);
        seed=lexical_cast<int>(argv[2]);
        set_stepsize(lexical_cast<double>(argv[3]));
        set_expansion(lexical_cast<double>(argv[4]));
        set_depth(lexical_cast<int>(argv[5]));
        reduce = lexical_cast<int>(argv[6]);
    } catch (...) {
        cerr << "usage: " << argv[0] << " maxevals seed" << endl;
        exit(1);
    }
    
    // read in the seed combo tree from stdin
    // a default seed is provided in file xor_tree
    combo_tree tr;
    cin >> tr; 
    
    opencog::MT19937RandGen rng(seed);

    // this will let representation building know that we are dealing
    // with an ANN, what it says is that the type of combo_tree to
    // evolve are ANN generators (no input, output an ANN)
    type_tree tt(id::lambda_type);
    tt.append_children(tt.begin(), id::ann_type, 1);


    // binary XOR task
    ann_score score;
    ann_bscore bscore;

    const reduct::rule* si = &(ann_reduction());
    if(!reduce)
        si = &(clean_reduction());

    metapopulation<ann_score, ann_bscore, univariate_optimization>
        metapop(rng, tr, tt, *si, score, bscore,
                univariate_optimization(rng));
    moses_parameters moses_param(max_evals);
    moses::moses(metapop, moses_param);

    //transform the best combo tree into an ANN
    tree_transform trans; 
    combo_tree best = metapop.best_tree();
    ann bestnet = trans.decodify_tree(best);
    
    
    //look at xor outputs
    double inputs[4][3] = { {0.0, 0.0, 1.0}, 
                            {0.0, 1.0, 1.0}, 
                            {1.0, 0.0, 1.0},
                            {1.0, 1.0, 1.0}};
    
    int depth = bestnet.feedforward_depth();
    
    for (int pattern = 0;pattern < 4;pattern++) {
        bestnet.load_inputs(inputs[pattern]);
        for (int x = 0;x < depth;x++)
            bestnet.propagate();
        cout << "Input [ " << inputs[pattern][0] << " " << inputs[pattern][1] << " ] : Output " << bestnet.outputs[0]->activation << endl;
    
    }
   
    //save the best network (can be viewed in any dot viewer)
    cout << "Best network: " << endl;
    cout << &bestnet << endl;
    bestnet.write_dot("best_nn.dot");
       
    //write out the best score (to be used in parameter sweep)
    cout << metapop.best_score() << endl;
}
