/*
 * Author: number
 * Date  : Nov 25, 2015
 */

#ifndef __SERVER_DEBUG_H_H___
#define __SERVER_DEBUG_H_H___

#define __DEBUG__

#ifdef __DEBUG__

#define LOGFATAL(format, ...)   ef::Logger(__FILE__, __LINE__).Fatal(format, ##__VA_ARGS__)
#define LOGERR(format, ...)     ef::Logger(__FILE__, __LINE__).Err(format, ##__VA_ARGS__)
#define LOGWARN(format, ...)    ef::Logger(__FILE__, __LINE__).Warn(format, ##__VA_ARGS__)
#define LOGINFO(format, ...)    ef::Logger(__FILE__, __LINE__).Info(format, ##__VA_ARGS__)
#define LOGDEBUG(format, ...)   ef::Logger(__FILE__, __LINE__).Debug(format, ##__VA_ARGS__)
#define LOGFRAME(format, ...)   ef::Logger(__FILE__, __LINE__).Frame(format, ##__VA_ARGS__)

#else

#define LogFatal
#define LogErr
#define LogWarn
#define LogInfo
#define LogDebug
#define LogFrame

#endif


#endif /* __SERVER_DEBUG_H__ */
