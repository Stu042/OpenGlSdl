//
// Created by stu on 31/07/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <GL/glew.h>
#include "Log.h"
#include "Std.h"


#define MAX_TIMESTAMP_LENGTH (40)


FILE *LogFile;																/* The File For Error Logging */
LOGLEVEL LogLevel = LOG_UNKNOWN;											/* Level of logging */


const char APP_NAME[] = "OpenGlSdl";
const char APP_VERSION[] = "0.1";
const char *LOG_FILE_PATHNAME = "OpenGlSdl.log";


const char *LogLevelStr[LOG_LEVEL_COUNT] = {
		"UNKNOWN LOG LEVEL: ",
		"DEBUG: ",
		"INFO: ",
		"WARN: ",
		"ERROR: ",
		"FATAL: "
};


/* Initialises Error Logging. */
void LogInit(void) {
	if (!(LogFile = fopen(LOG_FILE_PATHNAME, "w"))) {		/* If We Can't Open LOG_FILE For Writing */
		perror("Can't init Logfile!");									/* Report With perror() (Standard + Explains Cause Of The Error) */
		exit(EXIT_FAILURE);											/* And Exit, This Is Critical, We Want Logging */
	}
	LogInfo("%s V%s -- Log Init...", APP_NAME, APP_VERSION);		/* Print The Name Of The App In The Log */
}


/* Closes Error Logging */
void LogEnd(void) {
	LogInfo("...Closing Log\n");									/* Print The End Mark */
	if (NULL != LogFile) {													/* If The File Is Open */
		fclose(LogFile);											/* Close It */
		LogFile = NULL;														/* be safe */
	}
}


// output the start of a log line
void LogStart(LOGLEVEL level, const char *file, int line) {
	if ((LogLevel <= level) && LogFile) {
		char time[MAX_TIMESTAMP_LENGTH];
		Timestamp(time, MAX_TIMESTAMP_LENGTH);
		fprintf(LogFile, "%s %sFile: %s, Line %d, ", time, LogLevelStr[level], file, line);
	}
}

// log the description part of the line
void LogDescription(LOGLEVEL level, const char *format, ...) {
	if ((LogLevel <= level) && LogFile) {
		va_list args;
		va_start(args, format);
		vfprintf(LogFile, format, args);
		va_end(args);
		fputc('\n', LogFile);
		fflush(LogFile);
	}
}

void LogStartDescription(LOGLEVEL level, const char *file, int line, const char *format, ...) {
	LogStart(level, file, line);
	if ((LogLevel <= level) && LogFile) {
		va_list args;
		va_start(args, format);
		vfprintf(LogFile, format, args);
		va_end(args);
		fputc('\n', LogFile);
		fflush(LogFile);
	}
}


void LogVA(LOGLEVEL level, const char *szFormat, va_list arg) {
	//va_list a = arg;
	char time[MAX_TIMESTAMP_LENGTH];
	Timestamp(time, MAX_TIMESTAMP_LENGTH);
	fprintf(LogFile, "%s %s", time, LogLevelStr[level]);
	vfprintf(LogFile, szFormat, arg);
	fputc('\n', LogFile);
	fflush(LogFile);
	//if (LogLevel == LOG_DEBUG) {
	//	vprintf(szFormat, arg);
	//	fputc('\n', stdout);
	//	fflush(stdout);
	//}
}


/* Add A Line To The Log */
void Log(LOGLEVEL level, const char *szFormat, ...) {
	va_list arg;                                                      /* We're Using The Same As The printf() Family, A va_list */
	va_start(arg, szFormat);                              /* We Start The List */
	if ((LogLevel <= level) &&
		LogFile) {     /* If The Log Is Open and requested to log in a level we are accepting... */
		LogVA(level, szFormat,
			  arg);                        /* And Ensure The Line Is Written, The Log Must Be Quick */
	}
	va_end(arg);                                                /* We End The List */
	if (LOG_FATAL == level) {
		exit(EXIT_FAILURE);
	}
}

void LogDebug(const char *szFormat, ...) {
	LOGLEVEL level = LOG_DEBUG;
	va_list arg;                                                /* We're Using The Same As The printf() Family, A va_list */
	va_start(arg, szFormat);                              /* We Start The List */
	if ((LogLevel <= level) &&
		LogFile) {      /* If The Log Is Open and requested to log in a level we are accepting... */
		LogVA(level, szFormat,
			  arg);                        /* And Ensure The Line Is Written, The Log Must Be Quick */
	}
	va_end(arg);                                                /* We End The List */
}

