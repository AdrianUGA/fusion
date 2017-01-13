#!/bin/bash




#Fichier.o
touch Test/display_mult.txt
echo "**************************header**************************" > Test/display_obj.txt
echo "Version readelf :" >> Test/display_obj.txt
readelf -h Test/display/obj.o >> Test/display_obj.txt
echo " " >> Test/display_obj.txt
echo "Version projet :" >> Test/display_obj.txt
./phase1 -h Test/display/obj.o >> Test/display_obj.txt
echo " " >> Test/display_obj.txt


echo "**********************Section header**********************" >> Test/display_obj.txt
echo "Version readelf :" >> Test/display_obj.txt
readelf -S Test/display/obj.o >> Test/display_obj.txt
echo " " >> Test/display_obj.txt
echo "Version projet :" >> Test/display_obj.txt
./phase1 -S Test/display/obj.o >> Test/display_obj.txt
echo " " >> Test/display_obj.txt


echo "**************************Symbole**************************" >> Test/display_obj.txt
echo "Version readelf :" >> Test/display_obj.txt
readelf -s Test/display/obj.o >> Test/display_obj.txt
echo " " >> Test/display_mult.txt
echo "Version projet :" >> Test/display_obj.txt
./phase1 -s Test/display/obj.o >> Test/display_obj.txt
echo " " >> Test/display_obj.txt

echo "*****************Table de réimplantation*****************" >> Test/display_obj.txt
echo "Version readelf :" >> Test/display_obj.txt
readelf -r Test/display/obj.o >> Test/display_obj.txt
echo " " >> Test/display_obj.txt
echo "Version projet :" >> Test/display_obj.txt
./phase1 -r Test/display/obj.o >> Test/display_obj.txt





#Fichier.o avec plusieurs table de réimplantations
touch Test/display_mult.txt
echo "**************************header**************************" > Test/display_mult.txt
echo "Version readelf :" >> Test/display_mult.txt
readelf -h Test/display/mult.o >> Test/display_mult.txt
echo " " >> Test/display_mult.txt
echo "Version projet :" >> Test/display_mult.txt
./phase1 -h Test/display/mult.o >> Test/display_mult.txt
echo " " >> Test/display_mult.txt


echo "**********************Section header**********************" >> Test/display_mult.txt
echo "Version readelf :" >> Test/display_mult.txt
readelf -S Test/display/mult.o >> Test/display_mult.txt
echo " " >> Test/display_mult.txt
echo "Version projet :" >> Test/display_mult.txt
./phase1 -S Test/display/mult.o >> Test/display_mult.txt
echo " " >> Test/display_mult.txt


echo "**************************Symbole**************************" >> Test/display_mult.txt
echo "Version readelf :" >> Test/display_mult.txt
readelf -s Test/display/mult.o >> Test/display_mult.txt
echo " " >> Test/display_mult.txt
echo "Version projet :" >> Test/display_mult.txt
./phase1 -s Test/display/mult.o >> Test/display_mult.txt
echo " " >> Test/display_mult.txt

echo "*****************Table de réimplantation*****************" >> Test/display_mult.txt
echo "Version readelf :" >> Test/display_mult.txt
readelf -r Test/display/mult.o >> Test/display_mult.txt
echo " " >> Test/display_mult.txt
echo "Version projet :" >> Test/display_mult.txt
./phase1 -r Test/display/mult.o >> Test/display_mult.txt





#Fichier.o après fusion
touch Test/display_fusion.txt
echo "**************************header**************************" > Test/display_fusion.txt
echo "Version readelf :" >> Test/display_fusion.txt
readelf -h Test/display/fusion.o >> Test/display_fusion.txt
echo " " >> Test/display_fusion.txt
echo "Version projet :" >> Test/display_fusion.txt
./phase1 -h Test/display/fusion.o >> Test/display_fusion.txt
echo " " >> Test/display_fusion.txt


echo "**********************Section header**********************" >> Test/display_fusion.txt
echo "Version readelf :" >> Test/display_fusion.txt
readelf -S Test/display/fusion.o >> Test/display_fusion.txt
echo " " >> Test/display_fusion.txt
echo "Version projet :" >> Test/display_fusion.txt
./phase1 -S Test/display/fusion.o >> Test/display_fusion.txt
echo " " >> Test/display_fusion.txt


echo "**************************Symbole**************************" >>Test/display_fusion.txt
echo "Version readelf :" >> Test/display_fusion.txt
readelf -s Test/display/fusion.o >> Test/display_fusion.txt
echo " " >> Test/display_fusion.txt
echo "Version projet :" >> Test/display_fusion.txt
./phase1 -s Test/display/fusion.o >> Test/display_fusion.txt
echo " " >> Test/display_fusion.txt

echo "*****************Table de réimplantation*****************" >>Test/display_fusion.txt
echo "Version readelf :" >> Test/display_fusion.txt
readelf -r Test/display/fusion.o >> Test/display_fusion.txt
echo " " >> Test/display_fusion.txt
echo "Version projet :" >> Test/display_fusion.txt
./phase1 -r Test/display/fusion.o >> Test/display_fusion.txt
echo " " >> Test/display_fusion.txt





#Fichier exec
touch Test/display_exec.txt
echo "**************************header**************************" > Test/display_exec.txt
echo "Version readelf :" >> Test/display_exec.txt
readelf -h Test/display/exec >> Test/display_exec.txt
echo " " >> Test/display_exec.txt
echo "Version projet :" >> Test/display_exec.txt
./phase1 -h Test/display/exec >> Test/display_exec.txt
echo " " >> Test/display_exec.txt


echo "**********************Section header**********************" >> Test/display_exec.txt
echo "Version readelf :" >> Test/display_exec.txt
readelf -S Test/display/exec >> Test/display_exec.txt
echo " " >> Test/display_exec.txt
echo "Version projet :" >> Test/display_exec.txt
./phase1 -S Test/display/exec >> Test/display_exec.txt
echo " " >> Test/display_exec.txt


echo "**************************Symbole**************************" >> Test/display_exec.txt
echo "Version readelf :" >> Test/display_exec.txt
readelf -s Test/display/exec >> Test/display_exec.txt
echo " " >> Test/display_exec.txt
echo "Version projet :" >> Test/display_exec.txt
./phase1 -s Test/display/exec >> Test/display_exec.txt
echo " " >> Test/display_exec.txt

echo "*****************Table de réimplantation*****************" >> Test/display_exec.txt
echo "Version readelf :" >> Test/display_exec.txt
readelf -r Test/display/exec >> Test/display_exec.txt
echo " " >> Test/display_exec.txt
echo "Version projet :" >> Test/display_exec.txt
./phase1 -r Test/display/exec >> Test/display_exec.txt
echo " " >> Test/display_exec.txt



#Fichier non elf
touch Test/display_notELF.txt
echo "Version readelf :" > Test/display_notELF.txt
readelf -h Test/display/non_elf.o 2>> Test/display_notELF.txt
echo " " >> Test/display_notELF.txt
echo "Version projet :" >> Test/display_notELF.txt
./phase1 -r Test/display/non_elf.o 2>> Test/display_notELF.txt

