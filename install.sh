if test -f ./hexme; 
then
    rm -f /usr/bin/hexme
    cp ./hexme /usr/bin/hexme
else
    echo "Executable not found, please build the application first"
fi
