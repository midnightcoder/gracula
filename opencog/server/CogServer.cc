/*
 * opencog/server/CogServer.cc
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * Copyright (C) 2008 by Singularity Institute for Artificial Intelligence
 * All Rights Reserved
 *
 * Written by Andre Senna <senna@vettalabs.com>
 *            Gustavo Gama <gama@vettalabs.com>
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

#include "CogServer.h"

#include <time.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#include <boost/filesystem/operations.hpp>

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/server/Agent.h>
#include <opencog/server/ConsoleSocket.h>
#include <opencog/server/NetworkServer.h>
#include <opencog/server/SystemActivityTable.h>
#include <opencog/server/Request.h>
#include <opencog/server/load-file.h>
#include <opencog/util/Config.h>
#include <opencog/util/Logger.h>
#include <opencog/util/exceptions.h>
#include <opencog/util/misc.h>

#ifdef HAVE_SQL_STORAGE
#include <opencog/persist/sql/PersistModule.h>
#endif /* HAVE_SQL_STORAGE */

using namespace opencog;

namespace opencog {
struct equal_to_id : public std::binary_function<const Agent*, const std::string&, bool>
{
    bool operator()(const Agent* a, const std::string& cid) const {
        return (a->classinfo().id != cid);
    }
};
}

CogServer* CogServer::createInstance() {
    return new CogServer();
}

CogServer::~CogServer()
{
    disableNetworkServer();

    std::vector<std::string> moduleKeys;

    for (ModuleMap::iterator it = modules.begin(); it != modules.end(); ++it)
        moduleKeys.push_back(it->first);

    // unload all modules
    for (std::vector<std::string>::iterator k = moduleKeys.begin(); k != moduleKeys.end(); ++k) {
        // retest the key because it might have been removed already
        ModuleMap::iterator it = modules.find(*k);
        if (it != modules.end()) {
            logger().debug("[CogServer] removing module %s\"", it->first.c_str());
            ModuleData mdata = it->second;

            // cache filename and id to erase the entries from the modules map
            std::string filename = mdata.filename;
            std::string id = mdata.id;

            // invoke the module's unload function
            (*mdata.unloadFunction)(mdata.module);

            // erase the map entries (one with the filename as key, and one with the module)
            // id as key
            modules.erase(filename);
            modules.erase(id);
        }
    }
}

CogServer::CogServer() : cycleCount(1)
{
    delete atomSpace;  // global static, declared in BaseServer.
    atomSpace = new AtomSpace();
    _systemActivityTable.init(this);

    pthread_mutex_init(&messageQueueLock, NULL);
    pthread_mutex_init(&processRequestsLock, NULL);
    pthread_mutex_init(&agentsLock, NULL);
    
    agentsRunning = true;
}

NetworkServer& CogServer::networkServer()
{
    return _networkServer;
}

void CogServer::enableNetworkServer()
{
    // WARN: By using boost::asio, at least one listener must be added to
    // the NetworkServer before starting its thread. Other Listeners may 
    // be added later, though.
    _networkServer.addListener<ConsoleSocket>(config().get_int("SERVER_PORT"));
    _networkServer.start();

}

void CogServer::disableNetworkServer()
{
    _networkServer.stop();
}

SystemActivityTable& CogServer::systemActivityTable()
{
    return _systemActivityTable;
}

void CogServer::serverLoop()
{
    struct timeval timer_start, timer_end;
    time_t elapsed_time;
    time_t cycle_duration = config().get_int("SERVER_CYCLE_DURATION") * 1000;
    bool externalTickMode = config().get_bool("EXTERNAL_TICK_MODE");

    logger().info("opencog server ready.");

    gettimeofday(&timer_start, NULL);
    for (running = true; running;) {
        runLoopStep();
        if (!externalTickMode) {
            // sleep long enough so that the next cycle will only start
            // after config["SERVER_CYCLE_DURATION"] milliseconds
            gettimeofday(&timer_end, NULL);
            elapsed_time = ((timer_end.tv_sec - timer_start.tv_sec) * 1000000) +
                           (timer_end.tv_usec - timer_start.tv_usec);
            if ((cycle_duration - elapsed_time) > 0)
                usleep((unsigned int) (cycle_duration - elapsed_time));
            timer_start = timer_end;
        }
    }
}

