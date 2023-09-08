date
sleep 3
mkdir root 
cd root
date
sleep 3
ls / -tr > root.txt
cd ..
date
sleep 3
mkdir home
cd home
date 
sleep 3
ls ~ -tr > home.txt
cat home.txt
cat root.txt
