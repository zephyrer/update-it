@ECHO OFF

nant.exe -f:UpdateIt.2003.build @build.txt -D:AppName="UpdateIt!" -D:ExeName="UpdateIt" -D:CommitMsg="Development build for Google Code." rebuild-all commit-src
