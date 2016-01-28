@set PATH=
@set ROOT=%~d0/Developer

@set PATH=%~d0/Developer/vim/vim74;%PATH%
@set PATH=%ROOT%/mingw/w64/bin;%PATH%
@set PATH=%ROOT%/Librarys/gtk/w64/bin;%PATH%
@set PATH=%ROOT%/Librarys/gsl/bin;%PATH%

@set COPTION=-g -c -std=gnu99 -mms-bitfields -I%ROOT%/Librarys/gtk/w64/include/gtk-3.0 -I%ROOT%/Librarys/gtk/w64/include/cairo -I%ROOT%/Librarys/gtk/w64/include/pango-1.0 -I%ROOT%/Librarys/gtk/w64/include/atk-1.0 -I%ROOT%/Librarys/gtk/w64/include/cairo -I%ROOT%/Librarys/gtk/w64/include/pixman-1 -I%ROOT%/Librarys/gtk/w64/include -I%ROOT%/Librarys/gtk/w64/include/freetype2 -I%ROOT%/Librarys/gtk/w64/include -I%ROOT%/Librarys/gtk/w64/include/libpng15 -I%ROOT%/Librarys/gtk/w64/include/gdk-pixbuf-2.0 -I%ROOT%/Librarys/gtk/w64/include/libpng15 -I%ROOT%/Librarys/gtk/w64/include/glib-2.0 -I%ROOT%/Librarys/gtk/w64/lib/glib-2.0/include -I%ROOT%/Librarys/gtk/w64/x86_64-w64-mingw32/include -I%ROOT%/Librarys/mariadb/include -I%ROOT%/Librarys/gsl/include

@set CLIBS=-L%ROOT%/Librarys/gtk/w64/lib -L%ROOT%/Librarys/gsl/lib -lgtk-3 -lgdk-3 -lgdi32 -limm32 -lshell32 -lole32 -Wl,-luuid -lpangocairo-1.0 -lpangoft2-1.0 -lfreetype -lfontconfig -lpangowin32-1.0 -lgdi32 -lpango-1.0 -lm -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl
