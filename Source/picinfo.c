#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>
gchar*
getFileTitle ( const gchar *filename ) {
	gchar **strs = g_strsplit ( filename, "/", -1 );
	gint len = g_strv_length ( strs );

	gchar *rc = NULL;

	if ( g_strrstr ( strs[len-1], "." ) > strs[len-1] ) {
		rc = g_strndup ( strs[len-1], g_strrstr ( strs[len-1], "." ) - strs[len-1] );
	} else {
		rc = g_strdup ( strs[len-1] );
	}

	g_strfreev ( strs );
	return rc;
}

gchar*
getFilePath ( const gchar *filename ) {
	gchar *str = g_strrstr ( filename, "/" );
	if ( str ) return g_strndup ( filename, str-filename+1 );
	else return "";
}
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

gchar *JpgExecuteCmd ( gchar *path ) {
	gchar *result = "";
	gchar *contents;
	gchar **rows;
	gsize length, rowNum;
	g_file_get_contents( path, &contents, &length, NULL );
	rows = g_strsplit ( contents, "\n", 0 );

	gchar **words = g_strsplit ( rows[0], " ", 0 );
	gchar **wordss = g_strsplit ( words[10], "x", 0 );
	gint len = g_strv_length ( wordss );
	if ( len<2 ) {
		g_strfreev ( wordss );
		wordss = g_strsplit ( words[9], "x", 0 );
	}
	gint sslen = strlen(wordss[1]);
	if ( wordss[1][sslen-1] == ',' ) {
		result = g_strdup_printf ( " - %s - %s", wordss[0], 
				g_strndup(wordss[1],sslen-1) );
	} else {
		result = g_strdup_printf ( " - %s - %s", wordss[0], wordss[1] );
	}
	g_strfreev ( wordss );

	g_strfreev ( words );

	g_strfreev ( rows );
	g_free ( contents );
	return result;
}

gchar *GifExecuteCmd ( gchar *path ) {
	gchar *result = "";
	gchar *contents;
	gchar **rows;
	gsize length, rowNum;
	g_file_get_contents( path, &contents, &length, NULL );
	rows = g_strsplit ( contents, "\n", 0 );
	rowNum = g_strv_length ( rows );
	for ( int i=0; i<rowNum; i++ ) {
		if ( g_strrstr ( rows[i], "VO:" ) == rows[i] ) {
        	gchar **words = g_strsplit ( rows[i], " ", 0 );
        	if ( g_strv_length ( words ) <= 4 ) continue;
        	gchar **wordss = g_strsplit ( words[2], "x", 0 );
        	result = g_strdup_printf ( " - %s - %s", wordss[0], wordss[1] );
        	g_strfreev ( wordss );
        	g_strfreev ( words );
		}
		if ( g_strlstr ( rows[i], "V:" ) == rows[i] ) {
        	gchar **words = g_strsplit ( rows[i], "\r", 0 );
			gint len = g_strv_length ( words );
			while ( strlen(words[len-1]) < 30 ) len--;
        	gchar **wordss = g_strsplit ( words[len-1], " ", 0 );
			gint lens = g_strv_length ( wordss );
			if ( strlen(wordss[3]) < 2 || g_strrstr(wordss[3],"/") ) { 
        		result = g_strdup_printf ( "%s - %s", result, wordss[2] );
			} else {
				result = g_strdup_printf ( "%s - %s", result, wordss[3] );
			}
        	g_strfreev ( wordss );
        	g_strfreev ( words );
		}
	}
	g_strfreev ( rows );
	g_free ( contents );
	return result;
}

void GetJpgThumb ( const gchar *filename ) {
	gchar **words = g_strsplit ( filename, " - ", 0 );
	gchar *thumbname = g_strdup_printf ( "%s.pic/%s.jpg",
			getFilePath(filename), getFileTitle(filename) );
	int width = atoi(words[1]);
	int height = atoi(words[2]);
	gchar *command = "";
	if ( width>height ) {
		command = g_strdup_printf ( "%s \"%s\" \"%s\"", 
			"convert -resize x200 -gravity center -extent 200x200",
			filename, thumbname );
	} else {
		command = g_strdup_printf ( "%s \"%s\" \"%s\"", 
			"convert -resize 200x -gravity center -extent 200x200",
			filename, thumbname );
	}
	system ( command );
	g_free ( command );
	g_free ( thumbname );
	g_strfreev ( words );
}

