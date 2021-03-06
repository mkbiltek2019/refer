/* sic_builtin.c -- basic builtin commands loaded statically
   Copyright (C) 2000 Gary V. Vaughan
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "sic.h"
#include "sic_builtin.h"
#include "sic_repl.h"

/* List of built in functions. */
#define builtin_functions		\
	BUILTIN(cd,		0, 1)	\
	BUILTIN(exit,		0, 1)	\
	BUILTIN(source,		1, -1)	\
	BUILTIN(unknown,	0, -1)

/* Prototype Generator. */
#define SIC_BUILTIN(name)	\
	int name PARAMS((Sic *sic, int argc, char *const argv[]))

#define BUILTIN(name, min, max)	\
	extern SIC_BUILTIN(CONC(builtin_, name));
builtin_functions
#undef BUILTIN

static char *path_find PARAMS((const char *command));
static int path_execute PARAMS((Sic *sic, const char *path,
				char *const argv[]));

/* name to function map */
Builtin builtin_table[] = {

#define BUILTIN(name, min, max)	\
	{ STR(name), CONC(builtin_, name), (min), (max) },
  builtin_functions
#undef BUILTIN

  { NULL, NULL, -1, -1 }
};

BUILTIN_DECLARATION(exit)
{
  return SIC_EXIT;
}

BUILTIN_DECLARATION (source)
{
  int status = SIC_OKAY;
  int i;

  for (i = 1; status == SIC_OKAY && argv[i]; ++i)
    status = source (sic, argv[i]);

  return status;
}

BUILTIN_DECLARATION(cd)
{
  const char *destination = argv[1];
  int status = SIC_ERROR;

  if (!destination)
    destination = getenv ("HOME");
  
  if  (!destination)
      sic_result_set (sic, "unbound variable \"HOME\"", -1);
  else if (chdir (destination) < 0)
    {
      sic_result_clear (sic);
      sic_result_append (sic, destination, ": ", xstrerror (errno), NULL);
    }
  else
    status = SIC_OKAY;

  return status;
}

BUILTIN_DECLARATION(unknown)
{
  char *path = path_find (argv[0]);
  int status = SIC_ERROR;

  if (!path)
    sic_result_append (sic, "command \"", argv[0], "\" not found", NULL);
  else if (path_execute (sic, path, argv) != SIC_OKAY)
    sic_result_append (sic, "command \"", argv[0],"\" failed: ",
		       strerror (errno), NULL);
  else
    status = SIC_OKAY;

  return status;
}

static char *
path_find (command)
     const char *command;
{
  char *path = xstrdup (command);
  
  if (*command == '/')
    {
      if (access (command, X_OK) < 0)
	goto notfound;
    }
  else
    {
      char *PATH = getenv ("PATH");
      char *pbeg, *pend;
      size_t len;

      for (pbeg = PATH; *pbeg != '\0'; pbeg = pend)
	{
	  pbeg += strspn (pbeg, ":");
	  len = strcspn (pbeg, ":");
	  pend = pbeg + len;
	  path = XREALLOC (char, path, 2 + len + strlen(command));
	  *path = '\0';
	  strncat (path, pbeg, len);
	  if (path[len -1] != '/') strcat (path, "/");
	  strcat (path, command);
	  
	  if (access (path, X_OK) == 0)
	      break;
	}

      if (*pbeg == '\0')
	  goto notfound;
    }

  return path;

 notfound:
  XFREE (path);
  return NULL;
}  

static int
path_execute (sic, path, argv)
     Sic *sic;
     const char *path;
     char *const argv[];
{
  int status = SIC_OKAY;	/* assume successful for now */
  pid_t pid;

  pid = fork ();
  switch (pid)
    {
    case -1:			/* fork failed */
      status = SIC_ERROR;
      break;

    case 0:
      execv (path, argv);
      _exit (1);		/* execv in child failed */
      break;

    default:			/* wait for child to exit */
      {
	int stat;
	if (waitpid (pid, &stat, 0) == pid)
	  {
	    if (WIFSIGNALED (stat)
		|| (WIFEXITED (stat) && (WEXITSTATUS (stat) != 0)))
	      status = SIC_ERROR;
	  }
	else
	  status = SIC_ERROR;
      }
      break;
    }
  
  return status;
}
