#include "common.h"

GtkToolItem* 
createRecentMenu( GCallback call ) {
	CommonData* cdata = GetCommonData ();
	cdata->recentmenu = gtk_menu_new();
	GtkToolItem *new = gtk_menu_tool_button_new ( 
				gtk_image_new_from_icon_name ( "format-justify-fill", GTK_ICON_SIZE_BUTTON ),
				_("Recent") );
	gtk_menu_tool_button_set_menu ( 
				GTK_MENU_TOOL_BUTTON(new),
				cdata->recentmenu ); 
	insertRecent( call );
	return new;
}

void
updateRecent ( const gchar *filename, const gchar *desp ) {
	gchar *content;
	gboolean find = false;
	gchar *newcont = "";
	GString *str = g_string_new ( GetCommonData()->root );
	g_string_append_printf ( str, ".sharedata" );
	if ( g_file_test ( str->str, G_FILE_TEST_EXISTS ) ) {
		g_file_get_contents ( str->str, &content, NULL, NULL );
		gchar **rows = g_strsplit ( content, "\n", 0 );
		gint rowNum = g_strv_length ( rows );
		if ( rowNum > 8 ) rowNum = 8;
		newcont = g_strdup_printf ( "%s", rows[0] );
		if ( strcmp ( rows[0], filename ) == 0 ) find = true;
		for ( int i=1; i<rowNum; i++ ) {
			if ( strcmp ( rows[i], filename ) == 0 ) {
				i++;
				continue;
			}
			newcont = g_strdup_printf ( "%s\n%s", newcont, rows[i] );
		}
		g_strfreev ( rows );
		g_free ( content );
	}
	if( strcmp( newcont, "" ) == 0 )
		newcont = g_strdup_printf( "%s", desp );
	else 
		newcont = g_strdup_printf( "%s\n%s", desp, newcont );

	newcont = g_strdup_printf( "%s\n%s", filename, newcont );
	if( !find ) {
		g_file_set_contents( str->str, newcont, -1, NULL ); 
	}
	g_free( newcont );
	g_string_free( str, TRUE );
}

void 
insertRecent( GCallback call ) {
	CommonData* cdata = GetCommonData ();

	clearContainer ( cdata->recentmenu );

	GString *str = g_string_new ( cdata->root );
	g_string_append_printf ( str, ".sharedata" );
	if ( g_file_test ( str->str, G_FILE_TEST_EXISTS ) ) {
		gchar *content = NULL;
		g_file_get_contents ( str->str, &content, NULL, NULL );
		gchar **rows = g_strsplit ( content, "\n", 0 );
		gint rowNum = g_strv_length ( rows );

		for ( int i=0; i<rowNum-1; i+=2 ) {
			GtkWidget *muItem = gtk_menu_item_new_with_label(g_strdup_printf("%s(%s)",rows[i+1], rows[i]));
			gtk_menu_shell_append( GTK_MENU_SHELL(cdata->recentmenu), muItem );
			g_signal_connect ( 
					G_OBJECT(muItem), "activate",
					call, g_strdup(rows[i]) );
		}
		g_strfreev ( rows );
		g_free ( content );
	}
	g_string_free ( str, TRUE );

	gtk_widget_show_all ( cdata->recentmenu );
}

