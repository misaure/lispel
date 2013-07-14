RM	= rm -f


all:	htmldoc

htmldoc:
	ln -s ../../lispel lispel
	doc++ -H -nd -B footer.html lispel/*.hh
	rm lispel

texdoc:
	ln -s ../../lispel lispel
	doc++ -t -H -nd -ei -o lispel.tex lispel/*.hh
	rm lispel

.PHONY:	distclean

distclean:
	-$(RM) *.html
	-$(RM) *.class
	-$(RM) constructor.gif
	-$(RM) icon1.gif
	-$(RM) icon2.gif
	-$(RM) down.gif
	-$(RM) public.gif
	-$(RM) internal.gif

