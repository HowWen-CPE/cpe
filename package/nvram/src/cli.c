#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include "cli.h"
#include "nvram_rule.h"

void puts_trim_cr(char *str)
{   
	int len;

	if (!str) return;

	len = strlen(str);
	if (len && (str[len-1] == '\r')) len--;
	printf("%.*s\n", len, str);
}


int do_show(void)
{
	char buf[NVRAM_SPACE];
	char *name = buf;
	int size, crc;

	/* nvram show */
	nvram_getall(buf, sizeof(buf));
	memcpy(&crc, buf, 4);

	/*get crc size in nvram*/
	size = strlen(buf) + 1;

	/*show contents start second string, hide crc content*/
	for (name = buf + strlen(name) + 1; *name; name += strlen(name) + 1)
	{
		puts_trim_cr(name);
	}

	size = size + (int) name - (int) buf;

	fprintf(stderr, "crc = %x\n", crc);
	fprintf(stderr, "size: %d bytes (%d left)\n",
			size, NVRAM_SPACE - size);
	return 0;
}


int do_get(const char *var)
{
	const char *val;
	int stat = 1;

	if( (val = nvram_get(var)) != NULL )
	{
		printf("%s\n", val);
		stat = 0;
	}

	return stat;
}

int do_get_default(const char *var)
{
	const char *val;
	int stat = 1;

	if( (val = nvram_get_default(var)) != NULL )
	{
		printf("%s\n", val);
		stat = 0;
	}

	return stat;
}

int do_unset(const char *var)
{
	return nvram_unset(var);
}


int do_set(const char *pair)
{
	char *val = strstr(pair, "=");
	char var[strlen(pair)];
	int stat = 1;

	if( val != NULL )
	{
		memset(var, 0, sizeof(var));
		strncpy(var, pair, (int)(val-pair));
		stat = nvram_set(var, (char *)(val + 1));
	}

	return stat;
}

int do_fset(const char *pair)
{
	char *val = strstr(pair, "=");
	char var[strlen(pair)];
	int stat = 1;

	if( val != NULL )
	{
		memset(var, 0, sizeof(var));
		strncpy(var, pair, (int)(val-pair));
		stat = nvram_fset(var, (char *)(val + 1));
	}

	return stat;
}


