#ifndef NOB_H_
#define NOB_H_

#include<assert.h>
#include<stdbool.h>
#include<errno.h>
//#include<string.h>
#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<synchapi.h>
#include<time.h>
#include<sys/stat.h>

#ifdef _WIN32
#include<windows.h>
#include<WinBase.h>
#include<tchar.h>
#include<strsafe.h>
#endif

typedef enum {
	NOB_INFO,
	NOB_WARNING,
	NOB_ERROR,
} Nob_log_level;

void nob_log(Nob_log_level level, const char* fmt, ...);

typedef struct {
	const char** items;
	size_t count;
	size_t capacity;
} nob_Cmd;

typedef struct {
	char* items;
	size_t count;
	size_t capacity;
} nob_String_Builder;

const char* nob_string_builder_to_cstr(nob_String_Builder sb);

#define NOB_DA_INITIAL_CAP 256

#define nob_dynamic_array_append(da, item)														\
	{																						\
		if ((da)->count >= (da)->capacity) {												\
			(da)->capacity = (da)->capacity == 0 ? NOB_DA_INITIAL_CAP : (da)->capacity * 2;	\
			(da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));		\
			assert((da)->items != NULL && "RAM is full\n");									\
		}																					\
		(da)->items[(da)->count] = (item);													\
		(da)->count += 1;																	\
	};


#define nob_string_builder_append_char(sb, ch) nob_dynamic_array_append(sb, ch)
void nob_string_builder_append_string(nob_String_Builder* sb, const char* string);
#define nob_string_builder_append_strings_va(sb, ...) nob_string_builder_append_strings_va_null(sb, __VA_ARGS__, NULL)
void nob_string_builder_append_strings_va_null(nob_String_Builder* sb, ...);


#ifdef _WIN32
typedef HANDLE Pid;
typedef HANDLE Fd;
#else
typedef int Pid;
#endif // _WIN32

const char* nob_GetLastErrorAsString(void);

const char* nob_cmd_join(const char* joiner, nob_Cmd cmd);
void nob_cmd_show(nob_Cmd cmd);
void nob_nob_cmd_append_null(nob_Cmd* cmd, ...);
#define nob_cmd_append(cmd, ...) nob_nob_cmd_append_null(cmd, __VA_ARGS__, NULL)
Pid nob_cmd_run_async(nob_Cmd cmd, Fd* fdin, Fd* fdout);

#define NOB_SYNC_PROCESS_TIMEOUT_MILISECONDS 5000
bool nob_proc_wait(Pid pid);
bool nob_cmd_run_sync(nob_Cmd cmd, Fd* fdin, Fd* fdout);

bool nob_is_equal_Cstr(const char* s1, const char* s2);

bool nob_rename(const char* old_path, const char* new_path);
bool nob_remove(const char* file_path);

bool nob_read_entire_file(const char* path, nob_String_Builder* sb);

bool nob_check_file_modified(const char* filePath, time_t* modTime);
double nob_modified_time_difference(const char* filePath1, const char* filePath2);
bool nob_is_need_rebuild(const char* source_file_path, const char* executable_file_path);
bool NOB_GO_REBUILD_YOURSELF(void);


//      IMPL    IMPLE          MENTA    IMPLEMENTATI	    IMPL
//      EMEN    TIONI          MPLEM    ONIMPLEMENTATI      EMEN
//      TATI    ENTATI        ONIMPL    ONIM        PLEM    TATI
//      ONIM    EMENTA        TIONIM    ENTA        TION    ONIM
//      PLEM    PLEMENT      ATIONIM    IMPL        EMEN    PLEM
//      ENTA    PLEMENT      ATIONIM    TATI        ONIM    ENTA
//      TION    PLEMENTA    TIONIMPL    PLEMENTATIONIM      TION
//      IMPL    EMENTATI    ONIMPLEM    PLEMENTATION	    IMPL
//      EMEN    ENTA TION  IMPL EMEN    IMPL		        EMEN
//      TATI    TATI ONIM  PLEM ENTA    EMEN		        TATI
//      ONIM    TION  IMPLEMEN  TATI    TATI		        ONIM
//      PLEM    ONIM  PLEMENTA  TION    ONIM		        PLEM
//      ENTA    IMPL   EMENTA   TION    PLEM		        ENTA
//      TION    IMPL   EMENTA   TION    ENTA		        TION
//      IMPL    IMPL    EMEN    TATI    TION		        IMPLEMENTATIONIM
//      EMEN    ONIM    PLEM    ENTA    IMPL		        PLEMENTATIONIMPL


#define NOB_IMPLEMENTATION
#ifdef NOB_IMPLEMENTATION