void LogInfo(const char *szFormat, ...) {
	LOGLEVEL level = LOG_INFO;
	va_list arg;                                                /* We're Using The Same As The printf() Family, A va_list */
	va_start(arg, szFormat);                              /* We Start The List */
	if ((LogLevel <= level) &&
		LogFile) {      /* If The Log Is Open and requested to log in a level we are accepting... */
		LogVA(level, szFormat,
			  arg);                        /* And Ensure The Line Is Written, The Log Must Be Quick */
	}
	va_end(arg);                                                /* We End The List */
}

void LogWarn(const char *szFormat, ...) {
	LOGLEVEL level = LOG_WARN;
	va_list arg;                                                /* We're Using The Same As The printf() Family, A va_list */
	va_start(arg, szFormat);                              /* We Start The List */
	if ((LogLevel <= level) &&
		LogFile) {      /* If The Log Is Open and requested to log in a level we are accepting... */
		LogVA(level, szFormat,
			  arg);                        /* And Ensure The Line Is Written, The Log Must Be Quick */
	}
	va_end(arg);                                                /* We End The List */
}

void LogError(const char *szFormat, ...) {
	LOGLEVEL level = LOG_ERROR;
	va_list arg;                                                /* We're Using The Same As The printf() Family, A va_list */
	va_start(arg, szFormat);                              /* We Start The List */
	if ((LogLevel <= level) &&
		LogFile) {      /* If The Log Is Open and requested to log in a level we are accepting... */
		LogVA(level, szFormat,
			  arg);                        /* And Ensure The Line Is Written, The Log Must Be Quick */
	}
	va_end(arg);                                                /* We End The List */
}

void LogFatal(const char *szFormat, ...) {
	LOGLEVEL level = LOG_FATAL;
	va_list arg;                                                /* We're Using The Same As The printf() Family, A va_list */
	va_start(arg, szFormat);                              /* We Start The List */
	if ((LogLevel <= level) &&
		LogFile) {      /* If The Log Is Open and requested to log in a level we are accepting... */
		LogVA(level, szFormat,
			  arg);                        /* And Ensure The Line Is Written, The Log Must Be Quick */
	}
	va_end(arg);                                                /* We End The List */
	exit(EXIT_FAILURE);
}


void LogSetLevel(LOGLEVEL level) {
	if ((level > LOG_UNKNOWN) && (level < LOG_LEVEL_COUNT)) {
		LogLevel = level;
	}
}


int LogIsDebug(void) {
	return LogLevel == LOG_DEBUG;
}


void LogGLParams(void) {
	GLenum params[] = {
			GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			GL_MAX_DRAW_BUFFERS,
			GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
			GL_MAX_TEXTURE_IMAGE_UNITS,
			GL_MAX_TEXTURE_SIZE,
			GL_MAX_VARYING_VECTORS,
			GL_MAX_VERTEX_ATTRIBS,
			GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
			GL_MAX_VERTEX_UNIFORM_COMPONENTS,
			GL_MAX_VIEWPORT_DIMS,
			GL_STEREO,
	};
	const char *names[] = {
			"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
			"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
			"GL_MAX_DRAW_BUFFERS",
			"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
			"GL_MAX_TEXTURE_IMAGE_UNITS",
			"GL_MAX_TEXTURE_SIZE",
			"GL_MAX_VARYING_VECTORS",
			"GL_MAX_VERTEX_ATTRIBS",
			"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
			"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
			"GL_MAX_VIEWPORT_DIMS",
			"GL_STEREO",
	};
	LogInfo("GL Context Params:");
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv(params[i], &v);
		LogInfo("%s %i", names[i], v);
		LogGLDebug("In LogGLParams() loop");
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	LogInfo("%s %i %i", names[10], v[0], v[1]);
	LogGLDebug("In LogGLParams()");
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	LogInfo("%s %i", names[11], (unsigned int) s);
	LogGLDebug("In LogGLParams() loop");
	LogInfo("-----------------------------");
}


