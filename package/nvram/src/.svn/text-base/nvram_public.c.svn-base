#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "nvram.h"
#include "nvram_fw.h"
#include "nvram_rule.h"

/* Global */
extern struct nvram_fw_tuple nvram_fw_table[];
extern nvram_tuple_t nvram_factory_default[];
extern int _nvram_set(const char *name, const char *value);
extern struct rule rules[];
/**
 * **************Advanced Interface*************************
 */

/**
 *\brief Get the Option of an NVRAM variable. 
 *\return Return the option of the name accroding to Factory Default, NVRAM_UNDEFINED if the name doesn't exist.
 *\param[in] name The specified name
 **/
int nvram_get_option(const char *name)
{   
	struct nvram_tuple *v;
	for (v = &nvram_factory_default[0]; v->name ; v++) {
		if (!strcmp(v->name, name))
			return v->option;
	}

	/* No option is found. */
	return NVRAM_UNDEFINED;
}


/**
 * \brief Restore settings to Factory Default if it belongs to.
 * \return Return 0 on success
 * \test Test Case for Performance:
 *
 * count=1;while(true); do echo count=$count;
 * dd if=/dev/zero of=/dev/mtdblock* bs=1k count=*;
 * nvram init; nvram default;nvram commit;
 * count=$(($count+1)); done;
 * \sa nvram_default nvram_factory
 **/
int nvram_default(void)
{
	int stat = 0;
	struct nvram_tuple *v;

	for (v = &nvram_factory_default[0]; v->name ; v++) {
		stat += nvram_set(v->name, v->value);
	}
	return stat;
}

/**
 * \brief Restore a specified setting to Factory Default.
 * \return Return 0 on success
 * \param[in] rule
 **/
int nvram_default_rule(const char *name)
{       
	int stat = 1;
	struct nvram_tuple *v;
	for (v = &nvram_factory_default[0]; v->name ; v++) {
		if(!strcmp(v->name, name)) {
			stat = nvram_set(v->name, v->value);
			return stat;	
		}
	}   
	return stat;
}

/**
 * \brief Restore settings to Factory Default if it belongs to, followed by commit process.
 * \return Return 0 on success
 * \test Test Case for Performance:
 *
 * count=1;while(true); do echo count=$count;
 * dd if=/dev/zero of=/dev/mtdblock* bs=1k count=*;
 * nvram init; nvram default;nvram commit;
 * count=$(($count+1)); done;
 * \sa nvram_default nvram_factory
 **/
int nvram_factory(void)
{
	int stat = 1;
	stat = nvram_default();
	stat = nvram_commit();
	return stat;
}


/**
 * \brief 	Update the \ref NVRAM_DEFAULT rule if it's empty.
 *			This may be effect only once it revert to factory default.
 *			1.Empty \ref NVRAM_DEFAULT will be updated by it's _default brother.
 *			2.Valid \ref NVRAM_TEMP will be reverted to it's factory default.
 */

int nvram_set_boot(const char *name, const char *value)
{
    unsigned int opt = nvram_get_option(name);
    int ret = 0;
  
    //printf("==============in nvram_set=== opt  %x,name %s,value %s===========\n",opt,name,value);
    if (opt & NVRAM_UNDEFINED) {
        return EINVAL;
    }
    if (opt & NVRAM_PROTECTED) {
        char *exist = nvram_get(name);
        if (exist && *exist) {
            return EACCES; /* If anything exists, return permission denied. */
        }
    }
	
    ret = _nvram_set(name, value);
    return ret;
}

void nvram_boot(void)
{
	struct nvram_tuple *v;
    char *value;

    nvram_init(NULL);

	for (v = &nvram_factory_default[0]; v->name ; v++) {
		value = nvram_get(v->name);
        if (!value || !*value) {
            /* NULL or "\0" */
            if (v->option & NVRAM_EMPTY)
                continue; /* NULL or "\0" is allowed. */

            if (v->option & NVRAM_DEFAULT) {
                char default_name[64];
                /* Get the default value. */
                sprintf(default_name, "%s_default", v->name);
                v->value = nvram_get( default_name);
            }

            nvram_set_boot(v->name, v->value);
        } else {
            /* Some value exist. */
            if (v->option & NVRAM_TEMP)
               nvram_set_boot(v->name, v->value);
        }
	}
    nvram_close();
}

