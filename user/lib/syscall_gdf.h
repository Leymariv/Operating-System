#ifndef __SYSCALL_GDF_
#define __SYSCALL_GDF_

extern int ls_gdf(char *name); 

extern void get_dcour(char *tampon);

extern int cd_gdf(char *dossier);

extern int mkdir_gdf(char *name);

extern int rmdir_gdf(char *name);

extern int touch_gdf(char *name);

extern int rm_gdf(char *name);

extern int cat_gdf(char *name);

#endif
