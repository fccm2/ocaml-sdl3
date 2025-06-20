atlas-2025-06-13_1.bmp: atlas-2025-06-13_1.png atlas-2025-06-13_1.txt
	# Convert to .bmp for sdl3, with a color-key "#ff00ff"
	convert -size 63x65 'xc:#ff00ff' '(' $< ')' -compose Over -composite $@
atlas-2025-06-13_1.png:
	# Download the atlas result from the tag: "result-01"
	wget https://raw.githubusercontent.com/fccm2/abstract-rts-01/82eed488c41825b25ed928fa5020277dc0836743/atlas-2025-06-13_1.png
atlas-2025-06-13_1.txt:
	# Download the informations about the file.
	wget -O - https://raw.githubusercontent.com/fccm2/abstract-rts-01/5467fb2a0e85eab1490bf9d74ee960dc6bdd7905/License.txt > $@
clean:
	$(RM) atlas-2025-06-13_1.png
	$(RM) atlas-2025-06-13_1.bmp
	$(RM) atlas-2025-06-13_1.txt