/**
 * \brief Export settings with their current values to a specified file.
 * \return Return 0 on success
 * \param[in] filename The file to export settings.
 * \note Only NON NVRAM_PROTECTED and NON NVRAM_TEMP settings are allowed to be exported.
 **/
int nvram_export(const char *filename)
{   
	FILE *fp;
	struct nvram_tuple *v;
	char *value;


	if ( !(fp = fopen(filename, "wb") ))
		return EACCES;

	//HEADER of export
	fprintf(fp, 
		"[EZP_LOG ] %s %s %s [EZP_%s%s] " EZP_PROD_VERSION "\n",
		nvram_safe_get("brand"), 
		nvram_safe_get("model"),
		nvram_safe_get("board_model_rule"),
		nvram_safe_get("prod_cat"), 
		nvram_safe_get("prod_subcat")
	);

	for (v = &nvram_factory_default[0]; v->name ; v++) {
		if ((v->option & NVRAM_PROTECTED) ||
				(v->option & NVRAM_TEMP)) {
			continue;
		}
		value = nvram_safe_get(v->name);
		fprintf(fp, "%s=%s\n", v->name, value);
	}
	fclose(fp);
	return 0;
}

/**
 * \brief Import settings from specified file.
 * \return Return 0 on success
 * \param[in] filename The file to import settings.
 * \note	Only NON NVRAM_PROTECTED and NON NVRAM_TEMP settings are allowed to be imported. 
			The header line of the file will be checked.
 **/
int nvram_import(const char *filename)
{   
	FILE *fp;
	char *p, *q;
	char buf[MAX_VALUE_LEN];
	char old_str[32], new_str[32];
	int old, new = 0;
	int i=0;
	char board_model[32] = {0};

	struct nvram_tuple *v;
	struct nvram_fw_tuple *w;

	if ( !(fp = fopen(filename, "r") ))
		return 1;

	/* First line should begin with "EZP_LOG". */
	fgets(buf, sizeof(buf), fp);
	if ((p = strstr(buf, "EZP_LOG")) == NULL) {
		printf("log file format error\n");
		return 1;
	}

	strcpy(board_model, nvram_get_default("board_model_rule"));
	if ((p = strstr(buf, board_model)) == NULL) {
		printf("log file format error\n");
		return 1;
	}

	if ((p = strstr(p + strlen("EZP_LOG"), "EZP_")) == NULL) {
		printf("log file format error: product\n");
		return 1;
	}
	p += strlen("EZP_");

	/* prod_cat */
	q = nvram_safe_get("prod_cat");
	if (p[0] != q[0]) {
		printf("log file format error: prod_cat\n");
		return 1;
	}

	/* prod_subcat */
	q = nvram_safe_get("prod_subcat");
	if (p[1] != q[0]) {
		printf("log file format error: prod_subcat\n");
		return 1;
	}
	p = strchr(p, ']');
	p += 1;

	if (*p == '\n' || *p == '\0') {
		strcpy(old_str, "0.0.0");
	} else {
		strncpy(old_str, p + 1, strlen(p + 1));
	}

	/* XXX:We don't accept any thing higher than our current version. */
	strcpy(new_str, EZP_PROD_VERSION);
	/* Purify new_str. e.g. 1.6.5-RC1 => 1.6.5 */
	for (i = 0; old_str[i] == '.' || isdigit(old_str[i]) ; i++);
	old_str[i]='\0';
	for (i = 0; new_str[i] == '.' || isdigit(new_str[i]) ; i++);
	new_str[i]='\0';
	printf("[import]Firmware:%s\r\nConfiguration:%s\r\n", new_str, old_str);

	/* Very likely we cannot find the matched version since our firmware might
	 * be older than the config file. */
	old = 0x0FFFFFFF;

	for (w = &nvram_fw_table[0]; w->fw_str ; w++) {
		if (!strcmp(w->fw_str, old_str)) {
			old = w->fw_version;
		}
		if (!strcmp(w->fw_str, new_str)) {
			new = w->fw_version;
		}
	}

	if (old > new) {
		printf("log file format error: newer version configuration format\n");
		return 1;
	}

	while (fgets(buf, sizeof(buf), fp)) {
		if ((p = strchr(buf, '=')) == NULL)
			continue;

		/* Please the end of the string to replace "=". */
		*p = 0;

		for (v = &nvram_factory_default[0]; v->name ; v++) {
			if (!strcmp(v->name, buf)) {
				break;
			}
		}

		if (!v->name || (v->option & NVRAM_PROTECTED) ||
				(v->option & NVRAM_TEMP)) {
			/* No match or NVRAM_PROTECTED or NVRAM_TEMP. */
			//printf("invalid: %s=%s\n", buf, p + 1);
			continue;
		}

		if (*(p + 1) == '\n') {
			/* "key=\n" */
			nvram_set(buf, "");
		} else {
			/* "key=value\n" */
			p++;
			/* Replace \n with \0 */
			if (*(p + strlen(p) - 1) == '\n') {
				*(p + strlen(p) - 1) = '\0';
			}
			nvram_set(buf, p);
		}
	}
	fclose(fp);

	nvram_upgrade(old_str);

	return 0;
}

