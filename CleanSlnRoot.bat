:: Clean all build folders

SET RootPath=%~dp0

@RD /S /Q "%RootPath%.vs"
@RD /S /Q "%RootPath%Debug"
@RD /S /Q "%RootPath%Intermediate"
@RD /S /Q "%RootPath%lib"