void CogServer::runLoopStep(void) 
{
    if (getRequestQueueSize() != 0) {
        processRequests();
    }

    bool runCycle = customLoopRun();

    if (runCycle) {
        if (agentsRunning) {
            processAgents();
        }

        cycleCount++;
        if (cycleCount < 0) cycleCount = 0;
    }

}

bool CogServer::customLoopRun(void) 
{
    return true;
}

void CogServer::processRequests(void)
{
    pthread_mutex_lock(&processRequestsLock);
    Request* request;
    while ((request = popRequest()) != NULL) {
        request->execute();
        delete request;
    }
    pthread_mutex_unlock(&processRequestsLock);
}

void CogServer::runAgent(Agent *agent)
{
    struct timeval timer_start, timer_end;
    size_t mem_start, mem_end;
    size_t atoms_start, atoms_end;
    time_t elapsed_time;
    size_t mem_used, atoms_used;

    gettimeofday(&timer_start, NULL);
    mem_start = getMemUsage();
    atoms_start = atomSpace->getSize();

    agent->resetUtilizedHandleSets();
    agent->run(this);

    gettimeofday(&timer_end, NULL);
    mem_end = getMemUsage();
    atoms_end = atomSpace->getSize();
     
    elapsed_time = ((timer_end.tv_sec - timer_start.tv_sec) 
                                                    * 1000000) +
                    (timer_end.tv_usec - timer_start.tv_usec);
    if (mem_start > mem_end)
        mem_used = 0;
    else
        mem_used = mem_end - mem_start;
    if (atoms_start > atoms_end)
        atoms_used = 0;
    else
        atoms_used = atoms_end - atoms_start;

    _systemActivityTable.logActivity(agent, elapsed_time, mem_used, 
                                            atoms_used);
}

void CogServer::processAgents(void)
{
    pthread_mutex_lock(&agentsLock);
    std::vector<Agent*>::const_iterator it;
    for (it = agents.begin(); it != agents.end(); ++it) {
        Agent* agent = *it;
        if ((cycleCount % agent->frequency()) == 0)
            runAgent(agent);
    }
    pthread_mutex_unlock(&agentsLock);
}

bool CogServer::registerAgent(const std::string& id, AbstractFactory<Agent> const* factory)
{
    return Registry<Agent>::register_(id, factory);
}

bool CogServer::unregisterAgent(const std::string& id)
{
    destroyAllAgents(id);
    return Registry<Agent>::unregister(id);
}

std::list<const char*> CogServer::agentIds() const
{
    return Registry<Agent>::all();
}

Agent* CogServer::createAgent(const std::string& id, const bool start)
{
    Agent* a = Registry<Agent>::create(id);
    if (start) startAgent(a);
    return a; 
}

void CogServer::startAgent(Agent* agent)
{
    pthread_mutex_lock(&agentsLock);
    agents.push_back(agent);
    pthread_mutex_unlock(&agentsLock);
}

void CogServer::stopAgent(Agent* agent)
{
    pthread_mutex_lock(&agentsLock);
    AgentSeq::iterator ai = std::find(agents.begin(), agents.end(), agent); 
    if (ai != agents.end())
        agents.erase(ai);
    pthread_mutex_unlock(&agentsLock);
}

void CogServer::destroyAgent(Agent *agent)
{
    stopAgent(agent);
    delete agent;
}

void CogServer::destroyAllAgents(const std::string& id)
{
    pthread_mutex_lock(&agentsLock);
    // place agents with classinfo().id == id at the end of the container
    std::vector<Agent*>::iterator last = 
        std::partition(agents.begin(), agents.end(),
                       boost::bind(equal_to_id(), _1, id));

    // save the agents that should be deleted on a temporary container
    std::vector<Agent*> to_delete(last, agents.end());

    // remove those agents from the main container
    agents.erase(last, agents.end());

    // remove their activities
    for (size_t n = 0; n < to_delete.size(); n++)
        _systemActivityTable.clearActivity(to_delete[n]);

    // delete the selected agents; NOTE: we must ensure that this is executed
    // after the 'agents.erase' call above, because the agent's destructor might
    // include a recursive call to destroyAllAgents
    std::for_each(to_delete.begin(), to_delete.end(), safe_deleter<Agent>());
    pthread_mutex_unlock(&agentsLock);
}

void CogServer::startAgentLoop(void)
{
    agentsRunning = true;
}

void CogServer::stopAgentLoop(void)
{
    agentsRunning = false;
}

