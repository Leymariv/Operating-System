#include "mem.h"
#include "gdf.h"
#include <string.h>
#include "ecran.h"
#include "shm.h"
#include "alloc_physique.h"
#include "process.h"
#include "message_file.h"
#include "keyboard.h"

#define NB_CHAR 1840

void init_gdf()
{
	elem *root=mem_alloc(sizeof(elem));
	root->type='d';
	root->id=0;
	root->father=root;
	root->root=root;
	strncpy(root->name,"root",TAILLE_MAX_NAME);
	INIT_LIST_HEAD(&root->sous_elem);
	dcour=root;
}

int ls_gdf(char *name)
{
	elem *tmp;
	if (name!=NULL){
		tmp=file2path(name,0,dcour,0);
		if (tmp==NULL)
			return -1;
	} else 
		tmp=dcour;
	int i=0;
	elem *cour;
	queue_for_each(cour,&(tmp->sous_elem),elem,suiv){
		if (cour->type=='d')
			change_color(BLEU);
		printf("%-20s ",cour->name); // Attention 20=#taille_max_name
		change_color(BLANC);
		i++;
		if (i==3){
			printf("\n");
			i=0;
		}
	}
	if (i!=0)
		printf("\n");
	return 0;
}

void get_dcour(char *tampon)
{
	strncpy(tampon,dcour->name,TAILLE_MAX_NAME);
}

int cd_gdf(char *dossier)
{
	
	if (dossier==NULL){
		dcour=dcour->root;
		return 0;
	}
	elem *tmp=file2path(dossier,0,dcour,0);
	if (tmp==NULL)
		return -1;
	dcour=tmp;
	return 0;
}

int mkdir_gdf(char *name, char type_)
{
	elem *cour=mem_alloc(sizeof(elem));
	elem *tmp=file2path(name,2,dcour,cour->name);
	if (tmp==NULL)
		return -1;
	cour->type=type_;
	if (type_=='f'){
		palloc((int**)(&(cour->id)));
		cour->last_line = -1 ;
	}
	cour->father=tmp;
	cour->root=tmp->root;
	INIT_LIST_HEAD(&(cour->sous_elem));
	queue_add(cour,&(tmp->sous_elem),elem,suiv,id);
	return 0;
}

int rmdir_gdf(char *name,char type_)
{
	elem *cour=file2path(name,type_=='f'?1:0,dcour,0);
	if (cour==NULL)
		return -1;

	if (!queue_empty(&(cour->sous_elem)))
		return -2;
	if (cour->type=='f'){
		pfree((int*)(cour->id));
	}
	queue_del(cour,suiv);
	mem_free(cour,sizeof(elem));
	return 0;
}

void echo_fct_editor(char * str, int fid, int echo)
{
	if (strlen(str)==3 && (int)(str[0])==27 && (int)(str[1])==91){
		if ((int)(str[2])==65){ //haut
			if (lig_!=0)
				place_curseur(lig_-1,col_);
			return;
		} else if ((int)(str[2])==66){ //bas
			if (lig_!=HAUTEUR_ECRAN-3)
				place_curseur(lig_+1,col_);			
			return;
		} else if ((int)(str[2])==67){ //droite
			if (lig_!=HAUTEUR_ECRAN-3 || col_!=LARGEUR_ECRAN-1){
				if (col_==LARGEUR_ECRAN-1)
					place_curseur(lig_+1,0);
				else 
					place_curseur(lig_,col_+1);
			}
			return;
		} else if ((int)(str[2])==68){ //gauche
			if (lig_!=0 || col_!=0){
				if (col_==0)
					place_curseur(lig_-1,79);
				else 
					place_curseur(lig_,col_-1);
			}
			return;
		}
	}
	for (unsigned i=0; i<strlen(str); i++){
		if (echo){
			if ((int)(str[i])==13 && lig_!=HAUTEUR_ECRAN-3)
				printf("%c",(char)10);
			else if ((int)(str[i])==27 && strlen(str)==1){ //ECHAP
				psend(fid,27);
				str[i]=(char)13;
			} else if ((int)(str[i])<32 && 
				   (lig_!=HAUTEUR_ECRAN-3 || col_!=LARGEUR_ECRAN-1))
				printf("^%c",(char)(64+(int)(str[i])));
			else if ((int)(str[i])==127)
				printf("\b \b");
			else if (lig_!=HAUTEUR_ECRAN-3 || col_!=LARGEUR_ECRAN-1)
				printf("%c",str[i]);
		} 
		psend(fid,(int)str[i]);
	}
}

