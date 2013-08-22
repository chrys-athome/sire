#!/bin/sh

echo "\n#######################################################"
echo "#      Where would you like to install Sire?"
echo "#      Note that you can move Sire to any"
echo "#      directory you want after installation"
echo "#######################################################\n"

echo "Full Path to Install directory ($HOME): "

read install_dir

if [ -z "$install_dir" ]; then
    set install_dir = $HOME

else 
    if [ ! -d "$install_dir" ]; then
        echo "Directory $install_dir does not exist! Please create it or provide a valid path."
        exit -1
    fi
fi

echo "\nInstalling Sire to $install_dir"
mv sire.app $install_dir

echo "\nSire is now installed in $install_dir/sire.app"
echo "\nYou can run a Sire python script by typing $install_dir/sire.app/bin/python script.py"
echo "\nAll Sire binaries are available in $install_dir/sire.app/bin"
echo "\n#################################################"
echo "#   Thanks for installing Sire"
echo "#   Please get in touch if you have any problems"
echo "#   (http://siremol.org)"
echo "##################################################"