bool CogServer::registerRequest(const std::string& name, AbstractFactory<Request> const* factory)
{
    return Registry<Request>::register_(name, factory);
}

bool CogServer::unregisterRequest(const std::string& name)
{
    return Registry<Request>::unregister(name);
}

Request* CogServer::createRequest(const std::string& name)
{
    return Registry<Request>::create(name);
}

const RequestClassInfo& CogServer::requestInfo(const std::string& name) const
{
    return static_cast<const RequestClassInfo&>(Registry<Request>::classinfo(name));
}

std::list<const char*> CogServer::requestIds() const
{
    return Registry<Request>::all();
}

long CogServer::getCycleCount()
{
    return cycleCount;
}

void CogServer::stop()
{
    running = false;
}

Request* CogServer::popRequest()
{

    Request* request;

    pthread_mutex_lock(&messageQueueLock);
    if (requestQueue.empty()) {
        request = NULL;
    } else {
        request = requestQueue.front();
        requestQueue.pop();
    }
    pthread_mutex_unlock(&messageQueueLock);

    return request;
}

void CogServer::pushRequest(Request* request)
{
    pthread_mutex_lock(&messageQueueLock);
    requestQueue.push(request);
    pthread_mutex_unlock(&messageQueueLock);
}

int CogServer::getRequestQueueSize()
{
    pthread_mutex_lock(&messageQueueLock);
    int size = requestQueue.size();
    pthread_mutex_unlock(&messageQueueLock);
    return size;
}

