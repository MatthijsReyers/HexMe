if [[ $EUID -eq 0 ]]; then 
    echo "Installing HexMe version 3.0"
    sudo rm /usr/bin/hexme > /dev/null 2>&1
    sudo cp hexme /usr/bin
    echo "Finished installing..."
else
    echo "Please run as root"
fi