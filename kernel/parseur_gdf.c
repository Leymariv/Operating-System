#include "parseur_gdf.h"
#include <stddef.h>
#include <string.h>
#include "mem.h"

/* retourne 1 si le nom est bien composé que de lettres
 * 0 sinon
 */
int valide_name(char *name)
{
	int i=0;
	while ((int)name[i]!=0 && (int)name[i]!=47){
		if ((int)name[i]<65 || (int)name[i]>122 || 
		    ((int)name[i]>90 && (int)name[i]<97))
			return 0;
		i++;
	}
	return i!=0;
}

int end_word(char *name)
{
	int i=0;
	while (name[i]!=0 && name[i]!=47)
		i++;
	return i;
}

/*mode : 0 search directory
 *       1 search file
 *       2 create file or directory
 */
elem *file2path(const char *path, int mode, elem *dossier_courant, char *name)
{
	if (*path==0)
		return dossier_courant;
	int i=0;
	elem *cour=dossier_courant;
	elem *file_find;
	do {
		if (!strncmp(path+i,"~",1)){
			cour=dossier_courant->root;
			i+=2;
		} else if(!strncmp(path+i,"..",2)) {
			cour=cour->father;
			i+=3;
		} else if(!strncmp(path+i,".",1)) {
			i+=2;
		} else {
			file_find=NULL;
			int next=end_word((char*)path+i);
			char *tmp_word=mem_alloc(sizeof(char)*(next+1));
			strncpy(tmp_word,path+i,next);
			if (!valide_name((char*)path+i))
				return NULL;
			elem *c;
			elem *tmp=NULL;
			queue_for_each(c,&(cour->sous_elem),elem,suiv){
				if (!strncmp(tmp_word,c->name,strlen(tmp_word)) && 
					c->type=='d')
					tmp=c;
				if (!strncmp(tmp_word,c->name,strlen(tmp_word)) && 
				    c->type=='f')
					file_find=c;
			}
			if (tmp==NULL){
				if (path[i+next]==0 || (path[i+next]==47 &&
							path[i+next+1]==0)){
					if (mode==1 && file_find!=NULL) //search_file
						return file_find;
					if (mode==2 && file_find==NULL){ //create file or directory
						strncpy(name,tmp_word,strlen(tmp_word));
						return cour;
					}
				}
				return NULL;
			}
			cour=tmp;
			mem_free(tmp_word,sizeof(char)*(next+1));
			i+=(next+1);
		}
	} while (path[i-1]==47 && path[i]!=0);
	if (mode==2 || mode == 1)
		return NULL;
	return cour;
}