const char *glErrorString(GLenum error) {
	switch (error) {
		case GL_INVALID_ENUM:
			//An unacceptable value is specified for an enumerated argument.
			//The offending command is ignored
			//and has no other side effect than to set the error flag.
			return "Invalid enum.";
		case GL_INVALID_VALUE:
			//A numeric argument is out of range.
			//The offending command is ignored
			//and has no other side effect than to set the error flag.
			return "Invalid value.";
		case GL_INVALID_OPERATION:
			//The specified operation is not allowed in the current state.
			//The offending command is ignored
			//and has no other side effect than to set the error flag.
			return "Invalid operation.";
		case GL_STACK_OVERFLOW:
			//This command would cause a stack overflow.
			//The offending command is ignored
			//and has no other side effect than to set the error flag.
			return "Stack overflow.";
		case GL_STACK_UNDERFLOW:
			//This command would cause a stack underflow.
			//The offending command is ignored
			//and has no other side effect than to set the error flag.
			return "Stack underflow";
		case GL_OUT_OF_MEMORY:
			//There is not enough memory left to execute the command.
			//The state of the GL is undefined,
			//except for the state of the error flags,
			//after this error is recorded.
			return "Out of memory";
		case GL_TABLE_TOO_LARGE:
			//The specified table exceeds the implementation's maximum supported table
			//size.  The offending command is ignored and has no other side effect
			//than to set the error flag.
			return "Table is too large";
	}
	//case GL_NO_ERROR:
	//No error has been recorded.
	//The value of this symbolic constant is guaranteed to be 0.
	return "No error.";
}


/* Log a GL error */
void LogGlVa(const char *format, va_list args) {
	GLenum error;
	LOGLEVEL level;
	char buffer[256];
	error = glGetError();
	if (error != GL_NO_ERROR) {
		if (error == GL_OUT_OF_MEMORY) {
			level = LOG_FATAL;
		} else {
			level = LOG_WARN;
		}
		vsnprintf(buffer, 256, format, args);
		Log(level, "GLError:%d, %s, %s", error, buffer, glErrorString(error));
	}
}


/* Log a GL error IF in debug mode */
void LogGLDebug(const char *format, ...) {
	if (LogIsDebug()) {
		va_list args;
		va_start(args, format);
		LogGlVa(format, args);
		va_end(args);
	}
}


/* Log a GL error */
void LogGL(const char *format, ...) {
	va_list args;
	va_start(args, format);
	LogGlVa(format, args);
	va_end(args);
}


/* Log description of a GL error */
void LogGlVaDescription(const char *format, va_list args) {
	GLenum error;
	LOGLEVEL level;
	char buffer[256];
	error = glGetError();
	if (error != GL_NO_ERROR) {
		if (error != GL_OUT_OF_MEMORY) {
			level = LOG_FATAL;
		} else {
			level = LOG_WARN;
		}
		vsnprintf(buffer, 256, format, args);
		LogDescription(level, "%s, %s", buffer, glErrorString(error));
	}
}


/* Log a GL error IF in debug mode and file name and line nbr */
void LogGLDebugFL(const char *file, int line, const char *format, ...) {
	if (LogIsDebug()) {
		GLenum error;
		error = glGetError();
		if (error != GL_NO_ERROR) {
			LOGLEVEL level;
			va_list args;
			va_start(args, format);
			LogStart(LOG_WARN, file, line);
			char buffer[256];
			if (GL_OUT_OF_MEMORY == error) {
				level = LOG_FATAL;
			} else {
				level = LOG_WARN;
			}
			vsnprintf(buffer, 256, format, args);
			va_end(args);
			LogDescription(level, "%s, %s", buffer, glErrorString(error));
		} else {
			va_list args;
			va_start(args, format);
			LogVA(LOG_DEBUG, format, args);
			va_end(args);
		}
	}
}


/* Log a GL error and file name and line nbr */
void LogGlFl(const char *file, int line, const char *format, ...) {
	GLenum error;
	error = glGetError();
	if (error != GL_NO_ERROR) {
		LOGLEVEL level;
		va_list args;
		va_start(args, format);
		LogStart(LOG_WARN, file, line);
		char buffer[256];
		if (error == GL_OUT_OF_MEMORY) {
			level = LOG_FATAL;
		} else {
			level = LOG_WARN;
		}
		vsnprintf(buffer, 256, format, args);
		va_end(args);
		LogDescription(level, "%s, %s", buffer, glErrorString(error));
	}
}

