#include <syscall.h>
#include <syscall_gdf.h>
#include <mem.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	const char *help_d="help";
	const char *help_f="help/help";
	char *d=mem_alloc((strlen(help_d)+1)*sizeof(char));
	char *fi=mem_alloc((strlen(help_f)+1)*sizeof(char));
	strcpy(d,help_d);
	strcpy(fi,help_f);
	mkdir_gdf(d);
	touch_gdf(fi);
	FILE *f=fopen("help/help","w");
	const char *helper=
		"\thelp : Affiche cette aide\n"
		"\tps : Affiche les processus vivant\n"
		"\tls <path> : Affiche les fichiers et dossier\n"
		"\tcd <path> : Change le repertoire courant\n"
		"\tmkdir <dir_name>* : Creer un/des repertoire(s)\n"
		"\trmdir <dir_name>* : Supprime un/des repertoire(s)\n"
		"\ttouch <file_name>* : Creer un/des fichier(s)\n"
		"\trm <file_name>* : Supprime un/des fichier(s)\n"
		"\techo : Change le mode d'echo\n"
		"\tcat <file_name> : Affiche le contenu d'un fichier\n"
		"\tedit <file_name> : Permet d'editer un fichier existant\n"
		"\ttest <test_name> : Lance un test\n"
		"\texit : Reboot le systme\n"
		"\tkill <pid>* : Tue un/des processus\n"
		"\tsinfo : Affiche les informations sur les semaphores utilises\n"
		"\tpinfo : Affiche les informations sur les files de messages\n"
		"\tclear : Efface l'ecran\n"
		"\tnice : Change la priorite d'un processus\n";
	fwrite(helper,sizeof(char),(int)strlen(helper),f);
	fclose(f);
	return 0;
}