int main( int argc, const char *argv[] )
{
	int stat = 1;
	int done = 0;
	char res[EZPLIB_BUF_LEN];

	if( argc > 1 )
	{
		--argc;
		++argv;
		if( !strncmp(*argv, "show", 4) ) {
			--argc;
			++argv;
			/* nvram show */
			if( 0==argc ) {
				stat = do_show();
				done++;
			}
			/* nvram show <rule-set> <nth> */
			else if (argc == 2) {
				stat = ezplib_get_rule(argv[0], atoi(argv[1]), 
						res, EZPLIB_BUF_LEN);
				puts_trim_cr(res);
				done++;
			} 
			/* nvram show <rule-set> <nth> <attr-type> */
			else if (argc == 3) {
				stat = ezplib_get_attr_val(argv[0], atoi(argv[1]), 
					argv[2], res, EZPLIB_BUF_LEN, EZPLIB_USE_CLI);
				puts_trim_cr(res);
				done++;
			} 
			/* nvram show default <rule-set> <nth> <attr-type> */
			else if (argc == 4 && !strcmp(argv[0], "default")) {
				stat = ezplib_get_attr_val_default(argv[1], atoi(argv[2]), 
					argv[3], res, EZPLIB_BUF_LEN, EZPLIB_USE_CLI);
				puts_trim_cr(res);
				done++;
			} 
			/* nvram show <rule-set> <nth> subrule <start> <end> */
			else if (argc == 5 && !strncmp(argv[2], "subrule", strlen(argv[1]))) {
				stat = ezplib_get_subrule(argv[0], atoi(argv[1]), atoi(argv[3]),
						atoi(argv[4]), res, EZPLIB_BUF_LEN);
				puts_trim_cr(res);
				done++;
			}
			else {
				done = 0;
			}

		}
		/* nvram getdefault <rule>*/
		else if (!strncmp(*argv, "getdefault", 10)) {
			if (*++argv) {
				stat = do_get_default(*argv);
				done++;
			} else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*--argv);
				done = 0;
			}
		}
		/* nvram get <rule>*/
		else if (!strncmp(*argv, "get", 3)) {
			if (*++argv) {
				stat = do_get(*argv);
				done++;
			} else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*--argv);
				done = 0;
			}
		}
		/* nvram set <rule=value>*/
		else if (!strncmp(*argv, "set", 3)) {
			if (*++argv) {
				stat = do_set(*argv);
				done++;
			} else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*--argv);
				done = 0;
			}
		}
		/* nvram fset <rule=value>*/
		else if (!strncmp(*argv, "fset", 4)) {
			if (*++argv) {
				stat = do_fset(*argv);
				done++;
			} else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*--argv);
				done = 0;
			}
		}
		/* nvram unset <rule>*/
		else if (!strncmp(*argv, "unset", 5)) {
			if (*++argv) {
				stat = do_unset(*argv);
				done++;
			} else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*--argv);
				done = 0;
			}
		}
		/* nvram export <backup_file>*/
		else if (!strncmp(*argv, "export", 6)) {
			if (*++argv) {
				stat = nvram_export(*argv);
				done++;
			} else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*--argv);
				done = 0;
			}
		}
		/* nvram import <backup_file>*/
		else if (!strncmp(*argv, "import", 6)) {
			if (*++argv) {
				stat = nvram_import(*argv);
				done++;
			} else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*--argv);
				done = 0;
			}
		}
		/*nvram upgradeto version */
		else if (!strncmp(*argv, "upgradeto", 9)) {
			argc -= 1;
			argv += 1;
			if (argc == 1) {
				if (nvram_upgradeto(argv[0])) {
					nvram_commit();
					done++;
					stat = 0;
				}
			}  
			else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*--argv);
				done = 0;
			}
		}
		/*nvram upgrade <version> */
		else if (!strncmp(*argv, "upgrade", 7)) {
			argc -= 1;
			argv += 1;
			if (nvram_upgrade((argc >= 1)? argv[0]: NULL)) {
				nvram_commit();
				done++;
				stat = 0;
			}
			else {
				fprintf(stderr, "nvram upgrade error\n", 
						*(--argv));
				done = 0;
			}
		}
		else if (argc >= 2 && !strncmp(argv[0], "replace", strlen(argv[0]))) {
			/* nvram replace rule <rule-set> <nth> <new-rule> */
			if (!strncmp(argv[1], "rule", strlen(argv[1]))) {
				argc -= 2;
				argv += 2;
				if (argc == 3) {
					int ret;

					ret =
						ezplib_replace_rule(argv[0], atoi(argv[1]),
								argv[2]);
					if (ret < 0) {
						printf("NVRAM replace rule %s failed:%d!\n", argv[0], ret);
						return 1;
					} else  {
						return 0;
					}
				}
				else {
					fprintf(stderr, "Command '%s' requires an argument!\n", 
							*(argv-2));
					done = 0;
				}
			} 
			/* nvram replace attr <rule-set> <nth> <attr> <new-rule> */
			else if (!strncmp(argv[1], "attr", strlen(argv[1]))) {
				argc -= 2;
				argv += 2;
				if (argc == 4) {
					int ret;

					ret =
						ezplib_replace_attr(argv[0], atoi(argv[1]), argv[2],
								argv[3]);
					if (ret < 0) {
						printf("NVRAM replace attr %s failed: %d\n", argv[0], ret);
						return 1;
					} else  {
						return 0;
					}
				}
				else {
					fprintf(stderr, "Command '%s' requires an argument!\n", 
							*(argv-2));
					done = 0;
				}
			}
		}
		/* nvram append rule <rule-set> <new-rule> */
		else if (argc>=2 && !strncmp(argv[0], "append", strlen(argv[0])) &&
				!strncmp(argv[1], "rule", strlen(argv[1]))) {
			argc -= 2;
			argv += 2;
			if (argc == 2) {
				int ret;

				ret = ezplib_append_rule(argv[0], argv[1]);
				if (ret < 0) {
					printf("NVRAM append rule %s failed: %d\n", argv[0], ret);
					return 1;
				} else  {
					return 0;
				}
			}
			else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*(argv-2));
				done = 0;
			}
		} 
		/* nvram prepend rule <rule-set> <new-rule> */
		else if (argc >=2 && !strncmp(argv[0], "prepend", strlen(argv[0])) &&
				!strncmp(argv[1], "rule", strlen(argv[1]))) {
			argc -= 2;
			argv += 2;
			if (argc == 2) {
				int ret;

				ret = ezplib_prepend_rule(argv[0], argv[1]);
				if (ret < 0) {
					printf("NVRAM prepend rule %s failed: %d\n", argv[0], ret);
					return 1;
				} else  {
					return 0;
				}
			}
			else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*(argv-2));
				done = 0;
			}
		}
		/* nvram add rule <rule-set> <nth> <new-rule> */
		else if (argc >= 2 && !strncmp(argv[0], "add", strlen(argv[0])) &&
				!strncmp(argv[1], "rule", strlen(argv[1]))) {
			argc -= 2;
			argv += 2;
			if (argc == 3) {
				int ret;

				ret = ezplib_add_rule(argv[0], atoi(argv[1]), argv[2]);
				if (ret < 0) {
					printf("NVRAM add rule %s failed: %d\n", argv[0], ret);
					return 1;
				} else  {
					return 0;
				}
			}
			else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*(argv-2));
				done = 0;
			}
		} 
		/* nvram delete rule <rule-set> <nth> */
		else if (argc >= 2 && !strncmp(argv[0], "delete", strlen(argv[0])) &&
				!strncmp(argv[1], "rule", strlen(argv[1]))) {
			argc -= 2;
			argv += 2;
			if (argc == 2) {
				int ret;

				ret = ezplib_delete_rule(argv[0], atoi(argv[1]));
				if (ret < 0) {
					printf("NVRAM delete rule %s failed: %d\n", argv[0], ret);
					return 1;
				} else {
					return 0;
				}
			}
			else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*(argv-2));
				done = 0;
			}
		}
		/* nvram rule num <rule-set> */
		else if (argc >=2 && !strncmp(argv[0], "rule", strlen(argv[0])) &&
				!strncmp(argv[1], "num", strlen(argv[1]))) {
			argc -= 2;
			argv += 2;
			if (argc == 1) {
				int ret;

				ret = ezplib_get_rule_num(argv[0]);
				if (ret < 0) {
					printf("Rule %s doesn't exist: %d\n", argv[0], ret);
				}
				else {
					printf("%d\n", ret);
					return 0;
				}
			}
			else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*(argv-2));
				done = 0;
			}
		}
		else if (!strncmp(*argv, "boot", 4)) {
			nvram_boot();
			stat = 0;
			done++;
		}
		/* nvram default */
		else if( !strncmp(*argv, "default", 7) )
		{
			if(argc == 1) {
				stat = nvram_default();
			}
			else {
				/* Added for single rule default */
				argv++;
				stat = nvram_default_rule(*argv);
			}
			done++;
		}
		/* nvram factory */
		else if( !strncmp(*argv, "factory", 7) )
		{
			stat = nvram_factory();
			/* send SIGTERM to init for reboot */
			if(!stat)
				kill(1, 15);
			done++;
		}
		/* nvram commit */
		else if( !strncmp(*argv, "commit", 6) )
		{
			stat = nvram_commit();
			done++;
		}
