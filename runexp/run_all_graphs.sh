./run.sh -g german -w 0 -r 1 -p 1
./run.sh -g actor -w 0 -r 1 -p 1
./run.sh -g comp -w 0 -r 1 -p 1
./run.sh -g google -w 0 -r 1 -p 1
./run.sh -g hep -w 0 -r 1 -p 1
./run.sh -g www -w 0 -r 1 -p 1
./run.sh -g us -w 0 -r 1 -p 1
# Dangerous: Make sure to adjust the -n in every run.
./clustered_bar.sh -c f8a4753 -w 0 -r 1 -p 1 -n 1