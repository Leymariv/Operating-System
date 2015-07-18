#include "readcmd.h"
#include "stddef.h"
#include "mem.h"
#include "string.h"
#include "stdio.h"

static unsigned int read_word(char **cur)
{
	unsigned int i = 0 ;
	char c = **cur ;
	while((c != ' ') && (c != '\0'))
	{
		i ++ ;
		(*cur)++;
		c = **cur;
	}
	return i ;
}


struct cmdline *readcmd(char *line)
{
	if(!line)
	{
		return NULL ;
	}
	struct cmdline *commande = mem_alloc(sizeof(struct cmdline));
	commande->cmd = mem_alloc((NBPARAMS + 1) * sizeof(char *));
	char * cur = line ;
	char c ;
	int i = 0 ;
	while((c=*cur) != 0)
	{
		
		if(c == ' '){
			cur++;
		} else {
			
			char * tmp = cur ;
			unsigned int size = read_word(&cur);
			char *buf = mem_alloc(sizeof(char)*(size + 1));
			commande->cmd[i] = strncpy(buf, tmp, size);
			buf[size] = '\0' ;
			i ++;
		}
	}
	commande->nb_params = i - 1;
	return commande;
}

void free_cmd(struct cmdline *c)
{
	for(int j = 0; j <= (c->nb_params); j++)
	{
		mem_free(c->cmd[j], (strlen(c->cmd[j]) + 1) * sizeof(char));
	}
	mem_free(c->cmd, (NBPARAMS + 1) * sizeof(char *));
	mem_free(c, sizeof(struct cmdline));
}