void GetGifThumb ( const gchar *gifname ) {
	gchar *filename = g_strdup_printf ( "%s%s.jpg", 
			getFilePath(gifname), getFileTitle(gifname) );
	gchar *command = g_strdup_printf ( "ffmpeg -i \"%s\" \"%s\"", 
			gifname, filename );
	system ( command );
	g_free ( command );
	gchar **words = g_strsplit ( filename, " - ", 0 );
	gchar *thumbname = g_strdup_printf ( "%s.pic/%s.jpg",
			getFilePath(filename), getFileTitle(filename) );
	int width = atoi(words[1]);
	int height = atoi(words[2]);
	if ( width>height ) {
		command = g_strdup_printf ( "%s \"%s\" \"%s\"", 
			"convert -resize x200 -gravity center -extent 200x200",
			filename, thumbname );
	} else {
		command = g_strdup_printf ( "%s \"%s\" \"%s\"", 
			"convert -resize 200x -gravity center -extent 200x200",
			filename, thumbname );
	}
	system ( command );
	g_free ( command );

	command = g_strdup_printf ( "rm \"%s\"", filename );
	system ( command );
	g_free ( command );

	g_free ( thumbname );
	g_free ( filename );
	g_strfreev ( words );
}

gchar *GetJpgInf ( gchar *filename, gboolean clear ) {
	gchar **words = g_strsplit ( filename, " - ", 0 );
	gint len = g_strv_length ( words );
	gchar *result = "";
	if ( !clear && len == 1 ) {
		gchar *cmd = g_strdup_printf ( "ffmpeg -i \"%s\" 2>&1 | grep \"Video:\" > %s", filename, getFileTitle(filename) );
		printf ( "%s\n", cmd );
		system ( cmd );
		g_free ( cmd );
		cmd = JpgExecuteCmd ( getFileTitle(filename) );
		result = g_strdup_printf ( "%s%s.jpg",
			g_strndup(filename,strlen(filename)-4), cmd );
		cmd = g_strdup_printf ( "mv \"%s\" \"%s%s.jpg\"", 
			filename, g_strndup(filename,strlen(filename)-4), cmd );
		system ( cmd );
		g_free ( cmd );
		cmd = g_strdup_printf ( "rm %s", getFileTitle(filename) );
		system ( cmd );
		g_free ( cmd );
	} 

	if ( clear && len > 1 ) {
		gchar *cmd = g_strdup_printf ( "mv \"%s\" \"%s.jpg\"", 
			filename, words[0] );
		system ( cmd );
		g_free ( cmd );
	}
	g_strfreev ( words );
	return result;
}

gchar *GetGifInf ( gchar *filename, gboolean clear ) {
	gchar **words = g_strsplit ( filename, " - ", 0 );
	gint len = g_strv_length ( words );
	gchar *result = "";
	if ( !clear && len == 1 ) {
		gchar *res = g_strdup_printf ( "mplayer -vo null -ao null -identify \"%s\" > config" , filename );
		system ( res );
		g_free ( res );
		res = GifExecuteCmd ( "config" );
		result = g_strdup_printf ( "%s%s.gif",
			g_strndup(filename,strlen(filename)-4), res );
		res = g_strdup_printf ( "mv \"%s\" \"%s%s.gif\"", 
			filename, g_strndup(filename,strlen(filename)-4), res );
		system ( res );
		system ( "rm ./config" );
		g_free ( res );
	}

	if ( clear && len > 1 ) {
		gchar *cmd = g_strdup_printf ( "mv \"%s\" \"%s.gif\"", 
			filename, words[0] );
		system ( cmd );
		g_free ( cmd );
	}

	g_strfreev ( words );
	return result;
}

int main ( int argc, char *argv[] ) {
	if ( argc == 3 ) {
		if ( g_strcmp0(argv[1], "clear") == 0 ) {
			gchar *imgpath = argv[2];
			if ( g_strrstr(imgpath, "jpg" ) ) {
				GetJpgInf ( imgpath, TRUE );
				gchar *cmd = g_strdup_printf ( " rm %s.pic/*.jpg",
						getFilePath(imgpath) );
				system ( cmd );
				g_free ( cmd );
			} else if ( g_strrstr(imgpath, "gif" ) ) {
				GetGifInf ( imgpath, TRUE );
				gchar *cmd = g_strdup_printf ( " rm %s.pic/*.jpg",
						getFilePath(imgpath) );
				system ( cmd );
				g_free ( cmd );
			}
		} else {
			gchar *imgpath = argv[2];
			if ( g_strrstr(imgpath, "jpg" ) ) {
				GetJpgThumb ( GetJpgInf ( imgpath, FALSE ) );
			} else if ( g_strrstr(imgpath, "gif" ) ) {
				GetGifThumb ( GetGifInf ( imgpath, FALSE ) );
			}
		}
	}
	return 0;
}
