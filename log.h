#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define FILENAME "logfile.log"

#define ERROR_LOG(...) printf("%s(%d) %s[ERROR]:",__FILE__,__LINE__,__FUNCTION__),printf(__VA_ARGS__)
#define INFO_LOG(...) printf("%s(%d) %s[INFO]:",__FILE__,__LINE__,__FUNCTION__),printf(__VA_ARGS__)
#define DEBUG_LOG(...) printf("%s(%d) %s[DEBUG]:",__FILE__,__LINE__,__FUNCTION__),printf(__VA_ARGS__)
#define TRACE_LOG(...) printf("%s(%d) %s[TRACE]:",__FILE__,__LINE__,__FUNCTION__),printf(__VA_ARGS__)


typedef struct Logger{
 FILE *logfile;
 char *logfile_name;
 int loglevel;
 void (*traceLog)(struct Logger*,char *);
 void (*debugLog)(struct Logger*,char*);
 void (*infoLog)(struct Logger*,char *);
 void (*errorLog)(struct Logger*,char *);
 void (*init)(struct Logger*,unsigned int);
}logger;

void logSetting(logger *,unsigned int);
