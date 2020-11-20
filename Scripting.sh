#! /bin/bash

#for f in *.cpp; do 
#    mv -- "$f" "${f%.cpp}.cc"
#done

cd $1 || { exit 1; }

for f in *.$2; do 
    mv -- "$f" "${f%.$2}.$3"
done

value=`cat Copyright.txt` || { exit 1; }

for file in *; do

    if ! grep -q /*€€ $file; then
        echo "$value" | cat - $file > temp && mv temp $file
    fi

    sed -i 's/float/float/g' $file
    sed -i 's/float/float/g' $file

done