/**
 *\brief Upgrade configurations FROM specified source version
 *\return Return 0 on success
 *\param[in] source The specified version from which to upgrade
 */
int nvram_upgrade(const char *source)
{
    struct nvram_fw_tuple *v;
    int old = 0xEFFFFFFF;
    int new = 0;
    char old_str[32] = {0};
    char new_str[32] = {0};
    int i, change = 0;

    /* If source is empty, get it from fw_version. */
    if (source && *source) {
        strcpy(old_str, source);
    } else {
        strcpy(old_str, nvram_safe_get("fw_version"));
    }
   
    strcpy(new_str, EZP_PROD_VERSION);
    printf("Upgrade Configuration(%s -> %s)\n",old_str,new_str);


    /* Purify new_str. e.g. 1.6.5-RC1 => 1.6.5 */
    for (i = 0; old_str[i] == '.' || isdigit(old_str[i]) ; i++);
    old_str[i]='\0';
    for (i = 0; new_str[i] == '.' || isdigit(new_str[i]) ; i++);
    new_str[i]='\0';

    /* Use "0.0.0" instead of "". */
    if (!*old_str) {
        strcpy(old_str, nvram_fw_table[0].fw_str);
    }

    printf("Upgrade Configuration(%s -> %s)\n",old_str,new_str);

    /* We might not be able to find out the version. Skip upgrade if cannot. */
    for (v = &nvram_fw_table[0]; v->fw_str ; v++) {
        if (!strcmp(v->fw_str, old_str)) {
            old = v->fw_version;
        }
        if (!strcmp(v->fw_str, new_str)) {
            new = v->fw_version;
        }
    }
    printf("Upgrade Configuration(%d -> %d)\n",old,new);
	
    if (old < new) {
        printf("Upgrade [%s->%s], index[%d->%d]\n", old_str, new_str, old, new);
        /* Upgrade. Use the upgrade functions from (old + 1) to new. */
        for (i = old + 1; i <= new; i++) {
            if (nvram_fw_table[i].fw_upgrade_func) {
                (*nvram_fw_table[i].fw_upgrade_func)();
            }
        }
        nvram_fset("fw_version", EZP_PROD_VERSION);
        change = 1;
    }
	else if (old > new) {
        printf("Downgrade [%s->%s], [%d->%d]\n", old_str, new_str, old, new);
        /* Downgrade. Use the upgrade functions from old to (new + 1). */
        for (i = old; i >= new + 1; i-- ) {
            if (nvram_fw_table[i].fw_downgrade_func) {
                (*nvram_fw_table[i].fw_downgrade_func)();
            }
        }
        nvram_fset("fw_version", EZP_PROD_VERSION);
        change = 1;
    }
	else {
        printf("Unchanged [%s<->%s], [%d<->%d]\n", old_str, new_str, old, new);
        change = 1;
	}
    
    if(strcmp(nvram_safe_get("prod_subsubcat"), "")) {
        /* unset prod_subsubcat to make sure prod_subsubcat the same with 
         * special firmware */
        nvram_unset("prod_subsubcat"); 
        change = 1;
    }
    return change;
}

