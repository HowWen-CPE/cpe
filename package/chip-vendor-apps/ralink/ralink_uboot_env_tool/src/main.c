/*
 * (C) Copyright 2011-2012
 */

/*
 * Command line user interface to print|save board data.
 *
 * Implements:
 *	print_env [[ -n name ] | [ name ... ]]
 *              - prints the value of a single environment variable
 *                "name", the ``name=value'' pairs of one or more
 *                environment variables "name", or the whole
 *                environment if no names are specified.
 *	set_env name [ value ... ]
 *		- If a name without any values is given, the variable
 *		  with this name is deleted from the environment;
 *		  otherwise, all "value" arguments are concatenated,
 *		  separated by single blank characters, and the
 *		  resulting string is assigned to the environment
 *		  variable "name"
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board_data.h"

#define	CMD_PRINTBD "print_env"
#define CMD_SETBD   "set_env"

int
main(int argc, char *argv[])
{
	char *p;
	char *cmdname = *argv;

	if ((p = strrchr (cmdname, '/')) != NULL) {
		cmdname = p + 1;
	}

	if (strcmp(cmdname, CMD_PRINTBD) == 0) {

		if (bd_printenv (argc, argv) != 0)
			return (EXIT_FAILURE);

		return (EXIT_SUCCESS);

	} else if (strcmp(cmdname, CMD_SETBD) == 0) {

		if (bd_setenv (argc, argv) != 0)
			return (EXIT_FAILURE);

		return (EXIT_SUCCESS);

	}

	fprintf (stderr,
		"Identity crisis - may be called as `" CMD_PRINTBD
		"' or as `" CMD_SETBD "' but not as `%s'\n",
		cmdname);
	return (EXIT_FAILURE);
}