int *shell_mem;
elem *fichier_cour;

int editor_on(char *name)
{
	elem *cour=file2path(name,1,dcour,0);
	if (cour==NULL)
		return -1;
	fichier_cour=cour;
	shell_mem=shm_create("shell_mem");
	shell_mem[KB4/sizeof(int)-2]=lig_;
	shell_mem[KB4/sizeof(int)-1]=col_;
	memcpy((void *)shell_mem,(void *)BASE_ECRAN,NB_CHAR*sizeof(short));
	printf("\f");
	echo_fct=echo_fct_editor;
	// on affiche le contenu du fichier actuel
	if(fichier_cour->last_line != -1)
	{
		memcpy((void *)BASE_ECRAN,(void*)(fichier_cour->id),NB_CHAR*sizeof(short));
		int i = (fichier_cour->last_line) * LARGEUR_ECRAN ;
		col_ = LARGEUR_ECRAN ;
		while ((((short*)fichier_cour->id)[i]& 0xff)==0 && col_ > 0)
		{
			i --;
			col_--;
		}	
		col_ ++ ;
		lig_ = fichier_cour->last_line -1 ;
		place_curseur(lig_,col_);
	}
	return 0;
}

void editor_off()
{
	memcpy((void*)(fichier_cour->id),(void *)BASE_ECRAN,NB_CHAR*sizeof(short));
	memcpy((void*)BASE_ECRAN,(void *)shell_mem,NB_CHAR*sizeof(short));
	int i=1839;
	while ( i>=0 && ((((short*)fichier_cour->id)[i]& 0xff)==0))
		i--;
	fichier_cour->last_line=i/LARGEUR_ECRAN+(i%LARGEUR_ECRAN==0?0:1);
	lig_=shell_mem[KB4/sizeof(int)-2];
	col_=shell_mem[KB4/sizeof(int)-1];
	echo_fct=echo_fct_standard;
	shm_release("shell_mem");
}

int cat_gdf(char *name)
{
	elem *cour=file2path(name,1,dcour,0);
	if (cour==NULL)
		return -1;
	int i=0;
	while (i/LARGEUR_ECRAN<cour->last_line){
		printf("%c",(char)(((short*)(cour->id))[i] & 0xff));
		i++;
	}
	return 0;
}


FILE *file_error(int id)
{
	return (FILE *)id;
}


FILE *fopen_gdf(const char *path, const char *mode)
{
	elem *cour=file2path(path,1,dcour,0);
	if (cour==NULL)
		return file_error(1);
	
	FILE *f=mem_alloc(sizeof(FILE));
	f->pt_elem=cour;
	f->type_file=(int)(cour->type);
	f->indice=(void *)(cour->id);
	if (!strncmp("r",mode,1))
		f->right=0;
	else if (!strncmp("w",mode,1) && f->type_file=='f')
		f->right=1;
	else {
		mem_free(f,sizeof(FILE));
		return file_error(3); //droit incorrect
	}	
	return f;
}

int fclose_gdf(FILE *fp)
{
	if (fp==NULL)
		return 1;
	mem_free(fp,sizeof(FILE));
	return 0;
}

int fread_gdf(void *ptr, int size, int nmemb, FILE *stream)
{
	if (stream->right!=0)
		return -1;
	int i=0;
	while (stream->indice<stream->pt_elem->id+NB_CHAR*sizeof(char) && 
	       i<nmemb){
		for (int j=0; j<size;j++)
			*((char*)ptr+i*size+j)=*((char*)stream->indice+j);
		(stream->indice)+=size;
		i++;
	}
	return i;
}

int fwrite_gdf(const void *ptr, int size, int nmemb, FILE *stream)
{
	if (stream->right!=1)
		return -1;
	int i=0;
	while (stream->indice<stream->pt_elem->id+NB_CHAR*sizeof(char) && 
	       i<nmemb){
		for (int j=0; j<size;j++)
			*((char*)stream->indice+j)=*((char*)ptr+i*size+j);
		(stream->indice)+=size;
		i++;
	}
	return i;
}
