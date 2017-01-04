## Redis Intergration Layer

This repository contains a Helper-class in C++ in order to integrate the Redis-datastore in C++ applications.

The code depends on two third-party libraries:
 - [libev](http://github/fnchooft/libev)
 - [hiredis](http://github/fnchooft/hiredis)

Following features are supported:
 - Load scripts from a given ```scriptdir```.
 - Register call back functions for pub-sub or keyspace-notifications
 - Simple and complete interface to the RedisContext from hiredis.

## Code Example

In order to get a feeling for how the [hiredis-client](https://raw.githubusercontent.com/redis) works read its [README.md](https://raw.githubusercontent.com/redis/hiredis/master/README.md).

### Load scripts from a directory
```c
// Connect to redis-instance...
Redis::Integration::RedisHelper rh("127.0.0.1",6379,500000);
std::cout << "Database connection ... establishing..." << std::endl;
if(rh.connect() != 0)
{
	std::cout << "Redis Connection Failed - Fatal Error" << std::endl;
	exit(1);
}
std::cout << "Connected to database - preparing environment..." << std::endl;
int scripts_loaded = rh.loadScriptDir("./scripts","*.lua");
std::cout << "Loaded " << scripts_loaded << " db-scripts." << std::endl;

```

### Save a key in the datastore
```c
// Use the RedisHelper to connect to Redis...
Redis::Integration::RedisHelper rh("127.0.0.1",6379,500000);
int connection_status = rh.connect();
if(connection_status != 0)
{
  std::cout << rh.lastError() << std::endl;
}
else
{
  // Call the Script by name and pass the parameters
  redisReply *reply = (redisReply*)redisCommand(
    rh.getRedisContext(),
    "EVALSHA f49ae33bd0d21475b52cdf677bc762f5cc0f856f 2 SomeEntity %d-%d %s",
    input.getA(),
    input.getB(),
    input.toJsonString().c_str()
);
freeReplyObject(reply);
// RedisHelper will destroy the Context upon Destruction
}
```



### Register a Callback for a key-space notification
```c
// Define the callback-function.....
void triggerOnSomePrefixEvents(redisAsyncContext *c, void *reply, void *privdata) {
    redisReply *r = (redisReply *)reply;
    if (reply == NULL) return;

    printf("SomePrefix: Got a message... of type: %d\n",r->type);
    if (r->type == REDIS_REPLY_ARRAY) {
        unsigned int j = 0;
        for (j = 0; j < r->elements; j++) {
            printf("%u) %s\n", j, r->element[j]->str);
        }
    }

    if(r->type == REDIS_REPLY_STRING)
      printf("%s\n",r->str);
}

Redis::Integration::RedisHelper rnotifier("127.0.0.1",6379,500000);
// EA - see explanation at https://redis.io/topics/notifications#configuration
rnotifier.registerCallback("event",0,"SomePrefix:*",triggerOnSomePrefixEvents);
rnotifier.runLoop("EA");
```

## Motivation

After seeing some other client-[packages](https://redis.io/clients) such as [redis3m](https://github.com/luca3m/redis3m) which are really good, a simpler interface was needed for some projects.

``You should look at those first``, since we wanted a library which could run on very limited hardware and for these platforms we do not have recent compilers supporting C11 etc, this stripped down version was created.

## Installation

This library is compiled by using autoconf and pkg-config.
The library runs on:
 - Ubuntu (debian)
   - 12.04 / 14.04 / 16.04

The basic compile steps are:
 - prep your environment
 - compile sub-package
   - or install via ```sudo apt-get install libev hiredis-dev```
 - compile this-package
 - for more inspiration look at the [travis-configuration](.travis.yml)

```bash
export BD=$BUILD_DIR_WHERE_EVER_YOU_WANT
export PATH=$BD/bin:$PATH
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$BD/share/pkgconfig:$BD/lib/pkgconfig:
./autogen.sh --prefix $BD
make
make check
make install
```
## Tests
```bash
make check
```

## Examples
See the [examples](examples)-directory for use cases.  

## Contributors
 - [Fabian N.C. van 't Hooft](fnchooft@gmail.com)


## License
 - [libev-LICENSE](https://github.com/enki/libev/blob/master/)
 - [hiredis-COPYING](https://github.com/redis/hiredis/blob/master/COPYING)
