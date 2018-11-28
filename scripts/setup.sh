echo "Installing easy-logger..."
wget https://github.com/ahmad88me/easy-logger/archive/v1.0.zip
unzip v1.0.zip
rm  v1.0.zip
cd easy-logger-1.0;make install;cd ..;rm -Rf easy-logger-1.0
echo "Installing tabular-parser..."
wget https://github.com/ahmad88me/tabular-parser/archive/v1.0.zip
unzip v1.0.zip 
rm v1.0.zip 
cd tabular-parser-1.0;make install;cd ..;rm -Rf tabular-parser-1.0 
echo "Installing fcm-cpp..."
wget https://github.com/ahmad88me/fcm-cpp/archive/v1.0.zip
unzip v1.0.zip 
rm v1.0.zip 
cd fcm-cpp-1.0;make install;cd ..;rm -Rf fcm-cpp-1.0
echo "update linker caches..."
ldconfig