/**
 *\brief Upgrade configurations TO specified target version
 *\return Return 0 on success
 *\param[in] target The specified version to upgrade 
 */
int nvram_upgradeto(const char *target)
{
    struct nvram_fw_tuple *v;
    int old = 0;
    int new = 0;
    char old_str[32];
    char new_str[32];
    int i, change = 0;

    strcpy(old_str, nvram_safe_get("fw_version"));
    strcpy(new_str, target);
    printf("Upgrade Configuration(%s -> %s)\n",old_str,new_str);

    /* Purify new_str. e.g. 1.6.5-RC1 => 1.6.5 */
    for (i = 0; old_str[i] == '.' || isdigit(old_str[i]) ; i++);
    old_str[i]='\0';
    for (i = 0; new_str[i] == '.' || isdigit(new_str[i]) ; i++);
    new_str[i]='\0';

    /* Use "0.0.0" instead of "". */
    if (!*old_str) {
        strcpy(old_str, nvram_fw_table[0].fw_str);
    }
    if (!*new_str) {
    	int j = 0;
    	for (v = &nvram_fw_table[0]; v->fw_str ; j++,v++);

        strcpy(new_str, nvram_fw_table[j].fw_str);
    }

    /* We might not find out the version. Skip downgrade if cannot. */
    for (v = &nvram_fw_table[0]; v->fw_str ; v++) {
        if (!strcmp(v->fw_str, old_str)) {
            old = v->fw_version;
        }
        if (!strcmp(v->fw_str, new_str)) {
            new = v->fw_version;
        }
    }
    printf("Upgrade Configuration(%d -> %d)\n",old,new);

    if (old < new) {
        printf("Upgrade [%s->%s], index[%d->%d]\n", old_str, new_str, old, new);
        /* Upgrade. Use the upgrade functions from (old + 1) to new. */
        for (i = old + 1; i <= new; i++) {
            if (nvram_fw_table[i].fw_upgrade_func) {
                (*nvram_fw_table[i].fw_upgrade_func)();
            }
        }
        nvram_fset("fw_version", target);
        change = 1;
    }
    if (old > new) {
        printf("Downgrade [%s->%s], [%d->%d]\n", old_str, new_str, old, new);
        /* Downgrade. Use the upgrade functions from old to (new + 1). */
        for (i = old; i >= new + 1; i-- ) {
            if (nvram_fw_table[i].fw_downgrade_func) {
                (*nvram_fw_table[i].fw_downgrade_func)();
            }
        }
		nvram_fset("fw_version", target);
        change = 1;
    }
	else { 
        printf("Unchanged [%s<->%s], [%d<->%d]\n", old_str, new_str, old, new);
        change = 1;
	}	


    if(strcmp(nvram_safe_get("prod_subsubcat"), "")) {
        /* unset prod_subsubcat to make sure prod_subsubcat the same with 
         * special firmware */
        nvram_unset("prod_subsubcat"); 
        change = 1;
    }
    return change;
}

/**
 * \brief Match an NVRAM variable.
 * \return  TRUE if variable is defined and its value is string equal
 *      to match or FALSE otherwise
 * \param   name    name of variable to match
 * \param   match   value to compare against value of variable
 */
int nvram_match(char *name, char *match) 
{
	const char *value = nvram_get(name);
	return (value && !strcmp(value, match));
}

/**
 * \brief Inversely match an NVRAM variable.
 * \return  TRUE if variable is defined and its value is not string
 *      equal to invmatch or FALSE otherwise
 * \param   name    name of variable to match
 * \param   match   value to compare against value of variable
 */
