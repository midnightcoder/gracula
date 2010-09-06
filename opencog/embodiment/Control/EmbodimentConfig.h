/*
 * opencog/embodiment/Control/EmbodimentConfig.h
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Andre Senna, Nil Geisweiller
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


#ifndef EMBODIMENTCONFIG_H
#define EMBODIMENTCONFIG_H

#include <opencog/util/Config.h>
#include <string>
#include <map>

namespace Control
{

using namespace opencog;

namespace ImitationLearningAlgo {
static const char MOSES[] = "MOSES";
static const char HillClimbing[] = "HillClimbing";
}

class EmbodimentConfig : public Config
{

protected:

    const std::string* EMBODIMENT_DEFAULT()
    {
        static const std::string defaultConfig[] = {
            "CONFIG_FILE",                  "embodiment.conf",
            
            "AUTOMATED_SYSTEM_TESTS",       "false",

            // Ids and network settings
            "ROUTER_ID",                    "ROUTER",
            "ROUTER_IP",                    "127.0.0.1",
            "ROUTER_PORT",                  "16312",

            // interval to send alive notifications to NE's (in seconds)
            "ROUTER_AVAILABLE_NOTIFICATION_INTERVAL", 
                                            "15", 

            "SPAWNER_ID",                   "SPAWNER",
            "SPAWNER_IP",                   "127.0.0.1",
            "SPAWNER_PORT",                 "16313",

            "LS_ID",                        "LS",
            "LS_IP",                        "127.0.0.1",
            "LS_PORT",                      "16314",

            // These 2 parameters define the maximal number of pets monitored by a same spawner
            "MIN_OPC_PORT",                 "16326",
            "MAX_OPC_PORT",                 "16330",
            
            "COMBO_SHELL_ID",               "COMBO_SHELL",
            "COMBO_SHELL_IP",               "127.0.0.1",
            "COMBO_SHELL_PORT",             "16316",
            
            // IP and Port are informed by Proxy when it sends a LOGIN message to router.
            // Anyway, fyi, the mockyProxy uses port 16315 and the real Proxy 8211
            "PROXY_ID",                     "PROXY",

            // OPC settings
            "UNKNOWN_PET_OWNER",            "no_owner_id",
            "UNKNOWN_PET_NAME",             "no_name",
            "NAVIGATION_ALGORITHM",         "tangentbug", // options: tangentbug, astar or hpa
            "HPA_MAXIMUM_CLUSTERS",         "16",

            // in m/s . A non-positive number means a random value between 0.5 and 3.5 will be used.
            "PET_WALKING_SPEED",            "2.0", 
            
            // component log levels
            "PROXY_LOG_LEVEL",              "FINE",
            "LS_LOG_LEVEL",                 "FINE",
            "OPC_LOG_LEVEL",                "FINE",
            "ROUTER_LOG_LEVEL",             "FINE",
            "SPAWNER_LOG_LEVEL",            "FINE",
            "OPENCOG_LOG_LEVEL",            "FINE",
            "TIMESTAMP_ENABLED_IN_LOGS",    "true",
            // TODO: Convert all log level parameters above in string, which is more
            // readable (see opencog/util/Config.h and Logger::getLevelFromString), just
            // like bellow:
            "BACK_TRACE_LOG_LEVEL",         "error",
            
            // paths
            "LOG_DIR",                      "/tmp/$USER/Petaverse/Logs",
            "MESSAGE_DIR",                  "/tmp/$USER/Petaverse/queue",
            "PET_DATABASE",                 "/tmp/$USER/Petaverse/PetDatabase",
            "PROXY_DATABASE_DIR",           "/tmp/$USER/Petaverse/ProxyDataBase",
            "ROUTER_DATABASE_DIR",          "/tmp/$USER/Petaverse/RouterDataBase",
            
            // filenames
            "PET_DUMP",                     "pet.dump",
            "PROXY_DATA_FILE",              "proxy.dat",
            "ROUTER_DATA_FILE",             "router.dat",
            "VOCABULARY_FILE",              "petavese.voc",
            "ATOM_SPACE_DUMP",              "atomSpace.dump",
            "COMBO_STDLIB_REPOSITORY_FILE", "stdlib.combo",
            "COMBO_RULES_PRECONDITIONS_REPOSITORY_FILE",
                                            "rules_preconditions.combo",
            "COMBO_SELECT_RULES_PRECONDITIONS_REPOSITORY_FILE",
                                            "rules_preconditions.combo-select",
            "COMBO_RULES_ACTION_SCHEMATA_REPOSITORY_FILE",
                                            "rules_action_schemata.combo",
            
            // Rule Engine parameters
            "RE_CYCLE_PERIOD",              "2", //counted in term multiple idle cycles
            "RE_DEFAULT_MEAN",              "0.5",
            "RE_DEFAULT_COUNT",             "18",      // set to make confidence = 0.9 (see #define KKK
            // in SimpleTruthValue.cc and
            // SimpleTruthvalue::getConfidence()
            "RE_CYCLES_DURING_NOVELTY",     "5",
            "RE_FEELINGS_DECREASE_FACTOR",  "0.01",
            "RE_CYCLES_FOR_REQUESTED_SCHEMA",  
                                            "7",
            "RE_CYCLES_DURING_AGENT_LAST_ACTION",
                                            "3",
            
            // rule engine configurations
            "RE_CORE_FILE",                 "rules_core.lua",
            "RE_DEFAULT_PET_TRAITS",        "maxie",
            "RE_DEFAULT_HUMANOID_TRAITS",   "maria",
            "RE_RULES_FILENAME_MASK",       "%s_rules.lua",
            "RE_TRAITS_FILENAME_MASK",      "%s_traits_%s.lua",
            "RE_PET_DEFINITIONS_FILE",      "petDefinitions.lua",
            
            //about the randomization of schema rule selection
            //0 means, no random noise, the engine chooses the rule with max weight
            //1 means maximal noise
            //and any float in between represents the noise intensity
            "RE_SCHEMA_SELECTION_RANDOM_NOISE",
                                            "0",
            //false means no random selection the first found is taken
            //true means with random selection (uniform here)
            "RE_WILD_CARD_RANDOM_SELECTION","false",
            
            // rule validation configurations
            "RV_CORE_FILE",                 "validation_core.lua",
            
            // Reinforcement learning parameters
            "RL_REWARD",                    "0.05",  // max reward to be applied to a rule implication link
            "RL_PUNISH",                    "0.05",  // max punish to be applied to a rule implication link
            "RL_TIME_WINDOW",               "5.0",   // in secs
            "RL_GAUSSIAN_MEAN",             "2.5",   // in secs
            "RL_GAUSSIAN_STD_DEVIATION",    "0.5",
            
            "MAX_RULE_STRENGTH",            "0.95",   // the maximum strength allowd to a rule via
            // reinforcement learning
            
            // pet commands
            "STOP_LEARNING_CMD",            "stop learning",
            "TRY_SCHEMA_CMD",               "try",
            
            // reward values
            "POSITIVE_REWARD",              "1.0",
            "NEGATIVE_REWARD",              "-1.0",
            
            // Parameter to control the maximal time spent by a procedure to be executed by Procedure Interpreter (in seconds)
            "PROCEDURE_EXECUTION_TIMEOUT",  "90",
            
            // Indicates if walk actions sent to PVP can be canceled so that new navigation plan be sent to PVP when needed.
            "ALLOW_WALKING_CANCELATION",    "false",
            
            //LearningServer parameters
            //number of fitness estimations computed per cycle
            //between 1 and infty
            //the higher the less responsive but the lower the more wasted CPU
            "NUMBER_OF_ESTIMATIONS_PER_CYCLE",
                                            "100",
            
            // SpaceMap grid dimensions. X and Y directions
            "MAP_XDIM",                     "1024",
            "MAP_YDIM",                     "1024",

            "IMPORTANCE_DECAY_ENABLED",     "true",
            "ACTION_SELECTION_ENABLED",     "true",
            "COMBO_INTERPRETER_ENABLED",    "true",
            "SCHEMA_GOAL_MINING_ENABLED",   "true",
            "PROCEDURE_INTERPRETER_ENABLED","true",
            "ENTITY_EXPERIENCE_ENABLED",    "true",

            //true for disabled, false for enabled
            "DISABLE_LOG_OF_PVP_MESSAGES",  "true",
            
            "CHECK_OPC_MEMORY_LEAKS",       "false",
            "CHECK_OPC_MEMORY_USAGE",       "false",
            "VALGRIND_PATH",                "/usr/local/bin/valgrind",
            "MASSIF_DEPTH",                 "30",
            "MASSIF_DETAILED_FREQ",         "10",
            
	    // launching OPC with a debugger
	    // please note that CHECK_OPC_MEMORY_LEAKS, CHECK_OPC_MEMORY_USAGE
	    // and RUN_OPC_DEBUGGER
	    // are all exclusif (only one can be activated at a time)
            "RUN_OPC_DEBUGGER",             "false",
            "OPC_DEBUGGER_PATH",            "/usr/bin/gdb",

	    // Call the debugger with the OPC arguments directly
	    // appended at the end of the command line of the debugger call,
	    // some debuggers, like nemiver, accept
	    // the program to debug's argument in their command line
	    // If it is false then the arguments are printed on the standard
	    // output instead
	    "PASS_OPC_ARG_DEBUGGER_COMMAND","true",
            
            // NetworkElement's message reading parameters
            "UNREAD_MESSAGES_CHECK_INTERVAL",
                                            "10",
            "UNREAD_MESSAGES_RETRIEVAL_LIMIT",
                                            "1", // -1 for unlimited number
	                                         // of retrieved messages
            "NO_ACK_MESSAGES",              "false",
            "WAIT_LISTENER_READY_TIMEOUT",  "60",  // time (in seconds) to wait for socket Listener to be ready
            
            //------------------
            //for LearningServer
            //------------------
            //entropy threshold for the perception filter,
            //between 0 [completely open] and 1 [completely closed]
            "ENTROPY_PERCEPTION_FILTER_THRESHOLD",
                                            "0.01",
            //similarity threshold for the action filter
            //between 0 [completely open] and 1 [completely closed]
            "SIMILARITY_ACTION_FILTER_THRESHOLD",
                                            "0.01",
            //the max size of the action subsequence generated by the ActionFilter
            //if -1 then the max size is maximum
            "ACTION_FILTER_SEQ_MAX",        "2",

            //if true then returns only the sequences of min size (that is 1)
            //and max size (determined by ACTION_FILTER_SEQ_MAX)
            //but not the sequences of size between min and max
            "ACTION_FILTER_ONLY_MIN_MAX",   "true",

            //choose the imitation learning algorithm
            //see static strings defined in the header for the various options
            "IMITATION_LEARNING_ALGORITHM", ImitationLearningAlgo::HillClimbing,
            //"IMITATION_LEARNING_ALGORITHM",  ImitationLearningAlgo::MOSES;
            
            //evanescence delay of has_said perception in time unit
            "HAS_SAID_DELAY",               "200",
            
            //lower bound (high age) of atoms in atomTable
            //it overwrties MIN_STI default value defined in Config.h
            "MIN_STI",                      "-400",
            
            //false with no random operator optimization for NoSpaceLife
            //true with optimization (to avoid Monte Carlos simulations)
            "RANDOM_OPERATOR_OPTIMIZATION", "true",
            
            //that flag indicates to the neighborhood expension of
            //hillclimbing whether (true) or not (false) both branches of
            //a conditional action_boolean_if should be filled at once
            //instead of only one
            "ACTION_BOOLEAN_IF_BOTH_BRANCHES_HC_EXPENSION",
                                            "true",
            
            //depending on the option, in hillclimbing when a new exemplar comes
            //we either get restarted from the best program so far (value false)
            //or from the start (the empty combo_tree) (value true)
            "HC_NEW_EXEMPLAR_INITIALIZES_CENTER",
                                            "true",
            
            //signed integer that indicates the size of
            //1) while operators
            //2) conditional operators
            //3) contin constant
            //that is to favor (little size or negative) or unfavor (large size)
            //in the search process
            "WHILE_OPERATOR_SIZE",          "3",
            "CONDITIONAL_SIZE",             "1",
            "CONTIN_SIZE",                  "0",
            
            //SizePenalty coef A and B, determined using SPCTools
            "SIZE_PENALTY_COEF_A",          "0.03",
            "SIZE_PENALTY_COEF_B",          "0.34",
            
            //Defines the distance (in percentage of the diagonal of the SpaceMap)
            //under which an avatar is considered approaching something
            "DIST_PERCENTAGE_THRESHOLD_WALK_TO_GOTO",
                                            "1.0",
            
            // print logs message in standard io
            "PRINT_LOG_TO_STDOUT",          "false",
            
            //perform type checking after loading procedures
            "TYPE_CHECK_LOADING_PROCEDURES","true",
            "TYPE_CHECK_GENERATED_COMBO",   "true",
            
            "MANUAL_OPC_LAUNCH",            "false",

            "SCHEME_TYPEDEFS_PATH",         "type_constructors.scm",

            "FRAMES_2_RELEX_RULES_FILE",    "frames2relex.rules",

            "NLGEN_SERVER_PORT",            "5555",

            "NLGEN_SERVER_HOST",            "localhost",

            "VISUAL_DEBUGGER_ACTIVE",       "false",
            "VISUAL_DEBUGGER_HOST",         "localhost",
            "VISUAL_DEBUGGER_PORT",         "6000",

            "FRAMES_FILE",                  "frames.list",
            "FRAMES_INHERITANCE_FILE",      "framesInheritance.list",


            "IMPORTANCE_DECAY_CYCLE_PERIOD",      "15",
            "ENTITY_EXPERIENCE_MOMENT_CYCLE_PERIOD", "1",
            
            "DIALOG_CONTROLLERS",           "QuestionAnswering",
            "MEGAHAL_SERVER_HOST",          "localhost",
            "MEGAHAL_SERVER_PORT",          "6001",

            "ALICEBOT_SERVER_HOST",         "localhost",
            "ALICEBOT_SERVER_PORT",         "6002",

            "RAMONA_SERVER_HOST",           "localhost",
            "RAMONA_SERVER_PORT",           "6003",

            //used as halting condition by reset
            "",                             ""
        };
        return defaultConfig;
    }

public:

    // ***********************************************/
    // Constructors/destructors

    ~EmbodimentConfig();
    EmbodimentConfig();

    // Returns a new EmbodimentConfig instance
    static Config* embodimentCreateInstance(void);
    
    // reset configuration to default
    virtual void reset();

}; // class
}  // namespace Control

#endif
