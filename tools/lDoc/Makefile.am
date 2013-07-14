
AUTOMAKE_OPTIONS = foreign

bin_SCRIPTS = ldoc #qt2kdoc

kman_DATA = ldoc.1 qt2kdoc.1
kmandir= ${mandir}/man1/

pkgdata_DATA = Ast.pm kdocHTML.pm kdocTeX.pm kdocMan.pm

ldoc: ldoc.in
	cp ldoc.in ldoc
	${perl} -npi -e 's%^#\!.*$$%#!'${perl}'%g;' ldoc
	${perl} -npi -e 's#\s*PREFIX\s*=.*$$#PREFIX="'${prefix}'";#g;' ldoc
	${perl} -npi -e 's#\$$Version\$$#'"${Version}"'#g;' ldoc

qt2kdoc: qt2kdoc.in
	cp qt2kdoc.in qt2kdoc
	${perl} -npi -e 's%^#\!.*$$%#!'${perl}'%g;' qt2kdoc

CLEANFILES = $(bin_SCRIPTS)
DISTCLEANFILES = perlbin

tags:
	perltags ldoc.in qt2kdoc.in $(pkgdata_DATA)