int nvram_invmatch(char *name, char *invmatch)
{
	const char *value = nvram_get(name);
	return (value && strcmp(value, invmatch));
}

/**
 *\brief Get the Default Value of an NVRAM variable. 
 *\return	Return the default value of the name. 
			Retrun NULL if name does not exist or flash non-initialized
 *\param[in] name The specified name
 **/
char *	nvram_get_default(const char *name)
{
	struct nvram_tuple *v;
	for (v = &nvram_factory_default[0]; v->name ; v++) {
		if(!strcmp(name, v->name)) {
			return v->value;
		}
	}
	return NULL;
}

#ifdef DEBUG

void getOption(unsigned int enum_val, char *buf)
{
	sprintf(buf, "%s%s%s%s%s%s%s%s" 
		,enum_val == NVRAM_NONE ?       "[NVRAM_NONE]" :""
		,enum_val & NVRAM_PRIVATE ?    "[NVRAM_PRIVATE]" :""
		,enum_val & NVRAM_PROTECTED ?  "[NVRAM_PROTECTED]" :""
		,enum_val & NVRAM_TEMP ?       "[NVRAM_TEMP]" :""
		,enum_val & NVRAM_CUSTOMIZED ? "[NVRAM_CUSTOMIZED]" :""
		,enum_val & NVRAM_EMPTY ?      "[NVRAM_EMPTY]" :""
		,enum_val & NVRAM_DEFAULT ?    "[NVRAM_DEFAULT]" :""
		,enum_val & NVRAM_UNDEFINED ?  "[NVRAM_UNDEFINED]" :""
	);
}

struct rule *getRuleFromRules(const char *rule)
{
	struct rule *r = NULL;

	for ( r = rules; r->name !=NULL ; r++) {
		//has rule structure
		if ( !strcmp(rule, r->name) ) {
			return r;
		}
	}
	return NULL;
}

/**
 *\brief dump the current settings of nvram
 **/
int nvram_dump(void)
{
	char option[MAX_VALUE_LEN] = {0};
	char val[MAX_VALUE_LEN] = {0};
	printf("option, rule_set, column, attribute, value, scope, description\r\n");

	struct nvram_tuple *v;
	for (v = &nvram_factory_default[0]; v->name ; v++) {
		int hasrule = 0;
		struct rule *r = NULL;
		struct attr *a = NULL;

		for ( r = rules; r->name !=NULL ; r++) {
			//has rule structure
			if ( !strcmp(v->name, r->name) ) {
				hasrule = 1;
				int i = ezplib_get_rule_num(v->name);
				int j = 0;	

				if ( 0 == i ) {
					for( a = r->attr; a->name !=NULL; a++ ) {
						memset(option, 0, sizeof(option));
						getOption(v->option, option);
						//"OPTION, RULE, COLUMN_NO, ATTRIBUTE, VALUE"
						printf("%s,%s,%d,%s,\r\n", option, v->name, 0, a->name);	
					}
				}
				else {
					for( j = 0; j < i; ++j) {
						for( a = r->attr; a->name !=NULL; a++ ) {
							ezplib_get_attr_val(r->name, j, a->name, val, sizeof(val), EZPLIB_USE_CLI);

							memset(option, 0, sizeof(option));
							getOption(v->option, option);

							if ( strchr ( val, ',' ) ) {
								//"OPTION, RULE, COLUMN_NO, ATTRIBUTE, VALUE", val
								printf("%s,%s,%d,%s,\"%s\"\r\n", option, v->name, j, a->name, val);	
							}
							else {
								printf("%s,%s,%d,%s,%s\r\n", option, v->name, j, a->name, val);	
							}

						}
					}
				}
			}
		}
		if ( !hasrule ) {
			//has rule structure
			memset(option, 0, sizeof(option));
			getOption(v->option, option);
			printf("%s,%s,,,%s\r\n", option, v->name, nvram_safe_get(v->name));	
		}

	}
	return 0;
}


