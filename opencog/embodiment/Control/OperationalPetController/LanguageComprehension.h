/*
 * opencog/embodiment/Control/OperationalPetController/LanguageComprehension.h
 *
 * Copyright (C) 2009-2010 Novamente LLC
 * All Rights Reserved
 * Author(s): Samir Araujo
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

#ifndef LANGUAGECOMPREHENSION_H
#define LANGUAGECOMPREHENSION_H

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/embodiment/AtomSpaceExtensions/AtomSpaceUtil.h>
#include <opencog/embodiment/Control/PetInterface.h>
#include <opencog/embodiment/Control/OperationalPetController/OutputRelex.h>
#include <opencog/embodiment/Control/OperationalPetController/FramesToRelexRuleEngine.h>
#include <opencog/embodiment/Control/OperationalPetController/NLGenClient.h>

#include <opencog/guile/SchemeEval.h>

namespace OperationalPetController
{

    /**
     * LanguageComprehension is an OPC module responsible
     * for the management of dialogs between agents
     */
    class LanguageComprehension 
    {
    public: 
        
        /**
         * A DialogController is an element that can
         * produce sentences for a dialog session.
         * A Sentence might be just an answer for a given 
         * question or an spontaneous speech.
         */
        class DialogController 
        {
        public:

            /**
             * Result is a helper plain data object
             * used to hold the information returned by
             * un update step of the DialogController
             */
            class Result 
            {
            public:
                Result( void ) : 
                    sentence(""), questionAnswering(false), 
                        heardSentence(""), status(false) { }

                virtual ~Result( void ) { }
                
                std::string sentence;
                bool questionAnswering;
                std::string heardSentence;
                bool status;
            };

            DialogController( const std::string& name, LanguageComprehension* langComp ) : 
                name( name ), langComp(langComp), agent( &langComp->getAgent( ) )
            { 
                this->agentHandle = 
                    AtomSpaceUtil::getAgentHandle( agent->getAtomSpace( ), agent->getPetId( ) );

            }

            virtual ~DialogController( void ) { }

            inline const std::string& getName( void ) const {
                return this->name;
            }

            /**
             * It must be implemented by the concrete DialogController
             *
             * @param elapsedTime The current system timestamp (reference: PAI)
             * @return Result the resulting information after processing the DC
             */
            virtual Result processSentence( long elapsedTime ) = 0;

        protected:
            std::string name;
            LanguageComprehension* langComp;
            Control::PetInterface* agent;
            Handle agentHandle;
        };

        LanguageComprehension( Control::PetInterface& agent );
        
        virtual ~LanguageComprehension( void );        
        
        void resolveLatestSentenceReference( void );

        void resolveLatestSentenceCommand( void );
        
        /**
         * Store a new fact, extracted from a sentence
         * sent by a trusty agent
         */
        void storeFact( void );

        /**
         * Given a list of Frames, stored as a Predicate into the AtomTable,
         * this method will try to convert them into RelEx format and then
         * will call relex2Sentence to try to retrieve an English sentence
         * 
         * @return An English sentence translated from the Frames list
         */
        std::string resolveFrames2Relex( );
        
        /**
         * This is a helper function that retrieves the list of all
         * arguments of predicate, given its name
         *
         * @param predicateName The predicate name whose its arguments are desired.
         * @return A HandleSeq containing the arguments handles
         */
        HandleSeq getActivePredicateArguments( const std::string& predicateName );

        /**
         * A Factory Method that, given a DC name, instantiate a specific
         * Dialog Controller and return a memory pointer of it.
         *
         * @param name The name of the Dialog Controller
         */
        DialogController* createDialogController( const std::string& name );

        /**
         * Getter for the agent interface
         */
        inline Control::PetInterface& getAgent( void )
        {
            return this->agent;
        }

        /**
         * Responsible for updating all Dialog Controllers at 
         * each cycle
         *
         * @param elapsedTime The current system timestamp (reference: PAI)
         */
        void updateDialogControllers( long elapsedTime );

        /**
         * When an angent talk with other one a sentence is heard
         * by the listener. This methods returns a HandleSeq 
         * containing the most recent heard sentences in predicate format
         * i.e.
         * EvaluationLink
         *    PredicateNode "heard_sentence"
         *    ListLink
         *       SentenceNode "to:id_0001: What is your name?"
         *
         * @return HandleSeq containing the EvaluationLinks of the predicates
         */
        HandleSeq getHeardSentencePredicates( void );

        /**
         * Giving a predicate that contains just a simple SentenceNode
         * as argument, this method returns the text of the sentence.
         * i.e
         * EvaluationLink
         *    PredicateNode "heard_sentence"
         *    ListLink
         *       SentenceNode "to:id_0001: What is your name?"
         *
         * and the answer will be string(What is your name?)
         *
         * @param Handle EvaluationLink of the Predicate
         * @return a string containing the text of the sentence
         */
        std::string getTextFromSentencePredicate( Handle evalLink );

    protected:

        /**
         * This method converts a string containing a sentence formated
         * in Relex to English, by using NLGen.
         *
         * @param relexInput A sentence formated in relex
         * @return An English version of the given sentence
         */
        std::string resolveRelex2Sentence( const std::string& relexInput );

        void init(void);
        void loadFrames(void);

#ifdef HAVE_GUILE
        /** The static elements below will be replaced by a SchemePrimitive soon **/
        static SCM execute(SCM objectObserver, SCM figureSemeNode, SCM groundSemeNode, SCM ground2SemeNode );
#endif
        static void createFrameInstancesFromRelations( AtomSpace& atomSpace, HandleSeq& resultingFrames,
                                                       const std::list<spatial::Entity::SPATIAL_RELATION>& relations,
                                                       const std::string& objectA, const std::string& objectB, const std::string& objectC );
        static Control::PetInterface* localAgent;
        
        Control::PetInterface& agent;
        std::string nlgen_server_host;
        int nlgen_server_port;
        FramesToRelexRuleEngine framesToRelexRuleEngine;
        NLGenClient *nlgenClient;
        bool initialized;

        /** Dialog Controllers methods **/

        /**
         * Register a new Dialog Controller
         */
        void addDialogController( DialogController* dialogController );
        /**
         * Read from the config file which DCs must be loaded
         * and register them
         */
        void loadDialogControllers( void );

        std::list<DialogController* > dialogControllers;
        
    };

};

#endif // LANGUAGECOMPREHENSION_H
