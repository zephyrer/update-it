@ECHO OFF

nant.exe -f:UpdateIt.2003.build @build.txt -D:AppName="UpdateIt!" -D:ExeName="UpdateIt" rebuild-all
