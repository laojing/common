//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 项目名    ：通用库函数
//
// 文件描述  ：项目主数据结构体相关函数。
//
// 修改记录  ：
//
// 创建日期  ：2015年03月06日
//
// 项目作者  ：沈阳工业大学 老井 laojing@139.com
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "common.h"

static CommonData *data;

CommonData * 
GetCommonData () {
	return data;
}

// CommonData
void
AllocCommonData ( gchar *cmd, GtkWindow *window ) {
	if ( data != NULL ) return;
	data = g_slice_new0 ( CommonData );

	data->window = window;
	data->hasCommonData = FALSE;
	if( g_strcmp0( getFileTitle( cmd ), cmd ) == 0 ) {
		data->root = g_strdup_printf( "%s/", g_get_current_dir() );
	} else {
		data->root = getFilePath ( cmd );
	}

	gchar *langs = g_strdup_printf ( "%sLanguages", data->root );
	setlocale ( LC_ALL, "zh_CN.UTF-8" );
	bindtextdomain ( "laojing", langs );
	bind_textdomain_codeset ( "laojing", "UTF-8" );
	textdomain ( "laojing" );
	g_free ( langs );

	GError *error = NULL;
	GString *str = g_string_new( data->root );
	g_string_append_printf( str, "resource.gresource" );
	data->resource = g_resource_load( str->str, &error );
	g_string_free( str, TRUE );
	g_resources_register( data->resource );
	 
	GBytes *bytes;
	bytes = g_resources_lookup_data( "/gtk.css", 0, NULL);
	data->provider = GTK_STYLE_PROVIDER( gtk_css_provider_new () );
	gtk_css_provider_load_from_data( GTK_CSS_PROVIDER(data->provider), g_bytes_get_data ( bytes, NULL ), -1, NULL );
	g_bytes_unref ( bytes );

	gtk_window_set_icon ( GTK_WINDOW(window), 
							gdk_pixbuf_new_from_resource ( "/icon.png", NULL ) );
}

void
FreeCommonData () {
//	if ( data->hasCommonData ) CloseProjectData();
	g_resources_unregister ( data->resource );

	g_slice_free ( CommonData, data );
	data = NULL;
}


void
ApplyCss( GtkWidget *widget ) {
	gtk_style_context_add_provider (gtk_widget_get_style_context (widget), data->provider, G_MAXUINT);
	if (GTK_IS_CONTAINER (widget)) {
    	gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) ApplyCss, NULL);
  	}
}

gboolean 
HasProjectData ( ) {
	return data->hasCommonData;
}

void 
SetProjectData ( gboolean has ) {
	data->hasCommonData = has;
}
