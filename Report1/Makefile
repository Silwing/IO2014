article:
	latex master.tex
	latex master.tex
	dvips -t letter -o master.tmp master.dvi
	mv -f master.tmp master.ps
	ps2pdf14 -dPDFSETTINGS=/prepress -dEmbedAllFonts=true master.ps master.tmp
	mv -f master.tmp article.pdf