void nob_log(Nob_log_level level, const char* fmt, ...) {
	switch (level) {
	case NOB_INFO:
		fprintf(stderr, "[INFO] ");
		break;
	case NOB_WARNING:
		fprintf(stderr, "[WARNING] ");
		break;
	case NOB_ERROR:
		fprintf(stderr, "[ERROR] ");
		break;
	default:
		assert(false && "UNREACHABLE");
	}
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

const char* nob_string_builder_to_cstr(nob_String_Builder sb) {
	char* buffer = malloc(sb.count + 1);
	for (size_t i = 0; i < sb.count; i++) {
		buffer[i] = sb.items[i];
	}
	buffer[(size_t)sb.count] = '\0';
	return (const char*)buffer;
}

void nob_string_builder_append_string(nob_String_Builder* sb, const char* string) { // working with pointers feels scary :(
	for (size_t i = 0; i < strlen(string); i++) {
		nob_string_builder_append_char(sb, string[i]);
	}
}

void nob_string_builder_append_strings_va_null(nob_String_Builder* sb, ...) {
	va_list args;
	va_start(args, sb);

	const char* item = va_arg(args, const char*);
	while (item != NULL) {
		nob_string_builder_append_string(sb, item);
		item = va_arg(args, const char*);
	}

	va_end(args);
}

const char* nob_cmd_join(const char* joiner, nob_Cmd cmd) {
	nob_String_Builder sb = { 0 };
	nob_string_builder_append_string(&sb, cmd.items[0]);
	for (size_t i = 1; i < cmd.count; i++) {
		nob_string_builder_append_strings_va(&sb, joiner, cmd.items[i]);
	}
	return nob_string_builder_to_cstr(sb);
}

void nob_cmd_show(nob_Cmd cmd) {
	nob_log(NOB_INFO, "Command line is currently: %s", nob_cmd_join(" ", cmd));
}


void nob_nob_cmd_append_null(nob_Cmd* cmd, ...) {
	va_list args;
	va_start(args, cmd);

	const char* item = va_arg(args, const char*);
	while (item != NULL) {
		nob_dynamic_array_append(cmd, item);
		item = va_arg(args, const char*);
	}

	va_end(args);
}


// https://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror
// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessage?redirectedfrom=MSDN
const char* nob_GetLastErrorAsString(void) {
	DWORD errorId = GetLastError();
	if (errorId == 0)
		return "";

	LPSTR messageBuffer = NULL;

	DWORD size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorId,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0,
		NULL
	);

	(void)size;

	return messageBuffer;
}


Pid nob_cmd_run_async(nob_Cmd cmd, Fd* fdin, Fd* fdout) {
#ifdef _WIN32

	// https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output

	STARTUPINFO siStartupInfo;
	ZeroMemory(&siStartupInfo, sizeof(STARTUPINFO)); // if changed size siStar... -> STAR... I think it should be like this but not sure...
	siStartupInfo.cb = sizeof(STARTUPINFO);

	// https://learn.microsoft.com/en-us/windows/console/getstdhandle
	siStartupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	siStartupInfo.hStdOutput = fdout ? *fdout : GetStdHandle(STD_OUTPUT_HANDLE);
	siStartupInfo.hStdInput = fdin ? *fdin : GetStdHandle(STD_INPUT_HANDLE);
	siStartupInfo.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION piProcessInfo;
	ZeroMemory(&piProcessInfo, sizeof(PROCESS_INFORMATION));

	// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
	BOOL success = CreateProcess(
		NULL,								// executable path
		(LPSTR)nob_cmd_join(" ", cmd),			// command line 
		NULL,								// process security attributes 
		NULL,								// primary thread security attributes 
		TRUE,								// handles are inherited 
		0,									// creation flags 
		NULL,								// use parent's environment 
		NULL,								// use parent's current directory 
		&siStartupInfo,						// STARTUPINFO pointer 
		&piProcessInfo						// receives PROCESS_INFORMATION 
	);

	if (!success) {
		nob_log(NOB_ERROR, "Could not create child process %s: %s",
			nob_cmd_join(" ", cmd), nob_GetLastErrorAsString());
	}

	CloseHandle(piProcessInfo.hThread);

	return piProcessInfo.hProcess;

#else
#endif // _WIN32
}

bool nob_proc_wait(Pid pid) {
#ifdef _WIN32
	WaitForSingleObject(pid, NOB_SYNC_PROCESS_TIMEOUT_MILISECONDS);				// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject?redirectedfrom=MSDN
	DWORD exitCode;
	GetExitCodeProcess(pid, &exitCode);										// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getexitcodeprocess
	if (exitCode != 0) // There is a chance that this is the right way to do this. I am not sure.
		return false;  // It seems it works... for now
	return true;
#else
	return false;
#endif // _WIN32
}

bool nob_cmd_run_sync(nob_Cmd cmd, Fd* fdin, Fd* fdout) {
	Pid pid = nob_cmd_run_async(cmd, fdin, fdout);
	if (pid == INVALID_HANDLE_VALUE)
		return false;
	return nob_proc_wait(pid);
}


