/*
 * opencog/embodiment/Control/OperationalPetController/OPC.h
 *
 * Copyright (C) 2002-2009 Novamente LLC
 * All Rights Reserved
 * Author(s): Carlos Lopes
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

#ifndef OPC_H
#define OPC_H

#include <string>
#include <opencog/persist/file/SavingLoading.h>
#include <opencog/embodiment/Control/MessagingSystem/StringMessage.h>
#include <opencog/embodiment/Control/MessagingSystem/EmbodimentCogServer.h>
#include <opencog/embodiment/Control/PredicateUpdaters/PredicatesUpdater.h>
#include <opencog/embodiment/Control/PerceptionActionInterface/PAI.h>
#include <opencog/embodiment/Control/Procedure/ProcedureRepository.h>
#include <opencog/embodiment/Control/Procedure/ProcedureInterpreter.h>

#include "Pet.h"
#include "PetMessageSender.h"
#include "PVPActionPlanSender.h"

#include <opencog/embodiment/Control/OperationalPetController/ProcedureInterpreterAgent.h>
#include <opencog/embodiment/Control/OperationalPetController/ActionSelectionAgent.h>
#include <opencog/embodiment/Control/OperationalPetController/ImportanceDecayAgent.h>
#include <opencog/embodiment/Control/OperationalPetController/EntityExperienceAgent.h>

#include "RuleEngine.h"

namespace OperationalPetController
{

/* Defines a single factory template to allow insert a same agent
 * multiple times in the Cogserver schedule */
template< typename _Type, typename _BaseType >
class SingletonFactory : public Factory<_Type, _BaseType>
{
public:
    explicit SingletonFactory() : Factory<_Type, _BaseType>() {}
    virtual ~SingletonFactory() {}
    virtual _BaseType* create() const {
        static _BaseType* inst =  new _Type;
        return inst;
    }
};

class OPC : public EmbodimentCogServer
{

private:

    /**
     * Object used to save/load atomSpace dumps. Other componentes that
     * need persistence should extend SavableRepository interface and
     * add thenselfs to this savingLoading object.
     */
    SavingLoading savingLoading;

    /**
     * A repository to all combo and built-ins procedures
     */
    Procedure::ProcedureRepository* procedureRepository;

    /**
     * Component to deal with perceptions and actions from PVP proxy.
     */
    PerceptionActionInterface::PAI * pai;

    /**
     * Store metadata concerning a pet controled by the OPC. Also implements
     * an interface to comunicate with the LS.
     */
    Pet * pet;

    /**
     * Interpreter for the procedures to be executed by the OPC. Can
     * deal with combo and builtin procedures
     */
    Procedure::ProcedureInterpreter * procedureInterpreter;

    /**
     * Message sender for action plans. Used by PAI component
     */
    PVPActionPlanSender * planSender;

    /**
     * Message sender for learning comands, tricks. Used by Pet component
     */
    PetMessageSender * petMessageSender;

    /**
     * Update predicates necessary to the execution of built-ins procedures.
     */
    PredicatesUpdater * predicatesUpdater;

    /** opencog Agents */
    ProcedureInterpreterAgent* procedureInterpreterAgent;
    ImportanceDecayAgent* importanceDecayAgent;
    ActionSelectionAgent* actionSelectionAgent;
    EntityExperienceAgent* entityExperienceAgent;

    RuleEngine* ruleEngine;

    /**
     * Load pet metadata for a given pet.
     *
     * IMPORTANT: This method should be invoke only in OPC start up, i.e.
     * in the constructor.
     */
    void loadPet(const std::string & petId);

    /**
     * Load the AtomTable and other repositories (TimeServer, SpaceServer,
     * ProcedureRepository, etc).  Before invoking this method the
     * SavableRepository objects must be created and registered
     * (this may be done at the constructor).
     *
     * IMPORTANT: This method should be invoke only in OPC start up, i.e.
     * in the constructor.
     */
    void loadAtomSpace(const std::string & petId);

    /**
     * Process messages from the spwaner componente. Usually a SAVE_AND_EXIT
     * one.
     *
     * @param spawnerMessage The spawner message plain text format to be
     *                          processed
     *
     * @return True if the message was correctly processed
     */
    bool processSpawnerMessage(const std::string & spawnerMessage);

    /**
     * Configure the pet objet to be persisted. Actions executed includes:
     * - dropping grabbed itens, if any;
     * - finishing learning sessions, if started
     */
    void adjustPetToBePersisted();

public:

    static opencog::BaseServer* createInstance();

    /**
     * Constructor and destructor
     */
    OPC();
    ~OPC();

    void init(const std::string &myId, const std::string &ip, int portNumber,
              const std::string& petId, const std::string& ownerId,
              const std::string& agentType, const std::string& agentTraits);

    /**
     * Save the OCP state.
     *
     * IMPORTANT: This method should be invoke only on OPC shutdown,
     *            that is, when it receive a shutdown message from the
     *            spawner. Or when an exception has occured and the state
     *            should be persisted.
     */
    void saveState();

    /**
     * @return The Percpetion/Action Interface object used to exchange
     *            (send and receive) data with virtual world.
     */
    PerceptionActionInterface::PAI & getPAI();

    /**
     * Return the pet's cognitive component. This component is responsible
     * for updating the pet AttentionValues and for choosing a schema to
     * be executed.
     *
     * @return The pet's cognitive component.
     */
    Pet & getPet();

    /**
     * Get the RuleEngine associated with the OPC.
     *
     * @return The RuleEngine associated with the OPC.
     */
    RuleEngine & getRuleEngine();

    /**
     * Get the Procedure Interpreter associated with the OPC.
     *
     * @return The ProcedureInterpreter associated with the OPC.
     */
    Procedure::ProcedureInterpreter & getProcedureInterpreter();


    /**
     * Get the Procedure Repository associated with the OPC.
     *
     * @return The ProcedureRepository associated with the OPC.
     */
    Procedure::ProcedureRepository & getProcedureRepository( );


    /**
      * @return A reference to the PVPActionPlanSender of this OPC
      */
    PVPActionPlanSender & getPlanSender();

    /**
     * Method inherited from EmbodimentCogServer 
     */
    bool processNextMessage(MessagingSystem::Message *msg);

    /**
     * Method inherited from EmbodimentCogServer
     */
    void setUp();

    /**
      * Selects a pet schema to be executed. This schema will be interpreted
      * and sent to Proxy via PAI.
      */
    void schemaSelection();

    /**
      * Decays short term importance of all atoms in local AtomTable
      */
    void decayShortTermImportance();

    /**
     * Return the path to a file or a directory
     *
     * @param petId The id of the pet used to produce the file path
     * @param filename The name of the file to get the path
     *
     * @return The string representing the path to the given file
     */
    const std::string getPath(const std::string & petId, const std::string & filename = "");

    SingletonFactory<ProcedureInterpreterAgent, Agent> procedureInterpreterAgentFactory;
    SingletonFactory<ImportanceDecayAgent, Agent> importanceDecayAgentFactory;
    SingletonFactory<ActionSelectionAgent, Agent> actionSelectionAgentFactory;
    SingletonFactory<EntityExperienceAgent, Agent> entityExperienceAgentFactory;

}; // class
}  // namespace

#endif
