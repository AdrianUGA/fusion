#!/bin/bash




#Fichier.o
touch Test/display_header_obj.txt
echo "Version readelf :" > Test/display_header_obj.txt
readelf -h Test/file1.o >> Test/display_header_obj.txt
echo " " >> Test/display_header_obj.txt
echo "Version projet :" >> Test/display_header_obj.txt
./main -h Test/file1.o >> Test/display_header_obj.txt

touch Test/display_section_header_obj.txt
echo "Version readelf :" > Test/display_section_header_obj.txt
readelf -S Test/file1.o >> Test/display_section_header_obj.txt
echo " " >> Test/display_scetion_header_obj.txt
echo "Version projet :" >> Test/display_section_header_obj.txt
./main -S Test/file1.o >> Test/display_section_header_obj.txt

touch Test/display_symbole_obj.txt
echo "Version readelf :" > Test/display_symbole_obj.txt
readelf -s Test/file1.o >> Test/display_symbole_obj.txt
echo " " >> Test/display_symbole_obj.txt
echo "Version projet :" >> Test/display_symbole_obj.txt
./main -s Test/file1.o >> Test/display_symbole_obj.txt

touch Test/display_realoc_obj.txt
echo "Version readelf :" > Test/display_realoc_obj.txt
readelf -r Test/file1.o >> Test/display_realoc_obj.txt
echo " " >> Test/display_realoc_obj.txt
echo "Version projet :" >> Test/display_realoc_obj.txt
./main -r Test/file1.o >> Test/display_realoc_obj.txt





#Fichier.o après fusion
touch Test/display_header_fusion.txt
echo "Version readelf :" > Test/display_header_fusion.txt
readelf -h Test/file1.o >> Test/display_header_fusion.txt
echo " " >> Test/display_header_fusion.txt
echo "Version projet :" >> Test/display_header_fusion.txt
./main -h Test/file1.o >> Test/display_header_fusion.txt

touch Test/display_section_header_fusion.txt
echo "Version readelf :" > Test/display_section_header_fusion.txt
readelf -S Test/fusion_reussi.o >>Test/display_section_header_fusion.txt
echo " " >> Test/display_section_header_fusion.txt
echo "Version projet :" >> Test/display_section_header_fusion.txt
./main -S Test/fusion_reussi.o >> Test/display_section_header_fusion.txt

touch Test/display_symbole_fusion.txt
echo "Version readelf :" > Test/display_symbole_fusion.txt
readelf -s Test/fusion_reussi.o >> Test/display_symbole_fusion.txt
echo " " >> Test/display_symbole_obj.txt
echo "Version projet :" >> Test/display_symbole_fusion.txt
./main -s Test/fusion_reussi.o >> Test/display_symbole_fusion.txt

touch Test/display_realoc_fusion.txt
echo "Version readelf :" > Test/display_realoc_fusion.txt
readelf -r Test/fusion_reussi.o >> Test/display_realoc_fusion.txt
echo " " >> Test/display_realoc_fusion.txt
echo "Version projet :" >> Test/display_realoc_fusion.txt
./main -r Test/fusion_reussi.o >> Test/display_realoc_fusion.txt





#Fichier exec
touch Test/display_header_exec.txt
echo "Version readelf :" > Test/display_header_exec.txt
readelf -h Test/fileEXEC >> Test/display_header_exec.txt
echo " " >> Test/display_header_exec.txt
echo "Version projet :" >> Test/display_header_exec.txt
./main -h Test/fileEXEC >> Test/display_header_exec.txt

touch Test/display_section_header_obj.txt
echo "Version readelf :" > Test/display_section_header_exec.txt
readelf -S Test/fileEXEC >> Test/display_section_header_exec.txt
echo " " >> Test/display_section_header_exec.txt
echo "Version projet :" >> Test/display_section_header_exec.txt
./main -S Test/fileEXEC >> Test/display_section_header_exec.txt

touch Test/display_symbole_obj.txt
echo "Version readelf :" > Test/display_symbole_exec.txt
readelf -s Test/fileEXEC >> Test/display_symbole_exec.txt
echo " " >> Test/display_symbole_exec.txt
echo "Version projet :" >> Test/display_symbole_exec.txt
./main -s Test/fileEXEC >> Test/display_symbole_exec.txt

touch Test/display_realoc_obj.txt
echo "Version readelf :" > Test/display_realoc_exec.txt
readelf -r Test/fileEXEC >> Test/display_realoc_exec.txt
echo " " >> Test/display_realoc_exec.txt
echo "Version projet :" >> Test/display_realoc_exec.txt
./main -r Test/fileEXEC >> Test/display_realoc_exec.txt




#Fichier non elf
touch Test/display_notELF.txt
echo "Version readelf :" > Test/display_notELF.txt
readelf -h Test/file_non_ELF 2>> Test/display_notELF.txt
echo " " >> Test/display_notELF.txt
echo "Version projet :" >> Test/display_notELF.txt
./main -r Test/file_non_ELF 2>> Test/display_notELF.txt