bool CogServer::loadModule(const std::string& filename)
{
    if (modules.find(filename) !=  modules.end()) {
        logger().info("Module \"%s\" is already loaded.", filename.c_str());
        return false;
    }

    // reset error
    dlerror();

    logger().info("Loading module \"%s\"", filename.c_str());
#ifdef __APPLE__
    // Tell dyld to search runpath
    std::string withRPath("@rpath/");
    withRPath += filename;
    // Check to see if so extension is specified, replace with .dylib if it is.
    if (withRPath.substr(withRPath.size()-3,3) == ".so") {
        withRPath.replace(withRPath.size()-3,3,".dylib");
    }
    void *dynLibrary = dlopen(withRPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#else
    void *dynLibrary = dlopen(filename.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif
    const char* dlsymError = dlerror();
    if ((dynLibrary == NULL) || (dlsymError)) {
        logger().error("Unable to load module \"%s\": %s", filename.c_str(), dlsymError);
        return false;
    }

    // reset error
    dlerror();

    // search for id function
    Module::IdFunction* id_func = (Module::IdFunction*) dlsym(dynLibrary, Module::id_function_name());
    dlsymError = dlerror();
    if (dlsymError) {
        logger().error("Unable to find symbol \"opencog_module_id\": %s (module %s)", dlsymError, filename.c_str());
        return false;
    }

    // get and check module id
    const char *module_id = (*id_func)();
    if (module_id == NULL) {
        logger().error("Invalid module id (module \"%s\")", filename.c_str());
        return false;
    }

    // search for 'load' & 'unload' symbols
    Module::LoadFunction* load_func = (Module::LoadFunction*) dlsym(dynLibrary, Module::load_function_name());
    dlsymError = dlerror();
    if (dlsymError) {
        logger().error("Unable to find symbol \"opencog_module_load\": %s", dlsymError);
        return false;
    }

    Module::UnloadFunction* unload_func = (Module::UnloadFunction*) dlsym(dynLibrary, Module::unload_function_name());
    dlsymError = dlerror();
    if (dlsymError) {
        logger().error("Unable to find symbol \"opencog_module_unload\": %s", dlsymError);
        return false;
    }

    // load and init module
    Module* module = (Module*) (*load_func)();

    // store two entries in the module map:
    //    1: filename => <struct module data>
    //    2: moduleid => <struct module data>
    // we rely on the assumption that no module id will match the filename of
    // another module (and vice-versa). This is probably reasonable since most
    // module filenames should have a .dll or .so suffix, and module ids should
    // (by convention) be prefixed with its class namespace (i.e., "opencog::")
    std::string i = module_id;
    std::string f = filename;
    ModuleData mdata = {module, i, f, load_func, unload_func, dynLibrary};
    modules[i] = mdata;
    modules[f] = mdata;

    // after registration, call the module's init() method
    module->init();
 
    return true;
}

bool CogServer::unloadModule(const std::string& moduleId)
{
    logger().info("[CogServer] unloadModule(%s)", moduleId.c_str());
    ModuleMap::const_iterator it = modules.find(moduleId);
    if (it == modules.end()) {
        logger().info("[CogServer::unloadModule] module \"%s\" is not loaded.", moduleId.c_str());
        return false;
    }
    ModuleData mdata = it->second;

    // cache filename, id and handle
    std::string filename = mdata.filename;
    std::string id       = mdata.id;
    void*       handle   = mdata.handle;

    // invoke the module's unload function
    (*mdata.unloadFunction)(mdata.module);

    // erase the map entries (one with the filename as key, and one with the module
    // id as key
    modules.erase(filename);
    modules.erase(id);

    // unload dynamically loadable library
    logger().info("Unloading module \"%s\"", filename.c_str());

    dlerror(); // reset error
    if (dlclose(handle) != 0) {
        const char* dlsymError = dlerror();
        if (dlsymError) {
            logger().error("Unable to unload module \"%s\": %s", filename.c_str(), dlsymError);
            return false;
        }
    }

    return true;
}

CogServer::ModuleData CogServer::getModuleData(const std::string& moduleId)
{
    ModuleMap::const_iterator it = modules.find(moduleId);
    if (it == modules.end()) {
        logger().info("[CogServer::getModuleData] module \"%s\" was not found.", moduleId.c_str());
        ModuleData nulldata = {NULL, "", "", NULL, NULL, NULL};
        return nulldata;
    }
    return it->second;
}

Module* CogServer::getModule(const std::string& moduleId)
{
    return getModuleData(moduleId).module;
}

void CogServer::loadModules() 
{
    // Load modules specified in the config file
    std::vector<std::string> modules;
    tokenize(config()["MODULES"], std::back_inserter(modules), ", ");
    for (std::vector<std::string>::const_iterator it = modules.begin();
         it != modules.end(); ++it) {
        loadModule(*it);
    }
}

void CogServer::loadSCMModules(const char* config_paths[]) 
{
    // Load scheme modules specified in the config file
    std::vector<std::string> scm_modules;
    tokenize(config()["SCM_PRELOAD"], std::back_inserter(scm_modules), ", ");
#ifdef HAVE_GUILE
    std::vector<std::string>::const_iterator it;
    for (it = scm_modules.begin(); it != scm_modules.end(); ++it)
    {
        int rc = 2;
        const char * mod = (*it).c_str();
        if ( config_paths != NULL ) {
            for (int i = 0; config_paths[i] != NULL; ++i) {
                boost::filesystem::path modulePath(config_paths[i]);
                modulePath /= *it;
                if (boost::filesystem::exists(modulePath)) {
                    mod = modulePath.string().c_str();
                    rc = load_scm_file(mod);
                    if (0 == rc) break;
                }
            }
        } // if
        if (rc)
        {
           logger().error("Failed to load %s: %d %s", 
                 mod, rc, strerror(rc));
        }
        else
        {
            logger().info("Loaded %s", mod);
        }
    }
#else /* HAVE_GUILE */
    logger().warn(
        "Server compiled without SCM support");
#endif /* HAVE_GUILE */
}

void CogServer::openDatabase(void)
{
    // No-op if the user has not configured a storage backend
    if (!config().has("STORAGE")) return;

#ifdef HAVE_SQL_STORAGE
    const std::string &dbname = config()["STORAGE"];
    const std::string &username = config()["STORAGE_USERNAME"];
    const std::string &passwd = config()["STORAGE_PASSWD"];

    std::list<std::string> args;
    args.push_back(dbname);
    args.push_back(username);
    args.push_back(passwd);

    // Do this all very politely, by loading the required module,
    // and then calling methods on it, as needed.
    loadModule("libpersist.so");

    Module *mod = getModule("opencog::PersistModule");
    if (NULL == mod)
    {
        logger().error("Failed to pre-load database, because persist module not found!\n");
        return;
    }
    PersistModule *pm = static_cast<PersistModule *>(mod);
    const std::string &resp = pm->do_open(NULL, args);  

    logger().info("Preload >>%s: %s as user %s\n", 
        resp.c_str(), dbname.c_str(), username.c_str());

#else /* HAVE_SQL_STORAGE */
    logger().warn(
        "Server compiled without database support");
#endif /* HAVE_SQL_STORAGE */
}
