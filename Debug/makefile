################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include tests/reasoning/pln/unused/subdir.mk
-include tests/reasoning/subdir.mk
-include tests/persist/xml/subdir.mk
-include tests/embodiment/Control/Procedure/BuiltIn/subdir.mk
-include tests/embodiment/Control/PerceptionActionInterface/subdir.mk
-include opencog/web/subdir.mk
-include opencog/web/json_spirit/subdir.mk
-include opencog/util/subdir.mk
-include opencog/ubigraph/subdir.mk
-include opencog/spatial/math/subdir.mk
-include opencog/spatial/subdir.mk
-include opencog/shell/subdir.mk
-include opencog/server/subdir.mk
-include opencog/reasoning/pln/utils/subdir.mk
-include opencog/reasoning/pln/unused/utils/subdir.mk
-include opencog/reasoning/pln/unused/subdir.mk
-include opencog/reasoning/pln/rules/unify/subdir.mk
-include opencog/reasoning/pln/rules/simsubst/subdir.mk
-include opencog/reasoning/pln/rules/or/subdir.mk
-include opencog/reasoning/pln/rules/not/subdir.mk
-include opencog/reasoning/pln/rules/inference/subdir.mk
-include opencog/reasoning/pln/rules/implication/subdir.mk
-include opencog/reasoning/pln/rules/convert/subdir.mk
-include opencog/reasoning/pln/rules/auxiliary/subdir.mk
-include opencog/reasoning/pln/rules/and/subdir.mk
-include opencog/reasoning/pln/rules/subdir.mk
-include opencog/reasoning/pln/formulas/subdir.mk
-include opencog/reasoning/pln/subdir.mk
-include opencog/query/subdir.mk
-include opencog/python/util/subdir.mk
-include opencog/python/server/subdir.mk
-include opencog/python/subdir.mk
-include opencog/python/examples/hopfield/subdir.mk
-include opencog/python/examples/hello/subdir.mk
-include opencog/python/examples/custom_string/subdir.mk
-include opencog/python/dynamics/subdir.mk
-include opencog/python/dynamics/attention/subdir.mk
-include opencog/python/atomspace/subdir.mk
-include opencog/persist/xml/subdir.mk
-include opencog/persist/sql/subdir.mk
-include opencog/persist/memcache/subdir.mk
-include opencog/persist/hypertable/subdir.mk
-include opencog/persist/file/subdir.mk
-include opencog/nlp/wsd/subdir.mk
-include opencog/nlp/wordnet-import/subdir.mk
-include opencog/nlp/types/subdir.mk
-include opencog/nlp/question/subdir.mk
-include opencog/nlp/chatbot/subdir.mk
-include opencog/learning/moses/moses/subdir.mk
-include opencog/learning/moses/main/subdir.mk
-include opencog/learning/moses/eda/subdir.mk
-include opencog/learning/hillclimbing/main/subdir.mk
-include opencog/learning/hillclimbing/subdir.mk
-include opencog/guile/subdir.mk
-include opencog/embodiment/WorldWrapper/subdir.mk
-include opencog/embodiment/RuleValidation/VirtualWorldData/subdir.mk
-include opencog/embodiment/RuleValidation/Processor/subdir.mk
-include opencog/embodiment/PetComboVocabulary/subdir.mk
-include opencog/embodiment/Learning/main/subdir.mk
-include opencog/embodiment/Learning/behavior/subdir.mk
-include opencog/embodiment/Learning/RewritingRules/subdir.mk
-include opencog/embodiment/Learning/PetaverseMOSES/subdir.mk
-include opencog/embodiment/Learning/PetaverseHC/subdir.mk
-include opencog/embodiment/Learning/NoSpaceLife/subdir.mk
-include opencog/embodiment/Learning/LearningServerMessages/subdir.mk
-include opencog/embodiment/Learning/LearningServer/subdir.mk
-include opencog/embodiment/Learning/FitnessEstimator/subdir.mk
-include opencog/embodiment/Learning/Filter/subdir.mk
-include opencog/embodiment/Control/Procedure/subdir.mk
-include opencog/embodiment/Control/Procedure/BuiltIn/subdir.mk
-include opencog/embodiment/Control/PredicateUpdaters/subdir.mk
-include opencog/embodiment/Control/PerceptionActionInterface/subdir.mk
-include opencog/embodiment/Control/OperationalPetController/subdir.mk
-include opencog/embodiment/Control/MessagingSystem/subdir.mk
-include opencog/embodiment/Control/subdir.mk
-include opencog/embodiment/AutomatedSystemTest/subdir.mk
-include opencog/embodiment/AtomSpaceExtensions/subdir.mk
-include opencog/dynamics/attention/subdir.mk
-include opencog/dotty/subdir.mk
-include opencog/comboreduct/reduct/subdir.mk
-include opencog/comboreduct/main/subdir.mk
-include opencog/comboreduct/crutil/subdir.mk
-include opencog/comboreduct/combo/subdir.mk
-include opencog/comboreduct/ant_combo_vocabulary/subdir.mk
-include opencog/atomspace/subdir.mk
-include opencog/adaptors/tulip/subdir.mk
-include examples/server/subdir.mk
-include examples/modules/subdir.mk
-include examples/hopfield/subdir.mk
-include examples/atomtypes/subdir.mk
-include CMakeFiles/CompilerIdCXX/subdir.mk
-include CMakeFiles/CompilerIdC/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: opencog

# Tool invocations
opencog: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/home/bruce/code/AI/opencog/bin/opencog/reasoning -o"opencog" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) opencog
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
