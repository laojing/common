//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 项目名    ：通用库函数
//
// 文件描述  ：Gtk常用函数封装。
//
// 修改记录  ：
//
// 创建日期  ：2015年03月08日
//
// 项目作者  ：沈阳工业大学 老井 laojing@139.com
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "common.h"

GtkToolItem*
CreateToolButton ( gchar *iconpath,
					gchar *labtext,
					GCallback call ) {
	GtkToolItem *new = gtk_tool_button_new ( 
				gtk_image_new_from_icon_name ( iconpath, GTK_ICON_SIZE_BUTTON ),
				labtext );
	g_signal_connect(G_OBJECT(new), "clicked",
		G_CALLBACK(call), NULL);
	return new;
}

GtkWidget*
CreateMenuItem ( gchar *iconpath,
				gchar *labtext,
				gchar *alabtext,
				gint accelval,
				GCallback call,
				GtkAccelGroup *accel_group ) {

	GtkWidget *quit = gtk_menu_item_new ();
	gtk_widget_add_accelerator(quit, "activate", accel_group, accelval, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 

	GtkWidget *image = gtk_image_new_from_icon_name ( iconpath, GTK_ICON_SIZE_BUTTON );
	GtkWidget *label = gtk_label_new ( labtext );
	GtkWidget *alabel = gtk_label_new ( alabtext );

	gtk_widget_set_halign ( GTK_WIDGET(alabel), GTK_ALIGN_END );
	GtkWidget *mbox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	gtk_box_pack_start(GTK_BOX(mbox), image, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(mbox), label, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(mbox), alabel, TRUE, TRUE, 10);
	const char *format = "<span foreground=\"gray\">\%s</span>";
	char *markup = g_markup_printf_escaped (format, alabtext);
	gtk_label_set_markup (GTK_LABEL (alabel), markup);
	g_free (markup);
	gtk_container_add ( GTK_CONTAINER(quit), mbox );
	g_signal_connect(G_OBJECT(quit), "activate",
		call, NULL);
	return quit;
}


//--------------------------------------------------------------------
//
// 函数功能  ：显示Gtk通知信息对话框。
//
// 函数参数  ：gchar *info : 需要显示的信息字符串。
//
// 返回值    ：
//
// 用法示例  ：
//
// 修改记录  ：
//
//--------------------------------------------------------------------
void 
displayMessage ( gchar *info ) {
	GtkWidget *dialog = gtk_message_dialog_new ( GetCommonData()->window,
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			"%s", info );
	gtk_window_set_title ( GTK_WINDOW(dialog), _("Information") );
	gtk_dialog_run ( GTK_DIALOG(dialog) );
	gtk_widget_destroy ( dialog );
}

//--------------------------------------------------------------------
//
// 函数功能  ：显示确认信息对话框。
//
// 函数参数  ：gchar *info : 需要确认的信息字符串。
//
// 返回值    ：gboolean : 用户点击确认返回true，点击取消返回false。
//
// 用法示例  ：
//
// 修改记录  ：
//
//--------------------------------------------------------------------
gboolean 
determineInfo ( gchar *info ) {
	GtkWidget *dialog = gtk_message_dialog_new ( GetCommonData()->window,
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_QUESTION,
		GTK_BUTTONS_YES_NO,
		"%s", info );
	gtk_window_set_title ( GTK_WINDOW(dialog), _("Question") );
	gint res = gtk_dialog_run ( GTK_DIALOG(dialog) );
	gboolean result = false;
	if ( res == GTK_RESPONSE_YES ) result = true;
	gtk_widget_destroy ( dialog );
	return result;
}

//--------------------------------------------------------------------
//
// 函数功能  ：清除容器内所有子控件。
//
// 函数参数  ：GtkWidget *widget : 需要清除的容器。
//
// 返回值    ：
//
// 用法示例  ：
//
// 修改记录  ：
//
//--------------------------------------------------------------------
void
clearContainer ( GtkWidget *widget ) {
	GList *menus = gtk_container_get_children ( GTK_CONTAINER(widget) );
	gint length = g_list_length(menus);
	for ( int i=0; i<length; i++ ) {
		gtk_widget_destroy ( GTK_WIDGET(g_list_nth_data(menus,i)) );
	}
}

