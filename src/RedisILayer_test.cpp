#include "RedisHelper.hpp"
#include <gtest/gtest.h>
#include <sstream>

// http://stackoverflow.com/questions/4668760/converting-an-int-to-stdstring
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
            ( std::ostringstream() << std::dec << x ) ).str()



/*
TEST(RedisILayer, callBackTest){
  SomeObject a;


  Redis::Integration::RedisHelper rnotifier("127.0.0.1",6379,500000);
  rnotifier.setPrivateDataPointer((void*)&a);

  rnotifier.registerCallback("event",0,"OpticalLink:*",monitorOpticalLink);
  rnotifier.registerCallback("event",9,"*",monitorAmplificationSites);
}
*/


TEST(RedisILayer, connectTest){
  // Connect to redis-instance...
  Redis::Integration::RedisHelper rh("127.0.0.1",6379,500000);
  std::cout << "Database connection ... establishing..." << std::endl;
  EXPECT_EQ(rh.connect(),0);
}


// The eval-script has been taken from here: https://redis.io/commands/EVAL
TEST(RedisILayer, loadScripts){
  Redis::Integration::RedisHelper rh("127.0.0.1",6379,500000);
  std::cout << "Database connection ... establishing..." << std::endl;
  EXPECT_EQ(rh.connect(),0);
  EXPECT_EQ(rh.loadScriptDir("./scripts","*.lua"),1);

  EXPECT_EQ(rh.getShaByScriptname("./scripts/lua_test_script.lua"),"a983b50a9921e198c24bb64deef64491362ad575");
}


// Execute the test-script validating the return paramters....
TEST(RedisILayer, testScript){
  Redis::Integration::RedisHelper rh("127.0.0.1",6379,500000);
  std::cout << "Database connection ... establishing..." << std::endl;
  EXPECT_EQ(rh.connect(),0);

  redisReply *reply = (redisReply*)redisCommand(
    rh.getRedisContext(),
    "EVALSHA a983b50a9921e198c24bb64deef64491362ad575  2 key1 key2 first second"
  );
  // The return of the script is an array - check it...
  EXPECT_TRUE(reply->type == REDIS_REPLY_ARRAY);

  if (reply->type == REDIS_REPLY_ARRAY) {
    EXPECT_EQ(reply->elements,4);
    EXPECT_EQ(std::string(reply->element[0]->str),"key1");
    EXPECT_EQ(std::string(reply->element[1]->str),"key2");
    EXPECT_EQ(std::string(reply->element[2]->str),"first");
    EXPECT_EQ(std::string(reply->element[3]->str),"second");
  }
  else
  {
    EXPECT_EQ(1,0);
  }
  freeReplyObject(reply);
}


// Execute the test-script validating the return paramters....
TEST(RedisILayer, databaseSwitchAndEmptyCheck){
  Redis::Integration::RedisHelper rh("127.0.0.1",6379,500000);
  std::cout << "Database connection ... establishing..." << std::endl;
  EXPECT_EQ(rh.connect(),0);

  /* Switch to DB 9 for testing, now that we know we can chat. */
  redisReply *reply = (redisReply*)redisCommand(rh.getRedisContext(),"SELECT 15");
  EXPECT_TRUE(reply != NULL);
  freeReplyObject(reply);
  reply = NULL;

  /* Make sure the DB is empty */
  reply = reply = (redisReply*)redisCommand(rh.getRedisContext(),"DBSIZE");
  EXPECT_TRUE(reply != NULL);
  EXPECT_TRUE(reply->type == REDIS_REPLY_INTEGER);
  EXPECT_TRUE(reply->integer == 0);
  freeReplyObject(reply);
}


// Execute the test-script validating the return paramters....
TEST(RedisILayer, WriteReadDataCheck){
  Redis::Integration::RedisHelper rh("127.0.0.1",6379,500000);
  std::cout << "Database connection ... establishing..." << std::endl;
  EXPECT_EQ(rh.connect(),0);

  /* Switch to DB 9 for testing, now that we know we can chat. */
  redisReply *reply = (redisReply*)redisCommand(rh.getRedisContext(),"SELECT 15");
  EXPECT_TRUE(reply != NULL);
  freeReplyObject(reply);
  reply = NULL;

  int writes = 0;
  for( int i = 1; i <= 100; i++)
  {
    redisReply *reply = (redisReply*)redisCommand(
      rh.getRedisContext(),
      "SET %s-%d %d","RedisILayer",i,i
    );
    if(reply && reply->str == std::string("OK"))

      writes++;
    freeReplyObject(reply);
    reply = NULL;
  }
  EXPECT_EQ(writes,100);

  int validated_reads = 0;
  for (int r = 1; r <= 100; r++)
  {
    redisReply *reply = (redisReply*)redisCommand(
      rh.getRedisContext(),
      "GET %s-%d","RedisILayer",r
    );
    std::string rstr = SSTR(r);
    if(reply && reply->str == rstr)
      validated_reads++;
    freeReplyObject(reply);
    reply = NULL;
  }
  EXPECT_EQ(validated_reads,100);

  /* After testing writing and reading flush the db. */
  reply = (redisReply*)redisCommand(rh.getRedisContext(),"FLUSHDB");
  EXPECT_TRUE(reply != NULL);
  freeReplyObject(reply);
  reply = NULL;
}