/**
 *\brief dump the default settings of nvram(factory default)
 **/
int nvram_dump_default(void)
{
	char option[MAX_VALUE_LEN] = {0};
	char val[MAX_VALUE_LEN] = {0};
	printf("option, rule_set, column, attribute, value, scope, description\r\n");

	struct nvram_tuple *v;
	for (v = &nvram_factory_default[0]; v->name ; v++) {
		int hasrule = 0;
		struct rule *r = NULL;
		struct attr *a = NULL;

		for ( r = rules; r->name !=NULL ; r++) {
			//has rule structure
			if ( !strcmp(v->name, r->name) ) {
				hasrule = 1;
				int i = ezplib_get_rule_num_default(v->name);
				int j = 0;	

				if ( 0 == i ) {
					for( a = r->attr; a->name !=NULL; a++ ) {
						memset(option, 0, sizeof(option));
						getOption(v->option, option);
						//"OPTION, RULE, COLUMN_NO, ATTRIBUTE, VALUE"
						printf("%s,%s,%d,%s,\r\n", option, v->name, 0, a->name);	
					}
				}
				else {
					for( j = 0; j < i; ++j) {
						for( a = r->attr; a->name !=NULL; a++ ) {
							ezplib_get_attr_val_default(r->name, j, a->name, val, sizeof(val), EZPLIB_USE_CLI);

							memset(option, 0, sizeof(option));
							getOption(v->option, option);

							if ( strchr ( val, ',' ) ) {
								//"OPTION, RULE, COLUMN_NO, ATTRIBUTE, VALUE"
								printf("%s,%s,%d,%s,\"%s\"\r\n", option, v->name, j, a->name, val);	
							}
							else {
								printf("%s,%s,%d,%s,%s\r\n", option, v->name, j, a->name, val);	
							}

						}
					}
				}
			}
		}
		if ( !hasrule ) {
			//has rule structure
			memset(option, 0, sizeof(option));
			getOption(v->option, option);
			char * default_value = nvram_get_default(v->name);
			printf("%s,%s,,,%s\r\n", option, v->name, default_value == NULL ? "": default_value );	
		}

	}
	return 0;
}


#define SPR(Y, X, ...) do { if(bColor) sprintf(Y,"\033[1;33;40m"X"\033[0m",##__VA_ARGS__);else sprintf(Y,X,##__VA_ARGS__); }while(0)
#define PR(X, ...) do { if(bColor) printf("\033[1;33;40m"X"\033[0m",##__VA_ARGS__);else printf(X, ##__VA_ARGS__); }while(0)


