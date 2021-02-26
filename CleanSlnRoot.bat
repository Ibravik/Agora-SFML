:: Clean all build folders

SET RootPath=%~dp0

@RD /S /Q "%RootPath%.vs"
@RD /S /Q "%RootPath%Intermediate"