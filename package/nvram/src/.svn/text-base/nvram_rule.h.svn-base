#ifndef __EZPLIB_RULE_H_
#define __EZPLIB_RULE_H_

#define EZPLIB_BUF_LEN          8192

#define EZPLIB_USE_CLI          0
#define EZPLIB_USE_WEB          1

#define EZPLIB_INVALID          -1
#define EZPLIB_VAL_TRUNC        -2
#define EZPLIB_NO_RULE_SET      -3
#define EZPLIB_NO_RULE          -4
#define EZPLIB_NO_ATTRIBUTE     -5
#define EZPLIB_IDX_OUT_RANGE    -6

#define RULE_SEP "|"
#define ATTR_SEP "^"

#define MAX_ATTR_NUM    34

struct attr {
    char *name;
    int (*func_orig)(char *, char *, int);
    int (*func_wrap)(char *, char *, int);
};

struct rule {
    char *name;
    struct attr *attr;
};

enum opcode {
    ADD_RULE = 0,
    DELETE_RULE = 1,
    REPLACE_RULE = 2
};

/******************************************/
int show_enable_value(char *val, char *buf, int bsize);
int show_value(char *val, char *buf, int bsize);
int show_if_value(char *val, char *buf, int bsize);
int show_proto_value(char *val, char *buf, int bsize);
int show_mask_value(char *val, char *buf, int bsize);

int sep_string(char *word, const char *delim, char **idx_arr, int max_tok);

int ezplib_get_rule(const char *rule_set, int nth, 
	char *buf, int bsize);
int ezplib_get_subrule(const char *rule_set, int nth, 
	int start, int end, char *buf, int bsize);
int ezplib_get_attr_val(const char *rule_set, int nth, 
	const char *type, char *buf, int bsize, int use);

int ezplib_op_rule(const char *rule_set, enum opcode op, 
	int nth, const char *new_rule);

int ezplib_replace_rule(const char *rule_set, int nth, const char *new_rule);
int ezplib_replace_attr(const char *rule_set, int nth, 
	const char *attr, const char *new_rule);
int ezplib_append_rule(const char *rule_set, const char *new_rule);
int ezplib_prepend_rule(const char *rule_set, const char *new_rule);
int ezplib_add_rule(const char *rule_set, int nth, const char *new_rule);
int ezplib_delete_rule(const char *rule_set, int nth);

int ezplib_get_rule_num(const char *rule_set);

/**
 *
 * From default value 
 */
int ezplib_get_rule_num_default(const char *rule_set);
int ezplib_get_rule_default(const char *rule_set, int nth, 
	char *buf, int bsize);
int ezplib_get_attr_val_default(const char *rule_set, int nth, 
	const char *type, char *buf, int bsize, int use);


#endif
