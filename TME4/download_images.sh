#!/bin/bash

# Create input folder if it doesn't exist
mkdir -p input_images
cd input_images

# Download all 100 ESA Hubble images from the list
ids=("heic0108a" "hubble_earth" "heic0109a" "opo0006a" "opo0110a" "heic0206a" "opo0320a" "opo0415a" "heic0305a" "opo0417b" "opo0423a" "opo0511a" "heic0416a" "opo0607a" "heic0503a" "opo0613a" "opo0624a" "heic0514a" "opo0635a" "heic0515a" "opo0822a" "opo0834a" "opo0917a" "opo9545a" "heic0615a" "opo9941a" "heic0706a" "heic0715a" "heic0817a" "heic0905a" "heic1007e" "opo0917a" "heic0911b" "heic0911c" "heic0910f" "heic0910c" "opo0834a" "opo0822a" "hubble_earth" "heic0709b" "opo0635a"  )

for i in "${!ids[@]}"; do
num=$(printf "%03d" $((i+1)))
wget https://cdn.esahubble.org/archives/images/large/${ids[$i]}.jpg -O ${num}_${ids[$i]}.jpg
done

# # Filter out images larger than 10MB
# echo "Filtering out images larger than 10MB..."
# for file in *.jpg; do
# if [ -f "$file" ]; then
# size=$(stat -c%s "$file")
# if [ $size -gt 10485760 ]; then
# echo "Removing $file (size: $size bytes)"
# rm "$file"
# fi
# fi
# done
# echo "Filtering complete."

echo "Download and filtering complete. Images saved in input_images folder."