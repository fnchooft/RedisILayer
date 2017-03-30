//!  Redis Integration Class.


#ifndef __REDIS_ILAYER_HELPER_CLASS_H__
#define __REDIS_ILAYER_HELPER_CLASS_H__

// System includes
#include <map>
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

#include <stdint.h>

// External library includes
#include <hiredis.h>
#include <async.h>


namespace Redis {
namespace Integration {


/*!
  This Class provides helpers for the integration of the Redis-datastore in C++ applications.
  It provides helper functions to load scripts from a directory, register these by name
  returning the SHA1-checksum which allows it to call a script via EVALSHA.

  Furthermore it provides a registration-mechanism for asynchronous calls based on libev.
  This allows for the registration of pub-sub or keyspace notifications.
  Every time the pattern matches the callback-function will be called.

*/
class RedisHelper {
  public:
    RedisHelper(const std::string &hostname, uint16_t port, int timeout_usec);
    virtual ~RedisHelper();

    /*
     * @brief This load all scripts in the given path, with an optional pattern
     * @param [in] path path to the script-dir
     * @param [in] pattern glob-pattern , example * db_* etc
     * @param [out] count of loaded scripts
     */
    int loadScriptDir(const std::string &path, const std::string &pattern = "*.*");
    /*
     * @brief This command creates an outgoing optical link
     * ppm3-commandId: PADTEC_LIGHTPAD_INFRA_MANAGER_PPM3_CMD_DELETE_OPTICAL_LINK / 0xA002
     * @param [in] OpticalLinkId object that carries the parameters
     * @param [out] OpticalLinkResult object that is filled on positive response
     */
    std::string getShaByScriptname(const std::string &scriptname);
    std::map<std::string,std::string> getAllScripts(){ return mScripts; }

    std::string loadScript(const std::string &scriptblob);

    int connect();

    // notification type is space,event or *
    // database 0..9
    // prefix matching criteria for which the script should be called.
    int registerCallback(const std::string &notification_type, int database, const std::string &prefix, redisCallbackFn *fn);
    /*
    This function will never return...
    https://redis.io/topics/notifications#configuration
    You might have to create a thread for this...
    */
    int runLoop(const std::string &notification_configuration = "KEA");

    inline redisContext *getRedisContext(){ return mRedisContext; }

    std::string lastError();

    void setPrivateDataPointer(void *priv_data){ mPrivateDataPtr = priv_data; }

  private:
    struct timeval mTimeout;
    redisContext *mRedisContext;
    std::string mHostname;
    uint32_t mTimeout_usec;
    uint16_t mPort;
    std::map<std::string,std::string> mScripts;

    std::map<std::string,redisCallbackFn *> mNotificationPtrs;

    std::string mLastError;

    void* mPrivateDataPtr; //! Pointer to some helper structure.
}; // End of class RedisHelper

} // End of namespace Integration
} // End of namespace Redis

#endif /* __REDIS_ILAYER_HELPER_CLASS_H__ */

