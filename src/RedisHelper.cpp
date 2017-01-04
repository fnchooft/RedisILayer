// Glob-for globVector implementation
#include <glob.h>
// Stream for blob-read
#include <fstream>

// Main include
#include "RedisHelper.hpp"
#include <sstream>
#include <adapters/libev.h>

namespace Redis {
namespace Integration {

// Local helper functions ....
/* http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c */
static std::vector<std::string> globVector(const std::string& pattern){
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    std::vector<std::string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        files.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
} // globVector


static std::string readFileContent(const std::string &filename)
{
  std::ifstream ifs(filename.c_str());
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
  return content;
} // readFileContent


RedisHelper::RedisHelper(const std::string &hostname, uint16_t port, int timeout_usec)
{
    mHostname     = hostname;
    mPort         = port;
 
    mTimeout.tv_sec  = mTimeout_usec / 1000000 ;
    mTimeout.tv_usec = mTimeout_usec % 1000000 ;
 
    mRedisContext    = NULL;
} // RedisHelper::RedisHelper


RedisHelper::~RedisHelper()
{
    if(mRedisContext)
        redisFree(mRedisContext);
} // RedisHelper::~RedisHelper


int RedisHelper::connect()
{
  // If there is trash here we know we tried to connect before...  
  if(mRedisContext != NULL)
  {
    mLastError = "Reconnection needed context not clean";
    return -3;
  }
  
  mRedisContext = redisConnectWithTimeout(mHostname.c_str(), mPort, mTimeout);

  if(mRedisContext == NULL)
  {
    mLastError = "Context could not be created by redisConnectWithTimeout";
    return -2;
  }

    
  if (mRedisContext->err)
  {
    std::ostringstream oss;
    oss << "Context error: " << mRedisContext->errstr;
    mLastError = oss.str();
    redisFree(mRedisContext);
    mRedisContext = NULL;
    return -1;
  }
  return 0;
} // RedisHelper::connect


std::string RedisHelper::lastError()
{
    return mLastError;
}


std::string RedisHelper::loadScript(const std::string &scriptblob)
{
  if(mRedisContext == NULL)
    return std::string("connect-first...");

  std::string script_sha;
  std::string script_data;
  script_data.append( scriptblob );

  redisReply *reply = (redisReply*) redisCommand(mRedisContext, "SCRIPT LOAD %s",script_data.c_str());
  if (reply && reply->type == REDIS_REPLY_STRING)
  {
    script_sha = std::string(reply->str);
    //std::cout << std::endl << "SCRIPT-SHA1: " << script_sha << std::endl;
  }
  freeReplyObject(reply);
  return script_sha;
}


int RedisHelper::loadScriptDir(const std::string &path, const std::string &pattern)
{
  int scripts_loaded = 0;
  std::string path_plus_pattern = path + "/" + pattern;
  std::vector<std::string> files = globVector(path_plus_pattern);

  // For each file found
  //  - Try to load the script
  //    - The result must be a 40-char string with the sha1-checksum
  for( std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
  {
    std::string sha = loadScript(readFileContent(*it));
    std::cout << "\tSHA: [" << sha << "] script-name:[" << *it << "]" << std::endl;
    if(sha.length() == 40)
    {
      mScripts[*it] = sha;
      scripts_loaded++;
    }
  }
  return scripts_loaded;
} // RedisHelper::loadScriptDir


std::string RedisHelper::getShaByScriptname(const std::string &scriptname)
{
   return mScripts[scriptname];
} // RedisHelper::getShaByScriptname


/*
If you delete a key "mykey" two events are published:

PUBLISH __keyspace@0__:mykey del
PUBLISH __keyevent@0__:del mykey
 
Notification types (Key-space , Key-event)
 - Key-space notification
 - Key-event notification

The first kind of event, with keyspace prefix in the channel is called a 
  Key-space notification
, while the second, with the keyevent prefix, is called a 
Key-event notification.


*/ 
int RedisHelper::registerCallback(const std::string &notification_type, int database, const std::string &prefix, redisCallbackFn *fn)
{
    std::ostringstream command;
    command << "PSUBSCRIBE " << "__key" << notification_type << "@" << database << "__:" << prefix;
    mNotificationPtrs[command.str()] = fn;
    return 0;
} // RedisHelper::registerScript


int RedisHelper::runLoop(const std::string &notification_configuration)
{
  // See subscriber example from hiredis / libev ..
  redisAsyncContext *c = redisAsyncConnect(mHostname.c_str(),mPort);
  if (c->err) {
    printf("error: %s\n", c->errstr);
    return 1;
  }
  redisLibevAttach(EV_DEFAULT_ c);
  std::ostringstream config_cmd;
  config_cmd << "CONFIG SET notify-keyspace-events " << notification_configuration;

  redisAsyncCommand(c, NULL, NULL, config_cmd.str().c_str());
  for(std::map<std::string, redisCallbackFn *>::iterator it = mNotificationPtrs.begin(); it != mNotificationPtrs.end(); it++)
  {
    std::cout << "Registering callback for: [" << it->first << "]" << std::endl;
    redisAsyncCommand(c, it->second, NULL, it->first.c_str());
  }
  ev_loop(EV_DEFAULT_ 0);
  return 0;  
} // RedisHelper::runLoop



} // End of namespace Integration
} // End of namespace Redis