#ifdef DEBUG
		/* nvram dumpdefault <rule-set> */
		else if( !strncmp(*argv, "dumpdefault", 11) )
		{
			stat = nvram_dump_default();
			done++;
		}
		/* nvram dump <rule-set> */
		else if( !strncmp(*argv, "dump", 4) )
		{
			stat = nvram_dump();
			done++;
		}
		/* nvram desc <rule-set> */
		else if( argc >= 1 && !strncmp(*argv, "desc", 4) )
		{
			argc -= 1;
			argv += 1;
			if (argc == 1) {
				stat = nvram_show_desc(argv[0], 1);
				done++;
			}
			else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*(argv-1));
				done = 0;
			}
		}
		/* nvram des <rule-set> */
		else if( argc >= 1 && !strncmp(*argv, "des", 3) )
		{
			argc -= 1;
			argv += 1;
			if (argc == 1) {
				stat = nvram_show_desc(argv[0], 0);
				done++;
			}
			else {
				fprintf(stderr, "Command '%s' requires an argument!\n", 
						*(argv-1));
				done = 0;
			}
		}
#endif
		else
		{
			fprintf(stderr, "Unknown option '%s' !\n", *argv);
			done = 0;
		}
	}
	else if( !done )
	{
		fprintf(stderr,
				"Usage:\n"
				"	nvram show\n"
				"	nvram show <rule-set> <nth>\n"
				"	nvram show <rule-set> <nth> <attr-type>\n"
				"	nvram show default <rule-set> <nth> <attr-type>\n"
				"	nvram show <rule-set> <nth> subrule <start> <end>\n"
				"	nvram get/getdefault <rule>\n"
				"	nvram set/fset <rule=value> \n"
				"	nvram unset <rule> \n"
				"	nvram export/import <backup_file>\n"
				"	nvram upgrade <FROM_VERSION>\n"
				"	nvram upgradeto TO_VERSION\n"
				"	nvram replace rule <rule-set> <nth> <new-rule>\n"
				"	nvram replace attr <rule-set> <nth> <attr> <new-attr> \n"
				"	nvram append rule <rule-set> <new-rule>\n"
				"	nvram prepend rule <rule-set> <new-rule>\n"
				"	nvram add rule <rule-set> <nth> <new-rule>\n"
				"	nvram delete rule <rule-set> <nth>\n"
				"	nvram rule num <rule-set>\n"
				"	nvram boot\n"
				"	nvram default [rule-set]\n"
				"	nvram factory\n"
				"	nvram commit\n"
#ifdef DEBUG
				"	nvram dump/dumpdefault\n"
				"	nvram desc/des <rule>\n"
#endif
			   );
	}
	return stat;
}
