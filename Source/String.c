//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 项目名    ：通用库函数
//
// 文件描述  ：字符串处理相关函数。
//
// 修改记录  ：
//
// 创建日期  ：2015年03月05日
//
// 项目作者  ：沈阳工业大学 老井 laojing@139.com
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "common.h"

//--------------------------------------------------------------------
//
// 函数功能  ：在一个全路径文件名称中截取出去掉路径和后缀的文件名称。
//
// 函数参数  ：const gchar* filename : 全路径文件名称。
//
// 返回值    ：gchar* : 去掉路径和后缀的短文件名。
//
// 用法示例  ：getFileTitle ( "/usr/share/tomcat/startup.sh" );
//             返回 : startup
//
// 修改记录  ：2015-03-05 老井(laojing@139.com) 
//             原来是多个后缀名时全部截取掉，修改为只截取最后一个。
//
//--------------------------------------------------------------------
gchar*
getFileTitle ( const gchar *filename ) {
	//gchar **strs = g_strsplit ( filename, PATHSEPRATOR, -1 );
	gchar **strs = g_strsplit ( filename, "/", -1 );
	gint len = g_strv_length ( strs );

	gchar *rc = null;

	if ( g_strrstr ( strs[len-1], "." ) > strs[len-1] ) {
		rc = g_strndup ( strs[len-1], g_strrstr ( strs[len-1], "." ) - strs[len-1] );
	} else {
		rc = g_strdup ( strs[len-1] );
	}

	g_strfreev ( strs );
	return rc;
}

//--------------------------------------------------------------------
//
// 函数功能  ：在一个全路径文件名称中截取出去掉文件名的路径。
//
// 函数参数  ：const gchar* filename : 全路径文件名称。
//
// 返回值    ：gchar* : 去掉文件名的路径。
//
// 用法示例  ：getFilePath ( "/usr/share/tomcat/startup.sh" );
//             返回 : "/usr/share/tomcat"
//
// 修改记录  ：2015-03-05 老井(laojing@139.com) 
//             增加输入参数检查，对非法路径输入，返回原值。
//
//             2015-03-08 老井(laojing@139.com) 
//             增加输入参数检查，对非法路径输入，返回原值。
//
//             2015-03-09 老井(laojing@139.com) 
//             增加输入参数检查，对非法路径输入，返回原值。
//
//
//--------------------------------------------------------------------
gchar*
getFilePath ( const gchar *filename ) {
	gchar *str = g_strrstr ( filename, "/" );
	if ( str ) return g_strndup ( filename, str-filename+1 );
	else return g_strdup ( filename );
}

//--------------------------------------------------------------------
//
// 函数功能  ：从左边开始找子字符串。
//
// 函数参数  ：gchar* src : 一个空字符结束的字符串。
//             gchar* des : 空字符结束的要查询的子字符串。
//
// 返回值    ：gchar* : 子字符串开始的字符串或null(没有找到)。
//
// 用法示例  ：g_strlstr ( "welcome to china", "to" ); 
//             返回 : "to china"
//
// 修改记录  ：
//
//--------------------------------------------------------------------
gchar*
g_strlstr ( gchar *src, 
            gchar *des ) {
	gint len = strlen ( src );
	gint sub = strlen ( des );
	for ( gint i=0; i<len-sub; i++ ) {
		gint temp = 0;
		while ( *(src+i+temp) == *(des+temp) ) {
			if ( ++temp == sub ) {
				return src+i;
			}
		}
	}
	return NULL;
}

