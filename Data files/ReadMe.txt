This program will convert a Europa Universalis 3: Heir to the Throne or Europa Universalis 3: Divine Wind save game to a Victoria 2 save game.

----
IMPORTANT LINKS
----

Forum thread: http://forum.paradoxplaza.com/forum/showthread.php?490709-EU3toVic2-Vic2toHoi3-Converter-Project
Sourceforge project: https://sourceforge.net/projects/eu3tovic2/


----
REQUIREMENTS
----
EU3:HTTT version 4.1 or EU3:DW version 5.1 or later
Victoria 2 version 1.3 or later


----
INSTRUCTIONS
----

1) Find the desired save file in "[YOUR EU3 DIRECTORY]/save games".
2) Copy the save game in to the directory of this converter, and rename it to "input.eu3"
3) Open the file "configuration.txt".  In the file there is a line that reads 
	v2directory = "D:\Paradox Interactive\Victoria 2"
   Modify it so that it points to YOUR copy of Victoria 2.
4) In "configuration.txt", there is a line that reads
	EU3directory = "D:\Paradox Interactive\Europa Universalis III"
   Modify it so that it points to YOUR copy of Europa Universalis III.
5) Run "EU3toV2Converter.exe".  This will create a file in the converters directory called "output.v2".
6) Copy "output.v2" to "[YOUR VICTORIA DIRECTORY]/save games" and run Victoria 2
7) Load the output game and immediately save the game.  Then resign, and load that game.  Enjoy!

If you get an error complaining about MSVCP120.dll, go to Visual Studio Redistributable: http://www.microsoft.com/en-us/download/details.aspx?id=40784
	then click Download and select vcredist_x86.exe.
	Once it has downloaded, install it and run the converter again.


----
THE CONVERTER MOD for VICTORIA 2
----

Zonix's CONVERTER MOD for VICTORIA 2 adds all the nations that exist in EU3 which are missing in Victoria 2.  This mod allows for complete 1 to 1 country mappings rather than using nations where possible.

As of version 0.2, the mod also contains missing cultures.
As of version 0.3 the mod comes with Magc8Balls flag pack as standard

You can get it here: https://sourceforge.net/projects/eu3tovic2/files/Mods/EU3toVIC2%20Mod/

INSTRUCTIONS
1) Make a second copy of your VICTORIA 2 directory.  This will be the modified copy of victoria 2.  (Be sure to name it accordingly, so you don't get confused!)
2) Copy the three modded folders ("common", "gfx", and "localisation") in to your new VICTORIA 2 directory, overwriting the old files.
3) Copy the text files "country_mappings.txt" and "cultureMap.txt" in to the converter's directory, overwriting the old files.
4) Follow the instructions for converting a game, above.  You should now have a V2 game with all of your nations in tact!



----
Magc8Ball's FLAG PACK
----

http://forum.paradoxplaza.com/forum/showthread.php?571081-Complete-EU3-Nations-Flagpack-(For-certain-values-of-quot-complete-quot-)

This massive flag pack contains custom flags for all of the nations (and their political alignments) that are added by Zonix's converter mod.



----
CREDITS
----

A (incomplete) list of those who contributed effort to this mammoth undertaking

Sid Meier
Maksim17
Chaingun
SegFaultx64
Idhrendur
Dr_V
kalelovil
Alfred Packer
Anokmik
King of Men
DasGuntLord01
Zonix
Magc8Ball

 - AND ALL THE OTHERS WHO GAVE THEIR INPUT ON THE FORUM -