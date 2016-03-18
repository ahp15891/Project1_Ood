:run.bat

cd "./Debug/"

@echo "<----- No Options Provided ----->"
@echo "<-- Will print number of files and number of directory -->"
Project1_Ood.exe ../TestFiles/ *.h *.cpp
Pause

@echo "<----- Only "/f" option provided ----->"
@echo "<-- Will Print the file if the searched text is present in it-->" 
Project1_Ood.exe ../TestFiles/ /f HeyAakash *.h *.cpp
Pause

@echo "<----- For recursive call and Text search ----->"
@echo "<-- Will check for any folder in the current directory -->"
@echo "<-- Also search for the searched text in DataStore -->"
Project1_Ood.exe ../TestFiles/ /s /f HeyAakash *.h *.cpp
Pause

@echo "<----- For recursive call and Duplicate Files ----->"
@echo "<-- Will check for any folder in the current directory -->"
@echo "<-- Will print out duplicate files in the DataStore with their Paths -->"
Project1_Ood.exe ../TestFiles/ /s /d *.h *.cpp
Pause


@echo "<-- all present '/s' ,'/f','/d'  -->"
@echo "<-- Will check for any folder in the current directory -->"
@echo "<-- Will print out duplicate files in the DataStore with their Paths -->"
@echo "<-- Also search for the searched text in DataStore -->"
Project1_Ood.exe ../TestFiles/ /s /d /f HeyAakash *.h *.cpp
Pause