//--------------------------------------------------------------------
//
// 函数功能  ：对日期字符串转换为从1970-01-01 00:00:00 UTC开始的秒数。
//             支持格式为：
//				  1. YYYY-mm-DD HH:MM:SS；
//				  2. YYYY-mm-DD HH:MM；
//				  3. YYYY/mm/DD HH:MM:SS；
//				  4. YYYY/mm/DD HH:MM；
//
// 函数参数  ：gchar* strdate : 日期字符串。
//
// 返回值    ：gint64 : 对应秒数或0(非法日期格式)。
//
// 用法示例  ：getStrDate2Int ( "2014-01-01 00:00:00" );
//             返回 : 1388505600
//
// 修改记录  ：
//
//--------------------------------------------------------------------
gint64
getStrDate2Int ( gchar *strdate ) {
	gchar **datetime = g_strsplit ( strdate, " ", 0 );
	GDateTime *dt = NULL;
	if ( g_strv_length(datetime) == 2 ) {
		gint year, month, day, hour, minute, second;
		gchar **dates1 = g_strsplit ( datetime[0], "-", 0 );
		gchar **dates2 = g_strsplit ( datetime[0], "/", 0 );
		if ( g_strv_length(dates1) == 3 ) {
			year = atoi ( dates1[0] );
			month = atoi ( dates1[1] );
			day = atoi ( dates1[2] );
		} else if ( g_strv_length(dates2) == 3 ) {
			year = atoi ( dates2[2] );
			month = atoi ( dates2[1] );
			day = atoi ( dates2[0] );
		} else {
			g_strfreev ( dates1 );
			g_strfreev ( dates2 );
			g_strfreev ( datetime );
			return 0;
		} 
		gchar **times = g_strsplit ( datetime[1], ":", 0 );
		if ( g_strv_length(times) == 3 ) {
			hour = atoi ( times[0] );
			minute = atoi ( times[1] );
			second = atoi ( times[2] );
		} else if ( g_strv_length(times) == 2 ) {
			hour = atoi ( times[0] );
			minute = atoi ( times[1] );
			second = 0;
		} else {
			g_strfreev ( times );
			g_strfreev ( dates1 );
			g_strfreev ( dates2 );
			g_strfreev ( datetime );
			return 0;
		}
		dt = g_date_time_new_local ( year, month, day, hour, minute, second );
		g_strfreev ( dates1 );
		g_strfreev ( dates2 );
		g_strfreev ( times );
		g_strfreev ( datetime );
		return g_date_time_to_unix ( dt );
	}
	g_strfreev ( datetime );
	return 0;
}

gchar* 
toStandardDate ( gchar *strdate ) {
	gchar **datetime = g_strsplit ( strdate, " ", 0 );
	if ( g_strv_length(datetime) == 2 ) {
		gint year, month, day, hour, minute, second;
		gchar **dates1 = g_strsplit ( datetime[0], "-", 0 );
		gchar **dates2 = g_strsplit ( datetime[0], "/", 0 );
		if ( g_strv_length(dates1) == 3 ) {
			year = atoi ( dates1[0] );
			month = atoi ( dates1[1] );
			day = atoi ( dates1[2] );
		} else if ( g_strv_length(dates2) == 3 ) {
			year = atoi ( dates2[2] );
			month = atoi ( dates2[1] );
			day = atoi ( dates2[0] );
		} else {
			g_strfreev ( dates1 );
			g_strfreev ( dates2 );
			g_strfreev ( datetime );
			return "---";
		} 
		gchar **times = g_strsplit ( datetime[1], ":", 0 );
		if ( g_strv_length(times) == 3 ) {
			hour = atoi ( times[0] );
			minute = atoi ( times[1] );
			second = atoi ( times[2] );
		} else if ( g_strv_length(times) == 2 ) {
			hour = atoi ( times[0] );
			minute = atoi ( times[1] );
			second = 0;
		} else {
			g_strfreev ( times );
			g_strfreev ( dates1 );
			g_strfreev ( dates2 );
			g_strfreev ( datetime );
			return "---";
		}
		return g_strdup_printf ( "%02d-%02d-%02d-%02d-%02d-%02d", year, month, day, hour, minute, second );
	}
	g_strfreev ( datetime );
	return "---";
}
