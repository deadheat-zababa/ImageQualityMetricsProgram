#include"log.h"

#define ERROR 3
#define INFO 2
#define DEBUG 1
#define TRACE 0

void traceLog(logger *this,char *str){
 time_t timer;
 struct tm *date;
 char timelog[256];

 if(this->loglevel==TRACE){
  timer = time(NULL);
  date = localtime(&timer);
  strftime(timelog,sizeof(timelog),"[%Y%x %H:%M:%S]",date);
  TRACE_LOG("%s %s\n",timelog,str);
 }
 
 return;
}

void debugLog(logger *this,char *str){
 time_t timer;
 struct tm *date;
 char timelog[256];

 if(this->loglevel < INFO ){
  timer = time(NULL);
  date = localtime(&timer);
  strftime(timelog,sizeof(timelog),"[%Y%x %H:%M:%S]",date);
 
  DEBUG_LOG("%s %s\n",timelog,str);
 }

 return;
}

void infoLog(logger *this,char *str){
 time_t timer;
 struct tm *date;
 char timelog[256];

 if(this->loglevel < ERROR ){
  timer = time(NULL);
  date = localtime(&timer);
  strftime(timelog,sizeof(timelog),"[%Y%x %H:%M:%S]",date);
 
  INFO_LOG("%s %s\n",timelog,str);
 }

 return;
}

void errorLog(logger *this,char *str){
 time_t timer;
 struct tm *date;
 char timelog[256];

 timer = time(NULL);
 date = localtime(&timer);
 strftime(timelog,sizeof(timelog),"[%Y%x %H:%M:%S]",date);
 
 ERROR_LOG("%s %s\n",timelog,str);

 return;
}

void logSetting(logger *this,unsigned int level){
 this->loglevel = 0;
 
 this->traceLog = traceLog; 
 this->debugLog = debugLog; 
 this->infoLog = infoLog; 
 this->errorLog = errorLog; 

 if(level < TRACE){
  this->loglevel = TRACE;
 }
 else if(level > ERROR){
  this->loglevel = ERROR;
 }
 else{
  this->loglevel = level;
 }
}
