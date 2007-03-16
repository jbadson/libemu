/* @header@ */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <emu/emu.h>
#include <emu/emu_log.h>
#include <emu/emu_memory.h>
#include <emu/emu_cpu.h>
#include <emu/emu_fpu.h>


struct emu
{
	struct emu_logging *log;
	struct emu_memory *memory; 
	struct emu_cpu *cpu;
	struct emu_fpu *fpu;

	int 	errno;
	char 	*errorstr;
};


struct emu *emu_new()
{
	struct emu *e = (struct emu *)malloc(sizeof(struct emu));
	memset(e, 0, sizeof(struct emu));
	e->log = emu_log_new();
	e->memory = emu_memory_new(e);
	e->cpu = emu_cpu_new(e);
	e->fpu = emu_fpu_new(e);
	logDebug(e,"%s %x\n", __PRETTY_FUNCTION__,(unsigned int)e);
	return e;
}


void emu_free(struct emu *e)
{
	logDebug(e,"%s %x\n", __PRETTY_FUNCTION__,(unsigned int)e);
	emu_cpu_free(e->cpu);
	emu_memory_free(e->memory);
	emu_log_free(e->log);
	if (e->errorstr != NULL)
		free(e->errorstr);

	free(e);
}

inline struct emu_memory *emu_memory_get(struct emu *e)
{
	return e->memory;
}

inline struct emu_logging *emu_logging_get(struct emu *e)
{
	return e->log;
}

inline struct emu_cpu *emu_cpu_get(struct emu *e)
{
	return e->cpu;
}



void emu_errno_set(struct emu *e, int err)
{
	e->errno = err;
}

int emu_errno(struct emu *c)
{
	return c->errno;
}

void emu_strerror_set(struct emu *e, const char *format, ...)
{
	if (e->errorstr != NULL)
    	free(e->errorstr);

	va_list         ap;
	char            *message;
	va_start(ap, format);
	vasprintf(&message, format, ap);
	va_end(ap);

	e->errorstr = message;
}

const char *emu_strerror(struct emu *e)
{
	return e->errorstr;
}

