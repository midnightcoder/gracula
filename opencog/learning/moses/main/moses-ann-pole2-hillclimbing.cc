#include <opencog/comboreduct/combo/eval.h>

#include <iostream>
#include <opencog/learning/moses/moses/moses.h>
#include <opencog/learning/moses/moses/optimization.h>
#include <opencog/learning/moses/moses/scoring_functions.h>
#include <opencog/learning/moses/moses/scoring.h>
#include <opencog/learning/moses/moses/ann_scoring.h>

#include <opencog/util/mt19937ar.h>

#include <opencog/util/Logger.h>

using namespace moses;
using namespace reduct;
using namespace boost;
using namespace std;
using namespace opencog;


int main(int argc, char** argv)
{

    //set flag to print only cassert and other ERROR level logs on stdout
    opencog::logger().setPrintErrorLevelStdout();

    combo_tree tr;
    cin >> tr; 

    tree_transform trans;
    ann nn = trans.decodify_tree(tr);
    cout << tr << endl;
    cout << "Network depth: " << nn.feedforward_depth() << endl;
    cout << &nn << endl;

    opencog::MT19937RandGen rng(0);

    type_tree tt(id::lambda_type);
    tt.append_children(tt.begin(), id::ann_type, 1);


    //DOUBLE MARKOVIAN POLE TASK`
    ann_pole2_score p2_score;
    ann_pole2_bscore p2_bscore;

    metapopulation<ann_pole2_score, ann_pole2_bscore, iterative_hillclimbing>
        metapop_pole2(rng, tr, tt, clean_reduction(),
                      p2_score, p2_bscore,
                      iterative_hillclimbing(rng));

    moses::moses(metapop_pole2);

    combo_tree best = metapop_pole2.best_tree();
    ann bestnet = trans.decodify_tree(best);
    
    cout << "Best network: " << endl;
    cout << &bestnet << endl;
    bestnet.write_dot("best_nn.dot");    

    //for parameter sweet
    cout << metapop_pole2.best_score() << endl;
}




