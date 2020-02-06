set DSTDIR=C:\CWTW-Pro\
xcopy Readme.txt %DSTDIR% /D /Y
xcopy License.txt %DSTDIR% /D /Y
xcopy CWTW-Pro.txt %DSTDIR% /D /Y
xcopy CWTW-Pro.exe %DSTDIR% /D /Y
xcopy DATA %DSTDIR%DATA\ /D /S /Y
pause