int  nvram_show_desc(const char *rule, int bColor)
{
	struct nvram_tuple *v;
	for (v = &nvram_factory_default[0]; v->name ; v++) {
		//the rule is in factory
		if (!strcmp(v->name, rule)) {
			
			//NAME
			PR("%s \r\n", "NAME");
			printf("\t%s\r\n", rule);

			//OPTION
			char option[MAX_VALUE_LEN] = {0};
			PR("%s \r\n", "OPTION");
			getOption(v->option, option);
			printf("\t%s\r\n", option);

			//CURRENT VALUE
			PR("%s \r\n", "CURRENT VALUE");
			printf("\t\"%s\"\r\n\r\n",nvram_safe_get(rule));
			
			//DEFAULT VALUE
			PR("%s \r\n", "DEFAULT VALUE");
			printf("\t\"%s\"\r\n\r\n",v->value);

			//[INDEX]|ATTR: (CUR_VALUE)|[DEF_VALUE]
			struct rule *r = NULL;
			struct attr *a = NULL;
			r = getRuleFromRules(rule);
			if ( r ) {
				char val[MAX_VALUE_LEN] = {0};
				char defval[MAX_VALUE_LEN] = {0};
				PR("%s \r\n", "[INDEX]|ATTR:\t(CUR_VALUE)|[DEF_VALUE]");
				int i = ezplib_get_rule_num(r->name);
				int j = 0;	
				char index[8] = {0};
				char tab[8] = {0};
				for( j = 0; j <= (i==0 ? 0 : i-1); ++j) {
					sprintf(index, "[%2d]", j);
					for( a = r->attr; a->name !=NULL; a++ ) {
						ezplib_get_attr_val(r->name, j, a->name, val, sizeof(val), EZPLIB_USE_CLI);
						ezplib_get_attr_val_default(r->name, j, a->name, defval, sizeof(defval), EZPLIB_USE_CLI);
						switch((strlen(a->name)+6)/8) {
							case 0:sprintf(tab, "\t\t\t\t");
								   break;
							case 1:sprintf(tab, "\t\t\t");
								   break;
							case 2:sprintf(tab, "\t\t");
								   break;
							case 3:sprintf(tab, "\t");
								   break;
							default:sprintf(tab, "");
								   break;
						}
						// [INDEX]|ATTR: (CUR_VALUE)|[DEF_VALUE]
						printf("\t%s|%s:%s%s%s%s%s%s%s\r\n", index, a->name, tab,
							0==strlen(val)?"":"(", val, 0==strlen(val)?"":")",
							0==strlen(defval)?"":"|[", defval, 0==strlen(defval)?"":"]");	
						sprintf(index, "    ");
					}
					printf("\r\n");
				}
			}

			//DESCRIPTION
			char * desc = rule_attr_desc(rule);
			if(strcmp("", desc)) {
				PR("%s \r\n", "DESCRIPTION");
				printf("\t%s\r\n", desc);
			}
			
			//SEE ALSO
			char * sa = same_attr_rule(rule, bColor);
			if(strcmp("", sa)) {
				PR("%s \r\n", "SEE ALSO");
				printf("\t%s\r\n", sa);
			}
			return 0;
		}
	}
	printf("'%s' can't be found in factory default values!\r\n", rule);	
	return 1;
}

char *rule_attr_desc(const char *rule)
{
	enum attr_enum {
#define aa(x, y) x, 	
#include "attr_2_desc.h"
#undef aa
	};

	struct __rule_2_attr {
		char *rule;
		enum attr_enum attr_index;
	} rule_2_attr [] = {
#define aa(x, y) {x, y},
#include "rule_2_attr.h"
#undef aa
		{NULL, -1},
	};

	struct __attr_2_desc {
		enum attr_enum attr_index;
		char *desc;
	} attr_2_desc [] = {
#define aa(x, y) {x, y},
#include "attr_2_desc.h"
#undef aa
	};

	struct __rule_2_attr *r2a = NULL;
	for (r2a = rule_2_attr; r2a->rule != NULL ; r2a++) {
		if (!strcmp(r2a->rule, rule)) {
			return attr_2_desc[r2a->attr_index].desc;
		}
	}
	return "";
}

char *	same_attr_rule(const char *rule, int bColor)
{
	enum attr_enum {
#define aa(x, y) x, 	
#include "attr_2_desc.h"
#undef aa
	};

	struct __rule_2_attr {
		char *rule;
		enum attr_enum attr_index;
	} rule_2_attr [] = {
#define aa(x, y) {x, y},
#include "rule_2_attr.h"
#undef aa
		{NULL, -1},
	};

	struct __attr_2_desc {
		enum attr_enum attr_index;
		char *desc;
	} attr_2_desc [] = {
#define aa(x, y) {x, y},
#include "attr_2_desc.h"
#undef aa
	};

	char buf[32768] = {0};
	char tmp_rule[64] = {0};
	struct __rule_2_attr *r2a = NULL;
	for (r2a = rule_2_attr; r2a->rule != NULL ; r2a++) {
		if (!strcmp(r2a->rule, rule)) {
			enum attr_enum ai = r2a->attr_index;
			for (r2a = rule_2_attr; r2a->rule != NULL ; r2a++) {
				if ( (r2a->attr_index == ai) && strcmp(r2a->rule, rule) ) {
					SPR(tmp_rule, "%s,", r2a->rule);
					strcat(buf, tmp_rule);
				}
			}
			break;
		}
	}
	return buf;
}
#endif//ifdef DEBUG

