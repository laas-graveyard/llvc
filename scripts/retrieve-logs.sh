#!/bin/bash
# le nom du script est copie dans le parametre $0

grxuser="grxuser"
user=${USER}

# ------------- Repertoires Sources/Dest ------#
repSourceLocale=${HOME}"/devel/openrobots/var/log"
repSourceDistantVision="${user}@hrp2010v:~/devel-src/LLVS/build/src"
repSourceDistantControl="${grxuser}@hrp2010c:~/src/OpenHRP/Controller/IOserver/robot/HRP2JRL/bin"
repSauvegardeDistant="${grxuser}@jrlhrp01-u:~/Dune-Embarki/"


repExp="Exp$(date '+%Y%m%d')" 
repTime="$(date '+%Hh%M')"


# Reading the options
verbose=1
options="hvt:d:"
while getopts $options flag; do
	case "$flag" in
		"h")
			print_help 0
			;;
		"v")
			verbose=0
			;;
		"d")
			expDate=$OPTARG
			repExp="Exp2010${expDate}"
			;;
		"t")
			expTime=$OPTARG
			repTime="${expTime}"
			;;
	esac
done



# -------------- Demande d'aide -----------#
function print_help {
	echo " Usage : $0 [$options] ExpDate (ex:0622) ExpHeure (ex:22h43)"
	echo " Where option can be:"
	echo -e "\th: prints this help and exit"
	echo -e "\tv: verbose mode"
	echo -e "\td date: copy files for date \"date\""
	echo -e "\tt time: copy files for time \"time\""
	exit $1
}


tarName=${repExp}_${repTime}
repPath=~/data/results/ExpRobot/${repExp}/${repTime}

echo " "
echo " Les donnees seront stockees dans ${repPath}"
echo " "

if ! [ -d ~/data/results/ExpRobot/${repExp} ]; then
    mkdir ~/data/results/ExpRobot/${repExp}
fi


if [ -d ${repPath} ]
then
    echo " Le repertoire existe"
    echo " Voulez vous l'effacer ? y/n/a"; read OPT_DELETE
    if [ ${OPT_DELETE} = y ]
    then 
       	rm -r ${repPath}
        mkdir ${repPath}
    elif [ ${OPT_DELETE} = n ]
    then     
    echo " Etes vous sur ? y/n" ; read OPT_CONFIRM
	if [ ${OPT_CONFIRM} = n ]
        then
	    echo "exit >> fin du script de copie."
            exit 1
	fi
    else  
        echo "exit >> fin du script de copie."
	exit 1

    fi
else
    mkdir ${repPath}
fi



if [ -d  ${repSourceLocale} ]
then
    if ! [ -d ${repPath}/FromClient ]; then 
	mkdir  ${repPath}/FromClient
    fi
    if ! [ -d ${repPath}/FromRobot ]; then 
	mkdir  ${repPath}/FromRobot
    fi

    #copie locale
    echo " "
    echo "copie locale des donnees du Client"
    cp  ${repSourceLocale}/* ${repPath}/FromClient

    # copie a distance
    echo " "
    echo "copie distante des donnees de la vision ... " 
    scp ${repSourceDistantVision}/dump* ${repPath}/FromRobot

    echo " "
    echo "copie distante des donnees du control ... " 
    scp ${repSourceDistantControl}/WalkTask-* ${repPath}/FromRobot
  
    # create the tar gz
    echo " "
    echo "Creation de l'archive ${tarName}.tar.gz"
    

    cd ${repPath}
    echo " "
    echo "Envoie de l'archive ${tarName}.tar.gz dans ${repSauvegardeDistant} "
    
    tar -czf  ${tarName}.tar.gz FromClient FromRobot --checkpoint=5000
    scp ${tarName}.tar.gz ${repSauvegardeDistant}

    cd -
 
else 
    echo "WARNING >>  ${repSourceLocale} is not a valid directory"
fi