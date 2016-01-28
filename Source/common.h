#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __linux__
#define PATHSEPRATOR   "/"
#else
#define PATHSEPRATOR   "/"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#define FLUSH   fflush(stdout);
#define false FALSE
#define true  TRUE
#define null  NULL

// String.c 字符串相关函数
gchar* getFileTitle ( const gchar *filename );
gchar* getFilePath ( const gchar *filename );
gchar* g_strlstr ( gchar *src, gchar *des );
gint64 getStrDate2Int ( gchar *strdate );
gchar* toStandardDate ( gchar *strdate );

// GtkCommon.c Gtk常用小函数
void displayMessage ( gchar *info );
gboolean determineInfo ( gchar *info );
void clearContainer ( GtkWidget *widget );

typedef struct _ToolItemData ToolItemData;
struct _ToolItemData{
	gchar *iconpath;
	gchar *labtext;
	GCallback call;
};
typedef struct _MenuItemData MenuItemData;
struct _MenuItemData{
	gchar *iconpath;
	gchar *labtext;
	gchar *alabtext;
	gint accelval;
	GCallback call;
};
GtkToolItem*
CreateToolButton ( gchar *iconpath,
					gchar *labtext,
					GCallback call );
GtkWidget*
CreateMenuItem ( gchar *iconpath,
				gchar *labtext,
				gchar *alabtext,
				gint accelval,
				GCallback call,
				GtkAccelGroup *accel_group );

// 工程管理
typedef struct _CommonData CommonData;
struct _CommonData{
	gchar *root;
	GList *props;
	GList *object;
	GList *hards;
	gboolean hasCommonData;
	GtkStyleProvider *provider;
	GResource *resource;
	GtkWindow *window;
	GtkWidget *recentmenu;
};

CommonData * GetCommonData ();
void AllocCommonData ( gchar *cmd, GtkWindow *window );
void FreeCommonData ();
void ApplyCss( GtkWidget *widget );

GtkToolItem* createRecentMenu( GCallback call );
void insertRecent( GCallback call );
void updateRecent( const gchar *filename, const gchar *desp );

void SetProjectData ( gboolean has );
gboolean HasProjectData ( );

/*
gchar* getRecentFile ( void );
gchar* getRecentPath ( void );
void setRecentPath ( gchar *recentpath );
void updateRecentFile ();
gboolean ReadProjectData ( const gchar *filename );
*/


#endif 