bool nob_check_file_modified(const char* filePath, time_t* modTime) {
	struct stat attrs;

	if (stat(filePath, &attrs) != 0) { // https://www.gnu.org/software/libc/manual/html_node/Reading-Attributes.html
		modTime = NULL;
		return false;
	}
	*modTime = attrs.st_mtime; // https://www.gnu.org/software/libc/manual/html_node/File-Times.html
	// I'm stupid, don't understand pointers
	// Took me 2 days to debug a pointer thing... so sad...
	(void)modTime;
	return true;
}

double nob_modified_time_difference(const char* filePath1, const char* filePath2) {
	time_t time1;
	time_t time2;

	if (!nob_check_file_modified(filePath1, &time1)) {
		nob_log(NOB_ERROR, "Could not find file at address: %s", filePath1);
	}

	if (!nob_check_file_modified(filePath2, &time2)) {
		nob_log(NOB_ERROR, "Could not find file at address: %s", filePath2);
	}

	return difftime(time1, time2); // https://www.gnu.org/software/libc/manual/html_node/Calculating-Elapsed-Time.html
}

bool nob_is_need_rebuild(const char* source_file_path, const char* executable_file_path) {
	if (nob_modified_time_difference(source_file_path, executable_file_path) > 0)
		return true;
	return false;
}

bool nob_is_equal_Cstr(const char* s1, const char* s2) {
	size_t length = strlen(s1);
	if (length != strlen(s2))
		return false;
	for (size_t i = 0; i < length; i++) {
		if (s1[i] != s2[i])
			return false;
	}
	return true;
}

bool nob_rename(const char* old_path, const char* new_path) {
#ifdef _WIN32
	if (MoveFileEx(old_path, new_path, MOVEFILE_REPLACE_EXISTING) == 0) { // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-movefileexa
		nob_log(NOB_ERROR, "File could not be renamed: %s -> %s", old_path, new_path);
		return false;
	}
	return true;
#else
	if (rename(old_path, new_path) != 0) {
		nob_log(NOB_ERROR, "File could not be renamed: %s -> %s", old_path, new_path);
		return false;
	}
	return true;
#endif
}

bool nob_remove(const char* file_path) {
#ifdef _WIN32
	if (DeleteFile(file_path) == 0) { // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-deletefile
		nob_log(NOB_ERROR, "Could not delete file: %s", file_path);
		return false;
	}
	return true;
#else
	if (remove(file_path) != 0) {
		nob_log(NOB_ERROR, "Could not delete file: %s", file_path);
		return false;
	}
	return true;
#endif
}

bool nob_read_entire_file(const char* path, nob_String_Builder* sb) {
    bool result = true;

    FILE* f = fopen(path, "rb");
    if (f == NULL) {
        result = false;
        goto defer;
    }
    if (fseek(f, 0, SEEK_END) < 0) {
        result = false;
        goto defer;
    }
    long int m = ftell(f);
    if (m < 0) {
        result = false;
        goto defer;
    }
    if (fseek(f, 0, SEEK_SET) < 0) {
        result = false;
        goto defer;
    }

    size_t new_count = sb->count + m;
    if (new_count > sb->capacity) {
        sb->items = realloc(sb->items, new_count);
        assert(sb->items != NULL && "RAM is full");
        sb->capacity = new_count;
    }

    fread(sb->items + sb->count, 1, m, f);
    if (ferror(f)) {
        result = false;
        goto defer;
    }

defer:
    if (!result)
        nob_log(NOB_ERROR, "Could not read file %s: %s", path, strerror(errno));
    if (f)
        fclose(f);
    return result;
}

bool NOB_GO_REBUILD_YOURSELF(void) {
	if (!nob_is_need_rebuild("nob.c", "nob.exe") && !nob_is_need_rebuild("nob.h", "nob.exe")) {
		nob_log(NOB_INFO, "Nob is not need rebuild.");
		return true;
	}
	nob_log(NOB_INFO, "Nob is need rebuild! Starting rebuild!");
	nob_rename("nob.exe", "nob.old");
	nob_Cmd cmd = { 0 };
	nob_cmd_append(&cmd, "gcc", "-o", "nob", "nob.c");
	nob_cmd_append(&cmd, "-Wall", "-Wextra", "-ggdb", "-pedantic");
	nob_cmd_show(cmd);
	if (!nob_cmd_run_sync(cmd, NULL, NULL)) {
		nob_log(NOB_ERROR, "Nob rebuild is failiure, just like you.");
		nob_rename("nob.old", "nob.exe");
		return false;
	}
	nob_log(NOB_INFO, "Nob rebuild success");
	return true;
}

#endif // NOB_IMPLEMENTATION
#endif // NOB_H_
