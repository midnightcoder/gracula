#include "Testing.h"

#include "PLNModule.h"
#include "PLN.h"
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atomspace/SimpleTruthValue.h>
#include <opencog/util/Config.h>
#include <opencog/guile/SchemeEval.h>
#include <opencog/server/load-file.h>
#include <boost/filesystem.hpp>
#include <string>

#define BackInferenceTreeRootT BITNodeRoot

using namespace std;
using namespace opencog;
using namespace opencog::pln;
using namespace boost::filesystem;

// state variables for running multiple PLNShell commands.
extern Btr<BITNodeRoot> Bstate;
extern BITNodeRoot* temp_state, *state;

namespace test
{
    extern FILE *logfile;
    extern double custom_duration;
    extern double custom_duration2;
}

//namespace opencog {
//namespace pln {
namespace test {

FitnessEvaluatorT testFitnessEvaluator;

// the following is used for test statistics
vector< vector<vector<int> > >  INstatsVT;
vector<vector<int> > INstatsV;
vector<int> INstats;
int allTestsInferenceNodes=0;
int allTestsExpansions=0;

int tests_passed=0;
int tests_total=0;

float getCount(float c)
{ return SimpleTruthValue::confidenceToCount(c); }

// Now uses SCM. Won't search any alternative locations for the file.
void initAxiomSet(string premiseFile)
{
    AtomSpaceWrapper *atw = GET_ASW;
    atw->reset();
    atw->allowFWVarsInAtomSpace = true;

    //cprintf(-2,"loading...\n");
    std::cout << "loading " << premiseFile << std::endl;

#if 0
    atw->archiveTheorems = true;
    bool axioms_ok = atw->loadAxioms(premiseFile);
    if (!axioms_ok) throw std::string("failed to load file");
    atw->archiveTheorems = false;

#elif HAVE_GUILE
    int rc = load_scm_file(premiseFile.c_str());

    atw->archiveTheorems = true;
    atw->makeCrispTheorems();
    atw->archiveTheorems = false;

    // The error-checking is after the call to makeCrispTheorems, because
    // if there was an error, then the AS is now empty and makeCrispTheorems
    // will (correctly) record that there are none.
    if (rc) throw std::string("failed to load file");
#else
    throw std::string("Not allowed to use XML or SCM!");
#endif

    cprintf(-2,"%s loaded. Next test: ", premiseFile.c_str());
}

//! @todo Replace with an existing method, after tweaking various things to
//! make it suitable.
static SimpleTruthValue* parseSTV(const char* tvStr) {
    float mean, conf;
    sscanf(tvStr, "[%f,%f]", &mean, &conf);
    return new SimpleTruthValue(mean, getCount(conf));
}

Btr<PLNTest> setupSCMTarget(std::string conf_file, bool test_bc)
{
#ifdef HAVE_GUILE
    //!@ todo haxx
    FitnessEvaluatorT testFitnessEvaluator = BEST;

    Config test;
    std::cout << conf_file << std::endl;
    test.load(conf_file.c_str());
    //std::cout << test.to_string() << std::endl;

    std::cout << test["COMMENT"] << std::endl;

    //! @todo define this somewhere else.
    std::string PLN_TEST_DIR = "../tests/reasoning/pln/";
    // scm is now used instead of xml
    // std::string axiomsFile = PLN_TEST_DIR+"xml/"+test["LOAD"]+".xml";
    std::string axiomsFile = PLN_TEST_DIR+"scm/"+test["LOAD"]+".scm";

    initAxiomSet(axiomsFile);

    std::string targetScheme = test["TARGET"];
    std::cout << "Target: " << targetScheme << std::endl;

    // get target atom
    SchemeEval& eval = SchemeEval::instance();
    Handle h = eval.eval_h(targetScheme);

    pHandleSeq fakeHandles =
            ((AtomSpaceWrapper*)ASW())->realToFakeHandle(h);
    pHandle fakeHandle = fakeHandles[0];
    //meta target(new vtree(fakeHandle));

    meta target_(new vtree(fakeHandle));

    meta target = ForceAllLinksVirtual(target_);

    // Set parameters
    uint maxSteps = test.get_int("MAX_STEPS");
    /*
    // Make sure FC doesn't take too long / only test it on simpler tests.
    // It takes disproportionately long once the AtomSpace gets large, due
    // to the indexes probably.
    if (!TESTING_BACKWARD_CHAINING && maxSteps > 201) {
        std::cout << "Skipping test due to time constraints" << std::endl;
        return; //minEvalsOfFittestBIT = 500;
    }
     */
    uint minEvalsOfFittestBIT = maxSteps/100;

    // Changed because we don't want to have to specify the count
    // as well, in the test's .conf file
    //TruthValue* minTV = SimpleTruthValue::fromString(test["MIN_TV"].c_str());
    //TruthValue* maxTV = SimpleTruthValue::fromString(test["MAX_TV"].c_str());
    TruthValue* minTV = parseSTV(test["MIN_TV"].c_str());
    TruthValue* maxTV = parseSTV(test["MAX_TV"].c_str());

    std::cout << "Loaded parameters of test OK" << std::endl;



    state = new BITNodeRoot(target, new DefaultVariableRuleProvider(), config().get_bool("PLN_RECORD_TRAILS"), testFitnessEvaluator);
    Bstate.reset(state);

    return Btr<PLNTest>(
            new PLNTest(target,
                       minTV,
                       maxTV,
                       minEvalsOfFittestBIT,
                       0)); // Alternative for using exhaustive evaluation (obsolete).
#endif // HAVE_GUILE
}

/**
 * @param test_bc is a flag indicating whether to run the tests
 * with the backward chainer (true) or the forward chainer (false)
 */
bool runSCMTargets(string testDir, bool test_bc) {
    // TODO maybe use regex_iterator

    testDir+= "targets/";

    // This method is the main one for current and future tests.
    // Also, there are a lot of different errors that can happen.
    // So, it has/needs somewhat elaborate exception-handling.
    // This is partly because CXXTest doesn't give you any info
    // about an exception, just telling you it happened.
    try {

        // TODO apparently there is a way to make iterators only cover
        // either files or directories. The boost::filesystem
        // documentation is very obsolete however.
        for (recursive_directory_iterator end, dir(testDir);
              dir != end; ++dir) {
            if (!is_directory(dir->status())) {
                Btr<PLNTest> _test = setupSCMTarget(dir->path().string().c_str(), test_bc);
                if (_test) runPLNTest(_test, test_bc);
            }
        }
    }
    catch(std::string s) {
        std::cerr << "testSCMTargets:" << s;
        throw s;
    }
    catch(StandardException e) {
        std::cerr << "testSCMTargets:" << e.getMessage();
        throw e;
    }
    catch(PLNexception e)
    {
        std::cerr << "testSCMTargets:" << e.what();
        throw e;
    }
    catch(...)
    {
        std::cerr << "Unknown exception during SCM targets" << std::endl;
        throw;
    }

    //! @todo May get some interference from other uses of runPLNTest in PLNUTest.cxxtest.
    if (tests_passed==tests_total) {
        cout << "Passed all " << tests_total << " tests!" << endl;
        return true;
    } else {
        cout << "Failed " << (tests_total - tests_passed) << " out of "
             << tests_total << " tests." << endl;
        return false;
    }
}

Btr<PLNTest> findSCMTarget(std::string test_name, bool test_bc) {
    // Find the path to the test file with that name.
    std::string testDir = "../tests/reasoning/pln/targets/";
    std::string tmp = test_name + std::string("_test.conf");
    path conf_filename(tmp);

    std::string conf_file;

    for (recursive_directory_iterator end, dir(testDir);
            dir != end; ++dir) {
        //if (!is_directory(dir->status()) && dir->path() == conf_filename)
        //      std::cout << dir->filename() << " " << conf_filename.filename() << std::endl;
        //! @todo sigh. why doesn't this work?
        //if (equivalent(dir->path(), conf_filename))
        if (dir->path().filename() == conf_filename.filename())
            conf_file = dir->path().file_string();
    }

    //! @todo
    if (conf_file.empty()) return Btr<PLNTest>();
    else return setupSCMTarget(conf_file, test_bc);
}

bool runPLNTest(Btr<PLNTest> t, bool test_bc)
{
    AtomSpaceWrapper *asw = GET_ASW;
    stats::Instance().ITN2atom.clear();

    //currentDebugLevel=0;
    rawPrint(*t->target, t->target->begin(), -2);

    clock_t start, finish;
    double duration;

    test::custom_duration = 0.0;
    start = clock();

    asw->allowFWVarsInAtomSpace = true;

    fflush(stdout);
    //currentDebugLevel=-4;

    Btr<BackInferenceTreeRootT> state;
    //ForwardChainer fc;
    HybridForwardChainer fc;

    // Even with FC, we want to have a BIT available so that printTrail can be accessed
//        if (test_bc) {
        state.reset(new BITNodeRoot(t->target, new DefaultVariableRuleProvider(), config().get_bool("PLN_RECORD_TRAILS"), testFitnessEvaluator));
//        }
    uint s_i=0; // Expansion phase #
    pHandle eh=PHANDLE_UNDEFINED; // expected target handle
    TruthValue* etv = NULL; // expect target handle TV
    bool passed=false;

    set<VtreeProvider*> eres;

    if (test_bc)
        t->minEvalsOfFittestBIT *= 100; //Minimum "resolution"
    else
        // 12 is the maximum depth the BIT reaches, and each "step" of FC now does one level
        t->minEvalsOfFittestBIT = 12;

    const int expansions_per_run = 1000;
    int total_expansions = 0;

    if (t->minEvalsOfFittestBIT > 0) {
        do {
/*              for (int k=0;k<expansions_per_run;k++)
                state->expandFittest();

            eres = state->evaluate();*/

            cprintf(-3, "\n    Evaluating...\n");

            //if (foo42)
                //currentDebugLevel=4;

            int expansions = expansions_per_run;
            if (test_bc) {
                eres = state->infer(expansions, 0.000001f, 0.01f);
            } else {
                eh = fc.fwdChainToTarget(expansions, (t->target));
                //if (!results.empty()) eh = results[0];
            }

            total_expansions = expansions_per_run - expansions;

            if (expansions > 0)
                cprintf(-3, "Succeeded. Saved $%d / $%d (from the "
                        "beginning of the cycle).\n", expansions,
                        expansions_per_run);
            else
                cprintf(2, "Failed for now... Saved $%d / $%d (from the "
                        "beginning of the cycle).\n", expansions,
                        expansions_per_run);

            //currentDebugLevel=-4;

            if (test_bc)
                eh = (eres.empty() ? PHANDLE_UNDEFINED :
                        _v2h(*(*eres.rbegin())->getVtree().begin()));

            if (eh != PHANDLE_UNDEFINED )
                etv = asw->getTV(eh).clone();
//                else
//                    etv = new SimpleTruthValue(0.0f,0.0f);

           /* float c1=t->minTV->getConfidence();
            float c2=t->maxTV->getConfidence();
            float m1=t->minTV->getMean();
            float m2=t->maxTV->getMean(); */

            if (etv) {
                printf("c: %f min: %f\n", etv->getConfidence(),
                        t->minTV->getConfidence());
                printf("s: %f min: %f\n", etv->getMean(),
                        t->minTV->getMean());
                printf("c: %f max: %f\n", etv->getConfidence(),
                        t->maxTV->getConfidence());
                printf("s: %f max: %f\n", etv->getMean(),
                        t->maxTV->getMean());

                // This uses the BIT, but the trails are stored using some
                // global variables so work in FC.
                std::cout << "Inference trail: " << std::endl;
                state->printTrail(eh);
            }

            passed = (
                eh != PHANDLE_UNDEFINED &&
                etv &&
                etv->getConfidence() >= t->minTV->getConfidence() &&
                etv->getMean()       >= t->minTV->getMean() &&
                etv->getConfidence() <= t->maxTV->getConfidence() &&
                etv->getMean()       <= t->maxTV->getMean()
            );

            cprintf(-4, "TEST Expansion phase %d over.\n", s_i);
        }
        while ((++s_i)*expansions_per_run < t->minEvalsOfFittestBIT
                && !passed);
    }
    else if (t->minExhaustiveEvals > 0) {
        assert(0);
        /// This should be updated to reflect the new BITNode interface
        /*
        for (uint L=0;L<t->minExhaustiveEvals;L++)
            state->expandNextLevel();

        eres = state->evaluate();
        eh = (eres.empty() ? NULL : v2h(eres.rbegin()->value));
        if (eh) {
            if (etv != NULL) delete etv;
            etv = asw->TV(eh).clone();
        }

        passed = (eh && etv &&
            etv->getConfidence() > t->minTV->getConfidence() &&
            etv->getMean()          > t->minTV->getMean()
            );*/
    }
    else
        puts("ERROR IN TEST SETTINGS");

    if (passed) {
        printf("\n"
               "**********************************************\n"
               "passed: %s.\n"
               "**********************************************\n",
            (etv?etv->toString().c_str():"(null TV)"));

        finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        printf( "Test took %2.2f seconds TOTAL.\n", duration );

        printf( "Custom test time was %3.3f seconds.\n",
                test::custom_duration );
        printf( "Custom test time was %3.3f seconds.\n",
                test::custom_duration2 );
    }
    else {
        printf("\n**********************************************\n"
               "FAILED: %s!\n"
               "**********************************************\n",
        (etv?etv->toString().c_str():"(null TV)"));
    }

    if (test_bc) {
        printf("Test results: [");

        foreach(VtreeProvider* bv, eres) {
            const TruthValue& tv = asw->getTV(vt2h(*bv));
            if (!tv.isNullTv() && tv.getConfidence()>0.0001f)
                printf("%d ", vt2h(*bv));
        }
        printf("]\n");

        if (passed) {
            tests_passed++;

            allTestsInferenceNodes += state->inferenceNodes;
            allTestsExpansions += total_expansions;

            INstats.push_back(state->inferenceNodes);

            cout << "Total expansion steps: " << total_expansions <<
                    "(" << allTestsExpansions << " in all tests)";
            cout << endl << "Exec pool size: " << state->getExecPoolSize();
            cout << endl << "InferenceNodes: " << state->inferenceNodes <<
                    " (" << allTestsInferenceNodes << " in all tests)" << endl;
        }
        else
            INstats.push_back(0);
    }

    tests_total++;
/*      if (etv) {
        string stv(etv->toString());
        puts(stv.c_str());
    }*/

    //stats::Instance().print(stats::triviality_filterT());

//    TS_ASSERT(passed);
#if WAIT_KEY_ON_FAILURE
    if (!passed)
        getc(stdin);
#endif
    if (etv != NULL) delete etv;
    //asw->reset(NULL);
    return passed;
}

// helper for runPLNTest
bool maketest(meta target,
              TruthValue* minTV,
              TruthValue* maxTV,
              uint minEvalsOfFittestBIT,
              uint minExhaustiveEvals,
              bool test_bc) {
    Btr<PLNTest> t = Btr<PLNTest>(new PLNTest(target,
                                        minTV,
                                        maxTV,
                                        minEvalsOfFittestBIT,
                                        minExhaustiveEvals)
                                        );
    return runPLNTest(t, test_bc);
}

}//}}
